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
#define KEYPAD_ROW_SIZE 2
#define KEYPAD_COL_SIZE 2


void init_keypad_pins();
uint8_t keypad_scan();

#endif // KEYPAD_H