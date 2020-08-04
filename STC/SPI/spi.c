/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  spi.c
    作者:    DT9025A
    版本:    R1.0
    日期:    20/7/8
    描述:    STC系列的硬件/软件SPI驱动程序
	         部分配置通过unsigned char变量存储, 可能占据额外空间.
			 若空间紧张可将其通过define替换
			 本驱动在 STC12C5A60S2@12.0MHz 和 STC8A8K64S4A12@12.0MHz平台试验通过
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/4/5    A1.0    编写完成
    DT9025A  20/7/8    R1.0    完善文档注释
	DT9025A  20/7/9    R1.1    更改一些define为变量, 以适应STC8系列的可变端口
***********************************************************/
#include "spi.h"

#ifdef HARDWARE_SPI

/***********************************************************************
    函数名:    SPI_Init
    描述:      初始化SPI(硬件)
    调用:      无
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SPI_Init() {
#ifdef __STC8F_H_
    P_SW1 |= SPI_S << 1;        //SPI pin select
    SPDAT = 0;                  //initial SPI data
    SPSTAT = SPIF | WCOL;       //clear SPI status
#ifdef MASTER
    SPCTL = SPEN | MSTR | SPI_SPEED | CPx;        //master mode
#else
    SPCTL = SPEN;                                 //slave mode
#endif

}

/***********************************************************************
    函数名:    SPI_SendByte
    描述:      向SPI总线发送一字节(硬件)
    调用:      无
    参数:      [unsigned char] dat : 要发送的字节
    返回值:    [unsigned char] : MISO线上的数据
    其他说明:  SPI全双工, 接收发送一起进行
/**********************************************************************/
unsigned char SPI_SendByte (unsigned char dat) {
#ifdef MASTERWITHSS
    SPISS = 0;                  //pull low slave SS
#endif
    SPDAT = dat;                //trigger SPI send
    while (! (SPSTAT & SPIF));  //wait send complete
    SPSTAT = SPIF | WCOL;       //clear SPI status
#ifdef MASTERWITHSS
    SPISS = 1;                  //push high slave SS
#endif
    return SPDAT;               //return received SPI data
}

#else

/***********************************************************************
    函数名:    SPI_Init
    描述:      初始化SPI(软件)
    调用:      无
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SPI_Init() {
    SPIMOSI = 0;
    SPISCLK = 0;
}

/***********************************************************************
    函数名:    SPI_SendByte
    描述:      向SPI总线发送一字节(软件)
    调用:      无
    参数:      [unsigned char] dat : 要发送的字节
    返回值:    [unsigned char] : MISO线上的数据
    其他说明:  SPI全双工, 接收发送一起进行
/**********************************************************************/
unsigned char SPI_SendByte (unsigned char dat) {
    unsigned char temp, r = 0;
    SPISCLK = 0;
    for (temp = 0x80; temp != 0; temp >>= 1) {
        if (temp & dat == 0) {
            SPIMOSI = 0;
        } else {
            SPIMOSI = 1;
        }
        _nop_();
        _nop_();
        SPISCLK = 1;

        _nop_();
        _nop_();
        SPISCLK = 0;
        if (SPIMISO == 1) {
            r |= temp;
        } else {
            r &= ~temp;
        }
    }
    return r;
}

#endif
