/*
 * adc.c
 *
 *  Created on: 27.02.2014
 *      Author: embox
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

#define CHANNELS_COUNT	6 /* Number of ADC channels */

#define ADCPORT1 	PORTC
#define ADCPIN1 	PC1
#define ADCDDR1 	DDRC

#define ADCPORT2 	PORTC
#define ADCPIN2 	PC0
#define ADCDDR2 	DDRC

#define ADC_START_SINGLE	ADCSRA |= (1 << ADSC)
#define ADC_START_FREERUN	ADCSRA |= (1 << ADFR)
#define ADC_STOP_FREERUN	ADCSRA &= ~(1 << ADFR)

#define IS_CONVERTION_RUNNING	ADCSRA & (1 << ADSC)

volatile unsigned short adc_value[CHANNELS_COUNT]; 	/* Array of ADC conversion results for each channel */
volatile unsigned char adc_active_channel; 			/* ADC current active channel */

/* Interrupt on ADC conversion complete */
ISR(ADC_vect)
{
unsigned short buf;

buf = ADCL;
buf = (ADCH << 8)|(buf);
adc_value[adc_active_channel] = buf;
}

static void SwitchChannel(unsigned char n_channel)
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
	ADCDDR1 &= ~(1 << ADCPIN1);
	ADCPORT1 &= ~(1 << ADCPIN1);
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

/* Start new single conversion and get the ADC value. Blocking routine. */
unsigned short ADC_GetValue(unsigned char n_channel)
{
	unsigned short buf;

	/* Start new ADC conversion in single mode */
	ADC_StartConversion(ADC_SINGLE_MODE, n_channel);
	/* Waiting last conversion is finished */
	while (IS_CONVERTION_RUNNING) {
		;
	}

	buf = ADCL;
	buf = (ADCH << 8)|(buf);
	return buf;
}

/* Returns last stored ADC value. Non-blocking routine. */
inline unsigned short ADC_PopValue(unsigned char n_channel)
{
	return adc_value[n_channel];
}

