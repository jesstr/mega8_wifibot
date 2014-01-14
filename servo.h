/*
 * servo.h
 *
 *  Created on: 26.12.2013
 *      Author: embox
 */

#ifndef SERVO_H_
#define SERVO_H_

#define COUNT_OF_SERVOS	3 	//количество управляемых сервомашинок (ограничено количеством свободных портов)
#define TURR_VERT_SERVO	1 	//Индекс сервомашики вертикального отклонения турели
#define TURR_HOR_SERVO	2 	//Индекс сервомашики горизонтального отклонения турели

#define SRVPORT1 	PORTD
#define SRVPIN1 	PD3
#define SRVDDR1 	DDRD
#define SRVPORT2 	PORTD
#define SRVPIN2 	PD4
#define SRVDDR2 	DDRD
/* Unused pins
#define SRVPORT3 PORTD
#define SRVPIN3 4
*/

#define SERVO_MIN_PULSE_TIME	520		// Нижнее ограничение ширины импульса управляющего сигнала для сервомашинок
#define SERVO_MAX_PULSE_TIME	1216	// Верхнее ограничение ширины импульса управляющего сигнала для сервомашинок

unsigned int servo_pulse_time[COUNT_OF_SERVOS+1];  		//pulse_time[n] массив управляющих длительностей n-1 сервомашинок

void Servo_UpdateArrays(void);
void Servo_Demo(void);
void Servo_Init(void);

#endif /* SERVO_H_ */
