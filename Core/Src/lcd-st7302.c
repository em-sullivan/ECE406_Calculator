/*
 * Eric Sullivan and Elizabeth Willard
 * 10/10/2020
 *
 * Header for controlling LCD screen
 * with st7032 controller through I2C
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "stm32l4xx_hal.h"
#include "lcd-st7302.h"
#include "timer.h"

static I2C_HandleTypeDef lcd_handler;

void lcd_init_i2c_pins()
{
    GPIO_InitTypeDef i2c_pins;

    // Configures pins for i2c - alternate function
    i2c_pins.Pin = SDA_PIN | SCL_PIN;
    i2c_pins.Mode = GPIO_MODE_AF_OD;
    i2c_pins.Pull = GPIO_PULLUP;
    i2c_pins.Speed = GPIO_SPEED_FREQ_HIGH;
    i2c_pins.Alternate = GPIO_AF4_I2C1;

    // Enables GPIOB clock and pins
    __GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &i2c_pins);
}
}