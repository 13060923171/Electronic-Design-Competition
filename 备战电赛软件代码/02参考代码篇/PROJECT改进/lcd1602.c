/*
	LCD1602Һ������������ģ��
	1����ֱ��Ƕ�뵽��Ŀ��ʹ��
	2������Ƶ�ʣ�1M
	3���羧����ߵͲ�������ʱҪ����Ӧ�޸�
	4��������ԭ��������ֻʹ����PB�ڵĸ���λ����PB4��PB7��PB0��PB3����������
     
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

/*��ʾ������д�뺯��*/
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

/*��ʾ������д�뺯��*/
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

/*��ʾ�������ʾ*/
void LCD_clear(void) {

	LCD_write_com(0x01);
	delay_ms( 15);
}

/*��ʾ���ַ���д�뺯��*/
void LCD_write_str(unsigned char x,unsigned char y,unsigned char flash *s) {
	
    uchar i=0;
    LCDsetxy( x ,  y);
    while (*s) {
    	LCD_write_data(*s);
    	i++;
    	s ++;
    	}
}

/*��ʾ�����ַ�д�뺯��*/
void LCD_write_char(unsigned char x,unsigned char y,unsigned char data) {
	
    LCDsetxy( x ,  y);
    
    LCD_write_data( data);  
}       
/*void CursorScroll(void)
{
	
    LCD_write_com(0x10);	//�������
   
}      
void ScreenScroll(void)
{
	
	LCD_write_com(0x18);		//�������
	delay_ms(255);
		
} */

/*��ʾ����ʼ������*/
void LCD_init(void)
 {
	
							/*I/O�ڷ�������*/
	LCD_write_com(0x28);				/*��ʾģʽ����*/
	delay_ms( 15);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);
	
	LCD_write_com(0x08);				/*��ʾ�ر�*/
	LCD_write_com(0x01);				/*��ʾ����*/
	LCD_write_com(0x06);				/*��ʾ����ƶ�����*/
	delay_ms( 5);
	LCD_write_com(0x0c);				/*��ʾ�����������*/
}
