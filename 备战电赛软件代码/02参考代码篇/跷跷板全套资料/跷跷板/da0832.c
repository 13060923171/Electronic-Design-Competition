#include <reg51.h>
sbit ale=P3^5;
sbit p2_0=P2^0;
sbit p3_6=P3^6;
delay1ms(int t);
void da0832(int dadata)   //������ת��
{	ale=1;           //ls373Ƭѡ��Ч
	delay1ms(1);
	p2_0=0;         //0832Ƭѡcs��Ч
	ale=0;          //ls373��������
	P2=dadata;      //�����ݣ�����373����
   	p3_6=0;         //0832��������
	return;
	
}
