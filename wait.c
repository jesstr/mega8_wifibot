/*
 * wait.c
 *
 *  Created on: 13.01.2014
 *      Author: embox
 */
#include <util/delay.h>
#include "wait.h"

/* Wait routine, ms */
void wait_ms(short ms)
{
	while(ms > 0)
	{
		_delay_ms(1);
		ms--;
	}
}
