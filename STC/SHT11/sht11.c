#include "sht11.h"

typedef unsigned char uchar;
typedef unsigned int uint;

bit SHT11_ERROR = 0;

/***************************
��������:SHT11_Delay()
��������:SHT11 �ڲ���ʱ
****************************/
void SHT11_Delay() {
    ;
    ;
}

/***************************
��������:Delay_Ms()
��������:SHT11 ���ȴ���ʱ
����˵��:11ms/55ms/210ms �ֱ��Ӧ8 λ/12 λ/14 λ�������
��Ӧ���β�ΪN ����ʱNms
****************************/
void Delay_Ms (uint ms) {
    unsigned char i, j;
    while (ms--) {
        i = 2;
        j = 235;
        do {
            while (--j);
        } while (--i);
    }
}

/**************************
��������:SHT11 ����ʱ��
***************************/
void SHT11_Start() {
    SHT11_SCK = 1;
    SHT11_DATA = 1;
    SHT11_Delay();
    SHT11_DATA = 0;
    SHT11_Delay();
    SHT11_SCK = 0;
    SHT11_Delay();
    SHT11_SCK = 1;
    SHT11_Delay();
    SHT11_DATA = 1;
}

/**************************
��������:SHT11 ��λʱ��
***************************/
void SHT11_ConnectionReset () {
    unsigned char i;
    SHT11_DATA = 1;
    SHT11_SCK = 0;                  //׼��
    for (i = 0; i < 9; i++) {       //DATA���ָߣ�SCKʱ�Ӵ���9�Σ������������䣬ͨѸ����λ
        SHT11_SCK = 1;
        SHT11_Delay();
        SHT11_SCK = 0;
        SHT11_Delay();
    }
    SHT11_Start();
}

/******************************
�������ƣ� SHT11_Sendbyte(uchar dat)
��������: ��SHT11 ����8bit ����
******************************/
void SHT11_Sendbyte (uchar dat) {
    uchar i;
    SHT11_SCK = 0;
    SHT11_Delay();
    for (i = 0; i < 8; i++) {
        if (dat & 0x80) {
            SHT11_DATA = 1;
            SHT11_Delay();
        } else {
            SHT11_DATA = 0;
            SHT11_Delay();
        }
        dat = dat << 1;
        SHT11_SCK = 1;
        SHT11_Delay();
        SHT11_SCK = 0;
    }
}

/*********************************
��������SHT11_Answer() ��
��������:���SHT11 ����Ӧ�ź�(�ڵھŸ�ʱ������)
���� 1:��ʱ 0:����
***********************************/
bit SHT11_Answer() {
    uint cnt = 0;
    SHT11_SCK = 1;
    SHT11_Delay();
    while (SHT11_DATA == 1) {
        if (cnt == TIMEOUT_CNT)	   //��ⳬʱ
            return 1;
        cnt++;
		SHT11_Delay();
    }
    SHT11_SCK = 0;
    SHT11_DATA = 1;
    return 0;
}

/************************************
�������ƣ� SHT11_Test_Finish()
��������:���SHT11 ��ʪ�ȼ���Ƿ����
���� 1:��ʱ 0:����
*************************************/
bit SHT11_Test_Finish() {
    uint cnt = 0;
    while (SHT11_DATA == 1) {
        if (cnt == TIMEOUT_CNT)	   //��ⳬʱ
            return 1;
        cnt++;
		SHT11_Delay();
    }
    return 0;
}

/************************************
�������ƣ� SHT11_Receivebyte()
��������:��SHT11 ����8bit ����
*************************************/
uchar SHT11_Receivebyte() {
    uchar i;
    uchar dat;
    SHT11_SCK = 0;
    SHT11_Delay();
    for (i = 0; i < 8; i++) {
        SHT11_SCK = 1;
        SHT11_Delay();
        dat = dat << 1;
        if (SHT11_DATA) {
            dat = dat | 0x01;
            SHT11_Delay();
        } else {
            dat = dat & 0xfe;
            SHT11_Delay();
        }
        SHT11_SCK = 0;
        SHT11_Delay();
    }
    SHT11_DATA = 1; // �ͷ���������
    return (dat);
}

/***********************************
�������ƣ� MCU_Answer()
��������:��Ƭ����SHT11 ����Ӧ���ź�
*************************************/
void MCU_Answer() {
    SHT11_SCK = 0;
    SHT11_Delay();
    SHT11_DATA = 0;
    SHT11_Delay();
    SHT11_SCK = 1;
    SHT11_Delay();
    SHT11_SCK = 0;
    SHT11_Delay();
    SHT11_DATA = 1; //�ͷ���������
}

/***********************************
��������:SHT11_End()
����������8byte ���ݺ󲿽���CRC У����
************************************/
void SHT11_End() {
    SHT11_DATA = 1;
    SHT11_SCK = 1;
    SHT11_Delay();
    SHT11_SCK = 0;
    SHT11_Delay();
}

/*************************************
�������ƣ� void SHT11_Write_Register(uchar command ,uchar dat)
����˵������SHT11 ��״̬�Ĵ������ù���
command ΪREG_WRITE 0x06 д�Ĵ���
dat Ϊ����SHT11 �Ĺ��ܿ������ü�������λ��
���� 1:��ʱ 0:����
***************************************/
bit SHT11_Write_Register (uchar command, uchar dat) {
    SHT11_Start();
    SHT11_Sendbyte (command);
    if (SHT11_Answer())
        return 1;
    SHT11_Sendbyte (dat);
    if (SHT11_Answer())
        return 1;
    return 0;
}

