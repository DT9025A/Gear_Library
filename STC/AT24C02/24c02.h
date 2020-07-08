/************************************************************
    Copyright (C), 2020, DT9025A
    �ļ���:  24c02.c
    ����:    DT9025A
    �汾:    R 1.0
    ����:    20/7/7
    ����:    AT24C02ϵ��E2PROM����
             I2CЭ�����ڲ�ʵ��
             �������� STC12C5A60S2@12.0MHz �� STC89C54RD+@12.0MHz ƽ̨����ͨ��
             ������ͨ�ų�������ʱ�ɳ����޸�I2C_Delay������AT24C02_WRITE_DELAY����
    �޶���ʷ:
    <����>   <ʱ��>   <�汾>   <����>
    DT9025A  20/2/15   A1.0    ��д���
    DT9025A  20/7/7    R1.0    �����ĵ�ע��
***********************************************************/

#ifndef _24C02_H_
#define _24C02_H_

#include <STC89C5xRC.H>
#include <intrins.h>

//AT24C02��ͨ�ŵ�ַ
#define AT24C02_ADDR 0xAE

sbit SDA = P2 ^ 1;
sbit SCL = P2 ^ 0;

/***********************************************************************
    ������:    AT24C02_WriteByte
    ����:      AT24C02д��һ�ֽ�
    ����:      I2C_Start
               I2C_WriteByte
               I2C_Ack
               I2C_End
               AT24C02_WRITE_DELAY
    ����:      [unsigned char] addr : д���ַ
               [unsigned char] data : д������
    ����ֵ:    void
    ����˵��:  ��
/**********************************************************************/
void AT24C02_WriteByte (unsigned char addr, unsigned char dat);

/***********************************************************************
    ������:    AT24C02_WriteBuf
    ����:      AT24C02д��ö��ֽ�
    ����:      AT24C02_WriteByte
    ����:      [unsigned char]  addr : д���ַ
               [unsigned char*] data : д�����ݴ��ָ��
               [unsigned char] count : д�����ݸ���
    ����ֵ:    void
    ����˵��:  ��
/**********************************************************************/
void AT24C02_WriteBuf (unsigned char addr, unsigned char *dat, unsigned char count);

/***********************************************************************
    ������:    AT24C02_ReadByte
    ����:      AT24C02��ȡһ�ֽ�
    ����:      I2C_Start
               I2C_WriteByte
               I2C_Ack
               I2C_ReadByte
               I2C_End
    ����:      [unsigned char] addr : Ҫ��ȡ�ĵ�ַ
    ����ֵ:    [unsigned char] : ����������
    ����˵��:  ��
/**********************************************************************/
unsigned char AT24C02_ReadByte (unsigned char addr);

/***********************************************************************
    ������:    AT24C02_ReadBuf
    ����:      AT24C02��ȡ�ö��ֽ�
    ����:      AT24C02_ReadByte
    ����:      [unsigned char]  addr : Ҫ��ȡ�ĵ�ַ
               [unsigned char*] data : �������ݴ��ָ��
               [unsigned char] count : �������ݸ���
    ����ֵ:    void
    ����˵��:  data��Ԥ������
/**********************************************************************/
void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count);

#endif