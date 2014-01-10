#include "avr/io.h"
#include "uart.h"

void UART_Init(unsigned int ubrr1)
{
	//UBRRH = (unsigned char)(ubrr1>>8);	// ������� ������� ������
	UBRRL = (unsigned char) ubrr1;
	UCSRB = (1<<RXEN)|(1<<TXEN); // ���������� �������, �������� �������, ���������� �� ������ ��������
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	// ����������� �����, 8 ���, 1 ����-���, �������� �������� ����������
	UCSRB |= (1<<RXCIE);
}


void UART_SendByte(unsigned char byte1)
{
	while (!( UCSRA & (1<<UDRE)));
	UDR = byte1;
};


void UART_SendString(char *buffer)
{
	while (*buffer!=0)
	{
		 UART_SendByte(*buffer);
		 buffer++;
	}
}

/*
// ��������� ���������� �� ������ ����� �� UART (���������� � ������� ������)
ISR(USART1_RX_vect)
{
	unsigned char buff=UDR1;
}
*/
