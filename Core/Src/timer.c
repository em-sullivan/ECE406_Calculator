/*
 * Eric Sullivan and Elizabeth Willard
 * 08/17/2020
 * Hardware timers
 */

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "timer.h"

static TIM_HandleTypeDef tim_handle;
volatile static uint32_t timer_ticks;

void timer_init(void)
{
    timer_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
    TIM_ClockConfigTypeDef clk = {0};
    TIM_MasterConfigTypeDef mst = {0};

    // Enable clock for TIMER 2
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Init Timer handler
    tim_handle.Instance = TIMER;
    tim_handle.Init.Prescaler = timer_ticks - 1;
    tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim_handle.Init.Period = 65535;
    tim_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&tim_handle) != HAL_OK) {
        while (1);
    }

    // Config clock source
    clk.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&tim_handle, &clk) != HAL_OK) {
        while (1);
    }

    // Config master
    mst.MasterOutputTrigger = TIM_TRGO_RESET;
    mst.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&tim_handle, &mst) != HAL_OK) {
        while (1);
    }

    HAL_TIM_Base_Start(&tim_handle);
}

void udelay(uint16_t micro)
{
    tim_handle.Instance->CNT = 0;
    while (tim_handle.Instance->CNT < micro);
}

void mdelay(uint16_t milli)
{
    // Uses udelay to wait multiple milliseconds
    while (milli > 0) {
        tim_handle.Instance->CNT = 0;
        milli--;
        while (tim_handle.Instance->CNT < 1000);
    }
}