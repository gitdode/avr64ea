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

static void initClock(void) {
    // allow writing to protected register
    CPU_CCP = CCP_IOREG_gc;
    // enable and set main clock prescaler to DIV2 = 10 MHz
    CLKCTRL_MCLKCTRLB = CLKCTRL_PDIV_DIV2_gc | 0x01;
}

int main(void) {

    initClock();
    initUSART();

    // enable global interrupts
    sei();

    while (true) {
        // do something
        printString("Hello AVR64EA!\r\n");
        _delay_ms(1000);
    }

    return 0;
}
