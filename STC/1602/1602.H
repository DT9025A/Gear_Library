/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  1602.h
    作者:    DT9025A
    版本:    R1.0
    日期:    20/7/7
    描述:    4/8线驱动1602
             本驱动在 STC12C5A60S2@12.0MHz 和 STC8A8K64S4A12@12.0MHz 平台试验通过
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/4/7    A1.0    编写完成
    DT9025A  20/7/7    R1.0    完善四线驱动和文档注释
***********************************************************/

#ifndef _1602_H_
#define _1602_H_

//头文件
#include <STC8F.H>
#include <intrins.h>

/************************* 用户定义区 *****************************************/
//延时函数 nx100US, 在C文件实现
void __1602_delay (unsigned int us);

//接口定义
#define LCM1602_DBL_4                         // 总线位数(8,4) 4位总线默认LCM1602_DB0_DB7的4-7
#define LCM1602_DB0_DB7 P1                    // 数据端口
sbit LCM1602_RS   =     P2 ^ 0;               // 定义LCM1602的RS控制线
sbit LCM1602_RW   =     P2 ^ 1;               // 定义LCM1602的RW控制线
sbit LCM1602_E    =     P2 ^ 2;               // 定义LCM1602的E控制线
sbit LCM1602_Busy =     LCM1602_DB0_DB7 ^ 7;  // 定义LCM1602的测忙线（与LCM1602_DB0_DB7关联）

//测忙方法选择
//#define READ_BUSY                             //默认通过RW=1读取,注释掉为延时等待
#define WAIT_BUSY_TIME  20                    //延时等待时长 延时等待方式显示有问题改这个

//GRAM: 一个[2][17]数组
//是否使用GRAM
//#define USE_GRAM                            //不使用注释这个define
#define GRAM_POSITION   xdata                 //GRAM存放位置 data(0-127) idata(0-255) xdata

//CGRAM: 可自定义8个字符的的存储区
//不用可注释掉define
#define USE_CGRAM
//字符数据在C文件更改
/*******************************************************************************/


//指令定义
#define CMD_CLS         0x01                // 清除屏幕
#define CMD_CLR_PTR     0x02                // DDRAM指针回零位
#define CMD_DEC1        0x04                // 读入后AC（指针）减1，向左写
#define CMD_ADD1        0x06                // 读入后AC（指针）加1，向右写
#define CMD_DIS_ON_CF   0x0F                // 开显示_开光标_开光标闪烁
#define CMD_DIS_ON_CN   0x0E                // 开显示_开光标_关光标闪烁
#define CMD_DIS_ON_NN   0x0C                // 开显示_关光标_关光标闪烁
#define CMD_DIS_OFF     0x08                // 关显示_关光标_关光标闪烁
#define CMD_8B2L        0x38                // 8位总线_2行显示
#define CMD_8B1L        0x30                // 8位总线_1行显示（上边行）
#define CMD_4B2L        0x28                // 4位总线_2行显示
#define CMD_4B1L        0x20                // 4位总线_1行显示（上边行）
#define CMD_LINE_1      0x80                // 定位到第一行
#define CMD_LINE_2      0xC0                // 定位到第二行
#define CMD_CGRAM_0     0x40                // 定位到CGRAM首地址

/***********************************************************************
    函数名:    LCM1602_Print
    描述:      向屏幕输出字符串(不会自动折行)
    调用:      LCM1602_WriteCMD
               LCM1602_WriteData
    参数:      [unsigned char] dat : 要输出的数据
               [bit]       RSState : RS状态
    返回值:    void
    其他说明:  无
/**********************************************************************/
void LCM1602_Print (unsigned char a, unsigned char *str);
/***********************************************************************
    函数名:    LCM1602_PrintChar
    描述:      向屏幕输出字符串(不会自动折行)
    调用:      LCM1602_WriteCMD
               LCM1602_WriteData
    参数:      [unsigned char] pos : 要显示的位置 (0x00第一行, 0x40第二行)
               [unsigned char] t   : 输出字符
    返回值:    void
    其他说明:  无
/**********************************************************************/
void LCM1602_PrintChar (unsigned char a, unsigned char t);
/***********************************************************************
    函数名:    LCM1602_CLS
    描述:      清屏
    调用:      LCM1602_WriteCMD
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
void LCM1602_CLS();
/***********************************************************************
    函数名:    LCM1602_Init
    描述:      初始化屏幕
    调用:      LCM1602_WriteCMD
               LCM1602_WriteData
               <IF> LCM1602_CgramWrite
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
void LCM1602_Init ();

//GRAM相关
#ifdef USE_GRAM
#include <string.h>
/***********************************************************************
    函数名:    LCM1602_PrintGRAM
    描述:      将GRAM内容打印到屏幕
    调用:      LCM1602_CLS
               LCM1602_Print
               _1602_GRAM
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
void LCM1602_PrintGRAM();
extern char GRAM_POSITION _1602_GRAM[2][17];
#define GRAML(L) (char*)&_1602_GRAM[L]
#define LCM1602_GRAM_Init memset(GRAML(0),0,17);\
    memset(GRAML(1),0,17)

#endif  //USE_GRAM

//CGRAM相关
#ifdef USE_CGRAM
void LCM1602_CgramWrite();
#endif  //USE_CGRAM
#endif  //_1602_H_
