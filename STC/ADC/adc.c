#include "adc.h"

void Init_ADC() {
    //设置P1
    P1ASF = 1 << ADC_R_PIN;
    AUXR1 |= 1 << 2;	 	//1:ADC_RES<<8+ADC_RESL
    ADC_RES = 0;
    ADC_RESL = 0;
    ADC_CONTR = 0x80 | ADC_SPEED << 5;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

unsigned int Read_ADC () {
    //开始转换
    ADC_CONTR |= 0x08;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    //等待转换完成
    while ((ADC_CONTR & 0x10) == 0);
    //清标志位
    ADC_CONTR &= 0xEF;
    return (ADC_RES << 8) | (ADC_RESL);
}