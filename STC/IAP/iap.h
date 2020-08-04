/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  iap.h
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

#ifndef __STC_IAP_H_
#define __STC_IAP_H_

#include <STC8G.H>

//E2PROM起始地址 因使用IAP模式访问 此定义值恒为零
#define FLASH_START_ADDR  0x0000

//------->STC8G/H用户配置<-------
//晶振频率 STC8G/H用户具体参照数据手册关于IAP_TPS寄存器的介绍
#define CRYSTAL_MHz  11

//------->STC8A/F用户配置<-------
//STC8A/F用户参照数据手册关于IAP_CONTR寄存器中的IAP_WT位介绍
#define IAP_WT  3


#define IAP_IAP_BYTE_READ    1
#define IAP_IAP_BYTE_PROGRAM 2
#define IAP_IAP_SECTOR_ERASE 3

/***********************************************************************
    函数名:    ByteProgram
    描述:      编程1字节
    调用:      TRIGIAP
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned char]         x : 数据
    返回值:    [bit]                     : 1操作失败 0操作成功
    其他说明:  该地址必须先被擦除为0xFF再进行写操作
/**********************************************************************/
bit ByteProgram (unsigned int addrOffset, unsigned char x);

/***********************************************************************
    函数名:    BufProgram
    描述:      编程好多字节
    调用:      ByteProgram
    参数:      [unsigned int] addrOffset : 要进行操作的地址
               [unsigned int]        cnt : 数据量
               [unsigned char*]      buf : 数据指针
    返回值:    void
    其他说明:  该地址必须先被擦除为0xFF再进行写操作
               不进行操作失败判断
/**********************************************************************/
void BufProgram (unsigned int addrOffset, unsigned int cnt, unsigned char *x);

/***********************************************************************
    函数名:    ByteRead
    描述:      读取1字节
    调用:      TRIGIAP
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
    调用:      TRIGIAP
    参数:      [unsigned char] sectorNumber : 要进行操作的扇区号
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SectorErase(unsigned char sectorNumber);

#endif
