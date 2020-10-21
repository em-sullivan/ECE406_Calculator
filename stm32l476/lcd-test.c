/*
 * Eric Sullivan and Elizabeth Willard
 * Test file for i2c LCD
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "leds_hal.h"
#include "timer.h"
#include "lcd-i2c.h"
#include "stm32l4xx_hal.h"
#include "calc.h"

void system_clock_init(void);

int main()
{
    char exp[] = "1+2-3+9"; // Test expression
    char ans[20];
    int res;

    system_clock_init(); // 
    timer_init(); // Needed for the delays
    lcd_init();
   
    lcd_print("%s", exp); // Print expression
    infix_to_postfix(exp, ans); // Convert to postfix
    res = eval_postfix(ans); // Final result
    lcd_command(LCD_SET_RAM | LCD_LINE2);
    lcd_print_int_mode(res, 4);

    while (1);
}
void system_clock_init(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    RCC_PeriphCLKInitTypeDef per = {0};

    HAL_RCC_DeInit();
    /* Initialize Oscilation */
    osc.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    osc.MSIState = RCC_MSI_ON;
    osc.MSIClockRange = RCC_MSIRANGE_11; // 48 Mhz
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
        // Initilization error, wait forever
        while (1);
    }

    /* Initializse Clock */
    clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | 
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 ;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1; // SYSCLK not divided
    clk.APB1CLKDivider = RCC_HCLK_DIV1;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_4) != HAL_OK) {
        // Initilizatione error, wait forever
        while (1);
    }
    per.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    per.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&per) != HAL_OK) {
        while (1);
    }
}
