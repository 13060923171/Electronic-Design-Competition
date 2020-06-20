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
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
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

void xunji()
{
	if(P10==1&&P11==1&&P12==1)//全亮
	{
		P22=1;			  //全停
		P23=1; 
		P24=1;
		P25=1;		
	}
	if((P10==0&&P11==0&&P12==0)||(P10==0&&P11==1&&P12==0))//全灭或中间灯亮
	{
		ds(0xff,0xff);	  //全速前进
	}
	if(P10==1&&P11==0&&P12==0)	//左边亮
	{
		ds(0xff,0xe3);	  //减速右拐
	}
	if(P10==0&&P11==0&&P12==1) //右边亮
	{
		ds(0xff,0xe3);	 //减速左拐
	}
}
void main()
{

	while(1)
	{
	xunji();
	
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
	if((P10==0&&P11==0&&P12==0)||(P10==0&&P11==1&&P12==0))//全灭或中间灯亮
	{
		P23=0;	//全速前进
		P25=0;
	}
	if(P21==1&&P11==0&&P12==0)	//左边亮
	{
		P23=0;		 //右边走
		P25=1;
	}
		if(P10==0&&P11==0&&P12==1) //右边亮
	{
		P23=1;		 //左边走
		P25=0;
	}
}