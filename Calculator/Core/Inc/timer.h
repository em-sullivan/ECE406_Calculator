/*
 * Eric Sullivan and Elizabeth Willard
 * 08/17/2020
 * Hardware timers
 */

#ifndef TIMER_H
#define TIMER_H
 
#include <stdint.h>

/* Define which timer is being used */
#define TIMER TIM2 

/*
 * Initilize timer 2 for delays
 */
void timer_init(void);

/*
 * Delay function that uses Timer 2
 * to delay for a specified number
 * of micro seconds.
 */
void udelay(uint16_t micro);

/*
 * Delay function that uses Timer 2
 * to delay for a specified number
 * of milli seconds.
 */
void mdelay(uint16_t milli);

#endif // TIMER_H
