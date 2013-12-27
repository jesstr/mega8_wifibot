/*
 * servo.h
 *
 *  Created on: 26.12.2013
 *      Author: embox
 */

#ifndef SERVO_H_
#define SERVO_H_

#define SRVPORT1 PORTB
#define SRVPIN1 2
#define SRVPORT2 PORTB
#define SRVPIN2 3
#define SRVPORT3 PORTB
#define SRVPIN3 4

void Servo_UpdateArrays(void);
void Servo_InitPulses(void);
void Servo_Demo(void);
void Servo_TimerInit(void);

#endif /* SERVO_H_ */
