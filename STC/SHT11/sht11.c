#include "sht11.h"

typedef unsigned char uchar;
typedef unsigned int uint;

bit SHT11_ERROR = 0;

/***************************
函数名称:SHT11_Delay()
函数功能:SHT11 内部延时
****************************/
void SHT11_Delay() {
    ;
    ;
}

/***************************
函数名称:Delay_Ms()
函数功能:SHT11 检测等待延时
函数说明:11ms/55ms/210ms 分别对应8 位/12 位/14 位测量结果
对应的形参为N 则延时Nms
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
函数功能:SHT11 启动时序
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
函数功能:SHT11 复位时序
***************************/
void SHT11_ConnectionReset () {
    unsigned char i;
    SHT11_DATA = 1;
    SHT11_SCK = 0;                  //准备
    for (i = 0; i < 9; i++) {       //DATA保持高，SCK时钟触发9次，发送启动传输，通迅即复位
        SHT11_SCK = 1;
        SHT11_Delay();
        SHT11_SCK = 0;
        SHT11_Delay();
    }
    SHT11_Start();
}

/******************************
函数名称： SHT11_Sendbyte(uchar dat)
函数功能: 向SHT11 发送8bit 数据
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
函数名称SHT11_Answer() ：
函数功能:检测SHT11 的响应信号(在第九个时钟周期)
返回 1:超时 0:正常
***********************************/
bit SHT11_Answer() {
    uint cnt = 0;
    SHT11_SCK = 1;
    SHT11_Delay();
    while (SHT11_DATA == 1) {
        if (cnt == TIMEOUT_CNT)	   //检测超时
            return 1;
        cnt++;
		SHT11_Delay();
    }
    SHT11_SCK = 0;
    SHT11_DATA = 1;
    return 0;
}

/************************************
函数名称： SHT11_Test_Finish()
函数功能:检测SHT11 温湿度检测是否完毕
返回 1:超时 0:正常
*************************************/
bit SHT11_Test_Finish() {
    uint cnt = 0;
    while (SHT11_DATA == 1) {
        if (cnt == TIMEOUT_CNT)	   //检测超时
            return 1;
        cnt++;
		SHT11_Delay();
    }
    return 0;
}

/************************************
函数名称： SHT11_Receivebyte()
函数功能:从SHT11 接收8bit 数据
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
    SHT11_DATA = 1; // 释放数据总线
    return (dat);
}

/***********************************
函数名称： MCU_Answer()
函数功能:单片机向SHT11 发送应答信号
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
    SHT11_DATA = 1; //释放数据总线
}

/***********************************
函数名称:SHT11_End()
当接收两个8byte 数据后部接收CRC 校验码
************************************/
void SHT11_End() {
    SHT11_DATA = 1;
    SHT11_SCK = 1;
    SHT11_Delay();
    SHT11_SCK = 0;
    SHT11_Delay();
}

/*************************************
函数名称： void SHT11_Write_Register(uchar command ,uchar dat)
函数说明：向SHT11 的状态寄存器设置功能
command 为REG_WRITE 0x06 写寄存器
dat 为设置SHT11 的功能可以设置检测的数据位数
返回 1:超时 0:正常
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
函数名称： uchar SHT11_Read_Register(uchar command)
函数说明： command 为REG_READ 0x07// 读寄存器
返回值为状态寄存器的值
位6 显示当前检测完一次数据后电源供电情况
当位6 为0 时表明VDD>2.47V 当位6 为1 时表明VDD<2.47V 即电量不足
位0 表明当前的测量分辨率
当位0 为1 时表明测量精度： 8 位/湿度12 位温度
当位0 为0 时表明测量精度： 12 位湿度14 位温度
默认为0, 0xff超时
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
函数名称:SHT11_Measure(uchar command,uint time);
函数功能:设置SHT11 检测功能,并返回相应的检测结果
函数说明:command 形参用于设定温度检测还是湿度检测,
time 形参用于设定检测过程中的等待时间，以确定检测结果的位数
11ms/55ms/210ms 分别对应8 位/12 位/14 位
返回 温度数据 是否超时可检测SHT11_ERROR
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
函数名称:Convert_Tempeture(uint dat);
函数功能:将检测到的数据转化为相应的温度数据
函数说明:温度转换公式--T=d1+d2*SOt
公式中的参数d1=-40,d2=0.01
适用于14 位测量精度
*****************************************/
float SHT11_Convert_Tempeture (uint dat) {
    float tempeture1;
    tempeture1 = -40 + 0.01 * dat;
    return (tempeture1);
}

/*****************************************
函数名称:SHT11_Convert_Humidity(uint dat,float temp)
函数功能:将检测到的数据转化为相应的湿度数据
函数说明:相对湿度转换公式-----RHline=C1+C2*SOrh+C3*SOrh*SOrh( 检测数据的线性化
SOrh 为单片机接收到的数据)
-----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
公式中的参数:C1=-4,C2=0,0405,C3=-0.0000028
t1=0.01,t2=0.00008
适用于12 位测量精度
******************************************/
float SHT11_Convert_Humidity (uint dat, float temp) {
    float RHline, RHtrue;
    RHline = -4 + 0.0405 * dat - 0.0000028 * dat * dat;
    RHtrue = (temp - 25) * (0.01 + 0.00008 * dat) + RHline;
    return (RHtrue);
}

#else

/****************************************
函数名称:Convert_Tempeture12bit(uint dat);
函数功能:将检测到的数据转化为相应的温度数据
函数说明:温度转换公式--T=d1+d2*SOt
公式中的参数d1=-40,d2=0.04
适用于12 位测量精度
*****************************************/
float SHT11_Convert_Tempeture (uint dat) {
    float tempeture1;
    tempeture1 = -40 + 0.04 * dat;
    return (tempeture1);
}

/*****************************************
函数名称:SHT11_Convert_Humidity8bit(uint dat,float temp)
函数功能:将检测到的数据转化为相应的湿度数据
函数说明:相对湿度转换公式-----RHline=C1+C2*SOrh+C3*SOrh*SOrh( 检测数据的线性化
SOrh 为单片机接收到的数据)
-----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
公式中的参数:C1=-4,C2=0,648,C3=-0.00072
t1=0.01,t2=0.00128
适用于12 位测量精度
******************************************/
float SHT11_Convert_Humidity (uint dat, float temp) {
    float RHline, RHtrue;
    RHline = -4 + 0.648 * dat - 0.00072 * dat * dat;
    RHtrue = (temp - 25) * (0.01 + 0.00128 * dat) + RHline;
    return (RHtrue);
}
#endif
