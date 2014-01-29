/*
 * l293.h
 *
 *  Created on: 29.01.2014
 *      Author: embox
 */

#ifndef L293_H_
#define L293_H_

// Íàçíà÷åíèå âûâîäîâ äëÿ äðàéâåðà äâèãàòåëåé
#define CE1_PIN		PB1
#define CE1_PORT 	PORTB
#define CE1_DDR 	DDRB

#define CE2_PIN		PB2
#define CE2_PORT 	PORTB
#define CE2_DDR 	DDRB

#define INPUT1_PIN	PC5
#define INPUT1_PORT PORTC
#define INPUT1_DDR 	DDRC

#define INPUT2_PIN	PC4
#define INPUT2_PORT PORTC
#define INPUT2_DDR 	DDRC

#define INPUT3_PIN	PC3
#define INPUT3_PORT PORTC
#define INPUT3_DDR 	DDRC

#define INPUT4_PIN	PC2
#define INPUT4_PORT PORTC
#define INPUT4_DDR 	DDRC

// Ìàêðîñû äëÿ äðàéâåðà äâèãàòåëåé
#define CHIP1_DISABLE	CE1_PORT &= ~(1<<CE1_PIN);
#define CHIP2_DISABLE	CE2_PORT &= ~(1<<CE2_PIN);

#define CHIP1_ENABLE	CE1_PORT |= (1<<CE1_PIN);
#define CHIP2_ENABLE	CE2_PORT |= (1<<CE2_PIN);

#endif /* L293_H_ */
