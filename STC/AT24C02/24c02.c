#include "24c02.h"

/*****************
   I2C������ʱ
*****************/
void I2C_Delay() {
    unsigned char i, j;

	_nop_();
	i = 227;
	while (--i);
}

/*****************
   I2C��ʼ����
*****************/
void I2C_Start() {
    SDA = 1;
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
}

/*****************
   I2C��������
*****************/
void I2C_End() {
    SDA = 0;
    SCL = 1;
    I2C_Delay();
    SDA = 1;
    I2C_Delay();
}

/*****************
   I2C �ȴ����豸ACK
*****************/
void I2C_Ack() {
    unsigned char i = 0;
    SCL = 1;
    I2C_Delay();
    while (SDA == 1 && i < 250)  //��ֹ��ʱ����
        i++;
    SCL = 0;
    I2C_Delay();
}

/*****************
   ��������дһ�ֽ�
*****************/
void I2C_WriteByte (unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        dat = dat << 1;
        SCL = 0;
        I2C_Delay();
        SDA = CY;
        I2C_Delay();
        SCL = 1;
        I2C_Delay();
    }
    SCL = 0;
    SDA = 1;
    I2C_Delay();
}

/*****************
   �������϶�һ�ֽ�
*****************/
unsigned char I2C_ReadByte() {
    unsigned char dat = 0, i;
    SCL = 0;
    I2C_Delay();
    for (i = 0; i < 8; i++) {
        SCL = 1;
        I2C_Delay();
        dat = dat << 1;
        dat = dat | SDA;
        I2C_Delay();
        SCL = 0;
        I2C_Delay();
    }
    return dat;
}

/*****************
   AT24CO2 д��ȴ�
*****************/
void AT24C02_WRITE_DELAY() {	//STC Y1@12.000MHz@1ms
    unsigned char i, j;

	_nop_();
	i = 4;
	j = 146;
	do
	{
		while (--j);
	} while (--i);
}

/*****************
   AT24CO2 д��һ�ֽ�
   addr:��ַ
   dat:����
*****************/
void AT24C02_WriteByte (unsigned char addr, unsigned char dat) {
    I2C_Start();					   //��������
    I2C_WriteByte (AT24C02_ADDR + 0);  //дAT24C02��ַ+д
    I2C_Ack();						   //�ȴ�ACK
    I2C_WriteByte (addr);			   //д���ݵ�ַ
    I2C_Ack();						   //�ȴ�ACK
    I2C_WriteByte (dat);			   //д����
    I2C_Ack();						   //�ȴ�ACK
    I2C_End();						   //��������
    AT24C02_WRITE_DELAY();			   //�ȴ�Ƭ��д�����
}

/*****************

*****************/
void AT24C02_WriteBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        AT24C02_WriteByte (addr + ptr, * (dat + ptr));
}

/*****************

*****************/
unsigned char AT24C02_ReadByte (unsigned char addr) {
    unsigned char dat;

    I2C_Start();						//��������
    I2C_WriteByte (AT24C02_ADDR + 0);	//дAT24C02��ַ+д
    I2C_Ack();							//�ȴ�ACK
    I2C_WriteByte (addr);				//д���ݵ�ַ
    I2C_Ack();							//�ȴ�ACK
    I2C_Start();						//�ٴ���������, ׼����
    I2C_WriteByte (AT24C02_ADDR + 1);	//дAT24C02��ַ+��
    I2C_Ack();							//�ȴ�ACK
    dat = I2C_ReadByte();				//������
    I2C_End();							//��������
    return dat;
}

/*****************

*****************/
void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        * (dat + ptr) = AT24C02_ReadByte (addr + ptr);
}