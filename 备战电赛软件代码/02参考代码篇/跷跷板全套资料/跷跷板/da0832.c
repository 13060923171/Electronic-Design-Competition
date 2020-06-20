#include <reg51.h>
sbit ale=P3^5;
sbit p2_0=P2^0;
sbit p3_6=P3^6;
delay1ms(int t);
void da0832(int dadata)   //单极性转换
{	ale=1;           //ls373片选有效
	delay1ms(1);
	p2_0=0;         //0832片选cs有效
	ale=0;          //ls373锁存数据
	P2=dadata;      //送数据，不需373锁存
   	p3_6=0;         //0832锁存数据
	return;
	
}
