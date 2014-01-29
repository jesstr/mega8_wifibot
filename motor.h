/*
 * motor.h
 *
 *  Created on: 13.01.2014
 *      Author: embox
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "l293.h"

#define LEFT_DISABLE	CHIP1_DISABLE
#define LEFT_ENABLE		CHIP1_ENABLE
#define RIGHT_DISABLE	CHIP2_DISABLE
#define RIGHT_ENABLE	CHIP2_ENABLE

#define BOTH_ENABLE		do { 			\
						LEFT_ENABLE;	\
						RIGHT_ENABLE;	\
						} while(0)

#define BOTH_DISABLE	do { 			\
						LEFT_DISABLE; 	\
						RIGHT_DISABLE; 	\
						} while(0)

#define FORWARD			do { 							\
						INPUT1_PORT &= ~(1<<INPUT1_PIN);\
						INPUT2_PORT |= (1<<INPUT2_PIN);	\
						INPUT3_PORT |= (1<<INPUT3_PIN);	\
						INPUT4_PORT &= ~(1<<INPUT4_PIN);\
						} while(0)

#define BACKWARD		do { 							\
						INPUT2_PORT &= ~(1<<INPUT2_PIN);\
						INPUT1_PORT |= (1<<INPUT1_PIN);	\
						INPUT4_PORT |= (1<<INPUT4_PIN);	\
						INPUT3_PORT &= ~(1<<INPUT3_PIN);\
						} while(0)

#define RIGHT			do { 							\
						INPUT1_PORT &= ~(1<<INPUT1_PIN);\
						INPUT2_PORT |= (1<<INPUT2_PIN);	\
						INPUT4_PORT |= (1<<INPUT4_PIN);	\
						INPUT3_PORT &= ~(1<<INPUT3_PIN);\
						} while(0)

#define LEFT			do {							\
						INPUT2_PORT &= ~(1<<INPUT2_PIN);\
						INPUT1_PORT |= (1<<INPUT1_PIN);	\
						INPUT3_PORT |= (1<<INPUT3_PIN);	\
						INPUT4_PORT &= ~(1<<INPUT4_PIN);\
						} while(0)

#define BRAKE			do {							\
						INPUT2_PORT |= (1<<INPUT2_PIN);	\
						INPUT1_PORT |= (1<<INPUT1_PIN);	\
						INPUT4_PORT |= (1<<INPUT4_PIN);	\
						INPUT3_PORT |= (1<<INPUT3_PIN);	\
						} while(0)

#define MOTOR_INIT		do {							\
						BOTH_DISABLE;					\
						CE1_DDR |= (1<<CE1_PIN);		\
						CE2_DDR |= (1<<CE2_PIN);		\
						INPUT1_DDR |= (1<<INPUT1_PIN);	\
						INPUT2_DDR |= (1<<INPUT2_PIN);	\
						INPUT3_DDR |= (1<<INPUT3_PIN);	\
						INPUT4_DDR |= (1<<INPUT4_PIN);	\
						} while(0)

#define MOTOR_FORWARDRUN	do {			\
							FORWARD;		\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_BACKWARDRUN	do {			\
							BACKWARD;		\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_RIGHT			do { 			\
							RIGHT;			\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_LEFT			do {			\
							LEFT;			\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_FREEWHEEL		BOTH_DISABLE;

#define MOTOR_BRAKE			do { 			\
							BRAKE;			\
							_delay_ms(100);	\
							BOTH_DISABLE;	\
							} while(0)

#define MOTOR_TimerReset 	do {					\
							TCNT0 = 0; 				\
							Motor_TimerCurrentTick = 0;	\
							} while(0)

#define MOTOR_TimerStart 	do {							\
							MOTOR_TimerReset;				\
							/* 1024 divider, (~30Гц) on 8MHz */ 			\
							TCCR0 |= (1<<CS02)|(1<<CS00); 	\
							} while(0)

#define MOTOR_PWMStart 		TCCR1B |= (1<<CS10);	 /* No prescaling, PWM frequency is 15.625kHz  */
#define MOTOR_PWMStop 		TCCR1B &= ~(1<<CS10);

#define MOTOR_TimerStop		TCCR0 &= ~((1<<CS02)|(1<<CS00));

//#define MOTOR_TIMER_nTicksForKeyPressedRun	3	/* 1 = (~33ms on 8MHz and 1024 divider), Number of Timer periods to run motors when key was pressed	*/

volatile unsigned short MOTOR_TIMER_nTicksForKeyPressedRun;	/* 1 = (~33ms on 8MHz and 1024 divider), Number of Timer periods to run motors when key was pressed	*/

volatile unsigned long Motor_TimerCurrentTick; 	/* Timer ticks */


void Motor_DirectRun(int left, int right);
void Motor_TimerInit(void);
void Motor_Run(char* direction, unsigned char speed, unsigned char time);
void Motor_PWMInit(void);

#endif /* MOTOR_H_ */

