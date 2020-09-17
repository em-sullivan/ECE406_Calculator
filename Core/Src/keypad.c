/*
 * Eric Sullivan and Elizabeth Willard
 * 09/06/2020
 * Keypad control
 */

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "keypad.h"
#include "timer.h"

/* GPIO Pins for rows  (GPIOA)*/
uint16_t keypad_rows[] = {GPIO_PIN_0, GPIO_PIN_5};

/* GPIO Pins for cols (GPIOA) */
uint16_t keypad_cols[] = {GPIO_PIN_1, GPIO_PIN_2};

void init_keypad_pins()
{
    GPIO_InitTypeDef gpio_row;
    GPIO_InitTypeDef gpio_col;
    uint8_t i;

    // Set pins to be rows
    gpio_row.Pin = 0;
    for(i = 0; i < KEYPAD_ROW_SIZE; i++)
        gpio_row.Pin |= keypad_rows[i];

    // Configure row pins to be digital output
    gpio_row.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_row.Pull = GPIO_NOPULL;
    gpio_row.Speed = GPIO_SPEED_FREQ_HIGH;

    // Set pins to be cols
    gpio_col.Pin = 0;
    for (i = 0; i < KEYPAD_COL_SIZE; i++)
        gpio_col.Pin |= keypad_cols[i];

    // Configure col pins to be digital input
    gpio_col.Mode = GPIO_MODE_INPUT;
    gpio_col.Pull = GPIO_NOPULL;
    gpio_row.Speed = GPIO_SPEED_FREQ_HIGH;

    __GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &gpio_row);
    HAL_GPIO_Init(GPIOA, &gpio_col);
}

uint8_t keypad_scan()
{
    uint8_t row, col;

    // Set output rows to 0
    GPIOA->ODR &= ~(GPIO_PIN_0 | GPIO_PIN_5);

    // Read to see if and input is high
    if ((GPIOA->IDR & (GPIO_PIN_1 | GPIO_PIN_2)) == (GPIO_PIN_1 | GPIO_PIN_2))
        return 5;

    return 0;

    udelay(5);
}