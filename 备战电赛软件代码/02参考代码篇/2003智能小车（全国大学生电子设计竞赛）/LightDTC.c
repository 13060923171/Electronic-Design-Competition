#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"
#include"LightDTC.h"

//寻线函数：LightDTC()
void  Light_DTC(void)
{
	INT8U temp;
	temp = LightDTC_L;
	temp = (temp<<1)|LightDTC_M;
	temp = (temp<<1)|LightDTC_R;
	switch(temp&0x07)
	{
		//没检测到光源时，原地打转
		case 0x00:  LMotRun(LSPEED);RMotStop();     break;
		//仅右边检测到光源时，左电机快转，右电机慢转
		case 0x01:  LMotRun(HSPEED);RMotRun(LSPEED);break;
		//仅中间检测到光源时，正常，左右电机都快转
		case 0x02:  LMotRun(HSPEED);RMotRun(HSPEED);break;
		//中间以及右边检测到光源时，左电机快转，右电机慢转
		case 0x03:  LMotRun(HSPEED);RMotRun(LSPEED);break;
		//仅左边检测到光源时，左电机慢转，右电机快转
		case 0x04:  LMotRun(LSPEED);RMotRun(HSPEED);break;
		case 0x05:  break;  //不可能出现此情况
		//中间以及左边检测到光源时，左电机慢转，右电机快转
		case 0x06:  LMotRun(LSPEED);RMotRun(HSPEED);break;	
		//中间，左右边都检测到光源时，正常，左右电机都快转
		case 0x07:  LMotRun(HSPEED);RMotRun(HSPEED);break;
		default: break;
		}
	}