/*
 * adc.h
 *
 *  Created on: 27.02.2014
 *      Author: embox
 */

#ifndef ADC_H_
#define ADC_H_

#define CHANNELS_COUNT	6 /* Number of ADC channels */

#define ADCPORT1 	PORTC
#define ADCPIN1 	PC1
#define ADCDDR1 	DDRC

#define ADCPORT2 	PORTC
#define ADCPIN2 	PC0
#define ADCDDR2 	DDRC

#define ADC_ENABLE	ADCSRA |= (1 << ADEN)
#define ADC_DISABLE	ADCSRA &= ~(1 << ADEN)

#define ADC_START_SINGLE	ADCSRA |= (1 << ADSC)
#define ADC_START_FREERUN	ADCSRA |= (1 << ADFR)
#define ADC_STOP_FREERUN	ADCSRA &= ~(1 << ADFR)

#define IS_CONVERTION_RUNNING	ADCSRA & (1 << ADSC)

volatile unsigned short adc_value[CHANNELS_COUNT]; 	/* Array of ADC conversion results for each channel */
volatile unsigned char adc_active_channel; 			/* ADC current active channel */



/* ADC Initialization */
void ADC_Init(unsigned char n_channel);
/* Start ADC conversion on selected channel.
 * mode = 1 - free run mode, mode = 0 - single mode */
void ADC_StartConversion(unsigned char mode, unsigned char n_channel);

#endif /* ADC_H_ */
