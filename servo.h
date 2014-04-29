/*
 * servo.h
 *
 *  Created on: 26.12.2013
 *      Author: embox
 */

#include <avr/io.h>

#ifndef SERVO_H_
#define SERVO_H_

#define COUNT_OF_SERVOS	3 	//количество управляемых сервомашинок (ограничено количеством свободных портов)
#define TURR_VERT_SERVO	1 	//Индекс сервомашики вертикального отклонения турели
#define TURR_HOR_SERVO	2 	//Индекс сервомашики горизонтального отклонения турели
#define STEERING_SERVO	3	//Индекс рулевой сервомашики

#define SERVO_MIN_PULSE_WIDTH	520		// Нижнее ограничение ширины импульса управляющего сигнала для сервомашинок
#define SERVO_MAX_PULSE_WIDTH	1216	// Верхнее ограничение ширины импульса управляющего сигнала для сервомашинок

extern unsigned int servo_pulse_width[COUNT_OF_SERVOS+1];

void Servo_UpdateArrays(void);
void Servo_Demo(void);
void Servo_Init(void);

#endif /* SERVO_H_ */
