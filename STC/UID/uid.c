#include "uid.h"

void UID_Read(unsigned char *buf) {
	unsigned char idata *idata_point;
	unsigned char j;
	idata_point = 0xF1;
    for (j = 0; j < 15; j++) {
        buf[j] = *idata_point;
        idata_point++;
    }
}