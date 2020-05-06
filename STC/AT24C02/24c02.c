#include "24c02.h"

void delay() {
    unsigned char i, j;

	_nop_();
	i = 227;
	while (--i);
}

void I2C_Start() {
    SDA = 1;
    SCL = 1;
    delay();
    SDA = 0;
    delay();
}

void I2C_End() {
    SDA = 0;
    SCL = 1;
    delay();
    SDA = 1;
    delay();
}

void I2C_Ack() {
    unsigned char i = 0;
    SCL = 1;
    delay();
    while (SDA == 1 && i < 250)
        i++;
    SCL = 0;
    delay();
}

void I2C_WriteByte (unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        dat = dat << 1;
        SCL = 0;
        delay();
        SDA = CY;
        delay();
        SCL = 1;
        delay();
    }
    SCL = 0;
    SDA = 1;
    delay();
}

unsigned char I2C_ReadByte() {
    unsigned char dat = 0, i;
    SCL = 0;
    delay();
    for (i = 0; i < 8; i++) {
        SCL = 1;
        delay();
        dat = dat << 1;
        dat = dat | SDA;
        delay();
        SCL = 0;
        delay();
    }
    return dat;
}


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

void AT24C02_WriteByte (unsigned char addr, unsigned char dat) {
    I2C_Start();
    I2C_WriteByte (AT24C02_ADDR + 0);
    I2C_Ack();
    I2C_WriteByte (addr);
    I2C_Ack();
    I2C_WriteByte (dat);
    I2C_Ack();
    I2C_End();
    AT24C02_WRITE_DELAY();
}

void AT24C02_WriteBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        AT24C02_WriteByte (addr + ptr, * (dat + ptr));
}

unsigned char AT24C02_ReadByte (unsigned char addr) {
    unsigned char dat;

    I2C_Start();
    I2C_WriteByte (AT24C02_ADDR + 0);
    I2C_Ack();
    I2C_WriteByte (addr);
    I2C_Ack();
    I2C_Start();
    I2C_WriteByte (AT24C02_ADDR + 1);
    I2C_Ack();
    dat = I2C_ReadByte();
    I2C_End();
    return dat;
}

void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        * (dat + ptr) = AT24C02_ReadByte (addr + ptr);
}