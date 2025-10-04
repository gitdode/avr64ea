/*
 * File:   bme688.c
 * Author: torsten.roemer@luniks.net
 *
 * Thanks to https://github.com/boschsensortec/BME68x_SensorAPI/tree/master/examples
 *
 * Created on 4. Oktober 2025, 20:31
 */

#include "bme688.h"

/**
 * Writes given register - data pairs to respective register(s).
 *
 * @param reg register of first register - data pair
 * @param data array with data of first register - data pair as first element,
 *   with remaining register - data pairs following
 * @param len (number of register - data pairs) - 1
 * @param intfPtr
 * @return success
 */
static BME68X_INTF_RET_TYPE bme68xWrite(uint8_t reg,
                                        const uint8_t *data,
                                        uint32_t len,
                                        void *intfPtr) {
    PORTC_OUTCLR = (1 << PD2);
    transmit(reg);
    for (uint32_t i = 0; i < len; i++) {
        transmit(data[i]);
    }
    PORTC_OUTSET = (1 << PD2);

    // TODO
    return 0;
}

/**
 * Reads from register(s) into given data array starting with
 * given register auto-incrementing.
 *
 * @param reg start register
 * @param data array with data to be read from consecutive registers
 * @param len number of registers to read from
 * @param intfPtr
 * @return success
 */
static BME68X_INTF_RET_TYPE bme68xRead(uint8_t reg,
                                       uint8_t *data,
                                       uint32_t len,
                                       void *intfPtr) {
    PORTC_OUTCLR = (1 << PD2);
    transmit(reg);
    for (uint32_t i = 0; i < len; i++) {
        data[i] = transmit(0x00);
    }
    PORTC_OUTSET = (1 << PD2);

    // TODO
    return 0;
}

/**
 * Delay function for BME68x - requires __DELAY_BACKWARD_COMPATIBLE__
 * so delay does not have to be a compile time constant.
 *
 * @param period
 * @param intfPtr
 */
static void bme68xDelayUs(uint32_t period, void *intfPtr) {
    _delay_us(period);
}

int8_t initBME68x() {
    struct bme68x_dev dev;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heater_conf;
    struct bme68x_data data;
    int8_t result;

    uint8_t pin = PD2;

    dev.intf = BME68X_SPI_INTF;
    dev.write = bme68xWrite;
    dev.read = bme68xRead;
    dev.delay_us = bme68xDelayUs;
    dev.intf_ptr = &pin; // TODO not used
    dev.amb_temp = 20; // could use temp measured with thermistor ;-)

    result = bme68x_init(&dev);
    // TODO properly check result
    if (result < 0) {
        return result;
    }

    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE;
    conf.os_hum = BME68X_OS_8X;
    conf.os_pres = BME68X_OS_8X;
    conf.os_temp = BME68X_OS_8X;
    result = bme68x_set_conf(&conf, &dev);
    // TODO properly check result
    if (result < 0) {
        return result;
    }

    heater_conf.enable = BME68X_ENABLE;
    heater_conf.heatr_temp = 300;
    heater_conf.heatr_dur = 100;
    result = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heater_conf, &dev);
    // TODO properly check result
    if (result < 0) {
        return result;
    }

    result = bme68x_set_op_mode(BME68X_FORCED_MODE, &dev);
    // TODO properly check result
    if (result < 0) {
        return result;
    }

    uint32_t meas_dur = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev) +
            (heater_conf.heatr_dur * 1000);
    dev.delay_us(meas_dur, dev.intf_ptr);

    uint8_t n_data;
    result = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_data, &dev);
    // TODO properly check result
    if (result < 0) {
        return result;
    }

    div_t tdiv = div(data.temperature, 100);

    char buf[70];
    snprintf(buf, sizeof (buf), "%c%d.%d°, %ld Pa, %ld %%, %ld Ohm, %d\r\n",
            data.temperature < 0 ? '-' : ' ', abs(tdiv.quot), abs(tdiv.rem),
            (uint32_t)data.pressure,
            (uint32_t)(data.humidity / 1000),
            (uint32_t)data.gas_resistance,
            data.status);
    printString(buf);

    return true;
}
