#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
// ������ƿ�
sbit P13=P1^3;//��������ǰ��
sbit P14=P1^4;//������������
sbit P15=P1^5;//�����ҵ��ǰ��
sbit P16=P1^6;//�����ҵ��ǰ��
// Ѱ��������ƿ�  
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
uchar speed;
uchar a[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71},b[2];
void xunji()
{  if(P10==1&&P11==1&&P12==1)//ȫ��
	{   P13=1;			  //ȫͣ
		P14=1; 
		P15=1;
		P16=1;
		TR0=0;		
	}
   if(P10==0&&P11==1&&P12==0)//�м����
    { speed=0;TH1=(255-speed)/256;
  TL1=(255-speed)%256;TR1=1;}
  if(P10==1&&P11==0&&P12==0)//�����
  { speed=5; TH1=(255-speed)/256;
  TL1=(255-speed)%256;TR1=1;}
  if(P10==1&&P11==1&&P12==0)//��ߺ��м���
  { speed=3; TH1=(255-speed)/256;
  TL1=(255-speed)%256;TR1=1;}
   if(P10==0&&P11==0&&P12==1) //�ұ���
   { speed=5; TH1=(255-speed)/256;
  TL1=(255-speed)%256;TR1=1;
	}
 if((P10==0&&P11==1&&P12==1))//�ұߺ��м���
   {speed=3; TH1=(255-speed)/256;
  TL1=(255-speed)%256;TR1=1;}
}
 
void main()
{ uchar speed;
  TMOD=0x21;//00010001
  TH0=(65536-50000)/256;// ��ʱ50ms
  TL0=(65536-50000)%256;
  TH1=(255-speed)/256;
  TL1=(255-speed)%256;
  EA=1;
  ET0=1;
  ET1=1;
  TR0=1;
 while(1)
  { xunji();
  }
}
void time0() interrupt 1
{ uint tt,l,shi,min;
  TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
  tt++;
  l=tt%2;
  switch(l)
  { case 0x00 : P2=0xfe;P0=b[0];break;
    case 0x01:  P2=0xfd;P0=b[1];break;
  }	
  if( tt==20)
  { tt=0;
    min++;}
	if(min==10)
	{ min=0;
	  shi++;}
	if(shi==6)
	{ shi=0;}
	b[0]=a[min];
	b[1]=a[shi];
}
	    

void time1() interrupt 3
{ uchar i=0,j=0;  
  if(P10==1&&P11==0&&P12==0)//�����
  {  TR0=0;
     P13=0;P15=1;j=1;}
     if((P10==0&&P11==0&&P12==0)&&j==1)//�����	
   { P13=0;P15=1;j=0;}
if(P10==0&&P11==0&&P12==1)//�ұ���
  {  TR0=0;
     P13=1;P15=0;i=1;}
	 if(P10==0&&P11==0&&P12==0&&i==1)
   { P13=1;P15=0;i=0;}
	 if(P10==0&&P11==1&&P12==0)//�м����
  { TR0=0; P13=0;P15=0;}
  if(P10==1&&P11==1&&P12==0)//��ߺ��м���
  { TR0=0;
    P13=0;P15=1;}
   if(P10==0&&P11==1&&P12==1)//�ұߺ��м���
  { TR0=0;
    P13=1;P15=0;}
}