/*
 * wait.c
 *
 *  Created on: 13.01.2014
 *      Author: embox
 */
#include <util/delay.h>
#include "wait.h"

/* Wait routine, ms */
void wait_ms(unsigned short ms)
{
	while(ms > 0)
	{
		_delay_ms(1);
		ms--;
	}
}

/* Wait routine, us */
void wait_us(unsigned short us)	/* Delay function, that is 1.3815 times slower that _delay_us. Actual delay is us*1.3815 */
{
	unsigned int j;

	for (j=0; j<us; j++) {
		_delay_us(1);
		};
}
