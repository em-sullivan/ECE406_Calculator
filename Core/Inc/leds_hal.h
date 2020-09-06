/*
 * Code for controling the 
 * Red and Green LEDs on the
 * STM32L4 Discovery board
 */

#ifndef LEDS_HAL_H
#define LEDS_HAL_H

#include <stdint.h>

// void System_Clock_Init(void);
void Red_LED_Init(void);
void Green_LED_Init(void);
void LED_Blink(uint8_t col, uint8_t times);
void SysTick_init(void);
void SysTick_Handler(void);

#endif // LEDS_HAL_H