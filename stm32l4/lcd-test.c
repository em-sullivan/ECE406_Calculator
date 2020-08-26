/*
 * Eric Sullivan and Elizabeth Willard
 * Test file for i2c LCD
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "leds_hal.h"
#include "timer.h"
#include "lcd.h"
#include "stm32l4xx_hal.h"
#include "../calc.h"

void System_Clock_Init(void);

int main()
{
    char exp[] = "26/2+3-(29*2)";
    char ans[20];
    int res;

    System_Clock_Init();
    timer_init();
    Red_LED_Init();
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    lcd_init();
   
    lcd_print("%s", exp);
    infix_to_postfix(exp, ans);
    res = eval_postfix(ans);
    sprintf(ans, "%d", res);
    lcd_command(LCD_SET_RAM | LCD_LINE2);
    lcd_print("%d", res);
    while (1);

}
void System_Clock_Init(void)
{
    RCC_OscInitTypeDef Osc = {0};
    RCC_ClkInitTypeDef Clk = {0};
    RCC_PeriphCLKInitTypeDef Per = {0};

    HAL_RCC_DeInit();
    /* Initialize Oscilation */
    Osc.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    Osc.MSIState = RCC_MSI_ON;
    Osc.MSIClockRange = RCC_MSIRANGE_11; // 48 Mhz
    if (HAL_RCC_OscConfig(&Osc) != HAL_OK) {
        // Initilization error, wait forever
        while (1);
    }

    /* Initializse Clock */
    Clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | 
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 ;
    Clk.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    Clk.AHBCLKDivider = RCC_SYSCLK_DIV1; // SYSCLK not divided
    Clk.APB1CLKDivider = RCC_HCLK_DIV1;
    Clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&Clk, FLASH_LATENCY_4) != HAL_OK) {
        // Initilizatione error, wait forever
        while (1);
    }
    Per.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    Per.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&Per) != HAL_OK) {
        while (1);
    }

}
