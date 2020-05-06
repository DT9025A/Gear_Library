#include "ds18b20.h"

unsigned int DS18B20_ReadInt(){
	return DSS();
}

float DS18B20_ReadFloat() {
	unsigned int temper;
    float tp;
    temper = DSS();
    if (temper < 0) { //考虑负温度的情况
        temper = temper - 1;
        temper = ~temper;
    }
	tp = temper * 0.0625; //16位温度转换成10进制的温度
    temper = tp * 100 + 0.5; //留两个小数点，并四舍五入
}

unsigned int DS18B20_ReadIntWithFloat(){
	return DSS();
}

unsigned int temper_change() {
    
    return temper;
}