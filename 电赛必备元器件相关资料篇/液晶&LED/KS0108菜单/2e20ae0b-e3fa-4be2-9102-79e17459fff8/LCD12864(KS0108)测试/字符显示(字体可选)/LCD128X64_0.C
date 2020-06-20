/*--------------------------------------------------------------*/
#include <AT89X52.H>
#include "Font_code.c"
#include "LCD128X64_V1.H"



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
	unsigned char i, j;

	LCD_init_12864();

	while(*string)	{LCD_printc(0, i, *string++); i++; delay500ms();}	//字符输出演示
	delay500ms();
	LCD_prints(1, 0, "Haha,I love you.");			//字符串输出演示
	delay500ms();
	LCD_prints(2, 0, "I am glad to see");
	delay500ms();
	LCD_prints(3, 0, "you again(*^_^*)");
#if (Font_type == 0)	//(8*16字体)
	LCD_printc(4, 0, 131);
	LCD_printc(4, 1, 127);
	delay500ms();
	LCD_prints(4, 2, "love you from ");
	delay500ms();
	LCD_prints(5, 0, "the bottom of my");
	delay500ms();
	LCD_prints(6, 0, "heart,hui. Yes,I");
	LCD_printc(6, 5, 132);
	LCD_prints(7, 0, "'ll devote to it");
	delay500ms();delay500ms();delay500ms();
#endif

	delay500ms();
	LCD_clr_scr(ASCR);	//清屏演示


#if (Font_type == 0)	//(8*8字体)
	i = 0;
	while(i < 101)		//ASCII字符测试
	{ 
		LCD_printc(j & 0x07, i & 0x0f, i + 32); 
		delay500ms(); i++;
		if(!(i % 16)) j++;
	}
#else					//(8*16字体)
	i = 0;
	while(i < 95)		//ASCII字符测试
	{ 
		LCD_printc(j & 0x03, i & 0x0f, i + 32); 
		delay500ms(); i++;
		if(!(i % 16)) j++;
	}
#endif



#if (Font_type == 0)	//(8*8字体)	
	LCD_prints(7, 0, "www.Alex2008.com");
#else					//(8*16字体)
	LCD_prints(3, 0, "www.Alex2008.com");
#endif


#if (Font_type == 0)	//(8*8字体)	
	i = 0; j = 0;
	while(i < 128)
	{ 
		LCD_inversec(j & 0x07, i & 0x0f);
		i++; if(!(i % 16)) {j++; delay500ms();}
	}
#else					//(8*16字体)
	i = 0;  j = 0;
	while(i < 128)
	{ 
		LCD_inversec(j & 0x03, i & 0x0f);
		i++; if(!(i % 16)) {j++; delay500ms();}
	}
#endif



#if (Font_type == 0)	//(8*8字体)		
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 8 ;j++) LCD_inversec(j, i);
		delay500ms();
	}
	for(j = 0; j < 8 ;j++)
	{
		LCD_inverses(j, 0, 16);
		delay500ms();
		LCD_inverses(j, 0, 16);
	}
#else					//(8*16字体)
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 4 ;j++) LCD_inversec(j, i);
		delay500ms();
	}
	for(j = 0; j < 4 ;j++)
	{
		LCD_inverses(j, 0, 16);
		delay500ms();
		LCD_inverses(j, 0, 16);
	}
#endif


	LCD_sel_all		//全屏
	while(1)
	{
		if(P3_5 == 0) LCD_show_on  while(P3_5 == 0);
		if(P3_4 == 0) LCD_show_off while(P3_4 == 0);
	}
}

