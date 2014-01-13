/*
 * motor.c
 *
 *  Created on: 13.01.2014
 *      Author: embox
 */
#include <avr/io.h>
#include "motor.h"
#include "wait.h"

void Motor_DirectRun(int left, int right)
{
	if (left && right && left - right == 2 * left)
	{
		if (left > 0) MOTOR_RIGHT;
		else MOTOR_LEFT;
	}
	else if (left && right && left - right == 0)
	{
		if (left > 0) MOTOR_FORWARDRUN;
		else MOTOR_BACKWARDRUN;
	}
	if (left < 0) {
		left = 0 - left;
	}
	wait_ms(left);
	MOTOR_FREEWHEEL;
}

/* AVR mega8 Timer0 initialization */
void Motor_TimerInit(void)
{
	TIMSK |= (1<<TOIE0);
	Motor_TimerTickCount = 0;
	MOTOR_TimerStart;
}

void Motor_Run(void)
{
	Motor_TimerTickCount = 0;
	MOTOR_TimerStart;
}

