/*
 * turret.c
 *
 *  Created on: 30.01.2014
 *      Author: embox
 */

#include "turret.h"
#include "servo.h"
#include "wait.h"


/* General turret initialization */
void Turret_Init(void)
{
	GUN_INIT;
	LASER_INIT;
}

/* Start fire according to passed duration */
void Turret_Fire(unsigned char duration)
{
	TURRET_GUN_FIRE;
	/* TODO Non-blocking delay */
	wait_ms(duration);
	TURRET_GUN_STOPFIRE
}

/* Turret moving according to passed horizontal and vertical pulse widths */
void Turret_Move(unsigned short hor_pos, unsigned short vert_pos)
{
	if ((SERVO_MAX_PULSE_WIDTH * 2 + 1 > hor_pos + vert_pos) && (hor_pos + vert_pos > SERVO_MIN_PULSE_WIDTH * 2 - 1)) {
		servo_pulse_width[TURR_HOR_SERVO] = hor_pos;
		servo_pulse_width[TURR_VERT_SERVO] = vert_pos;
		Servo_UpdateArrays();
	}
}
