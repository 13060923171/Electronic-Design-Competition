#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"

/**��ߵ��**/
//��ָ���ٶ����к�����MotRun()
void LMotRun(INT16U speed)
{
	INT16U cnt;
	cnt = 12500/speed;
	LMot_AP = 1;
	LMot_AN = 0;
	LMot_BP = 0;
	LMot_BN = 0;
	LDelay(cnt);
	LMot_AP = 0;
	LMot_AN = 1;
	LDelay(cnt);
	LMot_AN = 0;
	LMot_BP = 1;
	LDelay(cnt);
	LMot_BP = 0;
	LMot_BN = 1;
	LDelay(cnt);
	LMot_BN = 0;	
	}

//ֹͣ���������MotStop()
void LMotStop(void)
{
	LMot_AP = 0;
	LMot_AN = 0;
	LMot_BP = 0;
	LMot_BN = 0;
	}

/**�ұߵ��**/
//��ָ���ٶ����к�����MotRun()
void RMotRun(INT16U speed)
{
	INT16U cnt;
	cnt = 12500/speed;
	RMot_AP = 1;
	RMot_AN = 0;
	RMot_BP = 0;
	RMot_BN = 0;
	LDelay(cnt);
	RMot_AP = 0;
	RMot_AN = 1;
	LDelay(cnt);
	RMot_AN = 0;
	RMot_BP = 1;
	LDelay(cnt);
	RMot_BP = 0;
	RMot_BN = 1;
	LDelay(cnt);
	RMot_BN = 0;	
	}

//ֹͣ���������MotStop()
void RMotStop(void)
{
	RMot_AP = 0;
	RMot_AN = 0;
	RMot_BP = 0;
	RMot_BN = 0;
	}