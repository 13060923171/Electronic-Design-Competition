#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"
#include"MetalDTC.h"

//������⺯����MetalDetec()
bit  Metal_Detec()
{
	bit x;
	if(Metal_DTC == 1) //��⵽����
	{
		Guide = 1;  //���ⱨ��
		x = 1;	   	
		}
	else
	{
		Guide = 0;
		x = 0;
		}
	return x;
	}