#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"SegDisp.h"

INT8U table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

//数码管显示函数：Seg_Disp()
void  Seg_Disp(INT8U temp)
{
	INT8U X1,X2,X3;
	if((temp<100)&(temp>10)) //两位显示
	{
		X1 = temp/10;
		X2 = temp - 10*X1;
		DS1CS = 0;
		DS0CS = 1;
		DispData = table[X1];
		LDelay(20);
		DS1CS = 1;
		DS0CS = 0;
		DispData = table[X2];
		}
	else if(temp<10)   //一位显示
	{
		X3 = temp;
		DS1CS = 1;
		DS0CS = 0;
		DispData = table[X3];
		}
	}