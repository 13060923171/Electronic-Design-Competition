#include "typedef.h"

#ifndef _24XX_H
#define _24XX_H

#define ERRORCOUNT 		10

enum eepromtype {M2401,M2402,M2404,M2408,M2416,M2432,M2464,M24128,M24256};

extern bit IICReadWrite(uchar *DataBuff,	/*DataBuff为读写数据输入／输出缓冲区的*/
		  uchar ByteQuantity,	/*为要读写数据的字节*/
		  uint16 Address,	/*为EEPROM的片内地*/
		  uchar ControlByte,	/**/
		  enum eepromtype EepromType);	/**/



#endif
