#include "uart.h"

void Uart_Init (void) {	//115200bps@11.0592MHz
    PCON |= 0x80;		//使能波特率倍速位SMOD
    SCON = 0x50;		//8位数据,可变波特率
    AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
    BRT = 0xFA;         //设定独立波特率发生器重装值
    AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
    AUXR |= 0x10;		//启动独立波特率发生器
    ES = 1;             // 打开接收中断
    PDATAPTR = 0;
}

void UART_SendString (char *s) {
    while (*s) {
        UART_SendData (*s++);
    }
}

void UART_SendData (unsigned char dat) {
    SBUF = dat;
    while (TI == 0);
    TI = 0;
}