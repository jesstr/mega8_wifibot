#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servo.h"
#include "wait.h"

#define SRVPORT1 	PORTD
#define SRVPIN1 	PD3
#define SRVDDR1 	DDRD
#define SRVPORT2 	PORTD
#define SRVPIN2 	PD4
#define SRVDDR2 	DDRD
#define SRVPORT3 	PORTD
#define SRVPIN3 	PD2
#define SRVDDR3		DDRD

unsigned int servo_pulse_width[COUNT_OF_SERVOS+1];	/* array of servo pulse width COUNT_OF_SERVOS servos */

static unsigned int delta_time[4];		//delta_time[n] массив разниц длительностей n-1 сервомашинок по порядку
static unsigned char sorted_index[4];	//упорядоченные индексы массива servo_pulse_width[n]

static void TimerInit(void);
static void InitPulses(void);


/* AVR mega8 Timer2 overflow interrupt service routine */
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
			case 3: SRVPORT3 &= ~(1<<SRVPIN3); break;
		}
	}
	PORTC ^= 0x01;
}

/* Used for sorting pulse width of servos  */
void BubbleSort(unsigned char n)
{
	unsigned char i=0;
	unsigned int Temp;

	while (n > 0) {
		n--;
		i = 0;
		while (i < n) {
			if (servo_pulse_width[(sorted_index[i])] > servo_pulse_width[(sorted_index[i + 1])]) {
				Temp = sorted_index[i + 1];
				sorted_index[i + 1] = sorted_index[i];
				sorted_index[i] = Temp;
			}
			i++;
		}
	}
}

/* Sort pulse width of servos */
void Servo_UpdateArrays(void)
{
	unsigned char i;

	sorted_index[0]=0;
	sorted_index[1]=1;
	sorted_index[2]=2;
	sorted_index[3]=3;
	BubbleSort(COUNT_OF_SERVOS+1);
	for (i=1; i<=COUNT_OF_SERVOS; i++) {
		delta_time[i]=servo_pulse_width[(sorted_index[i])]-servo_pulse_width[(sorted_index[i-1])];
	}
}

/* Initialization pulse width of servos
*  Pulse width of servos are abstract values, not real time.
*  They were computed and collibrated with oscilloscope. */
void InitPulses(void)
{
	servo_pulse_width[0]=0;              //651 - 0град(900мкс), 1085 - 30град(1500мкс), 1520 - 60град(2100мкс)
	servo_pulse_width[1]=800;            //552 - 0град(900мкс), 921 - 30град(1500мкс), 1290 - 60град(2100мкс)
	servo_pulse_width[2]=800;			//520 - 0град(900мкс), 868 - 30град(1500мкс), 1216 - 60град(2100мкс)	<-	current value
	servo_pulse_width[3]=800;
	sorted_index[0]=0;
	sorted_index[1]=1;
	sorted_index[2]=2;
	sorted_index[3]=3;
}

/* AVR mega8 Timer2 initialization */
void TimerInit(void)
{
	TCCR2 |= (1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);
	TCNT2 = 0x00;
	OCR2 = 156; // (~50Гц)
	TIMSK |= (1<<OCIE2);
}

/* Port initialization routine */
void IOInit(void)
{
	SRVDDR1 |= (1<<SRVPIN1);
	SRVDDR2 |= (1<<SRVPIN2);
	SRVDDR3 |= (1<<SRVPIN3);
}

/* Main initialization routine */
void Servo_Init(void)
{
	IOInit();
	TimerInit();
	InitPulses();
	Servo_UpdateArrays();
}

#if 0
void Servo_Demo(void)
{
	_delay_ms(500);
	servo_pulse_width[1]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[1]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[2]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[2]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[1]=1520;
	servo_pulse_width[2]=1520;
	servo_pulse_width[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[1]=651;
	servo_pulse_width[2]=651;
	servo_pulse_width[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[1]=651;
	servo_pulse_width[2]=1085;
	servo_pulse_width[3]=1520;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[1]=651;
	servo_pulse_width[2]=651;
	servo_pulse_width[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[1]=1520;
	servo_pulse_width[2]=1085;
	servo_pulse_width[3]=651;
	Servo_UpdateArrays();
	_delay_ms(500);
	servo_pulse_width[1]=651;
	servo_pulse_width[2]=651;
	servo_pulse_width[3]=651;
	Servo_UpdateArrays();
	_delay_ms(2000);
}
#endif
