/*--------------------------------------------------------------*/
#include <AT89X52.H>
#include "Font_code.c"
#include "LCD128X64_V0.C"


unsigned char code *string = {"I love you hui."};


/*--------------------------------------------------------------*/
//延时500000+0us 函数定义
void delay500ms(void)
{
    unsigned char i,j,k;
    for(i=205;i>0;i--)
    for(j=116;j>0;j--)
    for(k=9;k>0;k--);
}

//延时200000+0us 函数定义
void delay200ms(void)
{
    unsigned char i,j,k;
    for(i=201;i>0;i--)
    for(j=32;j>0;j--)
    for(k=14;k>0;k--);
}

/*--------------------------------------------------------------*/
//主函数
void main(void)
{
	unsigned char i;
	LCD_init_12864();
	
	LCD_clr_scr(ASCR);

	while(*string)	{LCD_printc(0, i, *string++); i++; delay500ms();}

	LCD_prints(1, 0, "Haha,I love you.");
	LCD_prints(2, 0, "I am glad to see");
	LCD_prints(3, 0, "you again!*^_^*!");
	delay500ms();


	LCD_sel_all		//全屏
	for(i = 0; i < 64; i++)
	{
		LCD_write_cmd(Start_Line + i);	//起始行
		delay200ms();
	}
	delay500ms();delay500ms();delay500ms();delay500ms();
	
	for(i = 0; i < 64; i++)
	{
		LCD_write_cmd(Start_Line + 63 - i);	//起始行
		delay200ms();
	}


	for(i = 0; i < 64; i++)
	{
		LCD_sel_left
		LCD_write_cmd(Start_Line + i);	//起始行
		LCD_sel_right
		LCD_write_cmd(Start_Line + 63 - i);	//起始行
		delay200ms();
	}
	LCD_sel_all		//全屏
	LCD_write_cmd(Start_Line);	//起始行
	delay500ms();
	
	for(i = 0; i < 64; i++)
	{
		LCD_sel_left
		LCD_write_cmd(Start_Line + 63 - i);	//起始行
		LCD_sel_right
		LCD_write_cmd(Start_Line + i);	//起始行
		delay200ms();
	}
	LCD_sel_all		//全屏
	LCD_write_cmd(Start_Line);	//起始行

//	LCD_clr_scr(ASCR);
	while(1)
	{
		if(P3_5 == 0) LCD_show_on  while(P3_5 == 0);
		if(P3_4 == 0) LCD_show_off while(P3_4 == 0);
	}
}

