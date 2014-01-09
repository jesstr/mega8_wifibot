#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servo.h"

char servo_number=0;
unsigned int delta_time[4];       //delta_time[n] массив разниц длительностей n-1 сервомашинок по порядку
unsigned int pulse_time[4];       //pulse_time[n] массив управляющих длительностей n-1 сервомашинок
unsigned char sorted_index[4];            //упорядоченные индексы массива pulse_time[n]
unsigned char count_of_servos=3; //количество управляемых сервомашинок (ограничено количеством свободных портов)

void wait_us(unsigned int us)   //имитация задержки (в 1.3815 раз медленне чем delay_us() при 8МГц и локальной переменной j) -  us*1.3815=мкс задержки
{                               //имитация задержки (в 1.628 раз медленне чем delay_us() при 8МГц и глобальной переменной j) -  us*1.628=мкс задержки
	unsigned int j;

	for (j=0; j<us; j++) {
		_delay_us(1);
		};
}

// AVR mega8 Timer 0 overflow interrupt service routine
ISR(TIMER2_COMP_vect)
{
	unsigned char i;

	SRVPORT1|=(1<<SRVPIN1);
	SRVPORT2|=(1<<SRVPIN2);
	SRVPORT3|=(1<<SRVPIN3);
	for (i=1; i<=count_of_servos; i++) {
		wait_us(delta_time[i]);
		switch (sorted_index[i]) {
			case 1: SRVPORT1&=~(1<<SRVPIN1); break;
			case 2: SRVPORT2&=~(1<<SRVPIN2); break;
			case 3: SRVPORT3&=~(1<<SRVPIN3); break;
		}
	}
	PORTC^=0x01;
}

void BubbleSort(unsigned char n)
{
	unsigned char i=0;
	unsigned int Temp;

		while (n>0) {
			n--;
			i=0;
			while (i<n) {
				if (pulse_time[(sorted_index[i])]>pulse_time[(sorted_index[i+1])]) {
					Temp=sorted_index[i+1];
					sorted_index[i+1]=sorted_index[i];
					sorted_index[i]=Temp;
				}
				i++;
			}
		}
}

void Servo_UpdateArrays(void)
{
	unsigned char i;

	sorted_index[0]=0;
	sorted_index[1]=1;
	sorted_index[2]=2;
	sorted_index[3]=3;
	BubbleSort(count_of_servos+1);
	for (i=1; i<=count_of_servos; i++) {
		delta_time[i]=pulse_time[(sorted_index[i])]-pulse_time[(sorted_index[i-1])];
	}
}

void Servo_InitPulses(void)
{
	pulse_time[0]=0;              //651 - 0град(900мкс), 1085 - 30град(1500мкс), 1520 - 60град(2100мкс)
	pulse_time[1]=651;             //552 - 0град(900мкс), 921 - 30град(1500мкс), 1290 - 60град(2100мкс)
	pulse_time[2]=651;
	pulse_time[3]=651;
	sorted_index[0]=0;
	sorted_index[1]=1;
	sorted_index[2]=2;
	sorted_index[3]=3;
}

void Servo_Demo(void)
{
	_delay_ms(500);
	pulse_time[1]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[1]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[2]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[2]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[1]=1520;
	pulse_time[2]=1520;
	pulse_time[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[1]=651;
	pulse_time[2]=651;
	pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[1]=651;
	pulse_time[2]=1085;
	pulse_time[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[1]=651;
	pulse_time[2]=651;
	pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[1]=1520;
	pulse_time[2]=1085;
	pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	pulse_time[1]=651;
	pulse_time[2]=651;
	pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(2000);
}

/* AVR mega8 Timer2 initialization */
void Servo_TimerInit(void)
{
	TCCR2 |= (1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);
	TCNT2 = 0x00;
	OCR2 = 156; // (~50Гц)
	TIMSK |= (1<<OCIE2);

	Servo_InitPulses();
	Servo_UpdateArrays();

	sei();

#if 0
	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 7,813 kHz
	// Mode: CTC top=OCR0A
	// OC0A output: Disconnected
	// OC0B output: Disconnected
	TCCR0A=0x02;
	TCCR0B=0x05;
	TCNT0=0x00;
	OCR0A=0x9C;    //156 (~50Гц)
	OCR0B=0x00;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK0=0x02;

	Servo_InitPulses();
	Servo_UpdateArrays();

	sei();
#endif
}

