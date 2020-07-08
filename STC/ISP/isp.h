/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  isp.h
    作者:     DT9025A
    版本:    R1.0
    日期:    20/7/8
    描述:    STC12系列的片上E2PROM驱动程序
             本驱动在 STC12C5A60S2@12.0MHz 平台试验通过
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/1/15   A1.0    编写完成
    DT9025A  20/7/8    R1.0    完善文档注释
***********************************************************/

#ifndef __STC12C2052_ISP_H_
#define __STC12C2052_ISP_H_

#include <STC12C2052AD.H>

//等待时间 具体参照数据手册
#define WAIT_TIME 3
//E2PROM起始地址 具体参照数据手册
#define FLASH_START_ADDR 0x1000

#define ISP_IAP_BYTE_READ 1
#define ISP_IAP_BYTE_PROGRAM 2
#define ISP_IAP_SECTOR_ERASE 3

/***********************************************************************
    函数名:    ByteProgram
    描述:      编程1字节
    调用:      TRIGISP
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned char]         x : 数据
    返回值:    void
    其他说明:  该地址必须先被擦除为0xFF再进行写操作
/**********************************************************************/
void ByteProgram (unsigned int addrOffset, unsigned char x);

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
void BufProgram (unsigned int addrOffset, unsigned int cnt, unsigned char *x);

/***********************************************************************
    函数名:    ByteRead
    描述:      读取1字节
    调用:      TRIGISP
    参数:      [unsigned int] addrOffset : 要进行操作的地址
    返回值:    [unsigned char] : 读取到的数据
    其他说明:  无
/**********************************************************************/
unsigned char ByteRead (unsigned int addrOffset);

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
void BufRead (unsigned int addrOffset, unsigned int cnt, unsigned char *buf);

/***********************************************************************
    函数名:    SectorErase
    描述:      擦除扇区
    调用:      TRIGISP
    参数:      [unsigned char] sectorNumber : 要进行操作的扇区号
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SectorErase(unsigned char sectorNumber);

#endif
