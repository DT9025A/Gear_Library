/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  ds3231.c
    作者:    DT9025A
    版本:    R1.0
    日期:    20/7/8
    描述:    DS3231基础驱动
             依赖外部库: VIIC_C51
             本驱动在 STC12C4052AD@12.0MHz 平台试验通过
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  19/8/15   A1.0    编写完成
    DT9025A  20/7/8    R1.0    完善文档注释
***********************************************************/
#include "ds3231.h"


/***********************************************************************
    函数名:    HEX2BCD
    描述:      HEX格式数据转换为BCD格式数据
    调用:      无
    参数:      [unsigned char] dat : 要进行转换的数据
    返回值:    [unsigned char] : 转换后的数据
    其他说明:  内部函数
/**********************************************************************/
unsigned char HEX2BCD (unsigned char dat) {
    return (((dat / 10) << 4) + (dat % 10));
}


/***********************************************************************
    函数名:    BCD2HEX
    描述:      BCD格式数据转换为HEX格式数据
    调用:      无
    参数:      [unsigned char] dat : 要进行转换的数据
    返回值:    [unsigned char] : 转换后的数据
    其他说明:  内部函数
/**********************************************************************/
unsigned char BCD2HEX (unsigned char dat) {
    return ((dat >> 4) * 10 + (dat & 0x0f));
}


/***********************************************************************
    函数名:    SetDS3231PPS
    描述:      设置DS3231的秒闪功能
    调用:      IRcvStr
               ISendStr
    参数:      [unsigned char] enable : 是否开启秒闪(非0开)
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SetDS3231PPS (unsigned char enable) {
    unsigned char temp;
    IRcvStr (DS3231_ADD, 0x0e, &temp, 1);
    temp &= 0xe3;
    if(enable == 0)
        temp |= 0x04;
    ISendStr (DS3231_ADD, 0x0e, &temp, 1);
}


/***********************************************************************
    函数名:    GetDS3231PPS
    描述:      读取DS3231的秒闪功能状态
    调用:      IRcvStr
    参数:      void
    返回值:    [bit] : 是否开启秒闪(非0开)
    其他说明:  无
/**********************************************************************/
bit GetDS3231PPS() {
    unsigned char temp;
    IRcvStr (DS3231_ADD, 0x0e, &temp, 1);
    if (temp & 0x1c == 0x1c)
        return 1;
    return 0;
}


/***********************************************************************
    函数名:    GetDS3231
    描述:      读取DS3231的时间
    调用:      IRcvStr
    参数:      [Time*] time : 存放时间的结构体指针
    返回值:    void
    其他说明:  无
/**********************************************************************/
void GetDS3231 (Time *time) {
    unsigned char buf[7];

    IRcvStr (DS3231_ADD, 0, buf, 7);
    time->second = BCD2HEX (buf[0]);
    time->minute = BCD2HEX (buf[1]);
    time->hour = BCD2HEX (buf[2]);
    time->dow = BCD2HEX (buf[3]);
    time->date = BCD2HEX (buf[4]);
    time->month = BCD2HEX (buf[5]);
    time->year = BCD2HEX (buf[6]);
}


/***********************************************************************
    函数名:    SetDS3231
    描述:      设置DS3231的时间
    调用:      ISendStr
    参数:      [Time*] time : 存放时间的结构体指针
    返回值:    void
    其他说明:  无
/**********************************************************************/
void SetDS3231 (Time *time) {
    unsigned char buf[7];
    
    buf[0] = HEX2BCD (time->second);
    buf[1] = HEX2BCD (time->minute);
    buf[2] = HEX2BCD (time->hour);
    buf[3] = HEX2BCD (time->dow);
    buf[4] = HEX2BCD (time->date);
    buf[5] = HEX2BCD (time->month);
    buf[6] = HEX2BCD (time->year);
    ISendStr (DS3231_ADD, 0, buf, 7);
}