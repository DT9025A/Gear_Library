#ifndef _SHT11_H_
#define _SHT11_H_

#include <STC89C5xRC.H>
#include <intrins.h>

sbit SHT11_DATA = P1 ^ 1;
sbit SHT11_SCK = P1 ^ 0;

#define TEM_TEST     0x03// �¶ȼ������
#define HUM_TEST     0x05// ʪ�ȼ������
#define REG_READ     0x07// ���Ĵ���
#define REG_WRITE    0x06// д�Ĵ���
#define FUNCTION_SET 0x01// ����SHT11 �Ĺ�������Ϊ8 λ/ʪ��12 λ�¶�

extern bit SHT11_ERROR;
#define ERR_OK           0
#define ERR_TEMP_TIMEOUT 1

#define TIMEOUT_CNT  35000

//�������ݸ�ʽ, �ϵ�Ĭ���¶�14λ,ʪ��12λ
#define TEMP14HUMD12
//#define TEMP12HUMD8

unsigned int SHT11_Measure (unsigned char command, unsigned char time);
void SHT11_ConnectionReset();
float SHT11_Convert_Tempeture (unsigned int dat);
float SHT11_Convert_Humidity (unsigned int dat, float temp);
//float SHT11_Convert_Tempeture12bit (unsigned int dat);
//float SHT11_Convert_Humidity8bit (unsigned int dat, float temp);

#endif