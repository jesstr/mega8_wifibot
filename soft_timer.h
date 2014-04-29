/*
 * soft_timer.h
 *
 *  Created on: 24.04.2014
 *      Author: embox
 */

#ifndef SOFT_TIMER_H_
#define SOFT_TIMER_H_

typedef struct {
	unsigned int counter;
	unsigned int load;
	unsigned char is_running;
} soft_timer_t;

#endif /* SOFT_TIMER_H_ */
