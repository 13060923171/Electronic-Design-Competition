#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
//sbit d0=P0^0;
uchar tie;
unsigned char a[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e},b[2];
/*void delay( uint z)
{ uint j;
  while( z--)
  { for(j=110;j>0;j--);}
}*/
void main()
{  
  TMOD=0X01;
  TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
  EA=1;
  ET0=1;
  TR0=1;
  EX0=1;
  //IT0=0;//
  IT0=1;//Ìø±ä
 while(1)
  {  /*for(i=0;i<8;i++)
     { d0=1;
	   P0=a[i];
	   P2=i;
	   delay(300);
	  }	*/
	  
  }
}
void exter0() interrupt 0
{ tie++;}
void time0() interrupt 1
{  uint tt,l,shi,min;
  TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
  tt++;
  l=tt%3;
  switch(l)
  { case 0x00 : P2=0x00;P0=b[0];break;
    case 0x01:  P2=0x01;P0=b[1];break;
	case 0x02 : P2=0x02;P0=a[tie];break;
  }	
  if( tt==20)
  { tt=0;
    min++;}
	if(min==10)
	{ min=0;
	  shi++;}
	if(shi==6)
	{ shi=0;}
	b[0]=a[min];
	b[1]=a[shi];
}
			