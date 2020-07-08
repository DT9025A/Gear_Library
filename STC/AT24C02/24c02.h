/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  24c02.c
    作者:    DT9025A
    版本:    R 1.0
    日期:    20/7/7
    描述:    AT24C02系列E2PROM驱动
             I2C协议在内部实现
             本驱动在 STC12C5A60S2@12.0MHz 和 STC89C54RD+@12.0MHz 平台试验通过
             当总线通信出现问题时可尝试修改I2C_Delay函数和AT24C02_WRITE_DELAY函数
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/2/15   A1.0    编写完成
    DT9025A  20/7/7    R1.0    完善文档注释
***********************************************************/

#ifndef _24C02_H_
#define _24C02_H_

#include <STC89C5xRC.H>
#include <intrins.h>

//AT24C02的通信地址
#define AT24C02_ADDR 0xAE

sbit SDA = P2 ^ 1;
sbit SCL = P2 ^ 0;

/***********************************************************************
    函数名:    AT24C02_WriteByte
    描述:      AT24C02写入一字节
    调用:      I2C_Start
               I2C_WriteByte
               I2C_Ack
               I2C_End
               AT24C02_WRITE_DELAY
    参数:      [unsigned char] addr : 写入地址
               [unsigned char] data : 写入数据
    返回值:    void
    其他说明:  无
/**********************************************************************/
void AT24C02_WriteByte (unsigned char addr, unsigned char dat);

/***********************************************************************
    函数名:    AT24C02_WriteBuf
    描述:      AT24C02写入好多字节
    调用:      AT24C02_WriteByte
    参数:      [unsigned char]  addr : 写入地址
               [unsigned char*] data : 写入数据存放指针
               [unsigned char] count : 写入数据个数
    返回值:    void
    其他说明:  无
/**********************************************************************/
void AT24C02_WriteBuf (unsigned char addr, unsigned char *dat, unsigned char count);

/***********************************************************************
    函数名:    AT24C02_ReadByte
    描述:      AT24C02读取一字节
    调用:      I2C_Start
               I2C_WriteByte
               I2C_Ack
               I2C_ReadByte
               I2C_End
    参数:      [unsigned char] addr : 要读取的地址
    返回值:    [unsigned char] : 读出的数据
    其他说明:  无
/**********************************************************************/
unsigned char AT24C02_ReadByte (unsigned char addr);

/***********************************************************************
    函数名:    AT24C02_ReadBuf
    描述:      AT24C02读取好多字节
    调用:      AT24C02_ReadByte
    参数:      [unsigned char]  addr : 要读取的地址
               [unsigned char*] data : 读出数据存放指针
               [unsigned char] count : 读出数据个数
    返回值:    void
    其他说明:  data须预先申请
/**********************************************************************/
void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count);

#endif