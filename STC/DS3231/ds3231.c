/************************************************************
    Copyright (C), 2020, DT9025A
    �ļ���:  ds3231.c
    ����:    DT9025A
    �汾:    R1.0
    ����:    20/7/8
    ����:    DS3231��������
             �����ⲿ��: VIIC_C51
             �������� STC12C4052AD@12.0MHz ƽ̨����ͨ��
    �޶���ʷ:
    <����>   <ʱ��>   <�汾>   <����>
    DT9025A  19/8/15   A1.0    ��д���
    DT9025A  20/7/8    R1.0    �����ĵ�ע��
***********************************************************/
#include "ds3231.h"


/***********************************************************************
    ������:    HEX2BCD
    ����:      HEX��ʽ����ת��ΪBCD��ʽ����
    ����:      ��
    ����:      [unsigned char] dat : Ҫ����ת��������
    ����ֵ:    [unsigned char] : ת���������
    ����˵��:  �ڲ�����
/**********************************************************************/
unsigned char HEX2BCD (unsigned char dat) {
    return (((dat / 10) << 4) + (dat % 10));
}


/***********************************************************************
    ������:    BCD2HEX
    ����:      BCD��ʽ����ת��ΪHEX��ʽ����
    ����:      ��
    ����:      [unsigned char] dat : Ҫ����ת��������
    ����ֵ:    [unsigned char] : ת���������
    ����˵��:  �ڲ�����
/**********************************************************************/
unsigned char BCD2HEX (unsigned char dat) {
    return ((dat >> 4) * 10 + (dat & 0x0f));
}


/***********************************************************************
    ������:    SetDS3231PPS
    ����:      ����DS3231����������
    ����:      IRcvStr
               ISendStr
    ����:      [unsigned char] enable : �Ƿ�������(��0��)
    ����ֵ:    void
    ����˵��:  ��
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
    ������:    GetDS3231PPS
    ����:      ��ȡDS3231����������״̬
    ����:      IRcvStr
    ����:      void
    ����ֵ:    [bit] : �Ƿ�������(��0��)
    ����˵��:  ��
/**********************************************************************/
bit GetDS3231PPS() {
    unsigned char temp;
    IRcvStr (DS3231_ADD, 0x0e, &temp, 1);
    if (temp & 0x1c == 0x1c)
        return 1;
    return 0;
}


/***********************************************************************
    ������:    GetDS3231
    ����:      ��ȡDS3231��ʱ��
    ����:      IRcvStr
    ����:      [Time*] time : ���ʱ��Ľṹ��ָ��
    ����ֵ:    void
    ����˵��:  ��
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
    ������:    SetDS3231
    ����:      ����DS3231��ʱ��
    ����:      ISendStr
    ����:      [Time*] time : ���ʱ��Ľṹ��ָ��
    ����ֵ:    void
    ����˵��:  ��
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