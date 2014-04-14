/*
 * int.c
 *
 *  Created on: 14.04.2014
 *      Author: embox
 */
#include "int.h"

#define INTPORT0 	PORTD
#define INTPIN0 	PD2
#define INTDDR0 	DDRD

#define INTPORT1 	PORTD
#define INTPIN1		PD3
#define INTDDR1 	DDRD

/* External interrupts (INT0, INT1, etc.) initialization */
void INT_Init() {
	/* Input pins initialization */
	INTDDR0 &= ~(1 << INTPIN0);
	INTPORT0 &= ~(1 << INTPIN0);
	INTDDR1 &= ~(1 << INTPIN1);
	INTPORT1 &= ~(1 << INTPIN1);
	/* External interrupts enable */
	GICR |= (1 << INT0)|(0 << INT1);
	/* Falling edge interrupt on INT1 */
	MCUCR |= (1 << ISC11) | (0 << ISC10);
	/* Falling edge interrupt on INT0 */
	MCUCR |= (1 << ISC01) | (0 << ISC00);
}
