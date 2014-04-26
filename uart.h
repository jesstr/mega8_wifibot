
#ifndef _UART_H_
#define _UART_H_

/* UART initialization */
void UART_Init(unsigned int ubrr1);
/* Send a byte over UART */
void UART_SendByte(unsigned char byte1);
/* Send text string over UART */
void UART_SendString(char *buffer);
/* Send data over UART */
void UART_SendData(char *buffer, unsigned short nbytes);

#endif /* _UART_H_ */
