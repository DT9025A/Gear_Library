/************************************************************
    Copyright (C), 2020, DT9025A
    �ļ���:  adc.c
    ����:    DT9025A
    �汾:    R1.0
    ����:    20/7/8
    ����:    STC12ϵ�е�Ƭ��E2PROM��������
    �޶���ʷ:
    <����>   <ʱ��>   <�汾>   <����>
    DT9025A  20/1/15   A1.0    ��д���
    DT9025A  20/7/8    R1.0    �����ĵ�ע��
***********************************************************/

#include "isp.h"


/***********************************************************************
    ������:    TRIGISP
    ����:      ����ISP����
    ����:      ��
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned char]       cmd : ���ݵ�ָ��
    ����ֵ:    void
    ����˵��:  �ڲ�����
/**********************************************************************/
void TRIGISP (unsigned int addrOffset, unsigned char cmd) {
    addrOffset += FLASH_START_ADDR;
    ISP_ADDRH = addrOffset >> 8;
    ISP_ADDRL = (unsigned char)addrOffset & 0xff;
    EA = 0;
    ISP_CONTR = WAIT_TIME | 0x80;
    ISP_CMD = cmd;
    ISP_TRIG = 0x46;
    ISP_TRIG = 0xB9;
    ISP_ADDRH = ISP_ADDRL = ISP_TRIG = ISP_CMD = ISP_CONTR = 0;
    EA = 1;
}


/***********************************************************************
    ������:    ByteProgram
    ����:      ���1�ֽ�
    ����:      TRIGISP
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned char]         x : ����
    ����ֵ:    void
    ����˵��:  �õ�ַ�����ȱ�����Ϊ0xFF�ٽ���д����
/**********************************************************************/
void ByteProgram (unsigned int addrOffset, unsigned char x) {
    ISP_DATA = x;
    TRIGISP (addrOffset, ISP_IAP_BYTE_PROGRAM);
}


/***********************************************************************
    ������:    BufProgram
    ����:      ��̺ö��ֽ�
    ����:      ByteProgram
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned int]        cnt : ������
               [unsigned char*]      buf : ����ָ��
    ����ֵ:    void
    ����˵��:  �õ�ַ�����ȱ�����Ϊ0xFF�ٽ���д����
/**********************************************************************/
void BufProgram (unsigned int addrOffset, unsigned int cnt, unsigned char *buf) {
    unsigned int i;
    for (i = 0; i < cnt; i++)
        ByteProgram (addrOffset + i, buf[i]);
}


/***********************************************************************
    ������:    ByteRead
    ����:      ��ȡ1�ֽ�
    ����:      TRIGISP
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
    ����ֵ:    [unsigned char] : ��ȡ��������
    ����˵��:  ��
/**********************************************************************/
unsigned char ByteRead (unsigned int addrOffset) {
    TRIGISP (addrOffset, ISP_IAP_BYTE_READ);
    return ISP_DATA;
}


/***********************************************************************
    ������:    BufRead
    ����:      ��ȡ�ö��ֽ�
    ����:      ByteRead
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned int]        cnt : ������
               [unsigned char*]      buf : ����ָ��
    ����ֵ:    void
    ����˵��:  ��
/**********************************************************************/
void BufRead (unsigned int addrOffset, unsigned int cnt, unsigned char *buf) {
    unsigned int i;
    for (i = 0; i < cnt; i++)
        buf[i] = ByteRead (addrOffset + i);
}


/***********************************************************************
    ������:    SectorErase
    ����:      ��������
    ����:      TRIGISP
    ����:      [unsigned char] sectorNumber : Ҫ���в�����������
    ����ֵ:    void
    ����˵��:  ��
/**********************************************************************/
void SectorErase (unsigned char sectorNumber) {
    unsigned int addrOffset;

    addrOffset = sectorNumber << 9;
    TRIGISP (addrOffset, ISP_IAP_SECTOR_ERASE);
}
