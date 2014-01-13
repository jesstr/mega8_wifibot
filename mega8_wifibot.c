/*
 * mega8_wifibot.c
 *
 * Created: 09.12.2012 19:19:45
 *  Author: jesstr
 */ 

//#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "servo.h"
#include "motor.h"
#include "wait.h"

#define UART_RX_BUFF_SIZE	32	// Ðàçìåð áóôåðà ïðèåìà UART
#define UART_LEX_MASS_SIZE	4	// Ðàçìåð ìàññèâà ëåêñåì
char uart_rx_buff[UART_RX_BUFF_SIZE];	// Áóôåð ïðèåìà UART
char uart_rx_packet[UART_RX_BUFF_SIZE];	// Ïðèíÿòàÿ ïî UART ïîñûëêà 
//char lex[UART_LEX_MASS_SIZE][UART_RX_BUFF_SIZE];	// Ìàññèâ ëåêñåì
char *lex_p[UART_LEX_MASS_SIZE];	// Ìàññèâ óêàçàòåëåé íà ëåêñåìû
char *command = NULL;
unsigned char n_butes = 0;	// Ñ÷åò÷èê ïðèíÿòûõ ïî UART áàéò
unsigned char lex_n = 0;	// Ñ÷åò÷èê ëåêñåì

unsigned char global_state = 0; // Ïåðåìåííàÿ ôëàãîâ ñîñòîÿíèÿ

#define UART_rx_complete_bit 0 // Ôëàã ïðèåìà UART ïîñûëêè  
#define UART_buffoverflow_bit 1 // Ôëàã ïåðåïîëíåíèÿ áöôåðà ïðèåìà UART  
#define UART_wrong_package_bit 2 // Ôëàã ïîòåðÿííîãî(ûõ) áàéòà(îâ) UART ïîñûëêè

#define IS_NEW_COMMAND global_state&(1<<UART_rx_complete_bit) // Ïðîâåðêà, íåò ëè íîâîé êîìàíäû äëÿ îáðàáîòêè

#define COMMAND_DONE { global_state &= ~(1<<UART_rx_complete_bit); lex_n = 0; } // Ñáðîñ ôëàãà íîâîé êîìàíäû ïîñëå îáðàáîòêè, ñáðîñ èíäåêñà ìàññèâà ëåêñåì


void Turret_Run(unsigned int hor_pos, unsigned int vert_pos)
{
	if ((SERVO_MAX_PULSE_TIME * 2 + 1 > hor_pos + vert_pos) && (hor_pos + vert_pos > SERVO_MIN_PULSE_TIME * 2 - 1)) {
		servo_pulse_time[TURR_HOR_SERVO] = hor_pos;
		servo_pulse_time[TURR_VERT_SERVO] = vert_pos;
		Servo_UpdateArrays();
	}
}

// Îáðàáîòêà ïðåðûâàíèÿ ïî ïðèåìó áàéòà ïî UART (ïîìåùàåòñÿ â ãëàâíûé ìîäóëü)
ISR(USART_RXC_vect)
{
	unsigned char buff=UDR;

	if ((n_butes < UART_RX_BUFF_SIZE - 1) && (buff != 0x0A)) {
		uart_rx_buff[n_butes++] = buff;
	}
	else
	{
		if (n_butes >= UART_RX_BUFF_SIZE)
		{
			global_state |= (1<<UART_buffoverflow_bit);
		}
		else
		{
			if ((global_state & (1<<UART_rx_complete_bit)) == 0) // åñëè ïðåäûäóùàÿ ïîñûëêà îáðàáîòàíà
			{
				uart_rx_buff[n_butes] = 0;
				global_state |= (1<<UART_rx_complete_bit);
				global_state &= ~(1<<UART_wrong_package_bit);
				strcpy(uart_rx_packet, uart_rx_buff);
			}			
			else
			{
				global_state |= (1<<UART_wrong_package_bit); //èíà÷å òåðÿåì ïðèøåäøèé ïàêåò
			}			
		}		
	n_butes = 0;
	}
}

/* AVR mega8 Timer0 overflow interrupt service routine */
ISR(TIMER0_OVF_vect)
{
	static unsigned char val;
	if (val < MOTOR_TIMER_IRQsPerPeriod) {
		val++;
	}
	else {
		Motor_TimerTickCount++;
		val = 0;
	}
	if (Motor_TimerTickCount >= MOTOR_TIMER_KeyPressedRunTime) {
		MOTOR_TimerStop;
	}
}

int main(void)
{
	/* Power supply for UART2COM adapter on PD2 */
	DDRD |= (1<<PD2);
	PORTD |= (1<<PD2);

	UART_Init(MYUBRR);
	MOTOR_INIT;
	Servo_TimerInit();
	sei();
    while(1) {
		if (IS_NEW_COMMAND) {
			command = strtok(uart_rx_packet, "=,");
			lex_p[lex_n++] = command;
			while( (command = strtok(NULL, "=,")) ) {
				lex_p[lex_n++] = command;
			}
			if (strcmp(lex_p[0], "DrvLR") == 0) {
				//Motor_DirectRun(atoi(lex_p[1]), atoi(lex_p[2]));
				Motor_TimerTickCount = 0;
				COMMAND_DONE;
			}
			else if (strcmp(lex_p[0], "TurrHV") == 0) {
				Turret_Run(atoi(lex_p[1]), atoi(lex_p[2]));
				COMMAND_DONE;
			}
			else if (strcmp(lex_p[0], "pong") == 0) {
				COMMAND_DONE;
			}
			else COMMAND_DONE;
		}
		_delay_us(2);
	}
}
