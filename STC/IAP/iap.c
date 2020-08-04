/************************************************************
    Copyright (C), 2020, DT9025A
    �ļ���:  iap.c
    ����:    DT9025A
    �汾:    R1.0
    ����:    20/8/4
    ����:    STC8G/Hϵ�е�Ƭ��E2PROM��������
	ʵ��ƽ̨:
	         [PASS] STC8G1K08A     @ 11.0592MHz
			 [PASS] STC8H8K64S4U   @ 11.0592MHz
			 [PASS] STC8A8K64S4A12 @ 11.0592MHz
    �޶���ʷ:
    <����>   <ʱ��>   <�汾>   <����>
    DT9025A  20/8/4    A1.0    ��д���
    DT9025A  20/8/4    R1.0    �����ĵ�ע��
***********************************************************/

#include "iap.h"


/***********************************************************************
    ������:    TRIGIAP
    ����:      ����IAP����
    ����:      ��
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned char]       cmd : ���ݵ�ָ��
    ����ֵ:    void
    ����˵��:  �ڲ�����
/**********************************************************************/
bit TRIGIAP (unsigned int addrOffset, unsigned char cmd) {
	bit result;
    addrOffset += FLASH_START_ADDR;
    IAP_ADDRH = addrOffset >> 8;
    IAP_ADDRL = (unsigned char)addrOffset & 0xff;
    EA = 0;
    IAP_CONTR = 0x80;
#ifdef __STC8F_H_
	IAP_CONTR |= IAP_WT;
#else
	IAP_TPS = WAIT_TIME;
#endif
    IAP_CMD = cmd;
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;
    IAP_ADDRH = IAP_ADDRL = IAP_TRIG = IAP_CMD = IAP_CONTR = 0;
    EA = 1;
    result = IAP_CONTR >> 4 & 1;
	IAP_CONTR &= 0xEF;
	return result;
}


/***********************************************************************
    ������:    ByteProgram
    ����:      ���1�ֽ�
    ����:      TRIGIAP
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned char]         x : ����
    ����ֵ:    void
    ����˵��:  �õ�ַ�����ȱ�����Ϊ0xFF�ٽ���д����
/**********************************************************************/
bit ByteProgram (unsigned int addrOffset, unsigned char x) {
    IAP_DATA = x;
    return TRIGIAP (addrOffset, IAP_IAP_BYTE_PROGRAM);
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
    ����:      TRIGIAP
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
    ����ֵ:    [unsigned char] : ��ȡ��������
    ����˵��:  ��
/**********************************************************************/
unsigned char ByteRead (unsigned int addrOffset) {
    if(TRIGIAP (addrOffset, IAP_IAP_BYTE_READ));
    return IAP_DATA;
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
    ����:      TRIGIAP
    ����:      [unsigned char] sectorNumber : Ҫ���в�����������
    ����ֵ:    void
    ����˵��:  ��
/**********************************************************************/
void SectorErase (unsigned char sectorNumber) {
    unsigned int addrOffset;

    addrOffset = sectorNumber << 9;
    TRIGIAP (addrOffset, IAP_IAP_SECTOR_ERASE);
}
