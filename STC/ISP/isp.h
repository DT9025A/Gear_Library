#ifndef __STC12C2052_ISP_H_
#define __STC12C2052_ISP_H_

#include <STC12C2052AD.H>


#define ISP_IAP_BYTE_READ 1
#define ISP_IAP_BYTE_PROGRAM 2
#define ISP_IAP_SECTOR_ERASE 3

#define WAIT_TIME 3

#define FLASH_START_ADDR 0x1000

void ByteProgram (unsigned int addrOffset, unsigned char x);
void BufProgram (unsigned int addrOffset, unsigned int cnt, unsigned char *x);
unsigned char ByteRead (unsigned int addrOffset);
void BufRead (unsigned int addrOffset, unsigned int cnt, unsigned char *buf);
void SectorErase(unsigned char sectorNumber);

#endif