/*
 * adc.h
 *
 *  Created on: 27.02.2014
 *      Author: embox
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC_ENABLE	ADCSRA |= (1 << ADEN)
#define ADC_DISABLE	ADCSRA &= ~(1 << ADEN)

#define ADC_SINGLE_MODE		0
#define ADC_FREERUN_MODE	1

extern volatile unsigned short adc_value[]; 	/* Array of ADC conversion results for each channel */
extern volatile unsigned char adc_active_channel; 			/* ADC current active channel */

/* ADC Initialization */
void ADC_Init(void);
/* Start ADC conversion on selected channel.
 * mode = 1 - free run mode, mode = 0 - single mode */
void ADC_StartConversion(unsigned char mode, unsigned char n_channel);
/* Start new single conversion and get the ADC value. Blocking routine. */
unsigned short ADC_GetValue(unsigned char n_channel);
/* Returns last stored ADC value. Non-blocking routine. */
unsigned short ADC_PopValue(unsigned char n_channel);

#endif /* ADC_H_ */
