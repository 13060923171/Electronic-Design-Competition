#include<reg51.h>
#include"Config.h"
#include"IOCfg.c"
#include"TimeDLY.h"
#include"StepMot.h"
#include"Follow.h"

//Ѱ�ߺ�����Follow()
void  Follow(void)
{
	INT8U temp;
	temp = Follow_L;
	temp = (temp<<1)|Follow_M;
	temp = (temp<<1)|Follow_R;
	switch(temp&0x07)
	{
		//û��⵽����ʱ��ԭ�ش�ת
		case 0x00:  LMotRun(LSPEED);RMotStop();     break;
		//���ұ߼�⵽����ʱ��������ת���ҵ����ת
		case 0x01:  LMotRun(HSPEED);RMotRun(LSPEED);break;
		//���м��⵽����ʱ�����������ҵ������ת
		case 0x02:  LMotRun(HSPEED);RMotRun(HSPEED);break;
		//�м��Լ��ұ߼�⵽����ʱ��������ת���ҵ����ת
		case 0x03:  LMotRun(HSPEED);RMotRun(LSPEED);break;
		//����߼�⵽����ʱ��������ת���ҵ����ת
		case 0x04:  LMotRun(LSPEED);RMotRun(HSPEED);break;
		case 0x05:  break;  //�����ܳ��ִ����
		//�м��Լ���߼�⵽����ʱ��������ת���ҵ����ת
		case 0x06:  LMotRun(LSPEED);RMotRun(HSPEED);break;	
		//�м䣬���ұ߶���⵽����ʱ��ԭ�ش�ת
		case 0x07:  LMotRun(LSPEED);RMotStop();     break;
		default: break;
		}
	}