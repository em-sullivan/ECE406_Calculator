/*
 * Eric Sullivan and Elizabeth Willard
 * Header for controlling LCD
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/* Slave Address */
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

/*
 * Init pins for i2c
 */
void lcd_init_i2c_pins();
/*
 * Init DMA1 channel 6 and 7 for i2c
 * Note: Work in progress
 */
void lcd_init_dma();
/*
 * Init I2C1
 */
void lcd_init_i2c();

/*
 * Init LCD screen, with i2c, pins, and necessary commands
 * to start-up
 */
void lcd_init();

/*
 * Write half a byte to LCD
 * Note: Phasing this out
 */
void write_nibble(uint8_t byte);

/*
 * Write a byte to LCD screen. If cw is 0,
 * sends command to LCD. Else, sends a char
 * for the LCD to write
 */
void write_byte(uint8_t byte, uint8_t cw);

/*
 * Send a command to LCD screen
 */
void lcd_command(uint8_t byte);

/*
 * Write a char to the LCD screen
 */
void lcd_write_char(uint8_t c);

/*
 * Print a string to the LCD screen, prints to whichever
 * line the cursor is currently on
 */
void lcd_print(char *string, ...);

/*
 * Print integer val in different modes. Modes are:
 *  0: Octal
 *  1: Hexadecimale
 *  2: Binary
 *  else: Decimale
 */
void lcd_print_int_mode(int val, uint8_t mode);

/*
 * Print an integer number in its binary form. May print
 * on both lines of the LCD screen is large enough
 */
void lcd_print_int_binary(int val);


#endif // LCD_H
