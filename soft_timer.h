/*
 * soft_timer.h
 *
 *  Created on: 24.04.2014
 *      Author: embox
 */

#ifndef SOFT_TIMER_H_
#define SOFT_TIMER_H_

/* Max count of system soft timers */
#define SOFT_TIMERS_MAX_COUNT 	5

/* TODO add pointer to timer handle function*/
typedef struct {
	unsigned int counter;
	unsigned int load;
	void (*handler)();
	unsigned char is_running;
} soft_timer_t;

extern soft_timer_t *system_timers[];

/* Register new soft timer in system */
#define SOFT_TIMER_REGISTER(timer)	system_timers[soft_timers_count++] = &timer;

/* Register new soft timer in system */
void SoftTimer_RegisterTimer();


#endif /* SOFT_TIMER_H_ */
