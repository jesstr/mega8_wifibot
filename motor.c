/*
 * motor.c
 *
 *  Created on: 13.01.2014
 *      Author: embox
 */
#include <avr/io.h>
#include <util/delay.h>
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
	Motor_TimerTick = 0;
}

void Motor_Run(char* direction, unsigned char speed, unsigned char time)
{
	switch (direction[0]) {
		case 'L': MOTOR_LEFT; break;
		case 'R': MOTOR_RIGHT; break;
		case 'F': MOTOR_FORWARDRUN; break;
		case 'B': MOTOR_BACKWARDRUN; break;
		default: PORTD &= ~(1<<PD2); _delay_ms(100); PORTD |= (1<<PD2); break;
	}




	Motor_TimerTick = 0;
	MOTOR_TimerStart;
}

