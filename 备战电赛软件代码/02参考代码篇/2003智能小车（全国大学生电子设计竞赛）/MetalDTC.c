#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"
#include"MetalDTC.h"

//金属检测函数：MetalDetec()
bit  Metal_Detec()
{
	bit x;
	if(Metal_DTC == 1) //检测到金属
	{
		Guide = 1;  //声光报警
		x = 1;	   	
		}
	else
	{
		Guide = 0;
		x = 0;
		}
	return x;
	}