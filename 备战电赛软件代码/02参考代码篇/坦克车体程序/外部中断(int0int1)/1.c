#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit d0=P0^0;
unsigned char a[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void delay( uint z)
{ uint j;
  while( z--)
  { for(j=110;j>0;j--);}
}
void main()
{ uchar i;
  EA=1;
  EX0=1;
  //IT0=0;//
  IT0=1;//Ìø±ä
 while(1)
  {  for(i=0;i<8;i++)
     { d0=1;
	   P0=a[i];
	   P2=i;
	   delay(300);
	  }
  }
}
void exter0() interrupt 1
{ P0=0xff; d0=0;}