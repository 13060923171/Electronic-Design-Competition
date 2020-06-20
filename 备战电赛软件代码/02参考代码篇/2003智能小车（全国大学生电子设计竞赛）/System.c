#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"
#include"Follow.h"
#include"LightDTC.h"
#include"MetalDTC.h"
#include"SegDisp.h"
#include"UltraDTC.h"

INT32U  RunTime=0;  //运行时间
INT8U   MetalNum=0; //直道的金属块数目

void InitIO()
{
	LMot_AP=0;LMot_AN=0;LMot_BP=0;LMot_BN=0;
	RMot_AP=0;RMot_AN=0;RMot_BP=0;RMot_BN=0;
	Follow_L=1;Follow_M=1;Follow_R=1;
	Metal_DTC=1;
	UltraDTC_TR=0;UltraDTC_RE=1;
	LightDTC_L=1;LightDTC_M=1;LightDTC_R=1;
	Guide=0;
	DS1CS=0;DS0CS=0;P2=0x00;
	}
	
void main()
{
	INT8U i;
	InitIO();
	LMotRun(LSPEED);
	RMotRun(LSPEED);
	TMOD = TMOD | 0x20 ;        //T1 工作在方式2（8位计数器,自动重载）
	TH1 = 0x00;
	TL1 = 0x9B;
	EA  = 1;
	ET1 = 1;
	TR1 = 1;
	while(1)
	{
		do //直道运行时间小于20秒，实际约为15秒
		{
			Follow();
			if(Metal_Detec()) MetalNum++;
			Seg_Disp((INT8U)(RunTime/10000));
			}
		while(RunTime<200000);
		do
		{ 
			Follow();
			Seg_Disp((INT8U)(RunTime/10000));
			}
		while(Metal_DTC != 1);
		//达到C点,停留5s，断续声光指示
		LMotStop();
		RMotStop();
		for(i=0;i<5;i++)
		{
			Guide = 1;
			T0_Delay(5000);
			Guide = 0;
			T0_Delay(5000);
			}
		do //进入障碍区
		{
			Ultra_DTC();
			Seg_Disp((INT8U)(RunTime/10000));
			}
		while(RunTime<500000);
		do //进入停车区
		{
			Light_DTC();
			Seg_Disp((INT8U)(RunTime/10000));
			}
		while(RunTime<800000);
			//停车
		LMotRun(HSPEED);
		RMotRun(HSPEED);
		LDelay(20000);
		LMotStop();
		RMotStop();
		for(i=0;i<100;i++)
			Seg_Disp((INT8U)(RunTime/10000)); 					
		}
	}
	
	
void Time1_IT(void) interrupt 3 
{
	RunTime ++;
	}