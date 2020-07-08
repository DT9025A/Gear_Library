/************************************************************
    Copyright (C), 2020, DT9025A
    文件名:  24c02.c
    作者:    DT9025A
    版本:    R 1.0
    日期:    20/7/7
    描述:    AT24C02系列E2PROM驱动
             I2C协议在内部实现
             本驱动在 STC12C5A60S2@12.0MHz 和 STC89C54RD+@12.0MHz 平台试验通过
             当总线通信出现问题时可尝试修改I2C_Delay函数和AT24C02_WRITE_DELAY函数
    修订历史:
    <作者>   <时间>   <版本>   <描述>
    DT9025A  20/2/15   A1.0    编写完成
    DT9025A  20/7/7    R1.0    完善文档注释
***********************************************************/

#include "24c02.h"

/***********************************************************************
    函数名:    I2C_Delay
    描述:      I2C总线延时
    调用:      无
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void I2C_Delay() {
    unsigned char i, j;

    _nop_();
    i = 227;
    while (--i);
}


/***********************************************************************
    函数名:    I2C_Start
    描述:      建立I2C总线起始条件
    调用:      I2C_Delay
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void I2C_Start() {
    SDA = 1;
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
}


/***********************************************************************
    函数名:    I2C_End
    描述:      建立I2C总线结束条件
    调用:      I2C_Delay
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void I2C_End() {
    SDA = 0;
    SCL = 1;
    I2C_Delay();
    SDA = 1;
    I2C_Delay();
}


/***********************************************************************
    函数名:    I2C_Ack
    描述:      等待ACK信号
    调用:      I2C_Delay
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void I2C_Ack() {
    unsigned char i = 0;
    SCL = 1;
    I2C_Delay();
    while (SDA == 1 && i < 250)  //防止超时阻塞
        i++;
    SCL = 0;
    I2C_Delay();
}


/***********************************************************************
    函数名:    I2C_WriteByte
    描述:      主设备发出ACK信号
    调用:      I2C_Delay
               使用了CY位
    参数:      void
    返回值:    void
    其他说明:  无
/**********************************************************************/
void I2C_WriteByte (unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        dat = dat << 1;
        SCL = 0;
        I2C_Delay();
        SDA = CY;
        I2C_Delay();
        SCL = 1;
        I2C_Delay();
    }
    SCL = 0;
    SDA = 1;
    I2C_Delay();
}


/***********************************************************************
    函数名:    I2C_ReadByte
    描述:      从总线上读一字节
    调用:      I2C_Delay
               使用了CY位
    参数:      void
    返回值:    [unsigned char] : 读到的数据
    其他说明:  无
/**********************************************************************/
unsigned char I2C_ReadByte() {
    unsigned char dat = 0, i;
    SCL = 0;
    I2C_Delay();
    for (i = 0; i < 8; i++) {
        SCL = 1;
        I2C_Delay();
        dat = dat << 1;
        dat = dat | SDA;
        I2C_Delay();
        SCL = 0;
        I2C_Delay();
    }
    return dat;
}


/***********************************************************************
    函数名:    AT24C02_WRITE_DELAY
    描述:      延时等待AT24C02写入
    调用:      无
    参数:      void
    返回值:    void
    其他说明:  内部函数
/**********************************************************************/
void AT24C02_WRITE_DELAY() {    //STC Y1@12.000MHz@1ms
    unsigned char i, j;

    _nop_();
    i = 4;
    j = 146;
    do
    {
        while (--j);
    } while (--i);
}


/***********************************************************************
    函数名:    AT24C02_WriteByte
    描述:      AT24C02写入一字节
    调用:      I2C_Start
               I2C_WriteByte
               I2C_Ack
               I2C_End
               AT24C02_WRITE_DELAY
    参数:      [unsigned char] addr : 写入地址
               [unsigned char] data : 写入数据
    返回值:    void
    其他说明:  无
/**********************************************************************/
void AT24C02_WriteByte (unsigned char addr, unsigned char dat) {
    I2C_Start();                       //启动总线
    I2C_WriteByte (AT24C02_ADDR + 0);  //写AT24C02地址+写
    I2C_Ack();                           //等待ACK
    I2C_WriteByte (addr);               //写数据地址
    I2C_Ack();                           //等待ACK
    I2C_WriteByte (dat);               //写数据
    I2C_Ack();                           //等待ACK
    I2C_End();                           //结束总线
    AT24C02_WRITE_DELAY();               //等待片内写入完成
}


/***********************************************************************
    函数名:    AT24C02_WriteBuf
    描述:      AT24C02写入好多字节
    调用:      AT24C02_WriteByte
    参数:      [unsigned char]  addr : 写入地址
               [unsigned char*] data : 写入数据存放指针
               [unsigned char] count : 写入数据个数
    返回值:    void
    其他说明:  无
/**********************************************************************/
void AT24C02_WriteBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        AT24C02_WriteByte (addr + ptr, * (dat + ptr));
}


/***********************************************************************
    函数名:    AT24C02_ReadByte
    描述:      AT24C02读取一字节
    调用:      I2C_Start
               I2C_WriteByte
               I2C_Ack
               I2C_ReadByte
               I2C_End
    参数:      [unsigned char] addr : 要读取的地址
    返回值:    [unsigned char] : 读出的数据
    其他说明:  无
/**********************************************************************/
unsigned char AT24C02_ReadByte (unsigned char addr) {
    unsigned char dat;

    I2C_Start();                        //启动总线
    I2C_WriteByte (AT24C02_ADDR + 0);    //写AT24C02地址+写
    I2C_Ack();                            //等待ACK
    I2C_WriteByte (addr);                //写数据地址
    I2C_Ack();                            //等待ACK
    I2C_Start();                        //再次启动总线, 准备读
    I2C_WriteByte (AT24C02_ADDR + 1);    //写AT24C02地址+读
    I2C_Ack();                            //等待ACK
    dat = I2C_ReadByte();                //读数据
    I2C_End();                            //结束总线
    return dat;
}


/***********************************************************************
    函数名:    AT24C02_ReadBuf
    描述:      AT24C02读取好多字节
    调用:      AT24C02_ReadByte
    参数:      [unsigned char]  addr : 要读取的地址
               [unsigned char*] data : 读出数据存放指针
               [unsigned char] count : 读出数据个数
    返回值:    void
    其他说明:  data须预先申请
/**********************************************************************/
void AT24C02_ReadBuf (unsigned char addr, unsigned char *dat, unsigned char count) {
    unsigned char ptr;

    for (ptr = 0; ptr < count; ptr++)
        * (dat + ptr) = AT24C02_ReadByte (addr + ptr);
}
