/*
 * Test program for progaming the
 * STM32L476 discovery board on Linux.
 * Used to control the leds
 * 
 * This uses the stm32l4_hal api
 */

#include <stdint.h>
#include <stdlib.h>
#include "timer.h"
#include "leds_hal.h"
#include "stm32l4xx_hal.h"

// #define M 1
#ifdef M 
void System_Clock_Init(void);

int main(void)
{
    System_Clock_Init();
    Red_LED_Init();
    Green_LED_Init();
    timer_init();

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
    while (1) {
        LED_Blink(1, 5);
        LED_Blink(0, 8);
        //HAL_GPIO_TogglePin(GAPIOB, GPIO_PIN_2);
        //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
        //mdelay(1000);
    }

}

void System_Clock_Init(void)
{
    RCC_OscInitTypeDef Osc = {0};
    RCC_ClkInitTypeDef Clk = {0};

    HAL_RCC_DeInit();
    /* Initialize Oscilation */
    Osc.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    Osc.MSIState = RCC_MSI_ON;
    Osc.MSIClockRange = RCC_MSIRANGE_11; // 8 Mhz
    if (HAL_RCC_OscConfig(&Osc) != HAL_OK) {
        // Initilization error, wait forever
        while (1);
    }

    /* Initializse Clock */
    Clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK;
    Clk.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    Clk.AHBCLKDivider = RCC_SYSCLK_DIV1; // SYSCLK not divided
    Clk.APB1CLKDivider = RCC_HCLK_DIV1;
    Clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&Clk, FLASH_LATENCY_4) != HAL_OK) {
        // Initilizatione error, wait forever
        while (1);
    }
}

#endif

void Red_LED_Init(void)
{
    /* Init PB2 for Red LED */
    GPIO_InitTypeDef Red;

    // Enable Clock for GPIOB
    __GPIOB_CLK_ENABLE();

    Red.Pin = GPIO_PIN_2;
    Red.Mode = GPIO_MODE_OUTPUT_PP; // Output, push-pull
    Red.Speed = GPIO_SPEED_FREQ_MEDIUM; // Freq range for 5 MHz to 25 MHz
    Red.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &Red);

}

void Green_LED_Init(void)
{
    /* Init PE8 for Green LED */
    GPIO_InitTypeDef Green;

    // Enable Clock for GPIOE
    __GPIOE_CLK_ENABLE();

    Green.Pin = GPIO_PIN_8;
    Green.Mode = GPIO_MODE_OUTPUT_PP;
    Green.Speed = GPIO_SPEED_FREQ_MEDIUM;
    Green.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &Green);
}

void LED_Blink(uint8_t col, uint8_t times)
{
    uint8_t i;

    for (i = 0; i < times * 2; i++) {
        if (col)
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
        else
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
        
        mdelay(1000);
    }
}

void SysTick_init(void)
{
    /* Init SysTick interupt for HAL_Delay */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    HAL_IncTick();
    HAL_GetTick();
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}