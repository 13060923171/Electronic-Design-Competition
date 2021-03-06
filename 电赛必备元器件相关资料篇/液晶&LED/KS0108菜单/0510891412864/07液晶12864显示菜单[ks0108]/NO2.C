#include <iom16v.h>
#include <macros.h>

#define uchar unsigned char
#define uint  unsigned int
#define LEFT 0
#define RIGHT 1
#define CMD 0
#define DATA 1
#define LCD_STATUS_BUSY 0x80
#define  START_LINE0   0xc0
#define  DISPLAY_ON    0x3f
#define  DISPLAY_OFF   0x3e
#define  PARA1         0x40
/*-----------------------------------------------------------------------
延时函数
系统时钟：8M
-----------------------------------------------------------------------*/

uchar  time_logo[5][32] = {
/*--  文字:  时  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0xFC,0x44,0x44,0x44,0xFC,0x10,0x90,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x00,
0x00,0x07,0x04,0x04,0x04,0x07,0x00,0x00,0x03,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,

/*--  文字:  爱 --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0xB0,0x92,0x96,0x9A,0x92,0xF2,0x9E,0x92,0x91,0x99,0x95,0x91,0x50,0x30,0x00,
0x00,0x00,0x40,0x30,0x8C,0x83,0x46,0x2A,0x12,0x2A,0x26,0x42,0xC0,0x40,0x00,0x00,

/*--  文字:  你 --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x20,0xF8,0x07,0x40,0x20,0x18,0x0F,0x08,0xC8,0x08,0x08,0x28,0x18,0x00,0x00,
0x00,0x00,0xFF,0x00,0x00,0x08,0x04,0x43,0x80,0x7F,0x00,0x01,0x06,0x0C,0x00,0x00,
/*--  文字:  我  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x20,0x24,0x24,0x24,0xfe,0x23,0x22,0x20,0xFF,0x20,0x22,0xac,0x20,0x30,0x20,0x00,
0x00,0x08,0x48,0x84,0x7F,0x02,0x21,0x10,0x09,0x06,0x1A,0x61,0x80,0xE0,0x00,0x00,

};

uchar  menu[5][32] = {
/*--  文字: 一 --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xc0,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字: 二 --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x008,0x0c,0x08,0x00,0x00,0x00,
0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
/*--  文字: 三 --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x084,0xc4,0x86,0x04,0x00,0x00,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x30,0x20,0x00,
/*--  文字: 四 --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0xfc,0x04,0x04,0x04,0xfc,0x04,0x04,0x04,0xfc,0x004,0x04,0x04,0xfe,0x04,0x00,
0x00,0x7f,0x20,0x28,0x24,0x23,0x20,0x20,0x20,0x21,0x20,0x22,0x22,0x7f,0x00,0x00,

};

 
 void delay_1us(void);                 //1us延时函数
 void delay_nus(unsigned int n);       //N us延时函数
 void delay_1ms(void) ;                //1ms延时函数
 void delay_nms(unsigned int n) ;      //N ms延时函数
 
 /***********************************************************************/
 /**********************************************************************/
void delay_1us(void)                 //1us延时函数
  {
   asm("nop");
  }

void delay_nus(unsigned int n)       //N us延时函数
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1us();
  }
  
void delay_1ms(void)                 //1ms延时函数
  {
   unsigned int i;
   for (i=0;i<1140;i++);
  }
  
void delay_nms(unsigned int n)       //N ms延时函数
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   {
      delay_1ms();
   }   
  }
  
  void delay(uint ms)
{
        uint i,j;
	for(i=0;i<ms;i++)
	   {
	   for(j=0;j<1141;j++);
       }
}
void Lcd_Write_Byte(uchar temp)
{   
    PORTB|=BIT(0);
    PORTB&=~BIT(1);    //W
    PORTB|=BIT(4); //E使能
    DDRA=0xff; //a输出
    PORTA=temp;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB&=~BIT(4);	 
    PORTA=0xff;
}
void Lcd_Write_COM(uchar COL)
{   
    PORTB&=~BIT(0);
    PORTB&=~BIT(1);    //W
    PORTB|=BIT(4); //E使能
    DDRA=0xff; //a输出
    PORTA=COL;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB&=~BIT(4);	 
 
}


void set_page_L(unsigned char page)	/*设置X地址 设置页*/  //0-7
{
   Lcd_Write_COM(0xb8|page);                     
}
void set_page_R(unsigned char page)	/*设置X地址 设置页*/ //0-7
{
    Lcd_Write_COM(0xb8|page);                     
}

void set_col_addr_L(unsigned char col) /*设置Y地址*/ //0-63
{
     
   Lcd_Write_COM(0x40|col);                     
}

void set_col_addr_R(unsigned char col) /*设置Y地址*/ //0-63
{
    Lcd_Write_COM(0x40|col);                        
}


