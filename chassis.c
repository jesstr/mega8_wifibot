/*
 * chassis.c
 *
 *  Created on: 29.01.2014
 *      Author: embox
 */

#include <avr/io.h>
#include <util/delay.h>
#include "chassis.h"
#include "servo.h"
#include "wait.h"

#ifdef _3WHEEL_2WD_
/* only for debugging */
void Chassis_DirectRun(signed short left_delay, signed short right_delay)
{
	if (left_delay_delay && right_delay && left_delay_delay - right_delay == 2 * left_delay_delay)
	{
		if (left_delay > 0) CHASSIS_RIGHT_RUN;
		else CHASSIS_LEFT_RUN;
	}
	else if (left_delay && right_delay && left_delay - right_delay == 0)
	{
		if (left_delay > 0) CHASSIS_FORWARD_RUN;
		else CHASSIS_BACKWARD_RUN;
	}
	if (left_delay < 0) {
		left_delay = 0 - left_delay;
	}
	wait_ms(left_delay);
	CHASSIS_FREEWHEEL;
}
#endif

#ifdef _4WHEEL_2WD_
/* only for debugging */
void Chassis_DirectRun(signed short delay)
{
	if (delay > 0) CHASSIS_FORWARD_RUN;
	else CHASSIS_BACKWARD_RUN;

	if (delay < 0) {
		delay = 0 - delay;
	}
	wait_ms(delay);
	CHASSIS_FREEWHEEL;
}
#endif

/* Timer initialization for non-blocking delay (ATmega8 Timer0) */
void Chassis_TimerInit(void)
{
	TIMSK |= (1<<TOIE0);
	CHASSIS_TIMER_RESET;
}

#ifdef _3WHEEL_2WD_
/* Non-blocking chassis run according to passed parrameters: direction, speed, time */
void Chassis_Run(char* direction, unsigned char speed, unsigned char time)
{
	switch (direction[0]) {
		case 'L' : OCR1A = speed; OCR1B = speed; CHASSIS_LEFT_RUN; break;
		case 'R' : OCR1B = speed; OCR1B = speed; CHASSIS_RIGHT_RUN; break;
		case 'F' : OCR1A = speed; OCR1B = speed; CHASSIS_FORWARD_RUN; break;
		case 'B' : OCR1A = speed; OCR1B = speed; CHASSIS_BACKWARD_RUN; break;
	}
	Chassis_TimerNTicksToRun = time;
	CHASSIS_TIMER_START;
}
#endif

#ifdef _4WHEEL_2WD_
/* Non-blocking chassis run according to passed parrameters: direction, speed, time */
void Chassis_Run(char* direction, unsigned char speed, unsigned char time)
{
	switch (direction[0]) {
		case 'F' : OCR1A = speed; CHASSIS_FORWARD_RUN; break;
		case 'B' : OCR1A = speed; CHASSIS_BACKWARD_RUN; break;
	}
	Chassis_TimerNTicksToRun = time;
	CHASSIS_TIMER_START;
}
#endif

/* Chassis steering according to passed pulse width */
void Chassis_Steer(unsigned short width)
{
	if ((SERVO_MAX_PULSE_WIDTH > width) && (width > SERVO_MIN_PULSE_WIDTH)) {
		servo_pulse_width[STEERING_SERVO] = width;
		Servo_UpdateArrays();
	}
}

/* Timer initialization for PWM (ATmega8 Timer1) */
void Chassis_PWMinit(void)
{
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10); /* PWM, Phase Correct, 8-bit, OC1A and OC1B are connected */
	TCCR1B |= (1<<CS10); /* No prescaling, PWM frequency is 15.625kHz  */
	TCNT1 = 0;
	OCR1A = 0;
	OCR1B = 0;
}

/* General chassis initialization */
void Chassis_Init(void)
{
	CHASSIS_MOTOR_INIT;
	Chassis_TimerInit();
	Chassis_PWMinit();
}
