#include<reg52.h>
unsigned int i=0;
sbit P36=P3^6;
unsigned char j;
unsigned int kop;
unsigned char a[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char b[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
void delaym(unsigned int t)
{ unsigned char p;	      
while(t--)
{
for(p=0; p<125; p++);		
}
}

void main()
{
    EA = 1;
    TH0=0x00;
	TL0=0x00;
	ET0=1;
	ET1=1;		
  	TMOD = 0x15;// �򿪶�ʱ���ͼ�����T0����˾�����������˽�P3.4
	TH1=0x3C; //50ms
	TL1=0xB0;		
    TR0 = 1;
	TR1 = 1;
while(1)
{ 
  for(j=0;j<3;j++) //��ʾ����
  {
  P0=b[j];
  P2=j;
  delaym(2);
   }
  }
}
timer3() interrupt 3  //��ʱ���ж�ʱ�䵽,�Ͷ�ȡ������ֵ
{ TH1=0X3C;
 TL1=0XB0;
 i++;
if(i==20) //1s�Ž��м�������ֵ��ʾ����ÿ�����һ���ٶ�
{
   kop=TH0;//�������������壬ÿ��һ�������������ֵ��1
   kop=kop<<8;
   kop=kop+TL0;

b[2]=a[kop/100];       //��ʾ��������ֵ��ʵ�ʲ���ٶ�
b[1]=a[kop%100/10];
b[0]=a[kop%100%10];
i=0;
TH0=0;//�������ٶȻ�ȡ�����㣬�����´λ�ȡ
TL0=0;
}
}