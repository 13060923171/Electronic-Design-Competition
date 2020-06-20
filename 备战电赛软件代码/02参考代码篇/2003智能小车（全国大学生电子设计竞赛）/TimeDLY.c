#include<reg51.h>
#include"Config.h"
#include"TimeDLY.h"

//����ʱ
void nNop(INT8U i)
{
	for(;i>0;i--) ;
	}
	
//����ʱ
void LDelay(INT16U i)
{
	INT16U j;
	for(;i>0;i--)
		{ for(j=80;j>0;j--);}
	}

//��ʱ��T0�������ʱ������T0_Delay()
void T0_Delay(INT16U temp)
{
	INT16U cnt;
	cnt = 65536 - 100*temp;	    //����12MHz
	TMOD = TMOD | 0x01 ;        //T1 �����ڷ�ʽ1��16λ��������
	TH0 = (INT8U)(cnt >> 8);
	TL0 = cnt & 0xff;
	TR0 = 1;
	while(TF0 != 1);
	TF0 = 0;
	TR0 = 0;
	}