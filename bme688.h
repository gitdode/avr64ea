/* 
 * File:   bme688.h
 * Author: torsten.roemer@luniks.net
 * 
 * Thanks to https://github.com/boschsensortec/BME68x_SensorAPI/tree/master/examples
 *
 * Created on 4. Oktober 2025, 20:31
 */

#ifndef BME688_H
#define BME688_H

/* Allow passing a variable to _delay_* functions */
#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdlib.h>
#include <util/delay.h>

#include "pins.h"
#include "spi.h"
#include "usart.h"
#include "bme68x/bme68x.h"

/**
 * Initializes the BME68x sensor using the given pin for SPI chip select.
 * 
 * @param pin SPI chip select pin
 */
void initBME68x(void);

#undef __DELAY_BACKWARD_COMPATIBLE__

#endif /* BME688_H */

