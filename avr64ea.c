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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "utils.h"
#include "usart.h"

/* Timer/Counter Type A 0 interrupts */
static volatile uint32_t tca0ints = 0;

/* Timer/Counter Type A 0 overflow/underflow interrupt */
ISR(TCA0_OVF_vect) {
    // generated about once per second, clear flag or it remains active
    TCA0_SINGLE_INTFLAGS |= (1 << TCA_SINGLE_OVF_bp);
    tca0ints++;
}

/* Sets CPU and peripherals clock speed */
static void initClock(void) {
    // allow writing to protected register
    CPU_CCP = CCP_IOREG_gc;
    // enable and set main clock prescaler to DIV2 = 10 MHz
    CLKCTRL_MCLKCTRLB = CLKCTRL_PDIV_DIV2_gc | CLKCTRL_PEN_bm;
}

/* Initializes Timer/Counter Type A 0 */
static void initTimer(void) {
    // set timer period/TOP value
    TCA0_SINGLE_PER = 9766;
    // timer clock select, enable timer
    TCA0_SINGLE_CTRLA |= TCA_SINGLE_CLKSEL_DIV1024_gc | (1 << TCA_SINGLE_ENABLE_bp);
    // enable overflow/underflow interrupt
    TCA0_SINGLE_INTCTRL |= (1 << TCA_SINGLE_OVF_bp);
}

int main(void) {

    initClock();
    initTimer();
    initUSART();

    // enable global interrupts
    sei();

    while (true) {
        if (tca0ints > 0) {
            tca0ints = 0;
            printString("Hello AVR64EA!\r\n");
        }
    }

    return 0;
}
