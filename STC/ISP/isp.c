/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  adc.c
    作者:    DT9025A
    版本:    R1.0
    日期:    20/7/8
    描述:    STC12系列的片上E2PROM驱动程序
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/1/15   A1.0    编写完成
    DT9025A  20/7/8    R1.0    完善文档注释
***********************************************************/

#include "isp.h"


/***********************************************************************
    函数名:    TRIGISP
    描述:      触发ISP操作
    调用:      无
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned char]       cmd : 传递的指令
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void TRIGISP (unsigned int addrOffset, unsigned char cmd) {
    addrOffset += FLASH_START_ADDR;
    ISP_ADDRH = addrOffset >> 8;
    ISP_ADDRL = (unsigned char)addrOffset & 0xff;
    EA = 0;
    ISP_CONTR = WAIT_TIME | 0x80;
    ISP_CMD = cmd;
    ISP_TRIG = 0x46;
    ISP_TRIG = 0xB9;
    ISP_ADDRH = ISP_ADDRL = ISP_TRIG = ISP_CMD = ISP_CONTR = 0;
    EA = 1;
}


/***********************************************************************
    函数名:    ByteProgram
    描述:      编程1字节
    调用:      TRIGISP
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned char]         x : 数据
    返回值:    void
    其他说明:  该地址必须先被擦除为0xFF再进行写操作
/**********************************************************************/
void ByteProgram (unsigned int addrOffset, unsigned char x) {
    ISP_DATA = x;
    TRIGISP (addrOffset, ISP_IAP_BYTE_PROGRAM);
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
    调用:      TRIGISP
    参数:      [unsigned int] addrOffset : 要进行操作的地址
    返回值:    [unsigned char] : 读取到的数据
    其他说明:  无
/**********************************************************************/
unsigned char ByteRead (unsigned int addrOffset) {
    TRIGISP (addrOffset, ISP_IAP_BYTE_READ);
    return ISP_DATA;
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
    调用:      TRIGISP
    参数:      [unsigned char] sectorNumber : 要进行操作的扇区号
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SectorErase (unsigned char sectorNumber) {
    unsigned int addrOffset;

    addrOffset = sectorNumber << 9;
    TRIGISP (addrOffset, ISP_IAP_SECTOR_ERASE);
}
