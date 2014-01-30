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

#define FIRE_DISABLE			CHIP2_DISABLE
#define FIRE_ENABLE				CHIP2_ENABLE

#define TURRET_GUN_FIRE			do { 							\
								INPUT3_PORT |= (1<<INPUT3_PIN);	\
								INPUT4_PORT &= ~(1<<INPUT4_PIN);\
								FIRE_ENABLE;					\
								} while(0)

#define TURRET_GUN_STOPFIRE		FIRE_DISABLE

#define GUN_INIT				do {							\
								FIRE_DISABLE;					\
								CE2_DDR |= (1<<CE2_PIN);		\
								} while(0)

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
