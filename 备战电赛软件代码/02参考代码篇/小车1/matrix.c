#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
uchar temp;
char num;
void bdelay(uchar);
void display(uchar);
int scanmtx()
{
		num = -1;
		P1=0xfe;
		temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			bdelay(22);
			if(temp!=0xf0)
			{
				switch(temp)
				{
					case 0xe0:num=1;
					break;
					case 0xd0:num=2;
					break;
					case 0xb0:num=3;
					break;
					case 0x70:num=10;
					break;
				}
			}
			while(temp!=0xf0)//键盘松开去抖动 
			{
				temp=P1;
				temp=temp&0xf0;
			}	
		}
		P1=0xfd;
		temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			bdelay(22);
			if(temp!=0xf0)
			{
				switch(temp)
				{
					case 0xe0:num=4;
					break;
					case 0xd0:num=5;
					break;
					case 0xb0:num=6;
					break;
					case 0x70:num=11;
					break;
				}
			}
			while(temp!=0xf0)//键盘松开去抖动 
			{
				temp=P1;
				temp=temp&0xf0;
			}		
		}
		P1=0xfb;
		temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			bdelay(22);
			if(temp!=0xf0)
			{
				switch(temp)
				{
					case 0xe0:num=7;
					break;
					case 0xd0:num=8;
					break;
					case 0xb0:num=9;
					break;
					case 0x70:num=12;
					break;
				}
			}
			while(temp!=0xf0)//键盘松开去抖动 
			{
				temp=P1;
				temp=temp&0xf0;
			}		
		}
		P1=0xf7;
		temp=P1;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			bdelay(22);
			if(temp!=0xf0)
			{
				switch(temp)
				{
					case 0xe0:num=15;
					break;
					case 0xd0:num=0;
					break;
					case 0xb0:num=14;
					break;
					case 0x70:num=13;
					break;
				}
			}
			while(temp!=0xf0)//键盘松开去抖动 
			{
				temp=P1;
				temp=temp&0xf0;
			}
		//display(num);
		}
	return num;
}
void bdelay(uchar x)
{	
	uchar i,j;
	for(i=x;i>0;i--)
		for(j=410;j>0;j--);
}
