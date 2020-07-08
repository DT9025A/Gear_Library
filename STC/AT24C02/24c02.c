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

#include "24c02.h"

/***********************************************************************
    ������:    I2C_Delay
    ����:      I2C������ʱ
    ����:      ��
    ����:      void
    ����ֵ:    void
    ����˵��:  �ڲ�����
/**********************************************************************/
void I2C_Delay() {
    unsigned char i, j;

    _nop_();
    i = 227;
    while (--i);
}


/***********************************************************************
    ������:    I2C_Start
    ����:      ����I2C������ʼ����
    ����:      I2C_Delay
    ����:      void
    ����ֵ:    void
    ����˵��:  �ڲ�����
/**********************************************************************/
void I2C_Start() {
    SDA = 1;
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
}


/***********************************************************************
    ������:    I2C_End
    ����:      ����I2C���߽�������
    ����:      I2C_Delay
    ����:      void
    ����ֵ:    void
    ����˵��:  �ڲ�����
/**********************************************************************/
void I2C_End() {
    SDA = 0;
    SCL = 1;
    I2C_Delay();
    SDA = 1;
    I2C_Delay();
}


/***********************************************************************
    ������:    I2C_Ack
    ����:      �ȴ�ACK�ź�
    ����:      I2C_Delay
    ����:      void
    ����ֵ:    void
    ����˵��:  �ڲ�����
/**********************************************************************/
void I2C_Ack() {
    unsigned char i = 0;
    SCL = 1;
    I2C_Delay();
    while (SDA == 1 && i < 250)  //��ֹ��ʱ����
        i++;
    SCL = 0;
    I2C_Delay();
}


/***********************************************************************
    ������:    I2C_WriteByte
    ����:      ���豸����ACK�ź�
    ����:      I2C_Delay
               ʹ����CYλ
    ����:      void
    ����ֵ:    void
    ����˵��:  ��
/**********************************************************************/
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


/***********************************************************************
    ������:    I2C_ReadByte
    ����:      �������϶�һ�ֽ�
    ����:      I2C_Delay
               ʹ����CYλ
    ����:      void
    ����ֵ:    [unsigned char] : ����������
    ����˵��:  ��
/**********************************************************************/
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


/***********************************************************************
    ������:    AT24C02_WRITE_DELAY
    ����:      ��ʱ�ȴ�AT24C02д��
    ����:      ��
    ����:      void
    ����ֵ:    void
    ����˵��:  �ڲ�����
/**********************************************************************/
void AT24C02_WRITE_DELAY() {    //STC Y1@12.000MHz@1ms
    unsigned char i, j;

    _nop_();
    i = 4;
    j = 146;
    do
    {
        while (--j);
    } while (--i);
}


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
void AT24C02_WriteByte (unsigned char addr, unsigned char dat) {
    I2C_Start();                       //��������
    I2C_WriteByte (AT24C02_ADDR + 0);  //дAT24C02��ַ+д
    I2C_Ack();                           //�ȴ�ACK
    I2C_WriteByte (addr);               //д���ݵ�ַ
    I2C_Ack();                           //�ȴ�ACK
    I2C_WriteByte (dat);               //д����
    I2C_Ack();                           //�ȴ�ACK
    I2C_End();                           //��������
    AT24C02_WRITE_DELAY();               //�ȴ�Ƭ��д�����
}


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
void AT24C02_WriteBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        AT24C02_WriteByte (addr + ptr, * (dat + ptr));
}


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
unsigned char AT24C02_ReadByte (unsigned char addr) {
    unsigned char dat;

    I2C_Start();                        //��������
    I2C_WriteByte (AT24C02_ADDR + 0);    //дAT24C02��ַ+д
    I2C_Ack();                            //�ȴ�ACK
    I2C_WriteByte (addr);                //д���ݵ�ַ
    I2C_Ack();                            //�ȴ�ACK
    I2C_Start();                        //�ٴ���������, ׼����
    I2C_WriteByte (AT24C02_ADDR + 1);    //дAT24C02��ַ+��
    I2C_Ack();                            //�ȴ�ACK
    dat = I2C_ReadByte();                //������
    I2C_End();                            //��������
    return dat;
}


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
void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        * (dat + ptr) = AT24C02_ReadByte (addr + ptr);
}
