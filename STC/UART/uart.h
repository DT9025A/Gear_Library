#ifndef _UART_H_
#define _UART_H_

#include <STC12C5A60S2.H>

void UART_Init (void);
void UART_SendString (char *s);
void UART_SendData (unsigned char dat);

#endif