/*
 * Eric Sullivan and Elizabeth Willard
 * 08/17/2020
 * Hardware timers
 */

#ifndef TIMER_H
#define TIMER_H

#define TIMER TIM2
#include <stdint.h>

void timer_init(void);
void udelay(uint16_t micro);
void mdelay(uint16_t milli);

#endif // TIMER_H