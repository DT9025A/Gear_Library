/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  iap.c
    作者:    DT9025A
    版本:    R1.0
    日期:    20/8/4
    描述:    STC8G/H系列的片上E2PROM驱动程序
	实验平台:
	         [PASS] STC8G1K08A     @ 11.0592MHz
			 [PASS] STC8H8K64S4U   @ 11.0592MHz
			 [PASS] STC8A8K64S4A12 @ 11.0592MHz
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/8/4    A1.0    编写完成
    DT9025A  20/8/4    R1.0    完善文档注释
***********************************************************/

#include "iap.h"


/***********************************************************************
    函数名:    TRIGIAP
    描述:      触发IAP操作
    调用:      无
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned char]       cmd : 传递的指令
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
bit TRIGIAP (unsigned int addrOffset, unsigned char cmd) {
	bit result;
    addrOffset += FLASH_START_ADDR;
    IAP_ADDRH = addrOffset >> 8;
    IAP_ADDRL = (unsigned char)addrOffset & 0xff;
    EA = 0;
    IAP_CONTR = 0x80;
#ifdef __STC8F_H_
	IAP_CONTR |= IAP_WT;
#else
	IAP_TPS = WAIT_TIME;
#endif
    IAP_CMD = cmd;
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;
    IAP_ADDRH = IAP_ADDRL = IAP_TRIG = IAP_CMD = IAP_CONTR = 0;
    EA = 1;
    result = IAP_CONTR >> 4 & 1;
	IAP_CONTR &= 0xEF;
	return result;
}


/***********************************************************************
    函数名:    ByteProgram
    描述:      编程1字节
    调用:      TRIGIAP
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned char]         x : 数据
    返回值:    void
    其他说明:  该地址必须先被擦除为0xFF再进行写操作
/**********************************************************************/
bit ByteProgram (unsigned int addrOffset, unsigned char x) {
    IAP_DATA = x;
    return TRIGIAP (addrOffset, IAP_IAP_BYTE_PROGRAM);
}


/***********************************************************************
    函数名:    BufProgram
    描述:      编程好多字节
    调用:      ByteProgram
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned int]        cnt : 数据量
               [unsigned char*]      buf : 数据指针
    返回值:    void
    其他说明:  该地址必须先被擦除为0xFF再进行写操作
/**********************************************************************/
void BufProgram (unsigned int addrOffset, unsigned int cnt, unsigned char *buf) {
    unsigned int i;
    for (i = 0; i < cnt; i++)
        ByteProgram (addrOffset + i, buf[i]);
}


/***********************************************************************
    函数名:    ByteRead
    描述:      读取1字节
    调用:      TRIGIAP
    参数:      [unsigned int] addrOffset : 要进行操作的地址
    返回值:    [unsigned char] : 读取到的数据
    其他说明:  无
/**********************************************************************/
unsigned char ByteRead (unsigned int addrOffset) {
    if(TRIGIAP (addrOffset, IAP_IAP_BYTE_READ));
    return IAP_DATA;
}


/***********************************************************************
    函数名:    BufRead
    描述:      读取好多字节
    调用:      ByteRead
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned int]        cnt : 数据量
               [unsigned char*]      buf : 数据指针
    返回值:    void
    其他说明:  无
/**********************************************************************/
void BufRead (unsigned int addrOffset, unsigned int cnt, unsigned char *buf) {
    unsigned int i;
    for (i = 0; i < cnt; i++)
        buf[i] = ByteRead (addrOffset + i);
}


/***********************************************************************
    函数名:    SectorErase
    描述:      擦除扇区
    调用:      TRIGIAP
    参数:      [unsigned char] sectorNumber : 要进行操作的扇区号
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SectorErase (unsigned char sectorNumber) {
    unsigned int addrOffset;

    addrOffset = sectorNumber << 9;
    TRIGIAP (addrOffset, IAP_IAP_SECTOR_ERASE);
}
