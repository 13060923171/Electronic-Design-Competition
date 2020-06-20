/*--------------------------------------------------------------*/
#include <AT89X52.H>
#include "Font_code.c"
#include "LCD128X64_V2.H"


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


/*--------------------------------------------------------------*/
//主函数
void main(void)
{
	unsigned char i;
	LCD_init_12864();
	while(*string)	{LCD_printc(0, i, *string++); i++; delay500ms();}
//	LCD_cursor(0, 0);
	LCD_prints(1, 0, "Haha,I love you.");
	LCD_prints(2, 0, "I am glad to see");
	LCD_prints(3, 0, "you again(*^_^*)");
	LCD_inverses(3, 0, 16);
	LCD_Cursor_on
#if (Font_type == 0)	//(8*16字体)
	LCD_printc(4, 0, 131);
	LCD_printc(4, 1, 127);
	LCD_prints(4, 2, "love you from ");
	LCD_prints(5, 0, "the bottom of my");
	LCD_prints(6, 0, "heart,hui. Yes,I");
	LCD_printc(6, 5, 132);
	LCD_prints(7, 0, "'ll devote to it");

#endif		
	delay500ms(); delay500ms(); delay500ms();
	LCD_cur_prints("Do you love me?");
	delay500ms();
	LCD_Cursor_off	delay500ms();delay500ms();
	LCD_Cursor_on
//	while(1);
//	LCD_clr_scr(ASCR);
	while(1)
	{
		if(P3_5 == 0) {LCD_sel_all LCD_show_on}  while(P3_5 == 0);
		if(P3_4 == 0) {LCD_sel_all LCD_show_off} while(P3_4 == 0);
		if(P3_3 == 0) {cursor_x++;  delay500ms(); while(P3_3 == 0);}
		if(P3_2 == 0) {cursor_y++;  delay500ms(); while(P3_2 == 0);}
	}

}

