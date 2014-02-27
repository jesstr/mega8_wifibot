/*
 * adc.c
 *
 *  Created on: 27.02.2014
 *      Author: embox
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"


/* Interrupt on ADC conversion complete */
ISR (ADC_vect) //оПЕПШБЮМХЕ ОН ГЮБЕПЬЕМХХ ЙНМБЕПРЮЖХХ южо
{
unsigned short buf;

buf = ADCL;
buf = (ADCH << 8)|(buf);
adc_value[adc_active_channel] = buf;
}

void SwitchChannel(unsigned char n_channel)
{
	/* Waiting last conversion is finished */
	while (IS_CONVERTION_RUNNING) {
		;
	}
	ADMUX |= (0x07 & n_channel);
	adc_active_channel = n_channel;
}

/* ADC Initialization */
void ADC_Init(void)
{
	/* Input pins initialization */
	ADCDDR1 |= (0 << ADCPIN1);
	ADCPORT1 |= (0 << ADCPIN1);
	/* ADC voltage reference is AREF */
	ADMUX = (0 << REFS0)|(0 << REFS0);
	/* Clock divider 64, interrupt enabled */
	ADCSRA = (1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0)|(1 << ADIE);
	ADC_ENABLE;
}

/* Start ADC conversion on selected channel.
 * mode = 1 - free run mode, mode = 0 - single mode */
void ADC_StartConversion(unsigned char mode, unsigned char n_channel)
{
	/* Waiting last conversion is finished */
	while (IS_CONVERTION_RUNNING) {
		;
	}
	/* Input channel selection */
	SwitchChannel(n_channel);
	switch (mode) {
		case 0: ADC_START_SINGLE; break;
		case 1: ADC_START_FREERUN; break;
		default: break;
	}
}

