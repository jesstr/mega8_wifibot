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

// Íàçíà÷åíèå âûâîäîâ äëÿ äðàéâåðà äâèãàòåëåé
#define CE1_PIN		PD6
#define CE1_PORT 	PORTD
#define CE1_DDR 	DDRD

#define CE2_PIN		PD6
#define CE2_PORT 	PORTD
#define CE2_DDR 	DDRD

#define INPUT1_PIN	PB1
#define INPUT1_PORT PORTB
#define INPUT1_DDR 	DDRB

#define INPUT2_PIN	PB2
#define INPUT2_PORT PORTB
#define INPUT2_DDR 	DDRB

#define INPUT3_PIN	PB0
#define INPUT3_PORT PORTB
#define INPUT3_DDR 	DDRB

#define INPUT4_PIN	PD7
#define INPUT4_PORT PORTD
#define INPUT4_DDR 	DDRD

// Ìàêðîñû äëÿ äðàéâåðà äâèãàòåëåé
#define CHIP1_DISABLE	CE1_PORT &= ~(1<<CE1_PIN);
#define CHIP2_DISABLE	CE2_PORT &= ~(1<<CE2_PIN);

#define CHIP1_ENABLE	CE1_PORT |= (1<<CE1_PIN);
#define CHIP2_ENABLE	CE2_PORT |= (1<<CE2_PIN);

#define LEFT_DISABLE	CHIP1_DISABLE
#define LEFT_ENABLE		CHIP1_ENABLE
#define RIGHT_DISABLE	CHIP2_DISABLE
#define RIGHT_ENABLE	CHIP2_ENABLE

#define BOTH_ENABLE		do { 			\
						LEFT_ENABLE;	\
						RIGHT_ENABLE;	\
						} while(0)

#define BOTH_DISABLE	do { 			\
						LEFT_DISABLE; 	\
						RIGHT_DISABLE; 	\
						} while(0)

#define FORWARD			do { 								\
						INPUT1_PORT &= ~(1<<INPUT1_PIN);	\
						INPUT2_PORT |= (1<<INPUT2_PIN);		\
						INPUT3_PORT |= (1<<INPUT3_PIN);		\
						INPUT4_PORT &= ~(1<<INPUT4_PIN);	\
						} while(0)
						
#define BACKWARD		do { 							\
						INPUT2_PORT &= ~(1<<INPUT2_PIN);	\
						INPUT1_PORT |= (1<<INPUT1_PIN);	\
						INPUT4_PORT |= (1<<INPUT4_PIN);	\
						INPUT3_PORT &= ~(1<<INPUT3_PIN);	\
						} while(0)
						
#define RIGHT			do { 							\
						INPUT1_PORT &= ~(1<<INPUT1_PIN);	\
						INPUT2_PORT |= (1<<INPUT2_PIN);	\
						INPUT4_PORT |= (1<<INPUT4_PIN);	\
						INPUT3_PORT &= ~(1<<INPUT3_PIN);	\
						} while(0)
						
#define LEFT			do {							\
						INPUT2_PORT &= ~(1<<INPUT2_PIN);	\
						INPUT1_PORT |= (1<<INPUT1_PIN);	\
						INPUT3_PORT |= (1<<INPUT3_PIN);	\
						INPUT4_PORT &= ~(1<<INPUT4_PIN);	\
						} while(0)
						
#define BRAKE			do {							\
						INPUT2_PORT |= (1<<INPUT2_PIN);	\
						INPUT1_PORT |= (1<<INPUT1_PIN);	\
						INPUT4_PORT |= (1<<INPUT4_PIN);	\
						INPUT3_PORT |= (1<<INPUT3_PIN);	\
						} while(0)
						
#define MOTOR_INIT		do {							\
						BOTH_DISABLE;					\
						CE1_DDR |= (1<<CE1_PIN);			\
						CE2_DDR |= (1<<CE2_PIN);			\
						INPUT1_DDR |= (1<<INPUT1_PIN);	\
						INPUT2_DDR |= (1<<INPUT2_PIN);	\
						INPUT3_DDR |= (1<<INPUT3_PIN);	\
						INPUT4_DDR |= (1<<INPUT4_PIN);	\
						} while(0)

#define MOTOR_FORWARDRUN	do {			\
							FORWARD;		\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_BACKWARDRUN	do {			\
							BACKWARD;		\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_RIGHT			do { 			\
							RIGHT;			\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_LEFT			do {			\
							LEFT;			\
							BOTH_ENABLE;	\
							} while(0)

#define MOTOR_FREEWHEEL		BOTH_DISABLE;

#define MOTOR_BRAKE			do { 			\
							BRAKE;			\
							_delay_ms(100);	\
							BOTH_DISABLE;	\
							} while(0)

void wait_ms(short ms)
{
	while(ms > 0)
	{
		_delay_ms(1);
		ms--;		
	}	
}

/*
inline void MotorDriver_Init(void)
{
	BOTH_DISABLE;
	CE1_DDR|=(1<<CE1_PIN);
	CE2_DDR|=(1<<CE2_PIN);
	
	INPUT1_DDR|=(1<<INPUT1_PIN);
	INPUT2_DDR|=(1<<INPUT2_PIN);
	INPUT3_DDR|=(1<<INPUT3_PIN);
	INPUT4_DDR|=(1<<INPUT4_PIN);
}


inline void MotorDriver_ForwardRun(void)
{
	FORWARD;
	BOTH_ENABLE;
}


inline void MotorDriver_BackwardRun(void)
{
	BACKWARD;
	BOTH_ENABLE;
}


inline void MotorDriver_Right(void)
{
	RIGHT;
	BOTH_ENABLE;
}


inline void MotorDriver_Left(void)
{
	LEFT;
	BOTH_ENABLE;
}


inline void MotorDriver_FreeStop(void)
{
	BOTH_DISABLE;
}


inline void MotorDriver_Brake(void)
{
	BRAKE;
	_delay_ms(100);
	BOTH_DISABLE;
}
*/

void Motor_Run(int left, int right)
{
	if (left && right && left - right == 2 * left)
	{
		if (left > 0) MOTOR_RIGHT;
		else MOTOR_LEFT;
	}	
	else if (left && right && left - right == 0)
	{
		if (left > 0) MOTOR_FORWARDRUN;
		else MOTOR_BACKWARDRUN;
	}
	if (left < 0) {
		left = 0 - left;
	}
	wait_ms(left);
	MOTOR_FREEWHEEL;
}

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
				Motor_Run(atoi(lex_p[1]), atoi(lex_p[2]));
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
