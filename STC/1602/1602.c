/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  1602.c
    作者:    DT9025A
    版本:    R1.1
    日期:    20/8/4
    描述:    4/8线驱动1602(ST7032)
    实验平台:
	         [PASS] STC12C5A60S2   @ 12.0MHz
			 [PASS] STC8A8K64S4A12 @ 12.0MHz
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/4/7    A1.0    编写完成
    DT9025A  20/7/7    R1.0    完善四线驱动和文档注释
	DT9025A  20/8/4    R1.1    更改文档注释
***********************************************************/

#include "1602.h"

unsigned char code Xword[] = {
    0X00, 0X15, 0X0E, 0X15, 0X1F, 0X11, 0X1F, 0X00, //打印中2， 代码 0x00
    0X00, 0X1F, 0X11, 0X11, 0X11, 0X11, 0X1F, 0X00, //缺纸，    代码 0x01
    0X00, 0X11, 0X1F, 0X11, 0X1F, 0X11, 0X1F, 0X00, //纸张就绪，代码 0x02
    0X00, 0X0A, 0X00, 0X11, 0X1F, 0X11, 0X1F, 0X00, //打印闲置，代码 0x03
    0X15, 0X0E, 0X04, 0X11, 0X1F, 0X11, 0X1F, 0X00, //打印中，  代码 0x04
    0x18, 0x18, 0x07, 0x08, 0x08, 0x08, 0x07, 0x00, //℃，       代码 0x05
    0X1D, 0X09, 0X09, 0X01, 0X15, 0X1C, 0X15, 0X00, //温度过高，代码 0x06
    0X1C, 0X08, 0X08, 0X01, 0X12, 0X0C, 0X08, 0X00  //温度正常，代码 0x07
};

/***********************************************************************
    函数名:    __1602_delay
    描述:      延时nx100us
    调用:      无
    参数:      [unsigned int] us : 延时时间
    返回值:    void
    其他说明:  内部函数,需要自行实现
/**********************************************************************/
void __1602_delay (unsigned int us) {
    unsigned char i, j;
    while (--us) {

        i = 2;
        j = 39;
        do {
            while (--j);
        } while (--i);
    }
}


/***********************************************************************
    函数名:    LCM1602_TestBusy
    描述:      总线测忙(忙阻塞,不忙返回)
    调用:      无
    参数:      <IF>    __1602_delay
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void LCM1602_TestBusy () {
#ifdef READ_BUSY
    LCM1602_Busy = 1;
    LCM1602_RS = 0;
    LCM1602_RW = 1;
    LCM1602_E = 1;
    while (LCM1602_Busy);           //等待不忙
    LCM1602_E = 0;
#else
    __1602_delay (WAIT_BUSY_TIME);
#endif
}


/***********************************************************************
    函数名:    LCM1602_DataLatch
    描述:      通知屏幕接收总线数据
    调用:      __1602_delay
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void LCM1602_DataLatch() {
    LCM1602_E = 1;
    __1602_delay (5);
    LCM1602_E = 0;
}

/***********************************************************************
    函数名:    LCM1602_DataOut
    描述:      向总线输出数据
    调用:      LCM1602_DataLatch
    参数:      [unsigned char] dat : 要输出的数据
               [bit]       RSState : RS状态
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void LCM1602_DataOut (unsigned char dat, bit RSState) {
    LCM1602_TestBusy();
    LCM1602_RS = RSState;
    LCM1602_RW = 0;
#ifndef LCM1602_DBL_4
    //8位
    LCM1602_DB0_DB7 = dat;
    LCM1602_DataLatch();
#else
    //先发高四位
    LCM1602_DB0_DB7 &= 0x0F;          //端口低四位数据保持
    LCM1602_DB0_DB7 |= dat & 0xF0;      //端口高四位上数据
    LCM1602_DataLatch();
    dat <<= 4;                        //低四位上高
    LCM1602_DB0_DB7 &= 0x0F;
    LCM1602_DB0_DB7 |= dat & 0xF0;
    LCM1602_DataLatch();
#endif
}


/***********************************************************************
    宏名:      LCM1602_WriteCMD
    描述:      向屏幕输出指令
    调用:      LCM1602_DataOut
    参数:      [unsigned char] cmd : 要输出的指令
    其他说明:  内部宏
/**********************************************************************/
#define LCM1602_WriteCMD(cmd)  LCM1602_DataOut(cmd,0)


/***********************************************************************
    宏名:      LCM1602_WriteData
    描述:      向屏幕输出数据
    调用:      LCM1602_DataOut
    参数:      [unsigned char] cmd : 要输出的数据
    其他说明:  内部宏
/**********************************************************************/
#define LCM1602_WriteData(cmd) LCM1602_DataOut(cmd,1)


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
void LCM1602_Print (unsigned char line, unsigned char *str) {
    line *= 0x40;
    LCM1602_WriteCMD (line | 0x80);
    while (*str != '\0') {
        LCM1602_WriteData (*str++);
    }
    *str = 0;
}


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
void LCM1602_PrintChar (unsigned char pos, unsigned char t) {
    LCM1602_WriteCMD (pos | 0x80);
    LCM1602_WriteData (t);
}


/***********************************************************************
    函数名:    LCM1602_CLS
    描述:      清屏
    调用:      LCM1602_WriteCMD
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
void LCM1602_CLS() {
    LCM1602_WriteCMD (CMD_CLS);
    LCM1602_WriteCMD (CMD_CLR_PTR);
}


#ifdef USE_CGRAM
/***********************************************************************
    函数名:    LCM1602_CgramWrite
    描述:      将用户定义的CGRAM内容写入屏幕
    调用:      LCM1602_WriteCMD
               LCM1602_WriteData
               <V> Xword
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void LCM1602_CgramWrite () {
    unsigned char i;
    LCM1602_WriteCMD (CMD_ADD1);
    LCM1602_WriteCMD (CMD_CGRAM_0);
    for (i = 0; i < 64; i++) {
        LCM1602_WriteData (Xword[i]);
    }
}
#endif


#ifdef USE_GRAM

char GRAM_POSITION _1602_GRAM[2][17];

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
void LCM1602_PrintGRAM() {
    LCM1602_CLS();
    LCM1602_Print (0, GRAML (0));
    LCM1602_Print (1, GRAML (1));
}
#endif


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
void LCM1602_Init () {
#ifndef LCM1602_DBL_4
    LCM1602_WriteCMD (CMD_8B2L);
#else
    LCM1602_WriteCMD (CMD_4B2L);
    LCM1602_DataLatch();
    LCM1602_WriteCMD (CMD_4B2L);
#endif
    LCM1602_WriteCMD (CMD_CLS);
    LCM1602_WriteCMD (CMD_DIS_ON_NN);
    LCM1602_WriteCMD (CMD_CLR_PTR);
    LCM1602_WriteCMD (CMD_ADD1);
    LCM1602_E = 0;
#ifdef USE_CGRAM
    LCM1602_CgramWrite();
#endif
#ifdef USE_GRAM
    LCM1602_GRAM_Init;
#endif
}

