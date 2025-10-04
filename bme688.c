/*
 * File:   bme688.h
 * Author: torsten.roemer@luniks.net
 *
 * Thanks to https://github.com/boschsensortec/BME68x_SensorAPI/tree/master/examples
 *
 * Created on 4. Oktober 2025, 20:31
 */

#include "bme688.h"

/**
 * Writes given data to given register(s).
 *
 * @param reg register of first register - data pair
 * @param data array with data of first register - data pair as first element,
 *   with remaining register - data pair following
 * @param len (number of register - data pairs) - 1
 * @param intfPtr
 * @return success
 */
static BME68X_INTF_RET_TYPE bme68xWrite(uint8_t reg,
                                        const uint8_t *data,
                                        uint32_t len,
                                        void *intfPtr) {
    PORTC_OUTCLR = (1 << PD2);
    transmit(reg | 0x7f);
    for (uint32_t i = 0; i < len; i++) {
        transmit(data[i]);
    }

    PORTC_OUTSET = (1 << PD2);

    // TODO
    return 0;
}

/**
 * Reads given data starting with given register auto-incrementing.
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
    transmit(reg | 0x80);
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

/**
 * Initializes the BME68x to use SPI interface.
 *
 * @param bme bme68x device descriptor
 * @param pin SPI chip select pin
 * @return success
 */
uint8_t initBME68xIntf(struct bme68x_dev *bme, uint8_t pin) {
    bme->intf = BME68X_SPI_INTF;
    bme->write = bme68xWrite;
    bme->read = bme68xRead;
    bme->delay_us = bme68xDelayUs;
    bme->intf_ptr = &pin;
    bme->amb_temp = 20;

    return 0;
}

void initBME68x() {
    struct bme68x_dev bme;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heater_conf;
    struct bme68x_data data;
    int8_t result;

    result = initBME68xIntf(&bme, PD2); // FIXME pin not used
    // TODO check result
    printInt(result);

    result = bme68x_init(&bme);
    // TODO check result
    printInt(result);

    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE;
    conf.os_hum = BME68X_OS_8X;
    conf.os_pres = BME68X_OS_8X;
    conf.os_temp = BME68X_OS_8X;
    result = bme68x_set_conf(&conf, &bme);
    // TODO check result
    printInt(result);

    heater_conf.enable = BME68X_ENABLE;
    heater_conf.heatr_temp = 300;
    heater_conf.heatr_dur = 100;
    result = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heater_conf, &bme);
    // TODO check result
    printInt(result);

    result = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
    // TODO check result
    printInt(result);

    uint32_t meas_dur = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) +
            (heater_conf.heatr_dur * 1000);
    bme.delay_us(meas_dur, bme.intf_ptr);

    uint8_t n_data;
    result = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_data, &bme);
    // TODO check result
    printInt(result);

    div_t tdiv = div(data.temperature, 100);

    char buf[70];
    snprintf(buf, sizeof (buf), "%c%d.%d°, %ld Pa, %ld %%, %ld Ohm, %d\r\n",
            data.temperature < 0 ? '-' : ' ', abs(tdiv.quot), abs(tdiv.rem),
            (uint32_t)data.pressure,
            (uint32_t)(data.humidity / 1000),
            (uint32_t)data.gas_resistance,
            data.status);
    printString(buf);
}
