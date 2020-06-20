#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
//sbit P20=P3^6;
//sbit P21=P3^7;
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
void main()
{
 while(1)
{
   // P20=1;
  //P21=1;
	P10=1;
	P11=0; 
	P12=0;
	P13=1;	
	}	
}
