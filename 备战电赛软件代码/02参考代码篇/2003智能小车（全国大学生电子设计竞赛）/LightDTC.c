#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"
#include"LightDTC.h"

//Ѱ�ߺ�����LightDTC()
void  Light_DTC(void)
{
	INT8U temp;
	temp = LightDTC_L;
	temp = (temp<<1)|LightDTC_M;
	temp = (temp<<1)|LightDTC_R;
	switch(temp&0x07)
	{
		//û��⵽��Դʱ��ԭ�ش�ת
		case 0x00:  LMotRun(LSPEED);RMotStop();     break;
		//���ұ߼�⵽��Դʱ��������ת���ҵ����ת
		case 0x01:  LMotRun(HSPEED);RMotRun(LSPEED);break;
		//���м��⵽��Դʱ�����������ҵ������ת
		case 0x02:  LMotRun(HSPEED);RMotRun(HSPEED);break;
		//�м��Լ��ұ߼�⵽��Դʱ��������ת���ҵ����ת
		case 0x03:  LMotRun(HSPEED);RMotRun(LSPEED);break;
		//����߼�⵽��Դʱ��������ת���ҵ����ת
		case 0x04:  LMotRun(LSPEED);RMotRun(HSPEED);break;
		case 0x05:  break;  //�����ܳ��ִ����
		//�м��Լ���߼�⵽��Դʱ��������ת���ҵ����ת
		case 0x06:  LMotRun(LSPEED);RMotRun(HSPEED);break;	
		//�м䣬���ұ߶���⵽��Դʱ�����������ҵ������ת
		case 0x07:  LMotRun(HSPEED);RMotRun(HSPEED);break;
		default: break;
		}
	}