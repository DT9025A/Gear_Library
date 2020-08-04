/************************************************************
    Copyright (C), 2020, DT9025A
    �ļ���:  iap.h
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

#ifndef __STC_IAP_H_
#define __STC_IAP_H_

#include <STC8G.H>

//E2PROM��ʼ��ַ ��ʹ��IAPģʽ���� �˶���ֵ��Ϊ��
#define FLASH_START_ADDR  0x0000

//------->STC8G/H�û�����<-------
//����Ƶ�� STC8G/H�û�������������ֲ����IAP_TPS�Ĵ����Ľ���
#define CRYSTAL_MHz  11

//------->STC8A/F�û�����<-------
//STC8A/F�û����������ֲ����IAP_CONTR�Ĵ����е�IAP_WTλ����
#define IAP_WT  3


#define IAP_IAP_BYTE_READ    1
#define IAP_IAP_BYTE_PROGRAM 2
#define IAP_IAP_SECTOR_ERASE 3

/***********************************************************************
    ������:    ByteProgram
    ����:      ���1�ֽ�
    ����:      TRIGIAP
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned char]         x : ����
    ����ֵ:    [bit]                     : 1����ʧ�� 0�����ɹ�
    ����˵��:  �õ�ַ�����ȱ�����Ϊ0xFF�ٽ���д����
/**********************************************************************/
bit ByteProgram (unsigned int addrOffset, unsigned char x);

/***********************************************************************
    ������:    BufProgram
    ����:      ��̺ö��ֽ�
    ����:      ByteProgram
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
               [unsigned int]        cnt : ������
               [unsigned char*]      buf : ����ָ��
    ����ֵ:    void
    ����˵��:  �õ�ַ�����ȱ�����Ϊ0xFF�ٽ���д����
               �����в���ʧ���ж�
/**********************************************************************/
void BufProgram (unsigned int addrOffset, unsigned int cnt, unsigned char *x);

/***********************************************************************
    ������:    ByteRead
    ����:      ��ȡ1�ֽ�
    ����:      TRIGIAP
    ����:      [unsigned int] addrOffset : Ҫ���в����ĵ�ַ
    ����ֵ:    [unsigned char] : ��ȡ��������
    ����˵��:  ��
/**********************************************************************/
unsigned char ByteRead (unsigned int addrOffset);

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
void BufRead (unsigned int addrOffset, unsigned int cnt, unsigned char *buf);

/***********************************************************************
    ������:    SectorErase
    ����:      ��������
    ����:      TRIGIAP
    ����:      [unsigned char] sectorNumber : Ҫ���в�����������
    ����ֵ:    void
    ����˵��:  ��
/**********************************************************************/
void SectorErase(unsigned char sectorNumber);

#endif
