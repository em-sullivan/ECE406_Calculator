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
#define LCD_OSC_FREQ_F		0x07 // Adjust bits 0 - 2 for Freq

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

/* I2C Pins */
#define SCL_PIN GPIO_PIN_9
#define SDA_PIN GPIO_PIN_10

/*
 * Init pins for i2c
 */
void lcd_init_i2c_pins();

#endif // LCD_ST7032
