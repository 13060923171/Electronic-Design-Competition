#ifndef __FONT_H
#define __FONT_H

#include "typedef.h"

typedef struct typFNT_GB24		// 汉字字模数据结构
{
	uchar Index[2];
	uchar Msk[72];
}FNT_GB24;


typedef struct typFNT_GB16		// 汉字字模数据结构
{
	uchar Index[2];
	uchar Msk[32];
}FNT_GB16;

typedef struct typFNT_GB12		// 汉字字模数据结构
{
	uchar Index[2];
	uchar Msk[24];
}FNT_GB12;

//字库
extern FNT_GB12 code HZK12[];
extern FNT_GB16 code HZK16[];
extern uchar code ASC8x16[256][16];
extern uchar code Font24X32[12][96];
extern uchar code font6x8[][8];

#endif