/*
 * Eric Sullivan and Elizabeth Willard
 * 
 * Keypad
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

/* Size of Matrix */
#define KEYPAD_ROW_SIZE 6
#define KEYPAD_COL_SIZE 4

#define BACKLIGHT_GPIO GPIO_PIN_11

/*
 * Init GPIO pins needed for keypad. This includes
 * the input pins for the columns and the pin to control
 * the backlight.
 */
void init_keypad_pins();

/*
 * Scan through keypad matrix by setting row pins to outputs
 * and checking inputs to see if key has been pressed. Returns
 * an index for the keypad map (which is row * 4 + col)
 */
uint8_t keypad_scan();

/*
 * Takes keypad index from keypad_scan and returns character
 * from keypad map. The func variable determines which keypad map is
 * used.
 * Func = 0: Regular keypad map
 * Func = else: Secondary keypad map
 */
uint8_t map_key(uint8_t key, uint8_t func);

/*
 * Reads the current key being pressed using the keypad_scan function.
 * It waits for 5 conesectuive keypad scans (for debouncing) and returns
 * the result after getting consecutive scans
 */
uint8_t read_key();

#endif // KEYPAD_H
