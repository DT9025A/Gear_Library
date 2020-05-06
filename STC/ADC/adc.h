#ifndef __ADC_H_
#define __ADC_H_

#include <STC12C5A60S2.H>
#include <intrins.h>

#define ADC_SPEED 0	//0-3
#define ADC_R_PIN 0

void Init_ADC();
unsigned int Read_ADC();

#endif