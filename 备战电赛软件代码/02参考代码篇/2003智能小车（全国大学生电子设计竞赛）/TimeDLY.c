#include<reg51.h>
#include"Config.h"
#include"TimeDLY.h"

//短延时
void nNop(INT8U i)
{
	for(;i>0;i--) ;
	}
	
//长延时
void LDelay(INT16U i)
{
	INT16U j;
	for(;i>0;i--)
		{ for(j=80;j>0;j--);}
	}

//定时器T0溢出做延时函数：T0_Delay()
void T0_Delay(INT16U temp)
{
	INT16U cnt;
	cnt = 65536 - 100*temp;	    //晶振：12MHz
	TMOD = TMOD | 0x01 ;        //T1 工作在方式1（16位计数器）
	TH0 = (INT8U)(cnt >> 8);
	TL0 = cnt & 0xff;
	TR0 = 1;
	while(TF0 != 1);
	TF0 = 0;
	TR0 = 0;
	}