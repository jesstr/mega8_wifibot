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

#define SRVPORT1 PORTB
#define SRVPIN1 2
#define SRVPORT2 PORTB
#define SRVPIN2 3
#define SRVPORT3 PORTB
#define SRVPIN3 4

#define SERVO_MIN_PULSE_TIME	651		// Нижнее ограничение ширины импульса управляющего сигнала для сервомашинок
#define SERVO_MAX_PULSE_TIME	1520	// Верхнее ограничение ширины импульса управляющего сигнала для сервомашинок

unsigned int servo_pulse_time[COUNT_OF_SERVOS+1];  		//pulse_time[n] массив управляющих длительностей n-1 сервомашинок

void Servo_UpdateArrays(void);
void Servo_InitPulses(void);
void Servo_Demo(void);
void Servo_TimerInit(void);

#endif /* SERVO_H_ */
