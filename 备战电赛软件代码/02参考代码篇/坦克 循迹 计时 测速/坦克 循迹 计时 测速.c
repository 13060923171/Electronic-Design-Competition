#include<reg52.h>
unsigned int i=0;
sbit P36=P3^6; 

sbit P10=P1^0;	 //ѭ����
sbit P11=P1^1;
sbit P12=P1^2;

sbit P14=P1^4;	 //�����
sbit P15=P1^5;
sbit P16=P1^6;			   
sbit P17=P1^7;
unsigned char j,time;
unsigned int kop,dr;
unsigned char a[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char c[4]={0xff,0xff,0xff,0xff},d[4];

void delay(unsigned char z)	  //��ʱ�ӳ���
{
 unsigned char j;
 while(z--)
 for(j=123;j>0;j--);
}

   void main()
  {	
    TMOD=0x15;              // �򿪶�ʱ���ͼ�����
	TH0=0x00;
	TL0=0x00;
    		
  	TH1=0XFc;               //1ms
	TL1=0X18;
	EA = 1;
    ET0=1;
	ET1=1;
    TR1 = 1;
	TR0 = 1;

   while(1)
      {  
	      if(P10==0&&P11==0&&P12==0)
	     { 
		  TR0=0;				  //��ʾʱ��	  ͣ
	      P0=d[0];		  
	      P2=0XFE;
	      delay(1);
	      P0=d[3];		  
	      P2=0XFB;
	      delay(3);

	     P0=d[2];				 //��ʾ����		   ͣ
	     P2=0XFD;
	     delay(3);
	     P0=d[3];		
	     P2=0XF7;
	     delay(3);
	     P14=1;P15=1;P16=1; P17=1;
        }
      }
  }
   void timer1() interrupt 3    //�ж� �������
 {
   unsigned int qq=0,ww=0;
   TH1=0XFc;
   TL1=0X18;//1ms
   i++;
    switch(i%4)
    {			
    case 0:P0=c[0],P2=0XFE;d[0]=c[0];break;
    case 1:P0=c[1],P2=0XFD;d[1]=c[1];break;
    case 2:P0=c[2],P2=0XFB;d[2]=c[2]; break;
    case 3:P0=c[3],P2=0XF7;d[3]=c[3]; break;
    default:break;
    }		   
    P14=1;P15=1;P16=1; P17=1;
	
	if(P10==0&&P11==0&&P12==0)		//ȫ�⵽
	dr=0;
	if(P10==1&&P11==0&&P12==1)		//�м�⵽
	dr=1; 
	if(P10==0&&P11==1&&P12==1)		 //��߲⵽
	dr=2;
    if(P10==1&&P11==1&&P12==0)		 //�ұ߲⵽
	dr=3;
	if(P10==0&&P11==0&&P12==1)		 //�����⵽
	dr=4;
	if(P10==1&&P11==0&&P12==0)		 //�����⵽	
	dr=5;
    
	switch(dr)
	 {
     case 0:P14=1,P15=1,P16=1,P17=1,TR1=0;break;	      //ȫ�⵽     ͣ
	  
     case 1:P14=0,P15=1,P16=0,P17=1;break;	      //ֱ��
	 
	 case 2:P14=1,P15=1,P16=0,P17=1,ww=1;break;    //���
	  
	 case 3:P14=0,P15=1,P16=1,P17=1,qq=1;break;	   //�ҹ�
	  
	 case 4:P14=1,P15=0,P16=0,P17=1,ww=1;break;	   //�����⵽   ���
 
	 case 5:P14=0,P15=1,P16=1,P17=0,qq=1;break;	   //�����⵽   �ҹ�
	 } 
	 /*�ܳ��ܵ������ �ص��ܵ��� */
 	 if(dr==2&&ww==1||dr==4&&ww==1)			  //���߻�˫�� �⵽�ܳ�ȥ 
	    {
			qq=0;
		 	P14=1;P15=1;P16=0;P17=1;
		}
	  if(dr==3&&qq==1||dr==5&&qq==1)
		{
		     ww=0;
		  P14=0;P15=1;P16=1;P17=1;
	    }

           if(i==1000) //1s
           {
             kop=TH0;//��������������
             kop=kop<<8;
             kop=kop+TL0;
             kop=kop*15;
             c[3]=a[kop/10];
             c[2]=a[kop%10];
             time++;
             c[1]=a[time/10];
             c[0]=a[time%10];
             i=0;
             TH0=0;
             TL0=0;
            }
} 
