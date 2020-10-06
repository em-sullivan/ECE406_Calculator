/*
 * Eric Sullivan and Elizabeth Willard
 * Functions for controlling LCD
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "stm32l4xx_hal.h"
#include "lcd-i2c.h"
#include "timer.h"

// I2C handler for LCD
static I2C_HandleTypeDef lcd_handler;
DMA_HandleTypeDef i2c_dma_tx;
DMA_HandleTypeDef i2c_dma_rx;

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
    __GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &i2c_pins);
}
/* DOES NOT WORK */
void lcd_init_dma()
{
    // Enable clock for DMA1
    __HAL_RCC_DMA1_CLK_ENABLE();

    // Init DMA I2C1_TX: Transfer data
    i2c_dma_tx.Instance = DMA1_Channel6;
    i2c_dma_tx.Init.Request = DMA_REQUEST_3;
    i2c_dma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    i2c_dma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    i2c_dma_tx.Init.MemInc = DMA_MINC_ENABLE;
    i2c_dma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    i2c_dma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    i2c_dma_tx.Init.Mode = DMA_NORMAL;
    i2c_dma_tx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&i2c_dma_tx) != HAL_OK) {
        while(1);
    }

    // Link to i2c handler
    __HAL_LINKDMA(&lcd_handler, hdmatx, i2c_dma_tx);

    // Init DMA I2C1_RX: Receive data
    i2c_dma_rx.Instance = DMA1_Channel7;
    i2c_dma_rx.Init.Request = DMA_REQUEST_3;
    i2c_dma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    i2c_dma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    i2c_dma_rx.Init.MemInc = DMA_MINC_ENABLE;
    i2c_dma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    i2c_dma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    i2c_dma_rx.Init.Mode = DMA_NORMAL;
    i2c_dma_rx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&i2c_dma_rx) != HAL_OK) {
        while (1);
    }

    // Link to i2c handler
    __HAL_LINKDMA(&lcd_handler, hdmarx, i2c_dma_rx);

    // Set up I2C interrupts
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    // Enable interupt for DMA
    HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void lcd_init_i2c()
{
    // I2C configuration
    lcd_handler.Instance = I2C1;
    lcd_handler.Init.Timing = 0x20E06893;
    //lcd_handler.Init.Timing =  0x10909CEC;
    lcd_handler.Init.OwnAddress1 = 0;
    lcd_handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    lcd_handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    lcd_handler.Init.OwnAddress2 = 0;
    lcd_handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    lcd_handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    lcd_handler.Init.NoStretchMode = I2C_NOSTRETCH_ENABLED;

    __HAL_RCC_I2C1_CLK_ENABLE();
    HAL_I2C_Init(&lcd_handler);
    if (HAL_I2C_IsDeviceReady(&lcd_handler, LCD_SLAVE, 10, 1000) != HAL_OK)
        while (1);
    
    HAL_I2CEx_AnalogFilter_Config(&lcd_handler, I2C_ANALOGFILTER_ENABLED);

}

void lcd_init()
{
    // Enable pins and I2C
    lcd_init_i2c_pins();
    //lcd_init_dma();
    lcd_init_i2c();
    
    mdelay(50);

    // Enable 8-bit mode 3 times, then 4 bit mode
    lcd_command(0b00110000);
    mdelay(5);
    lcd_command(0b00110000);
    mdelay(1);
    lcd_command(0b00110000);
    mdelay(5);
    lcd_command(0b00100000);

    // 4-bit mode, 5b8 characters
    lcd_command(LCD_FUNC | LCD_TWOL | LCD_5B8);

    // Turn of display
    lcd_command(LCD_DISPLAY);

    // Set cursor direction - INC from left to right
    lcd_command(LCD_ENTRY | LCD_ENTRY_INC);

    // Clear screen
    lcd_command(LCD_CLEAR);
    
    // Turn on display, cursor on, cursor blinks
    lcd_command(LCD_DISPLAY | LCD_DIS_ON | LCD_CURS_ON | LCD_BLINKS);
}

void write_byte(uint8_t byte, uint8_t cw)
{
    uint8_t buf[6];
    uint8_t i;

    // Upper nibble
    buf[0] = (byte & 0xF0) | LCD_EN | LCD_BL;
    buf[1] = buf[0];
    buf[2] = (byte & 0xF0) | LCD_BL;

    // Lower nibble
    buf[3] = ((byte << 4) & 0xF0) | LCD_EN | LCD_BL;
    buf[4] = buf[3];
    buf[5] = ((byte << 4) & 0xF0) | LCD_BL;

    // Determines if writing character to screen
    if (cw != 0) {
        for (i = 0; i < 6; i++)
            // Sets bit for writing character data
            buf[i] |= LCD_RS;
    }

    HAL_I2C_Master_Transmit(&lcd_handler, LCD_SLAVE, buf, 6, 1000);
    //HAL_I2C_Master_Transmit_DMA(&lcd_handler, LCD_SLAVE << 1, buf, 6);
    mdelay(2);
}

void lcd_command(uint8_t byte)
{
    // Send command to LCD
    write_byte(byte, 0);
    udelay(20);
}

void lcd_write_char(uint8_t c)
{
    // Write char to screen
    write_byte(c, 1);
    udelay(20);
}

void lcd_print(char *string, ...)
{
    char new_string[17];
    va_list args;
    uint8_t i;

    // Handles added arguments
    va_start(args, string);
    vsnprintf(new_string, sizeof(new_string), string, args);
    va_end(args);

    // Print each character (Max lenght is 16 chars)
    for (i = 0; i < strlen(new_string); i++)
        lcd_write_char(new_string[i]);
}

void lcd_del()
{
	// Shift cursor back one
	lcd_command(LCD_SHIFT | LCD_LSHIFT);

	// Write blank char
	lcd_write_char(' ');

	// Shift cursor again so its in the spot of the
	// deleted char
	lcd_command(LCD_SHIFT | LCD_LSHIFT);
}

void lcd_print_int_mode(int val, uint8_t mode)
{
    switch(mode) {
        case 0:
            lcd_print("%o", val);
            break;
        case 1:
            lcd_print("%X", val);
            break;
        case 2:
            lcd_print_int_binary(val);
            break;
        default:
            lcd_print("%d", val);
    }
}

void lcd_print_int_binary(int val)
{
    uint8_t mask_shift;
    char binary_num[33];

    // Shifts through each bit and adds it to
    // binary string
    for (mask_shift = 0; mask_shift < 32; mask_shift++) {
        if ((0x80000000U >> mask_shift) & val) {
            binary_num[mask_shift] = '1';
        } else {
            binary_num[mask_shift] = '0';
        }
    }

    if (val > 0) {
        // Print Only lower 16-bits of number if it is small enough
        lcd_print("%s", &binary_num[16]);
    } else {
        // Print full 32-bit number: Uses both rows of LCD screen
        lcd_command(LCD_CLEAR);
        lcd_print("%s", binary_num);
        lcd_command(LCD_SET_RAM | LCD_LINE2);
        lcd_print("%s", &binary_num[16]);
    }
}
