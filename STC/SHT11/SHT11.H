#ifndef _SHT11_H_
#define _SHT11_H_

#include <STC89C5xRC.H>
#include <intrins.h>

sbit SHT11_DATA = P1 ^ 1;
sbit SHT11_SCK = P1 ^ 0;

#define TEM_TEST     0x03// 温度检测命令
#define HUM_TEST     0x05// 湿度检测命令
#define REG_READ     0x07// 读寄存器
#define REG_WRITE    0x06// 写寄存器
#define FUNCTION_SET 0x01// 设置SHT11 的工作精度为8 位/湿度12 位温度

extern bit SHT11_ERROR;
#define ERR_OK           0
#define ERR_TEMP_TIMEOUT 1

#define TIMEOUT_CNT  35000

//定义数据格式, 上电默认温度14位,湿度12位
#define TEMP14HUMD12
//#define TEMP12HUMD8

unsigned int SHT11_Measure (unsigned char command, unsigned char time);
void SHT11_ConnectionReset();
float SHT11_Convert_Tempeture (unsigned int dat);
float SHT11_Convert_Humidity (unsigned int dat, float temp);
//float SHT11_Convert_Tempeture12bit (unsigned int dat);
//float SHT11_Convert_Humidity8bit (unsigned int dat, float temp);

#endif