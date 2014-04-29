/*
 * chassis.h
 *
 *  Created on: 29.01.2014
 *      Author: embox
 */

#ifndef CHASSIS_H_
#define CHASSIS_H_

#include <avr/io.h>
#include "l293.h"
#include "soft_timer.h"

/* Definition of chassis scheme. Chassis scheme which is actually used should be uncommented. */
 #define _4WHEEL_2WD_ 	1
//#define _3WHEEL_2WD_	1

soft_timer_t chassis_timer;

/* Defines for 3 wheel 2wd chassis scheme */
#ifdef _3WHEEL_2WD_
#define MOTOR_LEFT_DISABLE	CHIP1_DISABLE
#define MOTOR_LEFT_ENABLE		CHIP1_ENABLE
#define MOTOR_RIGHT_DISABLE	CHIP2_DISABLE
#define MOTOR_RIGHT_ENABLE	CHIP2_ENABLE

#define MOTOR_BOTH_ENABLE		do { 				\
								MOTOR_LEFT_ENABLE;	\
								MOTOR_RIGHT_ENABLE;	\
								} while(0)

#define MOTOR_BOTH_DISABLE		do { 			\
								LEFT_DISABLE; 	\
								RIGHT_DISABLE; 	\
								} while(0)

#define MOTOR_FORWARD			do { 							\
								INPUT1_PORT &= ~(1<<INPUT1_PIN);\
								INPUT2_PORT |= (1<<INPUT2_PIN);	\
								INPUT3_PORT |= (1<<INPUT3_PIN);	\
								INPUT4_PORT &= ~(1<<INPUT4_PIN);\
								} while(0)

#define MOTOR_BACKWARD		do { 							\
							INPUT2_PORT &= ~(1<<INPUT2_PIN);\
							INPUT1_PORT |= (1<<INPUT1_PIN);	\
							INPUT4_PORT |= (1<<INPUT4_PIN);	\
							INPUT3_PORT &= ~(1<<INPUT3_PIN);\
							} while(0)

#define MOTOR_RIGHT			do { 							\
							INPUT1_PORT &= ~(1<<INPUT1_PIN);\
							INPUT2_PORT |= (1<<INPUT2_PIN);	\
							INPUT4_PORT |= (1<<INPUT4_PIN);	\
							INPUT3_PORT &= ~(1<<INPUT3_PIN);\
							} while(0)

#define MOTOR_LEFT			do {							\
							INPUT2_PORT &= ~(1<<INPUT2_PIN);\
							INPUT1_PORT |= (1<<INPUT1_PIN);	\
							INPUT3_PORT |= (1<<INPUT3_PIN);	\
							INPUT4_PORT &= ~(1<<INPUT4_PIN);\
							} while(0)

#define MOTOR_BRAKE			do {							\
							INPUT2_PORT |= (1<<INPUT2_PIN);	\
							INPUT1_PORT |= (1<<INPUT1_PIN);	\
							INPUT4_PORT |= (1<<INPUT4_PIN);	\
							INPUT3_PORT |= (1<<INPUT3_PIN);	\
							} while(0)

#define CHASSIS_MOTOR_INIT	do {							\
							MOTOR_BOTH_DISABLE;				\
							CE1_DDR |= (1<<CE1_PIN);		\
							CE2_DDR |= (1<<CE2_PIN);		\
							INPUT1_DDR |= (1<<INPUT1_PIN);	\
							INPUT2_DDR |= (1<<INPUT2_PIN);	\
							INPUT3_DDR |= (1<<INPUT3_PIN);	\
							INPUT4_DDR |= (1<<INPUT4_PIN);	\
							} while(0)

#define CHASSIS_FORWARD_RUN		do {				\
								MOTOR_FORWARD;		\
								MOTOR_BOTH_ENABLE;	\
								} while(0)

#define CHASSIS_BACKWARD_RUN	do {				\
								MOTOR_BACKWARD;		\
								MOTOR_BOTH_ENABLE;	\
								} while(0)

#define CHASSIS_RIGHT_RUN		do { 				\
								MOTOR_RIGHT;		\
								MOTOR_BOTH_ENABLE;	\
								} while(0)

