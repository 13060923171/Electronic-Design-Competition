#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"

//�ϰ����⣬���ұ���ǰ��������Ultra_DTC()
void  Ultra_DTC(void)
{
	INT8U temp;
	UltraDTC_TR = 1;   //���䳬����
	if(UltraDTC_RE) temp=0x01;
	else            temp=0x00;
	switch(temp)
	{
		//���ϣ�ԭ�ش�ת
		case 0x00:  LMotRun(LSPEED);RMotStop();     break;
		//���ϰ���������ǰ��
		case 0x01:  LMotRun(HSPEED);RMotRun(HSPEED);break;
		default: break;
		}
	}