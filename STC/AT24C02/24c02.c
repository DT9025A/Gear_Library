#include "24c02.h"

/*****************
   I2C总线延时
*****************/
void I2C_Delay() {
    unsigned char i, j;

	_nop_();
	i = 227;
	while (--i);
}

/*****************
   I2C起始条件
*****************/
void I2C_Start() {
    SDA = 1;
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
}

/*****************
   I2C结束条件
*****************/
void I2C_End() {
    SDA = 0;
    SCL = 1;
    I2C_Delay();
    SDA = 1;
    I2C_Delay();
}

/*****************
   I2C 等待从设备ACK
*****************/
void I2C_Ack() {
    unsigned char i = 0;
    SCL = 1;
    I2C_Delay();
    while (SDA == 1 && i < 250)  //防止超时阻塞
        i++;
    SCL = 0;
    I2C_Delay();
}

/*****************
   向总线上写一字节
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
   从总线上读一字节
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
   AT24CO2 写入等待
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
   AT24CO2 写入一字节
   addr:地址
   dat:数据
*****************/
void AT24C02_WriteByte (unsigned char addr, unsigned char dat) {
    I2C_Start();					   //启动总线
    I2C_WriteByte (AT24C02_ADDR + 0);  //写AT24C02地址+写
    I2C_Ack();						   //等待ACK
    I2C_WriteByte (addr);			   //写数据地址
    I2C_Ack();						   //等待ACK
    I2C_WriteByte (dat);			   //写数据
    I2C_Ack();						   //等待ACK
    I2C_End();						   //结束总线
    AT24C02_WRITE_DELAY();			   //等待片内写入完成
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

    I2C_Start();						//启动总线
    I2C_WriteByte (AT24C02_ADDR + 0);	//写AT24C02地址+写
    I2C_Ack();							//等待ACK
    I2C_WriteByte (addr);				//写数据地址
    I2C_Ack();							//等待ACK
    I2C_Start();						//再次启动总线, 准备读
    I2C_WriteByte (AT24C02_ADDR + 1);	//写AT24C02地址+读
    I2C_Ack();							//等待ACK
    dat = I2C_ReadByte();				//读数据
    I2C_End();							//结束总线
    return dat;
}

/*****************

*****************/
void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        * (dat + ptr) = AT24C02_ReadByte (addr + ptr);
}