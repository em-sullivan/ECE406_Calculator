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

void lcd_init_i2c()
{
     // I2C configuration
    lcd_handler.Instance = I2C1;
    //lcd_handler.Init.Timing = 0x20E06893;
    lcd_handler.Init.Timing = 0x00702991;
    lcd_handler.Init.OwnAddress1 = 0;
    lcd_handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    lcd_handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    lcd_handler.Init.OwnAddress2 = 0;
    lcd_handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    lcd_handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    lcd_handler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

    __HAL_RCC_I2C1_CLK_ENABLE();
    HAL_I2C_Init(&lcd_handler);
    if (HAL_I2C_IsDeviceReady(&lcd_handler, LCD_SLAVE, 3, 1000) != HAL_OK)
        while (1);
    
    // Enable Analog filter
    HAL_I2CEx_AnalogFilter_Config(&lcd_handler, I2C_ANALOGFILTER_ENABLED);
}

void lcd_init()
{
    // Enable pins for I2C
    lcd_init_i2c_pins();

    // Configure I2C1
    lcd_init_i2c();

    // Init LCD screen

    // Function set: 8-bit commands and two lines
    // Also set IS for next commands
    lcd_command(LCD_FUNC | LCD_FUNC_DL | LCD_FUNC_TWOL | LCD_FUNC_IS);
    udelay(30);

    // Set internal oscilator frequencey with 221Hz frame frequency
    // and 1/4 bias 
    lcd_command(LCD_OSC_FREQ | LCD_OSC_FREQ_BS | LCD_OSC_FREQ_F2);
    udelay(30);

    // Set contrast
    lcd_command(LCD_CONTRAST | 0x3);
    udelay(30);

    // Power/ICON/Constrast control : Set ICON display on
    // button two bits also set contrast
    lcd_command(LCD_ICON_SET | LCD_ICON_ION | LCD_ICON_BON | 0x01);
    udelay(30);

    // Set LCD Follower control
    lcd_command(LCD_FOLLW | 0xD);

    // Wait 300 ms
    mdelay(300);

    // Clear IS 
    lcd_command(LCD_FUNC);
    udelay(30);

    // Turn display on, include blinking cursor
    lcd_command(LCD_DISPLAY | LCD_DIS_ON | LCD_CURS_ON | LCD_BLINKS);

    // Set cursor direction - INC from left to right
    lcd_command(LCD_ENTRY | LCD_ENTRY_INC);
    
    // Clear screen and set cursor to home
    lcd_command(LCD_CLEAR);
    lcd_command(LCD_HOME);
}

void lcd_command(uint8_t byte)
{
    uint8_t buf[2];

    // Load buffer with command to send
    buf[0] = 0;
    buf[1] = byte;

    // Transmit data
    HAL_I2C_Master_Transmit(&lcd_handler, LCD_SLAVE, buf, 2, 1000);
    mdelay(1);
}

void lcd_write_char(uint8_t c)
{
    uint8_t buf[2];

    buf[0] = LCD_CONTROL_RS;
    buf[1] = c;

    HAL_I2C_Master_Transmit(&lcd_handler, LCD_SLAVE, buf, 2, 1000);
    udelay(20);
}

void lcd_print(char *string, ...)
{
    char new_string[17];
    va_list args;
    uint8_t i;

    va_start(args, string);
    vsnprintf(new_string, sizeof(new_string), string, args);
    va_end(args);

    for(i = 0; i < 16; i++) {
        if (new_string[i] == 0)
            break;
        lcd_write_char(new_string[i]);
    }
}

void lcd_shift(int8_t dir)
{
    uint8_t shift;
    if (dir > 0) {
        shift = LCD_SHIFT | LCD_RSHIFT;
    } else {
        shift = LCD_SHIFT | LCD_LSHIFT;
    }

    lcd_command(shift);
}

void lcd_del()
{
    // Shift cursot back one
    lcd_shift(-1);

    // Write blank char
    lcd_write_char(' ');

    // Shift cursor again so it is in the spot
    // of the deleted char
    lcd_shift(-1);
}