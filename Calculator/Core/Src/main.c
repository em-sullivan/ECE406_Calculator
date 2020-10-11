/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "lcd-i2c.h"
#include "keypad.h"
#include "calc.h"

/* Private function prototypes -----------------------------------------------*/
void system_clock_init(void);
void LED_Init(void);

/*
 * Entry Point
 */
int main(void)
{
    uint8_t key;
    char exp_buffer[16];
    char post_fix[20];
    char *exp_p;
    int res;

    // Set expression pointer to buffer
    exp_p = exp_buffer;

    // Init System Clock
    system_clock_init();

    // Timer Init
    timer_init();

    // Init test LED
    LED_Init();

    // Init LCD
    lcd_init();

    // Init keypad
    init_keypad_pins();

    // Test LED that turns on when everything is set up correctly
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);

    while (1) {
        // Wait for key to be pressed
        while(keypad_scan() != 255);
        while(keypad_scan() == 255);

        // Scan key
        key = map_key(keypad_scan());

        switch(key) {
            case 'c':
            	// Clear screen
            	lcd_command(LCD_CLEAR);

            	// Clear expression buffer
            	memset(exp_buffer, 0, sizeof(exp_buffer));

            	// Set expression pointer back to buffer
            	exp_p = exp_buffer;
            	break;
            case 'd':
            	// Delete char from screen and buffer
            	lcd_del();
            	*exp_p = 0;
            	exp_p--;
            	break;

            case '=':
            	// Calculate expression and print answer on next line
            	infix_to_postfix(exp_buffer, post_fix);
            	res = eval_postfix(post_fix);
            	lcd_command(LCD_SET_RAM | LCD_LINE2);
            	lcd_print("%d", res);

            case 255:
            	break;
            default:
            	// Checks if expression if less then 16 chars
            	if (exp_p - exp_buffer < 16) {
            		// Write char to screen, save it to expression buffer
            	    lcd_write_char(key);
            	    *exp_p = key;

            	    // Move to next char of buffer
            	    exp_p++;
            	    // Set null character to end (current) end of buffer
            	    *exp_p = '\0';
            	}
        }
    }
}

/*
 * Initialize System Clock
 * (Need to work on error handling for this)
 */
void system_clock_init(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    RCC_PeriphCLKInitTypeDef per = {0};

    // Inits osc to run at 80 MHz with HSI and PLL
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc.HSIState = RCC_HSI_ON;
    osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    osc.PLL.PLLState = RCC_PLL_ON;
    osc.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    osc.PLL.PLLM = 1;
    osc.PLL.PLLN = 10;
    osc.PLL.PLLP = RCC_PLLP_DIV7;
    osc.PLL.PLLQ = RCC_PLLQ_DIV2;
    osc.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
        // Put error handler here later
        while (1);
    }

    // Init CPU clock, AHB, APB1 and APB2
    clk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV1;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;

    // NOTE: Does not seem to return HAL_OK, need to look into
    // that
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_4) != HAL_OK) {
        //while (1);
    }

    // Init Periph clock for I2C
    per.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    per.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&per) != HAL_OK) {
        while (1);
    }

    // Configure the main internal regulator output voltage
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        //Error_Handler();
    }
}

/*
 * Init Green LED on Nucleo borad.
 * For testing purposes
 */
void LED_Init(void)
{
    // Init PB13 for Green LED
    GPIO_InitTypeDef green;

    // Enable Clock for GPIOB
    __GPIOB_CLK_ENABLE();

    green.Pin = GPIO_PIN_13;
    green.Mode = GPIO_MODE_OUTPUT_PP; // Output, push-pull
    green.Speed = GPIO_SPEED_FREQ_MEDIUM; // Freq range for 5 MHz to 25 MHz
    green.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &green);
}
