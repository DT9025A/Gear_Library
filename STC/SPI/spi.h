/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  spi.h
    作者:    DT9025A
    版本:    R1.0
    日期:    20/7/8
    描述:    STC系列的硬件/软件SPI驱动程序
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/4/5    A1.0    编写完成
    DT9025A  20/7/8    R1.0    完善文档注释
***********************************************************/

#ifndef _SPI_H_
#define _SPI_H_

#include <STC12C5A60S2.h>
#include <intrins.h>


//注释该行为模拟SPI, CPOL=0 CPHA=0
#define HARDWARE_SPI

/*******硬件SPI设置*******/
//SPI主机模式, SLAVE需注释
#define MASTER

//主机带SS, 与包括MASTER定义
//#define MASTERWITHSS

//SPI速度 SPDHH, SPDH, SPDL, SPDLL可选
#define SPI_SPEED    SPDLL

//CPOL|CPHA = 0|0 = 0
#define CPx          0

#ifdef MASTERWITHSS
#define MASTER
sbit SPISS      =   P1 ^ 3;     //SPI slave select, connect to slave' SS(P1.4) pin
#endif

/**************************/

sbit SPIMOSI = P1 ^ 5;
sbit SPIMISO = P1 ^ 6;
sbit SPISCLK = P1 ^ 7;

#define SPIF        0x80        //SPSTAT.7
#define WCOL        0x40        //SPSTAT.6
#define SSIG        0x80        //SPCTL.7
#define SPEN        0x40        //SPCTL.6
#define DORD        0x20        //SPCTL.5
#define MSTR        0x10        //SPCTL.4
#define CPOL        0x08        //SPCTL.3
#define CPHA        0x04        //SPCTL.2
#define SPDHH       0x00        //CPU_CLK/4
#define SPDH        0x01        //CPU_CLK/16
#define SPDL        0x02        //CPU_CLK/64
#define SPDLL       0x03        //CPU_CLK/128


/***********************************************************************
    函数名:    SPI_Init
    描述:      初始化SPI
    调用:      无
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
unsigned char SPI_SendByte (unsigned char dat);

/***********************************************************************
    函数名:    SPI_SendByte
    描述:      向SPI总线发送一字节
    调用:      无
    参数:      [unsigned char] dat : 要发送的字节
    返回值:    [unsigned char] : MISO线上的数据
    其他说明:  SPI全双工, 接收发送一起进行
/**********************************************************************/
void SPI_Init();

#endif
