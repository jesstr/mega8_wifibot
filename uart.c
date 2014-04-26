#include "avr/io.h"
#include "uart.h"

#define BAUDRATE 38400
#define MYUBRR F_CPU/16/BAUDRATE-1

/* UART initialization */
void UART_Init(unsigned int ubrr1)
{
	/* UBRRH = (unsigned char)(ubrr1>>8); */	/* Setting operation frequency hight byte */
	UBRRL = (unsigned char) ubrr1;				/* Setting operation frequency low byte */
	UCSRB = (1<<RXEN)|(1<<TXEN); 				/* RX enabled, TX enabled */
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	/* Asynchronous mode, 8 bit, 1 stop bit, no parity check */
	UCSRB |= (1<<RXCIE);						/* RX interrupt enabled */
}

/* Send a byte over UART */
void UART_SendByte(unsigned char byte1)
{
	while (!( UCSRA & (1<<UDRE)));
	UDR = byte1;
};

/* Send text string over UART */
void UART_SendString(char *buffer)
{
	while (*buffer!=0) {
		 UART_SendByte(*buffer++);
		 //buffer++;
	}
}

/* Send data over UART */
void UART_SendData(char *buffer, unsigned short nbytes)
{
	while (nbytes > 0) {
		UART_SendByte(*buffer++);
		nbytes--;
	}
}

/* RX Interrupt routine (has to be moved to the main file) */
/*
ISR(USART1_RX_vect)
{
	unsigned char buff=UDR1;
}
*/
