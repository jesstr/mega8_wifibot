/*
 * turret.c
 *
 *  Created on: 30.01.2014
 *      Author: embox
 */

#include "turret.h"
#include "servo.h"
#include "wait.h"


/* Timer initialization for PWM (ATmega8 Timer1) */
void Turret_PWMInit(void)
{
	TCCR2 |= (1<<COM21)|(1<<WGM20); /* PWM, Phase Correct, 8-bit, OC2 is connected */
	TCCR1B |= (1<<CS20); /* No prescaling, PWM frequency is 15.625kHz  */
	TCNT2 = 0;
	OCR2 = 0;
}

/* Start fire according to passed duration */
void Turret_Fire(unsigned char duration)
{
/* TODO Change realization without using l293 chip */
#if 0
	TURRET_GUN_FIRE;
	/* TODO Non-blocking delay */
	wait_ms(duration);
	TURRET_GUN_STOPFIRE
#endif
}

void Turret_MoveHorDC(char* direction, unsigned char speed, unsigned char time) {
	switch (direction[0]) {
		case 'L' : OCR2 = speed; TURRET_HOR_LEFT_RUN; break;
		case 'R' : OCR2 = speed; TURRET_HOR_RIGHT_RUN; break;
	}
	Turret_TimerNTicksToRunHor = time;
	TURRET_TIMER_HOR_START;
}

void Turret_MoveVertDC(char* direction, unsigned char speed, unsigned char time) {

}

void Turret_MoveHorServo(unsigned short hor_pos) {
	if ((SERVO_MAX_PULSE_WIDTH + 1 > hor_pos) &&
			( hor_pos > SERVO_MIN_PULSE_WIDTH - 1)) {
		servo_pulse_width[TURR_HOR_SERVO] = hor_pos;
	}
	Servo_UpdateArrays();
}

void Turret_MoveVertServo(unsigned short vert_pos) {
	if ((SERVO_MAX_PULSE_WIDTH + 1 > vert_pos) &&
			( vert_pos > SERVO_MIN_PULSE_WIDTH - 1)) {
		servo_pulse_width[TURR_VERT_SERVO] = vert_pos;
	}
	Servo_UpdateArrays();
}


/* General turret initialization */
void Turret_Init(void)
{
	//GUN_INIT;
	LASER_INIT;
	Turret_PWMInit();
}


#if 0
/* Turret moving according to passed horizontal and vertical pulse widths */
/* If DC motor is used, it should be controlled like servo, *
 * passing pulse width. That's for compatibility. */
void Turret_Move(unsigned short hor_pos, unsigned short vert_pos)
{
	if ((SERVO_MAX_PULSE_WIDTH + 1 > hor_pos) &&
			( hor_pos > SERVO_MIN_PULSE_WIDTH - 1)) {

#ifdef _HOR_DC_
	/* TODO Horizontal DC run  */

#else
#ifdef _HOR_SERVO_
		servo_pulse_width[TURR_HOR_SERVO] = hor_pos;
#endif
#endif

	}
	if ((SERVO_MAX_PULSE_WIDTH + 1 > vert_pos) && ( vert_pos > SERVO_MIN_PULSE_WIDTH - 1)) {
		servo_pulse_width[TURR_VERT_SERVO] = vert_pos;
	}
	Servo_UpdateArrays();
}
#endif

/* TODO remove */
#if 0
/* Turret moving according to passed horizontal and vertical pulse widths */
/* Horizontal move: servo
 * Vertical move: servo */
void Turret_Move(unsigned short hor_pos, unsigned short vert_pos)
{
	if ((SERVO_MAX_PULSE_WIDTH * 2 + 1 > hor_pos + vert_pos) && (hor_pos + vert_pos > SERVO_MIN_PULSE_WIDTH * 2 - 1)) {
		servo_pulse_width[TURR_HOR_SERVO] = hor_pos;
		servo_pulse_width[TURR_VERT_SERVO] = vert_pos;
		Servo_UpdateArrays();
	}
}
#endif
