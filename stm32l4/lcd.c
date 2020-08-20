/*
 * Eric Sullivan and Elizabeth Willard
 * Functions for controlling LCD
 */

#include <stdlib.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "lcd.h"
#include "timer.h"

// I2C hadler for LCD
static I2C_HandleTypeDef lcd_handler;

void lcd_init_i2c_pins()
{
    GPIO_InitTypeDef i2c_pins;

    // Configures pins for i2c - alternate function
    i2c_pins.Pin = SDA_PIN | SCL_PIN;
    i2c_pins.Mode = GPIO_MODE_AF_OD;
    i2c_pins.Pull = GPIO_PULLUP;
    i2c_pins.Speed = GPIO_SPEED_FAST;
    i2c_pins.Alternate = GPIO_AF4_I2C1;

    // Enables GPIOB clock and pins
    __GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &i2c_pins);
}

void lcd_init_i2c()
{
    // I2C configuration
    lcd_handler.Instance = I2C1;
    lcd_handler.Init.Timing = 0x2000090E; // Fix this value later
    lcd_handler.Init.OwnAddress1 = 0;
    lcd_handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    lcd_handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    lcd_handler.Init.OwnAddress2 = 0;
    lcd_handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    lcd_handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    lcd_handler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

    //__I2C1_CLK_ENABLE();
    HAL_I2C_Init(&lcd_handler);
    if (HAL_I2C_IsDeviceReady(&lcd_handler, 0x3F << 1, 3, 1000) != HAL_OK)
        while (1);
}

void lcd_init()
{
    __I2C1_CLK_ENABLE();
    lcd_init_i2c_pins();
    lcd_init_i2c();

     // Enable 8-bit mode 3 times, then 4 bit mode
    write_nibble(0b00110000);
    udelay(4100);
    write_nibble(0b00110000);
    udelay(100);
    write_nibble(0b00110000);
    udelay(4100);
    write_nibble(0b00100000);

    // 4-bit mode, 5b8 characters
    lcd_command(LCD_FUNC | LCD_TWOL | LCD_5B8);

    // Turn of display
    lcd_command(LCD_DISPLAY);

    // Set cursor direction - INC from left to right
    lcd_command(LCD_ENTRY | LCD_ENTRY_INC);

    lcd_command(LCD_CLEAR);
    // Turn on display, cursor on, cursor blinks
    lcd_command(LCD_DISPLAY | LCD_DIS_ON | LCD_CURS_ON | LCD_BLINKS);
}

void write_nibble(uint8_t byte)
{
    uint8_t buf[2];
    buf[0] = byte | LCD_EN | LCD_BL;
    buf[1] = byte | LCD_BL;

    HAL_I2C_Master_Transmit(&lcd_handler, LCD_SLAVE << 1, &buf[0], 1, 1);
    mdelay(2);
    HAL_I2C_Master_Transmit(&lcd_handler, LCD_SLAVE << 1, &buf[1], 1, 1);
}

void lcd_command(uint8_t byte)
{
    uint8_t upper_nibble = (byte & 0xF0);
    uint8_t lower_nibble = ((byte << 4) & 0xF0);

    write_nibble(upper_nibble);
    write_nibble(lower_nibble);
    udelay(20);
}

void lcd_write_char(uint8_t c)
{
    uint8_t upper_nibble = (c & 0xF0) | LCD_RS;
    uint8_t lower_nibble = ((c << 4) & 0xF0) | LCD_RS;

    write_nibble(upper_nibble);
    write_nibble(lower_nibble);
    udelay(20);
}

void lcd_print(char *string)
{
    char *pt;
    pt = string;

    for (; *pt != '\0'; pt++)
        lcd_write_char(*pt);
}
