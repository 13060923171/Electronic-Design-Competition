/*connect us:
Web:https://sites.google.com/site/journeybuaa/
Email:stingshen1@gmail.com

 */

#include <reg52.h>
#include "matrix.c"
#include "1602lcd.c"
#include "ser.c"
#define uchar unsigned char
#define uint unsigned int
sbit lsen = P2^3;
sbit rsen = P2^4;
//sbit p10 = P1^0;
uchar oldstate = 4,state;
uchar tmFlag, stopwatch = 0, serRcved,tmFlag1;
void main()
{
	ser_init();
	lcd_init();
//	putstr("Hello");
	lcddelay(10);
	lcd_pos(0x40);
	lcddelay(10);
	lcd_putstr("  00 00 00 00");
	TH0 = 0x00;
	TL0 = 0x00;
	TH2 = 0x4c;
	TL2 = 0x00;
	RCAP2H = 0x4c;
	RCAP2L = 0x00;
	stopwatch = 0;
	ET0 = 0;
	TR0 = 0;
	/******第一项******/
	while(scanmtx() == -1);
	putchar(0x0c);
	while(serRcved != 0x01);
	ET2 = 1;
	TR2 = 1;
	while(serRcved != 0x02)
	{
		if(!tmFlag)
		{
			lcddelay(10);
			lcd_pos(0x42);
			lcddelay(10);
			lcd_wdat(stopwatch/10 + 48);
			lcddelay(10);
			lcd_wdat(stopwatch%10 + 48);
		}
	}
	serRcved = 0x00;
	TR2 = 0;
	TH2 = 0x4c;
	TL2 = 0x00;
	tmFlag = 0;
	stopwatch = 0;
	
	
	
	
	
														//	while(serRcved != 0x01);
														//	TR2 = 1;
														//	while(serRcved != 0x02)
														//	{
														//		if(!tmFlag)
														//		{
														//			lcddelay(10);
														//			lcd_pos(0x4b);
														//			lcddelay(10);
														//			lcd_wdat(stopwatch/10 + 48);
														//			lcddelay(10);
														//			lcd_wdat(stopwatch%10 + 48);
														//		}
														//	}	
														//	TR2 = 0;
														//	serRcved = 0;


/***第二项***/
//	while(scanmtx() == -1);
//	putchar(0x0c);
	serRcved = 0x00;
	TR2 = 0;
	TH2 = 0x4c;
	TL2 = 0x00;
	tmFlag = 0;
	stopwatch = 0;
	while(serRcved != 0x01);	
	ET0 = 1;
	TR0 = 1;
	TR2 = 1;
	while(serRcved != 0x02)
	{
		if(!tmFlag)
		{
			lcddelay(10);
			lcd_pos(0x45);
			lcddelay(10);
			lcd_wdat(stopwatch/10 + 48);
			lcddelay(10);
			lcd_wdat(stopwatch%10 + 48);
			lcddelay(10);
		}
		
		if(serFlag)
		{
			serFlag = 0;
			switch(serRcved)
			{
				case '9':  			
					lcd_pos(3);
					lcddelay(10);
					lcd_putstr("R:L");
					break;
				case 'a':
					lcd_pos(3);
					lcddelay(10);
					lcd_putstr("R:R");
					break;
				case 'b':
					lcd_pos(3);
					lcddelay(10);
					lcd_putstr("R:B");
				default:
					break;
			}
//			lcd_pos(40);
//			lcd_putstr("Suc   ");
		}
		if(!lsen)
		{
			state = 1;
		}
		else if(!rsen)
		{
			state = 2;
		}
		else
		{
			state = 3;
		}
		if(state != oldstate)
		{
			oldstate = state;
			putchar(0x55);
			putchar(0xaa);
			putchar(0x01);
			putchar(0x01);
			putchar(0x08 + state);	
			switch(state)
			{
				case 1:
					lcd_pos(0);
					lcddelay(10);
					lcd_putstr("Le");
					break;
				case 2:
					lcd_pos(0);
					lcddelay(10);
					lcd_putstr("Ri");
					break;
				case 3:
					lcd_pos(0);
					lcddelay(10);
					lcd_putstr("Ba");
					break;
			}
			
			//putchar(state);	
		}
	}
	TR0 = 0;
	serRcved = 0x00;
	TR2 = 0;
	TH2 = 0x4c;
	TL2 = 0x00;
	tmFlag = 0;
	tmFlag1 = 0;
	stopwatch = 0;
	/********第三阶段*******/	
	while(serRcved != 0x01);
	TR2 = 1;
	while(serRcved != 0x02)
	{
		if(!tmFlag)
		{
			lcddelay(10);
			lcd_pos(0x48);
			lcddelay(10);
			lcd_wdat(stopwatch/10 + 48);
			lcddelay(10);
			lcd_wdat(stopwatch%10 + 48);
		}
	}
	serRcved = 0;
	TR2 = 0;
	TH2 = 0x4c;
	TL2 = 0x00;
	tmFlag = 0;
	stopwatch = 0;
	/********第四阶段*********/
	while(serRcved != 0x01);
	TR2 = 1;
	while(serRcved != 0x02)
	{
		if(!tmFlag)
		{
			lcddelay(10);
			lcd_pos(0x4b);
			lcddelay(10);
			lcd_wdat(stopwatch/10 + 48);
			lcddelay(10);
			lcd_wdat(stopwatch%10 + 48);
		}
	}	
	serRcved = 0;
	TR2 = 0;
	TH2 = 0x4c;
	TL2 = 0x00;
	tmFlag = 0;
	stopwatch = 0;
	while(1);	
}
void tm2() interrupt 5
{
	TF2 = 0;
	if(++tmFlag == 40)
	{
//		p10 = ~p10;
		tmFlag = 0;
		stopwatch++;
	}
}
void tm0() interrupt 1
{
	TH0 = 0x00;
	TL0 = 0x00;
	if(++tmFlag1 == 40)
	{
//		p10 = ~p10;
		tmFlag1 = 0;
		putchar(0x55);
		putchar(0xaa);
		putchar(0x01);
		putchar(0x01);
		putchar(0x08 + state);
	}	
}



















//void lzhe() interrupt 0
//{
//	EX0 = 0;
//	putstr("lzhe");
//	EX0 = 1;	
//}
//void rzhe() interrupt 2
//{
//	EX1 = 0;
//	putstr("rzhe");
//	EX1 = 1;
//}