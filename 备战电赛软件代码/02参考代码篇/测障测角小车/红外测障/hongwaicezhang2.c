#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned  int
sbit p30=P3^0;
sbit p20=P2^0;
sbit p32=P3^2;
uint n;
//ºìÍâ²¨
void delay(uint t, uchar i)
{	for(;t>0;t--)
		{ for(;i>0;i--); }				 	
}

void time0() interrupt 1
{n++;
  if(n>=308)
	{	p30=0;
		if(n==616)	
			n=0;
	}
  else  p30=~p30;
}
void inter1() interrupt 2
{p20=0;
delay(8000,125);//0.5S
}
void main()
{
	TMOD=0X02;TH0=0XF3;TL0=0XF3;
	ET0=1;TR0=1;
	EX1=1;IT1=1;
	PX1=1;
	EA=1;
	while(1)
		{p20=1;p30=0;
		}
}