/********************************************************************************************
* 函数名称：Lcd_Set_X_Y(uchar x, uchar y )
* 功    能：LCD设置x y坐标
* 入口参数：x y
* 出口参数：无
*********************************************************************************************/
void Lcd_Set_X_Y(uchar x, uchar y)
{   
   if(y<64)
      {   PORTB&=~BIT(3);
	      PORTB|=BIT(2);
	   
		set_col_addr_L(y);
	  }
   else
      {   
		   PORTB|=BIT(3);
	    PORTB&=~BIT(2);
		set_col_addr_R(y);
	  }
   set_page_L( x);
   set_page_R( x);
}

/****************************************************************************
* 名称：Lcd_Character_16X16( bit bit_flag, uchar x, uchar y, uchar code *point )
* 功能：显示16*16字符（汉字）       
* 入口参数：x y data
* 出口参数：无
*说明:bit_flag = 1 正常显示 bit_flag = 0 黑白反相显示
****************************************************************************/
void Lcd_Character_16X16( uchar bit_flag, uchar x, uchar y, uchar  *point )
{
   uchar i,j,temp ;
   temp=y;
   if( bit_flag )
   {
      for( i=0; i<2; i++ )
      {
         x+=i;
	     y=temp;
         for( j=0;j<16;j++ )
         {
            Lcd_Set_X_Y( x, y ); y++;
            Lcd_Write_Byte( point[ i*16 + j] );  
         } 
      }
   }
   else
   {
      for( i =0; i<2; i++ )
      {
         x+=i;
         y=temp;
         for( j = 0; j < 16; j++ )
         {
            Lcd_Set_X_Y( x, y );
			y++;
            Lcd_Write_Byte( ~ point[ i * 16 + j ] );
			
         }
		
      }
   }
}
uchar key_press()
{
    uchar j;
    DDRD|=0X0F;
	PORTD|=0X0F;
	
	DDRD&=0XF0;
	
	j=PIND;
	j=j&0X0F;//00001111
	if(j==0X0F)
	{
	  return 0;
	}
	else
	{
	 return 1;
	}
	
}

uchar key_scan()
{
    uchar key;
    delay(10);
	if(key_press())
	{
	key=PIND;
	key&=0X0F;
	switch(key)
	{
	   case 0X0E: 
	       key=1;
		   break;
	   case 0X0D:
	       key=2;
		   break;
	   	 case 0X0B: 
	       key=3;
		   break;
	   case 0X07:
	       key=4;
		   break; 
	   default:	
	       key=16;     	   
	}
	  while(key_press());
	}
	else
	{
	  key=16;
	}
	return key;
}

void Lcd_Function( uchar temp )
{
   uchar time_flag = 0;
   uchar alarm_flag = 0;
   uchar m=0;
   uchar n=0;
    switch(temp)
	{case 1:
	  time_flag = 1; break;
	 case 2:
	  alarm_flag= 1; break;
	 case 3:
	   m=1; break;
	 case 4:
	   n=1; break;
	 default:
	  ;

	}

   //时间设置
   Lcd_Character_16X16( time_flag, 0, 0, time_logo[3] );
   Lcd_Character_16X16( time_flag, 0, 16, time_logo[1] );
   Lcd_Character_16X16( time_flag, 0, 32, time_logo[2] );
   Lcd_Character_16X16( time_flag, 0, 48, menu[0] );
   //闹钟设置
   Lcd_Character_16X16( alarm_flag, 2, 0, time_logo[3] );
   Lcd_Character_16X16( alarm_flag, 2, 16, time_logo[1] );
   Lcd_Character_16X16( alarm_flag, 2, 32, time_logo[2] );
   Lcd_Character_16X16( alarm_flag, 2, 48, menu[1] );
   
   Lcd_Character_16X16( m, 4, 0, time_logo[3] );
   Lcd_Character_16X16( m, 4, 16, time_logo[1] );
   Lcd_Character_16X16( m, 4, 32, time_logo[2] );
   Lcd_Character_16X16( m, 4, 48, menu[2] );
   
   Lcd_Character_16X16( n, 6, 0, time_logo[3] );
   Lcd_Character_16X16( n, 6, 16, time_logo[1] );
   Lcd_Character_16X16( n, 6, 32, time_logo[2] );
   Lcd_Character_16X16( n, 6, 48, menu[3] );
}
void main()
{
    unsigned int i,j,k=1;
   //init_port();//自己加,主要是控制线所在端口为输出
    DDRB = 0xff; 
	Lcd_Function(1);
	
	while(1)
	{   i=key_press();
	if(i)
	{
	 j=key_scan();
	switch(j)
	  {
	   case 1: 
	       k=k+1;
		   break;
	   case 2:
	       k=k-1;
		   break;
		default:
		;
		
	  }  
	  
	 if(k>4) k=1;
	 if(k<1) k=4;
     
	Lcd_Function(k);
	}
	 
   }
}
