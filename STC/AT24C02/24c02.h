#ifndef _24C02_H_
#define _24C02_H_

#include <STC89C5xRC.H>
#include <intrins.h>

//AT24C02的通信地址
#define AT24C02_ADDR 0xAE

sbit SDA = P2 ^ 1;
sbit SCL = P2 ^ 0;

void AT24C02_WriteByte (unsigned char addr, unsigned char dat);
void AT24C02_WriteBuf (unsigned char addr, unsigned char *dat, unsigned char count);
unsigned char AT24C02_ReadByte (unsigned char addr);
void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count);

#endif