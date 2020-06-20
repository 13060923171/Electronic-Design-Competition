#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int 
sbit p32=P3^2;
sbit p20=P2^0;
void inter0(void) interrupt 0
{p20=0;} 
void main()
{ 
  IE=0;EX0=1;EA=1;  p20=1;
}
