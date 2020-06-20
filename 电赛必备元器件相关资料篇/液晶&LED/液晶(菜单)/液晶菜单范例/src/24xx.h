#include "typedef.h"

#ifndef _24XX_H
#define _24XX_H

#define ERRORCOUNT 		10

enum eepromtype {M2401,M2402,M2404,M2408,M2416,M2432,M2464,M24128,M24256};

extern bit IICReadWrite(uchar *DataBuff,	/*DataBuffΪ��д�������룯�����������*/
		  uchar ByteQuantity,	/*ΪҪ��д���ݵ��ֽ�*/
		  uint16 Address,	/*ΪEEPROM��Ƭ�ڵ�*/
		  uchar ControlByte,	/**/
		  enum eepromtype EepromType);	/**/



#endif
