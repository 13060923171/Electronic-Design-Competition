#include <reg51.h>
sbit p1_2=P1^2;
sbit p1_5=P1^5;
int setnumber,count;
void pwm(int number)
int number;
{       setnumber=number;			/*��Ч����ĸ��������趨��ֵ*/
	TMOD=0x01;
        TH0=0xfc;
        TL0=0x18;
        EA=1;			
        ET0=1;
        TR0=1;					/*������ʱ��0*/
		}
time0() interrupt 0 using 1
{  	TR0=0;					/*�ض�ʱ��0*/
	if (count < setnumber)			/*�����������С�ڸ���ֵ����������Ч��ƽ*/
  	{	p1_2=0;
  	 	p1_5=0;
     	}
    else					/*����ص������*/
	{  	p1_2=1;
  	    	p1_5=1;
    	}
   count++;
   if (count>=10)				/*�趨һ�����ڵ��������Ϊ10*/
   count=0;
   TH0=0xfc;					/*�����趨������ֵ������ʱ��*/
   TL0=0x18;
   TR0=1;
}
main()
{ for(;;)
  pwm(2);
}


