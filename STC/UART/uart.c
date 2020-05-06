#include "uart.h"

void Uart_Init (void) {	//115200bps@11.0592MHz
    PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
    SCON = 0x50;		//8λ����,�ɱ䲨����
    AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
    BRT = 0xFA;         //�趨���������ʷ�������װֵ
    AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
    AUXR |= 0x10;		//�������������ʷ�����
    ES = 1;             // �򿪽����ж�
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