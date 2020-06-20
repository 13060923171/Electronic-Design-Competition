#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned  int
uchar jianhao _at_ 0x30;
sbit P31=P3^1;
/*sbit p20=P2^0;
sbit p21=P2^1;
sbit p22=P2^2;
sbit p23=P2^3;
sbit p24=P2^4;
sbit p25=P2^5;
sbit p26=P2^6;*/
sbit p27=P2^7;
sbit P34=P3^4;
sbit p32=P3^2;
sbit p33=P3^3;
sbit p34=P3^4;
sbit p35=P3^5;
bit flag;

//uchar code a[5]={0x7f,0xdc,0xe3,0x9c,0xce};
//ºìÍâ²¨
void time0() interrupt 1
{
  P31=~P31;
}

void Delay_1ms(uint j)
{ uchar i=0;
    for(;j>0;j--)
      for(i=0;i<125;i++);
}

void fashong() 
{ uchar k;
ET0=1;TR0=1;
Delay_1ms(3);
for(k=0;k<jianhao;k++)
{ET0=0;TR0=0;P31=0;
Delay_1ms(1);
ET0=1;TR0=1;
Delay_1ms(1);
}
ET0=0;TR0=0;P31=0;
Delay_1ms(3);
}

void  key()
{if(p32==0)
{jianhao=1;flag=1;}
if(p33==0)
{jianhao=2;flag=1;}
if(p34==0)
{jianhao=3;flag=1;}
if(p35==0)
{jianhao=4;flag=1;}

}

void main()
{

 TMOD=0X02;TH0=0XF3;TL0=0XF3;EA=1;P31=0;jianhao=0;
while(1)
  {//p20=1;p21=1;p22=1;p23=1;
 // p24=0;p25=0;p26=0;p27=0;
 p32=1;p33=1;p34=1;p35=1;
    // P1=a[jianhao];
	 key();
     if(flag) 
	{ p27=0;
	fashong();
	 flag=0;}
	 p27=1;
	}
}