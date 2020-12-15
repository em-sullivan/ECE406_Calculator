/*
 * Eric Sullivan and Elizabeth Willard
 * 09/06/2020
 * Keypad control
 */

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "keypad.h"
#include "timer.h"

/* GPIO Pins for rows  (GPIOC)*/
uint16_t keypad_rows[] = {GPIO_PIN_10, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9};

/* GPIO Pins for cols (GPIOC) */
uint16_t keypad_cols[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};

void init_keypad_pins()
{
    GPIO_InitTypeDef gpio_backlight;
    GPIO_InitTypeDef gpio_col;
    uint8_t pin;

    // Set pin to control backlight
    gpio_backlight.Pin = BACKLIGHT_GPIO;

    // Configure row pins to be digital output
    gpio_backlight.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_backlight.Pull = GPIO_NOPULL;
    gpio_backlight.Speed = GPIO_SPEED_FREQ_HIGH;

    // Set pins to be cols
    gpio_col.Pin = 0;
    for (pin = 0; pin < KEYPAD_COL_SIZE; pin++)
        gpio_col.Pin |= keypad_cols[pin];

    // Configure col pins to be digital input
    gpio_col.Mode = GPIO_MODE_INPUT;
    gpio_col.Pull = GPIO_PULLUP;
    gpio_col.Speed = GPIO_SPEED_FREQ_HIGH;

    // Enable clock, and pins for columns and backlight
    __GPIOC_CLK_ENABLE();
    HAL_GPIO_Init(GPIOC, &gpio_backlight);
    HAL_GPIO_Init(GPIOC, &gpio_col);
}

uint8_t keypad_scan()
{
    uint8_t row, col, pin;
    GPIO_InitTypeDef output = {0};
    GPIO_InitTypeDef input = {0};

    // For setting rows as outputs
    output.Mode = GPIO_MODE_OUTPUT_PP;
    output.Pull = GPIO_NOPULL;
    output.Speed = GPIO_SPEED_FREQ_HIGH;

    // For setting rows as inputs
    input.Mode = GPIO_MODE_INPUT;
    input.Pull = GPIO_NOPULL;
    input.Speed = GPIO_SPEED_FREQ_HIGH;

    // Set All rows as outputs
    output.Pin = 0;
    for (pin = 0; pin < KEYPAD_ROW_SIZE; pin++) {
        output.Pin |= keypad_rows[pin];
    }
    HAL_GPIO_Init(GPIOC, &output);

    // Read to see if and input is high
    if ((GPIOC->IDR & (keypad_cols[0] | keypad_cols[1] | keypad_cols[2] | keypad_cols[3])) == (keypad_cols[0] | keypad_cols[1] | keypad_cols[2] | keypad_cols[3]))
        return 255;

    // Short delay before setting rows
    udelay(40);
    
    // Cycles through the rows
    for (row = 0; row < KEYPAD_ROW_SIZE; row++) {
        
        // Sets current row to output, all other rows are inputs
        output.Pin = 0;
        input.Pin = 0;
        for (pin = 0; pin < KEYPAD_ROW_SIZE; pin++) {
            // Sets row being checked to output
            if (pin == row)
                output.Pin |= keypad_rows[pin];
            else
                input.Pin |= keypad_rows[pin];
        }
        HAL_GPIO_Init(GPIOC, &output);
        HAL_GPIO_Init(GPIOC, &input);
        udelay(40);

        // Checks if an input is being read (if and col pin is low)
        if ((GPIOC->IDR & (keypad_cols[0] | keypad_cols[1] | keypad_cols[2] | keypad_cols[3])) != (keypad_cols[0] | keypad_cols[1] | keypad_cols[2] | keypad_cols[3])) {
            
            // Finds which column it is
            for (col = 0; col < KEYPAD_COL_SIZE; col++) {
                if ((GPIOC->IDR & keypad_cols[col]) == 0) {
                    return 4 * row + col;
                }
            }
        } 
    }

    return 255;
}

uint8_t map_key(uint8_t key, uint8_t func)
{
    // Map of keys for keypad
    uint8_t mapped_keys[2][24] = {
    {'s', 'm', 'd', 'c',
    'l', '(', ')', '/',
    '1', '2', '3', '+',
    '4', '5', '6', '-',
    '7', '8', '9', '*',
    '.', '0', '_', '='},
    // Keymap 2 has bit operators
    {'s', 'm', 'd', 'c',
    'l', '(', ')', '%', 
    'o', 'x', 'b', '&',
    'A', 'B', 'C', '|',
    'D', 'E', 'F', '^',
    '<', '0', '>', '='}};

    // When func is greater then 0 it chooses the 2nd
    // map for keys
    if (func > 0) {
        func = 1;
    }

    // If input key is out of range, return max value
    // else return mapped key
    if (key > 23)
        return 255;
    else
        return mapped_keys[func][key];
}


/*
 * Fuction to read key with debouncing
 * work in progress
 */
uint8_t read_key()
{
    uint16_t count;
    uint8_t current_read, last_read;

    count = 0;
    last_read = 255;
    current_read = 255;

    // Waits for 5 scans for debounce
    while (count < 5) {
        current_read = keypad_scan();
    	    if (current_read == last_read) {
                count++;
          } else {
                count = 0;
        }
    	
        last_read = current_read;
        udelay(20);
    }

    return last_read;
}