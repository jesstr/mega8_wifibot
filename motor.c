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
	MOTOR_TimerReset;
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
	OCR1A = 255 - speed;
	OCR1B = 255 - speed;
	MOTOR_PWMStart;
	MOTOR_TimerStart;
}

/* AVR mega8 Timer1 initialization */
void Motor_PWMInit(void)
{
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0)|(1<<COM1B1)|(1<<COM1B0)|(1<<WGM10); /* PWM, Phase Correct, 8-bit, OC1A and OC1B are connected */
	//TCCR1B |= (1<<CS10); /* No prescaling, PWM frequency is 15.625kHz  */
	TCNT1 = 0;
	OCR1A = 0;
	OCR1B = 0;
}
