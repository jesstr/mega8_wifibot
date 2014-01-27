#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servo.h"

unsigned int delta_time[4];		//delta_time[n] массив разниц длительностей n-1 сервомашинок по порядку
unsigned char sorted_index[4];	//упорядоченные индексы массива servo_pulse_time[n]


void Servo_TimerInit(void);
void Servo_InitPulses(void);

void wait_us(unsigned int us)   //имитация задержки (в 1.3815 раз медленне чем delay_us() при 8МГц и локальной переменной j) -  us*1.3815=мкс задержки
{                               //имитация задержки (в 1.628 раз медленне чем delay_us() при 8МГц и глобальной переменной j) -  us*1.628=мкс задержки
	unsigned int j;

	for (j=0; j<us; j++) {
		_delay_us(1);
		};
}

// AVR mega8 Timer2 overflow interrupt service routine
ISR(TIMER2_COMP_vect)
{
	unsigned char i;

	SRVPORT1 |= (1<<SRVPIN1);
	SRVPORT2 |= (1<<SRVPIN2);
	SRVPORT3 |= (1<<SRVPIN3);
	for (i = 1; i <= COUNT_OF_SERVOS; i++) {
		wait_us(delta_time[i]);
		switch (sorted_index[i]) {
			case 1: SRVPORT1 &= ~(1<<SRVPIN1); break;
			case 2: SRVPORT2 &= ~(1<<SRVPIN2); break;
			case 2: SRVPORT3 &= ~(1<<SRVPIN3); break;
		}
	}
	PORTC ^= 0x01;
}

void BubbleSort(unsigned char n)
{
	unsigned char i=0;
	unsigned int Temp;

	while (n > 0) {
		n--;
		i = 0;
		while (i < n) {
			if (servo_pulse_time[(sorted_index[i])] > servo_pulse_time[(sorted_index[i + 1])]) {
				Temp = sorted_index[i + 1];
				sorted_index[i + 1] = sorted_index[i];
				sorted_index[i] = Temp;
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
	BubbleSort(COUNT_OF_SERVOS+1);
	for (i=1; i<=COUNT_OF_SERVOS; i++) {
		delta_time[i]=servo_pulse_time[(sorted_index[i])]-servo_pulse_time[(sorted_index[i-1])];
	}
}

/* Условные числовые значения для требуемых задержек, расчитываются теоритически и колибруются осциллографом */
void Servo_InitPulses(void)
{
	servo_pulse_time[0]=0;              //651 - 0град(900мкс), 1085 - 30град(1500мкс), 1520 - 60град(2100мкс)
	servo_pulse_time[1]=800;            //552 - 0град(900мкс), 921 - 30град(1500мкс), 1290 - 60град(2100мкс)
	servo_pulse_time[2]=800;			//520 - 0град(900мкс), 868 - 30град(1500мкс), 1216 - 60град(2100мкс)	<-	current value
	servo_pulse_time[3]=800;
	sorted_index[0]=0;
	sorted_index[1]=1;
	sorted_index[2]=2;
	sorted_index[3]=3;
}

void Servo_Demo(void)
{
	_delay_ms(500);
	servo_pulse_time[1]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[1]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[2]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[2]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[1]=1520;
	servo_pulse_time[2]=1520;
	servo_pulse_time[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[1]=651;
	servo_pulse_time[2]=651;
	servo_pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[1]=651;
	servo_pulse_time[2]=1085;
	servo_pulse_time[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[1]=651;
	servo_pulse_time[2]=651;
	servo_pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[1]=1520;
	servo_pulse_time[2]=1085;
	servo_pulse_time[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_time[1]=651;
	servo_pulse_time[2]=651;
	servo_pulse_time[3]=651;
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
}

/* Port initialization routine */
void Servo_PortInit(void)
{
	SRVDDR1 |= (1<<SRVPIN1);
	SRVDDR2 |= (1<<SRVPIN2);
	SRVDDR3 |= (1<<SRVPIN3);
}

/* Main initialization routine */
void Servo_Init(void)
{
	Servo_PortInit();
	Servo_TimerInit();
	Servo_InitPulses();
	Servo_UpdateArrays();
}
