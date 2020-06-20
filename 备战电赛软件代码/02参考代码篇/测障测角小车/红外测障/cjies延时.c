#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int 
//har code LED[]= {0x7f,0xc0,0xf9,0xa4,0xb0};
//uchar code LCD[]={0x20,0x5e,0x76,0x54,0x42};
sbit  SH_CP=P2^2;	//595
sbit  ST_CP=P2^3;
sbit  DS=P2^4;
sbit  ACC7=ACC^7;

sbit  RS=P2^5;		//Lcd
sbit  RW=P2^6;
sbit  E =P2^7;
sbit p30=P3^0;
sbit p31=P3^1;
sbit p32=P3^2;
sbit p00=P0^0;                       
uchar flag;
//uchar t=0;
uint time=0;

void delay(uchar t, uchar i)
{	for(;t>0;t--)
		{ for(;i>0;i--); }				 	
}

void sent(unsigned char dat)
{	uchar i;
   	ACC=dat;
   	for(i=8;i>0;i--)
   		{ 	ST_CP=0;	
			DS=ACC7;
			SH_CP=0;
		    ACC=ACC<<1;
			SH_CP=1;
   		}  
   	delay(1,3);
    ST_CP=1;
}

void lcdwd(uchar da)
{	RS=1;
	RW=0;
	E=1;
	sent(da);
	delay(1,100);
	E=0;
}

void lcdwc(uchar c)
{	RS=0;
	RW=0;
	E=1;
	sent(c);
	delay(1,100);
	E=0;
}

void lcdinit()
{	delay(15,125);
	lcdwc(0x38);
	delay(5,125);
	lcdwc(0x38);
	delay(5,125);
	lcdwc(0x38);
	delay(5,125);
	lcdwc(0x08);
	lcdwc(0x01);
	lcdwc(0x06);
	lcdwc(0x0c);
}

void WriteString(uchar *s,uchar a,uchar b)//字符串由N个单字字符组成
{	for(;*s!=0;s++)
		{	lcdwc(0x80+(a++)+0x40*b);//地址
			lcdwd(*s);				 //数据
	 	}			
}

void display()
{uchar s1[2],*ps;
s1[0]='s';
s1[1]='d';
ps=&s1;
WriteString(ps,0,0);}
//中断接收

//红外波
void time0() interrupt 1
{
  p30=~p30;
}
void inter0(void) interrupt 0
{ uchar i;
EX0=0;
//Delay_1ms(2);
if(p32==0)
for(i=0;i<250;i++)
{TR0=0;
display();
}
EX0=1;
p32=1;
}
//主程序 
void main()
{ TMOD=0X02;TH0=0XF3;TL0=0XF3;
  IE=0;EX0=1;ET0=1;TR0=1;EA=1;
  lcdinit();
   for(;;)
   {//display();
   
	}
}

