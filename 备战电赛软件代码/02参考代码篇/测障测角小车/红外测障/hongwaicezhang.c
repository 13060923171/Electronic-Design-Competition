#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned  int
sbit p30=P3^0;
sbit p20=P2^0;
sbit p32=P3^2;
sbit p10=P1^0;
uint n;
bit flag;
//ºìÍâ²¨
void delay(uint t, uchar i)
{	for(;t>0;t--)
		{ for(;i>0;i--); }				 	
}

void time0() interrupt 1
{
  p30=~p30;
}
/*void inter0(void) interrupt 0
{p20=0;
flag=1;
} */
void inter1() interrupt 2
{p20=0;
delay(8000,125);//0.5S
}
void main()
{
	TMOD=0X02;TH0=0XF3;TL0=0XF3;
	IT1=1;
    EX1=1;
	PX1=1;

	EA=1;
	p30=0;//p20=1;
	while(1)
	{p20=1;
	ET0=1;TR0=1;
	delay(80,125);
	ET0=0;TR0=0;
	delay(80,125);}

	
}

