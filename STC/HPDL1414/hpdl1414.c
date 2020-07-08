/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  hpdl1414.h
    作者:    DT9025A
    版本:    A1.1
    日期:    20/7/8
    描述:    HPDL1414驱动 (数据线占一组端口)
             本驱动在 STC12C4052AD@12.0MHz 平台试验通过
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/4/12   A1.0    编写完成
    DT9025A  20/7/8    A1.1    完善文档注释
***********************************************************/

#include "hpdl1414.h"

/***********************************************************************
    函数名:    Digital
    描述:      位选
    调用:      无
    参数:      [unsigned char] d : 位选的位,从0开始
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void Digital (unsigned char d) {
    d = ~d;
    d &= 0x03;
    A1 = d >> 1;
    A0 = d & 1;
}

/***********************************************************************
    函数名:    W
    描述:      W/R
    调用:      无
    参数:      [unsigned char] i : 为HPDL1414编号 从1开始 对应W_R
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void W (unsigned char i) {
#if (MODELNUM == 2)
    if (i) {
        W_R = 1;
        W_R_1 = 0;
    } else {
        W_R = 0;
        W_R_1 = 1;
    }
#else
    W_R = i;
#endif
}

/***********************************************************************
    函数名:    CWR
    描述:      清除所有W/R状态
    调用:      无
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void CWR() {
#if (MODELNUM == 2)
    W_R = W_R_1 = 1;
#else
    W_R = 1;
#endif
}

/***********************************************************************
    函数名:    dispChar
    描述:      显示一个字符
    调用:      Digital
               W
               CWR
    参数:      [char]            x : 要显示的字符
               [unsigned char] pos : 要显示的位置
               [unsigned char]   i : 要显示的HPDL1414编号
    返回值:    void
    其他说明:  无
/**********************************************************************/
void dispChar (char x, unsigned char pos, unsigned char i) {
    if (x < ' ' || x > '_')
        return;
    Digital (pos);
    DATA_OUT = x;
    W (i);
    _nop_();
    _nop_();
    CWR ();
}


/***********************************************************************
    函数名:    dispString
    描述:      显示字符串
    调用:      dispChar
    参数:      [char*] str : 要显示的字符串
    返回值:    void
    其他说明:  支持多块并联，超出显示范围则折回开头继续显示
/**********************************************************************/
void dispString (char *str) {
    unsigned char pos = 0;
    while (*str) {
        dispChar (*str++, pos % 4, pos / 4);
        if (pos == MODELNUM * 4 - 1)
            pos = 0;
        else
            pos += 1;
    }
}
