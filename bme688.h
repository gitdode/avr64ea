/* 
 * File:   bme688.h
 * Author: torsten.roemer@luniks.net
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
 * Initializes the BME68x sensor.
 * 
 * @return error code
 */
int8_t initBME68x(void);

#undef __DELAY_BACKWARD_COMPATIBLE__

#endif /* BME688_H */

