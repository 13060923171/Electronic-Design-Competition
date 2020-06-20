#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"

//障碍物检测，并且避障前进函数：Ultra_DTC()
void  Ultra_DTC(void)
{
	INT8U temp;
	UltraDTC_TR = 1;   //发射超声波
	if(UltraDTC_RE) temp=0x01;
	else            temp=0x00;
	switch(temp)
	{
		//避障：原地打转
		case 0x00:  LMotRun(LSPEED);RMotStop();     break;
		//无障碍：正常，前进
		case 0x01:  LMotRun(HSPEED);RMotRun(HSPEED);break;
		default: break;
		}
	}