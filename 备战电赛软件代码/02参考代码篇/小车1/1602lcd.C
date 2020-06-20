/*  描述:                                                                       *
*         1602字符型LCD显示演示程序                                            *
*        在第一行显示 Welcome to use                                              *
*        在第二行显示 Power-I Mini51!!                                         *
*                                                                              *
********************************************************************************
* 【声明】 此程序仅用于学习与参考信息！          
*******************************************************************************/


//#include <reg51.h>
#include <intrins.h>

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL ; 

sbit rs	= P2^0;		//
sbit rw = P2^1;
sbit ep = P2^2;
//
//BYTE code dis1[] = {"Welcome to use"};
//BYTE code dis2[] = {"Power-I Mini51!!"};

lcddelay(BYTE ms)
{							// 延时子程序
	BYTE i;
	while(ms--)
	{
		for(i = 0; i< 250; i++)
		{
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		}
	}
}

BOOL lcd_bz()
{							// 测试LCD忙碌状态
	BOOL result;
	rs = 0;
	rw = 1;
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result = (BOOL)(P0 & 0x80);
	ep = 0;
	return result;	
}

lcd_wcmd(BYTE cmd)
{							// 写入指令数据到LCD
	while(lcd_bz());
	rs = 0;
	rw = 0;
	ep = 0;
	_nop_();
	_nop_();	
	P0 = cmd;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 0;		
}

lcd_pos(BYTE pos)
{							//设定显示位置
	lcd_wcmd(pos | 0x80);
}

lcd_wdat(BYTE dat)	
{							//写入字符显示数据到LCD
	while(lcd_bz());
	rs = 1;
	rw = 0;
	ep = 0;
	lcddelay(1);
	P0 = dat;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 0;	
}

lcd_init()
{							//LCD初始化设定
	lcd_wcmd(0x38);			//
	lcddelay(10);
	lcd_wcmd(0x0c);			//
	lcddelay(10);
	lcd_wcmd(0x06);			//
	lcddelay(10);
	lcd_wcmd(0x01);			//清除LCD的显示内容
	lcddelay(10);
}


void lcd_putstr(unsigned char *s)
{
   while (*s)
   {
      lcd_wdat(*s);
      s++;
   }
}
//main()
//{
//	BYTE i;
//	lcd_init();				// 初始化LCD			
//	lcddelay(10);
//	lcd_pos(2);				// 设置显示位置为第一行2223第3个字符
//	i = 0;
//	while(dis1[i] != '\0')
//	{						// 显示字符Welcome to use"
//		lcd_wdat(dis1[i]);
//		i++;
//	}
//	lcdlcddelay(10);
//	lcd_pos(0x40);			// 设置显示位置为第二行第二个字符
//	i = 0;
//	while(dis2[i] != '\0')
//	{
//		lcd_wdat(dis2[i]);	// 显示字符"Power-I Mini51!!  "
//		i++;
//	}
//	while(1);				// 
//}


