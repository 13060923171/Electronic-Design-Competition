/*
	LCD1602液晶屏四线驱动模块
	1、可直接嵌入到项目中使用
	2、晶振频率：1M
	3、如晶振提高低层驱动延时要作相应修改
	4、程序在原来基础上只使用了PB口的高四位，即PB4—PB7，PB0—PB3可以做它用
     
	SunnyTrip
	www.iccavr.com
*/

#include "config.h"

#define RS_CLR	PORTA.1=0
#define RS_SET	PORTA.1=1

#define RW_CLR	PORTA.2=0
#define RW_SET	PORTA.2=1

#define EN_CLR	PORTA.3=0
#define EN_SET	PORTA.3=1  


void LCDsetxy(unsigned char x , unsigned char y)
 {
  	 if (y == 0) 
 	 	{
    	 LCD_write_com(0x80 + x);
    	}
    else 
		{
    	 LCD_write_com(0xC0 + x);
    	}
 
 }

/*显示屏命令写入函数*/
void LCD_write_com(unsigned char com) {
	
	RS_CLR;
	RW_CLR;
	EN_SET;
	PORTA&=0x0f;
	PORTA|= com&0xf0;
	com = com<<4;
	EN_CLR;
	delay_us( 10);
	EN_SET;
	PORTA&=0x0f;
	PORTA|= com&0xf0;
	EN_CLR;
	delay_us(10);
}

/*显示屏命令写入函数*/
void LCD_write_data(unsigned char data) {

	RS_SET;
	RW_CLR;
	EN_SET;
	PORTA&=0x0f;
	PORTA|= data&0xf0;
	data = data<<4;
	EN_CLR;
	delay_us( 10);
	EN_SET;
	PORTA&=0x0f;
	PORTA|= data&0xf0;
	EN_CLR;
}

/*显示屏清空显示*/
void LCD_clear(void) {

	LCD_write_com(0x01);
	delay_ms( 15);
}

/*显示屏字符串写入函数*/
void LCD_write_str(unsigned char x,unsigned char y,unsigned char flash *s) {
	
    uchar i=0;
    LCDsetxy( x ,  y);
    while (*s) {
    	LCD_write_data(*s);
    	i++;
    	s ++;
    	}
}

/*显示屏单字符写入函数*/
void LCD_write_char(unsigned char x,unsigned char y,unsigned char data) {
	
    LCDsetxy( x ,  y);
    
    LCD_write_data( data);  
}       
/*void CursorScroll(void)
{
	
    LCD_write_com(0x10);	//光标左移
   
}      
void ScreenScroll(void)
{
	
	LCD_write_com(0x18);		//画面左滚
	delay_ms(255);
		
} */

/*显示屏初始化函数*/
void LCD_init(void)
 {
	
							/*I/O口方向设置*/
	LCD_write_com(0x28);				/*显示模式设置*/
	delay_ms( 15);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);
	
	LCD_write_com(0x08);				/*显示关闭*/
	LCD_write_com(0x01);				/*显示清屏*/
	LCD_write_com(0x06);				/*显示光标移动设置*/
	delay_ms( 5);
	LCD_write_com(0x0c);				/*显示开及光标设置*/
}
