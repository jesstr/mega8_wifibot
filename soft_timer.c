/*
 * soft_timer.c
 *
 *  Created on: 24.04.2014
 *      Author: embox
 */

#include "soft_timer.h"

/* Current count of soft timers in system */
static unsigned char soft_timers_count;
/* Array of system timers */
soft_timer_t *system_timers[SOFT_TIMERS_MAX_COUNT];

/* Register new soft timer in system */
void SoftTimer_RegisterTimer(soft_timer_t *timer){
	unsigned char i;

	/*Check if max timers count is not reached */
	if (soft_timers_count >= SOFT_TIMERS_MAX_COUNT)
		return;
	/* Check if timer is already registered */
	for (i = 0; i < soft_timers_count; i++) {
		if (system_timers[i] == timer)
				return;
	}
	system_timers[soft_timers_count++] = timer;
}