/***************************************
�������ƣ� uchar SHT11_Read_Register(uchar command)
����˵���� command ΪREG_READ 0x07// ���Ĵ���
����ֵΪ״̬�Ĵ�����ֵ
λ6 ��ʾ��ǰ�����һ�����ݺ��Դ�������
��λ6 Ϊ0 ʱ����VDD>2.47V ��λ6 Ϊ1 ʱ����VDD<2.47V ����������
λ0 ������ǰ�Ĳ����ֱ���
��λ0 Ϊ1 ʱ�����������ȣ� 8 λ/ʪ��12 λ�¶�
��λ0 Ϊ0 ʱ�����������ȣ� 12 λʪ��14 λ�¶�
Ĭ��Ϊ0, 0xff��ʱ
*******************************************/
uchar SHT11_Read_Register (uchar command) {
    uchar dat;
    SHT11_Start();
    SHT11_Sendbyte (command);
    if (SHT11_Answer())
        return 0xff;
    dat = SHT11_Receivebyte();
    SHT11_End();
    return (dat);
}

/***************************************
��������:SHT11_Measure(uchar command,uint time);
��������:����SHT11 ��⹦��,��������Ӧ�ļ����
����˵��:command �β������趨�¶ȼ�⻹��ʪ�ȼ��,
time �β������趨�������еĵȴ�ʱ�䣬��ȷ���������λ��
11ms/55ms/210ms �ֱ��Ӧ8 λ/12 λ/14 λ
���� �¶����� �Ƿ�ʱ�ɼ��SHT11_ERROR
****************************************/
uint SHT11_Measure (uchar command, uchar time) {
    uchar data_high, data_low;
	SHT11_ERROR = ERR_OK;
    SHT11_Start();
    SHT11_Sendbyte (command);
    if (SHT11_Answer())	{
        SHT11_ERROR = ERR_TEMP_TIMEOUT;
		return 0;
	}
    Delay_Ms (time);
    if (SHT11_Test_Finish()){
        SHT11_ERROR = ERR_TEMP_TIMEOUT;
		return 0;
	}
    data_high = SHT11_Receivebyte();
    MCU_Answer();
    data_low = SHT11_Receivebyte();
    SHT11_End();
    return (data_high << 8) | data_low;
}

#ifdef TEMP14HUMD12
/****************************************
��������:Convert_Tempeture(uint dat);
��������:����⵽������ת��Ϊ��Ӧ���¶�����
����˵��:�¶�ת����ʽ--T=d1+d2*SOt
��ʽ�еĲ���d1=-40,d2=0.01
������14 λ��������
*****************************************/
float SHT11_Convert_Tempeture (uint dat) {
    float tempeture1;
    tempeture1 = -40 + 0.01 * dat;
    return (tempeture1);
}

/*****************************************
��������:SHT11_Convert_Humidity(uint dat,float temp)
��������:����⵽������ת��Ϊ��Ӧ��ʪ������
����˵��:���ʪ��ת����ʽ-----RHline=C1+C2*SOrh+C3*SOrh*SOrh( ������ݵ����Ի�
SOrh Ϊ��Ƭ�����յ�������)
-----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
��ʽ�еĲ���:C1=-4,C2=0,0405,C3=-0.0000028
t1=0.01,t2=0.00008
������12 λ��������
******************************************/
float SHT11_Convert_Humidity (uint dat, float temp) {
    float RHline, RHtrue;
    RHline = -4 + 0.0405 * dat - 0.0000028 * dat * dat;
    RHtrue = (temp - 25) * (0.01 + 0.00008 * dat) + RHline;
    return (RHtrue);
}

#else

/****************************************
��������:Convert_Tempeture12bit(uint dat);
��������:����⵽������ת��Ϊ��Ӧ���¶�����
����˵��:�¶�ת����ʽ--T=d1+d2*SOt
��ʽ�еĲ���d1=-40,d2=0.04
������12 λ��������
*****************************************/
float SHT11_Convert_Tempeture (uint dat) {
    float tempeture1;
    tempeture1 = -40 + 0.04 * dat;
    return (tempeture1);
}

/*****************************************
��������:SHT11_Convert_Humidity8bit(uint dat,float temp)
��������:����⵽������ת��Ϊ��Ӧ��ʪ������
����˵��:���ʪ��ת����ʽ-----RHline=C1+C2*SOrh+C3*SOrh*SOrh( ������ݵ����Ի�
SOrh Ϊ��Ƭ�����յ�������)
-----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
��ʽ�еĲ���:C1=-4,C2=0,648,C3=-0.00072
t1=0.01,t2=0.00128
������12 λ��������
******************************************/
float SHT11_Convert_Humidity (uint dat, float temp) {
    float RHline, RHtrue;
    RHline = -4 + 0.648 * dat - 0.00072 * dat * dat;
    RHtrue = (temp - 25) * (0.01 + 0.00128 * dat) + RHline;
    return (RHtrue);
}
#endif
