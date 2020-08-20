/*
 * Eric Sullivan and Elizabeth Willard
 * Test file for i2c LCD
 */

#include <stdint.h>
#include "leds_hal.h"
#include "timer.h"
#include "lcd.h"
#include "stm32l4xx_hal.h"

void System_Clock_Init(void);

int main()
{
    System_Clock_Init();
    timer_init();
    Red_LED_Init();
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    lcd_init();
    
    lcd_print("Hello");
    lcd_command(LCD_SET_RAM | LCD_LINE2);
    lcd_print("22");
    while (1);

}
void System_Clock_Init(void)
{
    RCC_OscInitTypeDef Osc = {0};
    RCC_ClkInitTypeDef Clk = {0};

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
    Clk.ClockType = RCC_CLOCKTYPE_SYSCLK;
    Clk.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    Clk.AHBCLKDivider = RCC_SYSCLK_DIV1; // SYSCLK not divided
    Clk.APB1CLKDivider = RCC_HCLK_DIV1;
    Clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&Clk, FLASH_LATENCY_4) != HAL_OK) {
        // Initilizatione error, wait forever
        while (1);
    }
}
