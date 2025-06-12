/*
 * avr64ea.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 *
 * Playground to get into the newer AVRs.
 *
 * Created on: 16.05.2025
 *     Author: torsten.roemer@luniks.net
 *
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "utils.h"
#include "usart.h"

/* Timebase used for timing internal delays */
#define TIMEBASE_VALUE ((uint8_t) ceil(F_CPU * 0.000001))

/* 0°C in Kelvin */
#define TMP_0C      273.15

/* Specifications of the NTC Thermistor 0.1°C */
#define TH_RESI     100000
#define TH_TEMP     298.15
#define TH_BETA     3892
/* Serial resistance */
#define TH_SERI     100000

/* Periodic interrupt timer interrupt count */
static volatile uint32_t pitints = 0;

/* Timer/Counter Type A 0 interrupt count */
static volatile uint32_t tca0ints = 0;

/* Read only data in program memory visible in RAM address space */
const char rostr[] = "This is a string in .rodata in program memory\r\n";

/* Periodic interrupt timer interrupt */
ISR(RTC_PIT_vect) {
    // clear flag or interrupt remains active
    RTC_PITINTFLAGS |= RTC_PI_bm;
    pitints++;
}

/* Timer/Counter Type A 0 overflow/underflow interrupt */
ISR(TCA0_OVF_vect) {
    // clear flag or interrupt remains active
    TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_OVF_bm;
    tca0ints++;
}

/* ADC0 result ready interrupt... */
/*
ISR(ADC0_RESRDY_vect) {
    // can use ISR or just check flags
    // flag also cleared when reading result
    ADC0_INTFLAGS |= ADC_RESRDY_bm;
}
 */

/* ...or just empty */
EMPTY_INTERRUPT(ADC0_RESRDY_vect);

/* Disables digital input buffer on all pins to save (a lot of) power */
static void initPins(void) {
    // disable digital input buffer on all PORTA pins
    PORTA_PINCONFIG = PORT_ISC_INPUT_DISABLE_gc;
    // copy pin config to all pins
    PORTA_PINCTRLUPD = 0xff;
    PORTC_PINCTRLUPD = 0xff;
    PORTD_PINCTRLUPD = 0xff;
    PORTF_PINCTRLUPD = 0xff;

    // PA5 powers the thermistor
    PORTA_DIRSET |= (1 << PA5);
}

/* Sets CPU and peripherals clock speed */
static void initClock(void) {
    // allow writing to protected register
    CPU_CCP = CCP_IOREG_gc;
    // enable and set main clock prescaler to DIV2 = 10 MHz
    CLKCTRL_MCLKCTRLB = CLKCTRL_PDIV_DIV2_gc | CLKCTRL_PEN_bm;
    // set timebase used for timing internal delays
    CLKCTRL_MCLKTIMEBASE |= (TIMEBASE_VALUE << CLKCTRL_TIMEBASE_gp);
}

/* Initializes the Periodic Interrupt Timer */
static void initRTC(void) {
    // clock RTC with 32.768 kHz internal osciallator
    RTC_CLKSEL |= RTC_CLKSEL_OSC32K_gc;
    // enable periodic interrupt
    RTC_PITINTCTRL |= RTC_PI_bm;
    // wait for PITCTRLA to be synchronized
    loop_until_bit_is_clear(RTC_PITSTATUS, RTC_CTRLBUSY_bp);
    // set periodic interrupt period in RTC clock cycles, enable PIT
    RTC_PITCTRLA |= RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;
}

/* Initializes Timer/Counter Type A 0 */
static void initTimer(void) {
    // set timer period/TOP value
    TCA0_SINGLE_PER = F_CPU / 1024;
    // timer clock select, enable timer
    TCA0_SINGLE_CTRLA |= TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;
    // enable overflow/underflow interrupt
    TCA0_SINGLE_INTCTRL |= (1 << TCA_SINGLE_OVF_bp);
    // keep on running in standby sleep mode
    TCA0_SINGLE_CTRLA |= TCA_SINGLE_RUNSTDBY_bm;
}

/* Initializes the ADC */
static void initADC(void) {
    // enable ADC0
    ADC0_CTRLA |= ADC_ENABLE_bm;
    // set ADC clock prescaler
    ADC0_CTRLB |= ADC_PRESC_DIV6_gc;
    // use VDD as reference voltage
    ADC0_CTRLC |= ADC_REFSEL_VDD_gc;
    // set sample duration to 16.5 * CLK_ADC
    ADC0_CTRLE |= (16 << ADC_SAMPDUR_gp);
    // configure positive input to PA6/AIN26
    ADC0_MUXPOS |= ADC_MUXPOS_AIN26_gc;
    // configure single 12-bit mode of operation
    ADC0_COMMAND |= ADC_MODE_SINGLE_12BIT_gc;
    // enable result ready interrupt
    ADC0_INTCTRL |= ADC_RESRDY_bm;
}

/**
 * Starts an immediate conversion and returns the result.
 *
 * @return 12-bit conversion result
 */
static uint32_t convert(void) {
    // start an immediate conversion
    ADC0_COMMAND |= ADC_START_IMMEDIATE_gc;
    // wait for "result ready" interrupt flag
    do {} while (!(ADC0_INTFLAGS & ADC_RESRDY_bm));

    // return conversion result (clears flag)
    return ADC0_RESULT;
}

/**
 * Measures the temperature and returns it in °C * 10. Powers on the thermistor
 * before AD conversion and again off after, saving about 22µA during sleep mode.
 *
 * @return temperature in °C * 10
 */
static int16_t measure(void) {
    PORTA_OUTSET |= (1 << PA5);
    uint32_t adc = convert();
    PORTA_OUTCLR |= (1 << PA5);

    // resistance of the thermistor
    float resTh = (4096.0 / fmax(1, adc) - 1) * TH_SERI;
    // temperature in °C * 10
    float temp = 10.0 / (1.0 / TH_BETA * log(resTh / TH_RESI) + 1.0 / TH_TEMP) - TMP_0C * 10;

    return temp;
}

int main(void) {

    initPins();
    initClock();
    initRTC();
    // initTimer();
    initADC();
    initUSART();

    // enable global interrupts
    sei();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    printString("Hello AVR64EA!\r\n");
    char rev[16];
    snprintf(rev, sizeof (rev), "Rev. %c%d\r\n",
            (SYSCFG_REVID >> 4) - 1 + 'A',
            SYSCFG_REVID & SYSCFG_MINOR_gm);
    printString(rev);

    printString(rostr);

    while (true) {
        if (pitints % 3 == 0) {
            uint16_t temp = measure();
            div_t tmp = div(temp, 10);
            char buf[18];
            snprintf(buf, sizeof (buf), "%4d.%d°C\r\n", tmp.quot, abs(tmp.rem));
            printString(buf);
            wait_usart_tx_done();
        }

        // save some power
        sleep_mode();
    }

    return 0;
}
