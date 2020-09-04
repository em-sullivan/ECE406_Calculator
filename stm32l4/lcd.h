/*
 * Eric Sullivan and Elizabeth Willard
 * Header for controlling LCD
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/* Slave Adress */
#define LCD_SLAVE           0x3F

/* Bits */
#define LCD_RS              0x01
#define LCD_RW              0x02
#define LCD_EN              0x04
#define LCD_BL              0x08

/* Basic LCD Commands */
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02

/* LCD Entry Mode */
#define LCD_ENTRY           0x04
#define LCD_ENTRY_INC       0x02
#define LCD_ENTRY_SHIFT     0x01

/* LCD Display Control */
#define LCD_DISPLAY         0x08
#define LCD_DIS_ON          0x04
#define LCD_CURS_ON         0x02
#define LCD_BLINKS          0x01

/* LCD Cursor/Display Shift */
#define LCD_SHIFT           0x10
#define LCD_LSHIFT          0x00 // Deafult shift
#define LCD_RSHIFT          0x04
#define LCD_LSHIFT_DIS      0x08
#define LCD_RSHIFT_DIS      0x0C

/* LCD Function Set */
#define LCD_FUNC            0x20
#define LCD_FUNC_DL         0x10 // 1 = 8-bit, 0 = 4-bit
#define LCD_TWOL            0x08
#define LCD_ONEL            0x00
#define LCD_5B8             0x00
#define LCD_5B10            0x04

/* LCD Set Display Data RAM */
#define LCD_SET_RAM         0x80
#define LCD_LINE2           0x40

/* i2c pins */
#define SCL_PIN GPIO_PIN_6
#define SDA_PIN GPIO_PIN_7

void lcd_init_i2c_pins();
void lcd_init_dma();
void lcd_init_i2c();
void lcd_init();

void write_nibble(uint8_t byte);
void write_byte(uint8_t byte, uint8_t cw);
void lcd_command(uint8_t byte);
void lcd_write_char(uint8_t c);
void lcd_print(char *string, ...);
void lcd_print_int_mode(int val, uint8_t mode);
void lcd_print_int_binary(int val);


#endif // LCD_H