#define CHASSIS_LEFT_RUN		do {				\
								MOTOR_LEFT;			\
								MOTOR_BOTH_ENABLE;	\
								} while(0)

#define CHASSIS_FREEWHEEL		MOTOR_BOTH_DISABLE;

#define CHASSIS_BRAKE			do { 				\
								MOTOR_BRAKE;		\
								_delay_ms(100);		\
								MOTOR_BOTH_DISABLE;	\
								} while(0)
#endif /* _3WHEEL_2WD_ */


/* Defines for 4 wheel 2wd chassis scheme */
#ifdef _4WHEEL_2WD_
#define MOTOR_DISABLE		CHIP1_DISABLE
#define MOTOR_ENABLE		CHIP1_ENABLE

#define MOTOR_FORWARD		do { 							\
							INPUT1_PORT &= ~(1<<INPUT1_PIN);\
							INPUT2_PORT |= (1<<INPUT2_PIN);	\
							} while(0)

#define MOTOR_BACKWARD		do { 							\
							INPUT2_PORT &= ~(1<<INPUT2_PIN);\
							INPUT1_PORT |= (1<<INPUT1_PIN);	\
							} while(0)

#define MOTOR_BRAKE			do {							\
							INPUT2_PORT |= (1<<INPUT2_PIN);	\
							INPUT1_PORT |= (1<<INPUT1_PIN);	\
							} while(0)

#define CHASSIS_MOTOR_INIT	do {							\
							MOTOR_DISABLE;				\
							CE1_DDR |= (1<<CE1_PIN);		\
							INPUT1_DDR |= (1<<INPUT1_PIN);	\
							INPUT2_DDR |= (1<<INPUT2_PIN);	\
							} while(0)

#define CHASSIS_FORWARD_RUN		do {			\
								MOTOR_FORWARD;	\
								MOTOR_ENABLE;	\
								} while(0)

#define CHASSIS_BACKWARD_RUN	do {			\
								MOTOR_BACKWARD;	\
								MOTOR_ENABLE;	\
								} while(0)

#define CHASSIS_FREEWHEEL		MOTOR_DISABLE;

#define CHASSIS_BRAKE			do { 			\
								MOTOR_BRAKE;	\
								_delay_ms(100);	\
								MOTOR_DISABLE;	\
								} while(0)
#endif /* _4WHEEL_2WD_ */

#if 0
#define CHASSIS_TIMER_RESET 	Chassis_TimerCurrentTick = 0;

#define CHASSIS_TIMER_START 	do {							\
								CHASSIS_TIMER_RESET;			\
								Chassis_TimerIsRunning = 1;		\
								} while(0)

#define CHASSIS_TIMER_STOP		do {							\
								CHASSIS_TIMER_RESET;			\
								Turret_TimerHorIsRunning = 0;	\
								} while(0)
#endif

#define CHASSIS_TIMER_RESET		chassis_timer.counter = 0

#define CHASSIS_TIMER_START 	do {							\
								CHASSIS_TIMER_RESET;			\
								chassis_timer.is_running = 1;	\
								} while(0)

#define CHASSIS_TIMER_STOP		do {							\
								CHASSIS_TIMER_RESET;			\
								chassis_timer.is_running = 0;	\
								} while(0)

#define CHASSIS_PWM_START 		TCCR1B |= (1<<CS10);	 /* No prescaling, PWM frequency is 15.625kHz  */
#define CHASSIS_PWM_STOP 		TCCR1B &= ~(1<<CS10);

#if 0
volatile unsigned short Chassis_TimerNTicksToRun;	/* 1 = (~33ms on 8MHz and 1024 divider), Number of timer periods to run motors */
volatile unsigned long Chassis_TimerCurrentTick; 	/* Current timer ticks */
volatile unsigned char Chassis_TimerIsRunning;
#endif

#ifdef _3WHEEL_2WD_
void Chassis_DirectRun(signed short left_delay, signed short right_delay);
#endif /* _3WHEEL_2WD_ */
#ifdef _4WHEEL_2WD_
void Chassis_DirectRun(signed short delay);
#endif /* _4WHEEL_2WD_ */
void Chassis_Run(char* direction, unsigned char speed, unsigned char time);
void Chassis_Steer(unsigned short width);
void Chassis_Init(void);

#endif /* CHASSIS_H_ */
