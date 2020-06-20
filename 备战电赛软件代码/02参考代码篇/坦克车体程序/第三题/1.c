#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit P10=P1^0; 
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P14=P1^4;
fun1()
{ P10=0;P11=1;P12=0;P13=1;}
fun2()
{ P10=1;P11=0;P12=1;P13=0;}
fun3()
{ P10=1;P11=1;P12=1;P13=1;}
void init()
{ TMOD=0x01;
  TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
  EA=1;
  ET0=1;
  TR0=1;
}
void main()
{ init();
  while(1)
 { 
 }
}
void time0() interrupt 1
{  	uchar tt,k;
   TH0=(65536-50000)/256;
   TL0=(65536-50000)%256;
 	tt++;
	if(tt==100)
	{ tt=0;
	  k++;
	  if(k==1)
	  { fun1();}
	  if(k==2)
	  { fun2();}
	   if(k==3)
	  { fun3();}
	  if(k>3)
	  {k=0;}
	 }
}
	  				  