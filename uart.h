
#ifndef _UART_H_
#define _UART_H_

#define BAUDRATE 38400
#define MYUBRR F_CPU/16/BAUDRATE-1


void UART_Init(unsigned int ubrr1);

void UART_SendByte(unsigned char byte1);

void UART_SendString(char *buffer);

#endif /* _UART_H_ */
