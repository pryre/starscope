#ifndef _STARSCOPE_CONFIG_H
#define _STARSCOPE_CONFIG_H

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define STARSCOPE_I2C_HARDWARE i2c1
#define STARSCOPE_PIN_I2C_SDA 6
#define STARSCOPE_PIN_I2C_SCL 7
#define STARSCOPE_PIN_LED PICO_DEFAULT_LED_PIN

#endif
