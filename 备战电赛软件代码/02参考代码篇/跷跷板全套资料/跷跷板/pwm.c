#include <reg51.h>
sbit p1_2=P1^2;
sbit p1_5=P1^5;
int setnumber,count;
void pwm(int number)
int number;
{       setnumber=number;			/*有效脉冲的个数等于设定的值*/
	TMOD=0x01;
        TH0=0xfc;
        TL0=0x18;
        EA=1;			
        ET0=1;
        TR0=1;					/*启动定时器0*/
		}
time0() interrupt 0 using 1
{  	TR0=0;					/*关定时器0*/
	if (count < setnumber)			/*如果计数变量小于给定值则给电机送有效电平*/
  	{	p1_2=0;
  	 	p1_5=0;
     	}
    else					/*否则关电机驱动*/
	{  	p1_2=1;
  	    	p1_5=1;
    	}
   count++;
   if (count>=10)				/*设定一个周期的脉冲个数为10*/
   count=0;
   TH0=0xfc;					/*重新设定计数初值，开定时器*/
   TL0=0x18;
   TR0=1;
}
main()
{ for(;;)
  pwm(2);
}


