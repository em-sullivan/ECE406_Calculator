/*
 * Eric Sullivan and Elizabeth Willard
 * Test file for st7302 LCD screen
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "leds_hal.h"
#include "timer.h"
#include "lcd-st7302.h"
#include "stm32l4xx_hal.h"
#include "calc.h"

void system_clock_init(void);

int main()
{
    int val = 25;
    // Init system clock and LEDs
    system_clock_init();
    Green_LED_Init();
    timer_init();

    // Init LCD  
    lcd_init();

    // Have to write this in order to have two lines, 
    // may have to change lcd_init
    lcd_command(LCD_FUNC | LCD_FUNC_DL | LCD_FUNC_TWOL);
    lcd_print("The anser is %d", val);
    lcd_set_cursor(3, 2);
    lcd_print("Cursor check");
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
    return 0;
}

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
        while (1);
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