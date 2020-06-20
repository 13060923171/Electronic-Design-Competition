#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar x3,x4;

sbit P20=P3^6;
sbit P21=P3^7;
sbit P22=P1^3;
sbit P23=P1^4;
sbit P24=P1^5;
sbit P25=P1^6;

void ds(uchar x1,uchar x2)
{
	TMOD=0X01;
	TH0=x1;
	TL0=x2;
	x3=x1;
	x4=x2;

	EA=1;ET0=1;
	TR0=1;	
}


void main()
{

	while(1)
	{
	
	ds(0xff,0xff);	  //全速前进
	ds(0xff,0xe1);	  //减速
	P20=1;
	P21=1;
	P22=1;
	P23=1; 
	P24=1;
	P25=1;	
	}	
}
void time0()interrupt 1
{
	TH0=x3;
	TL0=x4;
	
	if(x4==0xff)	//左边亮
	{
		P23=0;		 //右边走
		P25=1;
	}
		if(x4==0xe1) //右边亮
	{
		P23=1;		 //左边走
		P25=0;
	}
}