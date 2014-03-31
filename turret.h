/*
 * turret.h
 *
 *  Created on: 30.01.2014
 *      Author: embox
 */

#ifndef TURRET_H_
#define TURRET_H_

#include <avr/io.h>
#include "l293.h"

/* Definition of turret mechanics. Turret mechanics scheme which is actually used should be uncommented. */
#define _HOR_DC_ 			/* DC motor for horizontal moving, servo for vertical moving */
/* #define _HOR_SERVO_ */	/* Servo for horizontal moving, servo for vertical moving */

/* TODO Change realization without using l293 chip */
#if 0
#define GUN_DISABLE				CHIP2_DISABLE
#define GUN_ENABLE				CHIP2_ENABLE

#define TURRET_GUN_FIRE			do { 							\
								INPUT3_PORT |= (1<<INPUT3_PIN);	\
								INPUT4_PORT &= ~(1<<INPUT4_PIN);\
								GUN_ENABLE;						\
								} while(0)

#define TURRET_GUN_STOPFIRE		GUN_DISABLE

#define GUN_INIT				do {							\
								GUN_DISABLE;					\
								CE2_DDR |= (1<<CE2_PIN);		\
								} while(0)
#endif

#ifdef _HOR_DC_
#define MOTOR_HOR_DISABLE		CHIP2_DISABLE
#define MOTOR_HOR_ENABLE		CHIP2_ENABLE

#define MOTOR_HOR_LEFT			do { 							\
								INPUT3_PORT |= (1<<INPUT3_PIN);	\
								INPUT4_PORT &= ~(1<<INPUT4_PIN);\
								} while(0)

#define MOTOR_HOR_RIGH			do { 							\
								INPUT4_PORT |= (1<<INPUT4_PIN);	\
								INPUT3_PORT &= ~(1<<INPUT3_PIN);\
								} while(0)

#define TURRET_HOR_LEFT_RUN		do { 				\
								MOTOR_HOR_LEFT		\
								MOTOR_HOR_ENABLE	\
								} while(0)

#define TURRET_HOR_RIGHT_RUN	do { 				\
								MOTOR_HOR_RIGHT		\
								MOTOR_HOR_ENABLE	\
								} while(0)

#define TURRET_HOR_STOP			MOTOR_HOR_DISABLE

#define TURRET_MOTOR_HOR_INIT	do {							\
								MOTOR_HOR_DISABLE;				\
								CE2_DDR |= (1<<CE2_PIN);		\
								INPUT3_DDR |= (1<<INPUT3_PIN);	\
								INPUT4_DDR |= (1<<INPUT4_PIN);	\
								} while(0)
#endif

#define LASER_PIN				PB0
#define LASER_PORT 				PORTB
#define LASER_DDR 				DDRB

#define TURRET_LASER_ON			LASER_PORT |= (1<<LASER_PIN)
#define TURRET_LASER_OFF		LASER_PORT &= ~(1<<LASER_PIN)

#define LASER_INIT				do {							\
								TURRET_LASER_OFF;				\
								LASER_DDR |= (1<<LASER_PIN);	\
								} while(0)

/* General turret initialization */
void Turret_Init(void);
/* Start fire according to passed duration */
void Turret_Fire(unsigned char duration);
/* Turret moving according to passed horizontal and vertical pulse widths */
void Turret_Move(unsigned short hor_pos, unsigned short vert_pos);

#endif /* TURRET_H_ */
