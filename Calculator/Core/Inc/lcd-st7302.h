/*
 * Eric Sullivan and Elizabeth Willard
 * 10/10/2020
 *
 * Header for controlling LCD screen
 * with st7032 controller through I2C
 */

#ifndef LCD_ST7032_H
#define LCD_ST7032_H

#include <stdint.h>
#include <stdarg.h>

/* Slave Address */
#define LCD_SLAVE           0x3E << 1

/* Clear Display */
#define LCD_CLEAR           0x01

/* Return Home */
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

/* LCD Function Set */
#define LCD_FUNC            0x20
#define LCD_FUNC_DL         0x10 // 1 = 8-bit, 0 = 4-bit
#define LCD_FUNC_TWOL       0x08 // Two lines, left 0 for one line
#define LCD_FUNC_DH         0x04 // Double Height font
#define LCD_FUNC_IS			0x01 // Instruction table Select

/* LCD DDRAM */
#define LCD_SET_DDRAM       0x80
#define LCD_SET_DDRAM_LINE2 0x40

/* Instruction Table 0 (IS = 0) */
#define LCD_SHIFT           0x10
#define LCD_LSHIFT          0x00 // Deafult shift
#define LCD_RSHIFT          0x04
#define LCD_LSHIFT_DIS      0x08
#define LCD_RSHIFT_DIS      0x0C
#define LCD_CGRAM           0x40

/* Instruction Table 1 (IS = 1) */

/* LCD Internal Oscillator */
#define LCD_OSC_FREQ        0x10
#define LCD_OSC_FREQ_BS     0x08 //BS = 1/4 bias, 0 = 1/5
#define LCD_OSC_FREQ_F2		0x04 // Adjust bits 0 - 2 for Freq
#define LCD_OSC_FREQ_F1     0x02
#define LCD_OSC_FREQ_F0

/* LCD ICON Address */
#define LCD_ICON_ADDR       0x40 // Lower 4 bits sets ICON address

/* LCD Power ICON Control */
#define LCD_ICON_SET        0x50
#define LCD_ICON_ION        0x08 // ICON display on/off
#define LCD_ICON_BON		0x04 // Set booster circuit on/off
// Last two bits set Follower contrast

/* LCD Follower Control */
#define LCD_FOLLW           0x60
#define LCD_FOLLW_FON       0x08 // Set follower circuit on/off
// Last 3 bits select follower amplify ratio

/* LCD Contrast Set */
#define LCD_CONTRAST        0x70 // Set four lower bits for contrast

/* Control Byte bits */
#define LCD_CONTROL_CO      0x80
#define LCD_CONTROL_RS      0x40

/* I2C Pins */
#define SCL_PIN             GPIO_PIN_9
#define SDA_PIN             GPIO_PIN_10

/* Modes to print values in */
enum modes { 
    OCT, HEX, BIN, DEC, FLOAT
};


/*
 * Init pins for i2c
 */
void lcd_init_i2c_pins();

/*
 * Init I2C1 protocol
 */
void lcd_init_i2c();

/*
 * Init LCD screen, with i2c, pins, and necessary commands
 * to start-up
 */
void lcd_init();

/*
 * Send a command to the LCD screen
 */
void lcd_command(uint8_t byte);

/*
 * Print a character to the LCD screen
 */
void lcd_write_char(uint8_t c);

/*
 * Print string to LCD screen. Prints where
 * the current position of the cursor is
 */
void lcd_print(char *string, ...);

/*
 * Load a custom character to CD-RAM, custom char is a
 * 8 by 8 char array, and address is the CD-RAM address to store
 * the custom character in.
 */
void lcd_custom_char(uint8_t *custom_char, uint8_t address);

/*
 * Shift LCD cursor to the right (if dir is poistive)
 * or left (if dir is negative)
 */
void lcd_shift(int8_t dir);

/*
 * Deletes character where cursor is currently pointing
 */
void lcd_del();

/*
 * Move cursor to any poisition on the LCD screen.
 * x is between 0 and 15, y determines the line its on.
 */
void lcd_set_cursor(uint8_t x, uint8_t y);

/*
 * Clear LCD screen and set cursor to the
 * starting position
 */
void lcd_clear();

/*
 * Prints integers in different forms
 * 0 = OCT
 * 1 = HEX
 * 2 = BINARY
 * 3 = Integer-based decimale
 * else = Floatingpoint based decimale
 *
 */
void lcd_print_int_mode(double val, int mode);

/*
 * Prints the binary base of an integer value.
 * If the number is small enough it will only brint
 * 16-bits, but will print 32 if the value is large (or
 * negative).
 */
void lcd_print_int_binary(int val);


#endif // LCD_ST7032
