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
uint16_t keypad_rows[] = {GPIO_PIN_9, GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_7};

/* GPIO Pins for cols (GPIOC) */
uint16_t keypad_cols[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};

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

    __GPIOC_CLK_ENABLE();
    HAL_GPIO_Init(GPIOC, &gpio_row);
    HAL_GPIO_Init(GPIOC, &gpio_col);
}

uint8_t keypad_scan()
{
    uint8_t row, col;

    // Set output rows to 0
    GPIOC->ODR &= ~(keypad_rows[0] | keypad_rows[1] | keypad_rows[2] | keypad_rows[3] | keypad_rows[4]);

    // Read to see if and input is high
    if ((GPIOC->IDR & (keypad_cols[0] | keypad_cols[1] | keypad_cols[2] | keypad_cols[3])) == (keypad_cols[0] | keypad_cols[1] | keypad_cols[2] | keypad_cols[3]))
        return 255;

    // Short delay before setting rows
    udelay(40);
    
    // Cycles through the rows
    for (row = 0; row < KEYPAD_ROW_SIZE; row++) {
        
        // Sets current row to 0, all other rows are high
        GPIOC->ODR |= (keypad_rows[0] | keypad_rows[1] | keypad_rows[2] | keypad_rows[3] | keypad_rows[4]);
        GPIOC->ODR &= ~(keypad_rows[row]);
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
    uint8_t mapped_keys[2][20] = {
    {'s', '(', ')', '/',
    '1', '2', '3', '+',
    '4', '5', '6', '-',
    '7', '8', '9', '*',
    '.', '0', '_', '='},
    {'s', 'm', 'd', 'c',
    '1', '2', '3', '&',
    '4', '5', '6', '|',
    '7', '8', '9', '^',
    '<', '0', '>', '='}};

    // When func is greater then 0 it chooses the 2nd
    // map for keys
    if (func > 0) {
        func = 1;
    }

    // If input key is out of range, return max value
    // else return mapped key
    if (key > 20)
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

