#include"reg51.h"
#define uchar unsigned char
#define uint unsigned int
uchar code seg[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code segnode[10]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
#define digport P0
#define dataport P1
#define wordport P2
#define r 45
sbit  eoc= P3^3;
sbit  clk= P3^4;
sbit  start= P3^6;
sbit  oe= P3^7;
bit flag;
uchar a[4]={0xff,0xff,0xff,0xff};//ºÚÆÁ
uchar dig=0x01;

void delay(uint t)
{for(;t<0;t--);}

/*void display()
{
	uchar i;
		wordport=0xff;
	for(i=0;i<4;i++)
		{
		
			wordport=a[i];
			digport=dig<<1;
			delay(300);
		 }
}*/
void Display(void)
{
	uchar i,k,select=0x01;
       for(i=0;i<=5;i++)
             { digport=select;
			   wordport=a[i];
               select<<=1;
			    for(k=0;k<=100;k++)//¸Ä
				{;}
	         }

}
void time0() interrupt 1
{	clk=~clk; }		
void chuli()
{			
	unsigned long  int da;
	//eoc=0; 
	start=1;
	oe=1;
	da=dataport;
	delay(10);
	oe=0;
//	da=da*196;
    if(da<127)a[3]=0xbf;//128¸Ä127//127.5=127
    if(da>=127)a[3]=0xff;
//	if(da<5000||da>45000){da=0;}
   
    if(a[3]==0xff) {da=da-127;	da=da*1961;} //}if(da>49000)da=50000;}
    if(a[3]==0xbf) {da=128-da;	da=da*1961;}//128¸Ä127
   
	da=da*r;
	if(da>9000000)da=9000000;
	a[0]=seg[da/10000%10];
	if((da/1000%10)>5) 
	a[0]=seg[da/10000%10+1];
	a[1]=segnode[da/100000%10];
	a[2]=seg[da/1000000];
	start=0;	
	delay(10);
 }

void main(void)
{   uchar k;
	delay(30);
//	IT1=1;EX1=1;
	TMOD=0x02;TH0=253;TL0=253;
	TR0=1;ET0=1;PT0=1;
	EA=1;
	//eoc=0;
	start=0;
	start=1;
	start=0;
	for(;;)
	{		
		if(eoc)	chuli();
		for(k=0;k<100;k++)
{	Display();}
	}
	
}	
