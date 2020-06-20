/*--------------------------------------------------------------*/
#include <AT89X52.H>
#include "LCD128X64_V3.H"
#include "Beep.H"


/*--------------------------------------------------------------*/
//功能选择
#define Menu_Mode		 0		//菜单显示模式
//						=0		//反色
//						=1		//箭头
//						=2		//下划线

#define Menu_roll_repeat 1		//是否重复滚动
//						=0		//不重复
//						=1		//重复

#define Roll_Bar		1		//是显示滚动条
//						=0		//不显示
//						=1		//显示

/*--------------------------------------------------------------*/
//按键定义
sbit up 	= P3^5;
sbit down 	= P3^4;
sbit ok 	= P3^3;
sbit cancel = P3^2;


/*--------------------------------------------------------------*/
//主函数
void main(void)
{
	char i, tempi;

	LCD_init_12864();

	LCD_clr_scr(ASCR);		//清屏演示

/*--------------------------------------------------------------*/
#if		(Menu_Mode == 0)	//模式一	
	LCD_prints(0, 1, "Time Set");
	LCD_prints(1, 1, "Data Set");
	LCD_prints(2, 1, "Sound Set");
	LCD_prints(3, 1, "Display Set");
	LCD_prints(4, 1, "Camera Set");
	LCD_prints(5, 1, "Irda Set");
	LCD_prints(6, 1, "Keybord Set");
	LCD_prints(7, 1, "Mouse Set");
	for(i = 0; i < 8; i++) LCD_printc(i, 0, 127);	//->
	LCD_printc(0, 14, 129); LCD_printc(7, 14, 130);	//上下箭头
	LCD_inverses(0, 0, 12);
#if		Roll_Bar			//显示滚动条
	LCD_prints(0, 12, "18");
	LCD_write_dat_pos(0, 103, 0xff);	//|
	for(i = 0; i < 8; i++)LCD_write_dat_pos(i, 125, 0xff);	//滚动栏
	for(i = 0; i < 8; i++)LCD_write_dat_pos(i, 127, 0xff);	//滚动栏
	i = 0;
	LCD_write_dat_pos(0, 126, 0xff);
#else
	LCD_prints(0, 12, "1/8");
#endif
	while(1)
	{	
		if(tempi != i)  
		{
			LCD_inverses(tempi, 0, 12);
			LCD_inverses(i, 0, 12); 
#if		Roll_Bar			//显示滚动条
			LCD_write_dat_pos(tempi, 126, 0x00);
			LCD_write_dat_pos(i, 126, 0xff);
#endif
			LCD_printc(0, 12, i+1+48);
			tempi = i;
		}
#if		 Menu_roll_repeat	//重复滚动
		if(!up)			{speaker(); i--; i &= 0x07;}
		if(!down)		{speaker(); i++; i &= 0x07;}	
#else						//不重复滚动
		if(!up)			{speaker(); i--; if(i < 1) i = 0;}
		if(!down)		{speaker(); i++; if(i > 6) i = 7;}
#endif			
	}
/*--------------------------------------------------------------*/
#elif	(Menu_Mode == 1)	//模式二
	LCD_prints(0, 2, "Time Set");
	LCD_prints(1, 2, "Data Set");
	LCD_prints(2, 2, "Sound Set");
	LCD_prints(3, 2, "Display Set");
	LCD_prints(4, 2, "Camera Set");
	LCD_prints(5, 2, "Irda Set");
	LCD_prints(6, 2, "Keybord Set");
	LCD_prints(7, 2, "Mouse Set");

	LCD_prints(0, 0, "=>");
	LCD_prints(0, 13, "1/8");
	while(1)
	{	
		if(tempi != i)  
		{
			LCD_prints(tempi, 0, "  ");
			LCD_prints(i, 0, "=>");
			LCD_printc(0, 13, i+1+48);
			tempi = i;
		}
#if		 Menu_roll_repeat	//重复滚动
		if(!up)			{speaker(); i--; i &= 0x07;}
		if(!down)		{speaker(); i++; i &= 0x07;}	
#else						//不重复滚动
		if(!up)			{speaker(); i--; if(i < 1) i = 0;}
		if(!down)		{speaker(); i++; if(i > 6) i = 7;}
#endif
	}
/*--------------------------------------------------------------*/
#elif	(Menu_Mode == 2)	//模式三
	LCD_prints(0, 2, "Time Set");
	LCD_prints(1, 2, "Data Set");
	LCD_prints(2, 2, "Sound Set");
	LCD_prints(3, 2, "Display Set");
	LCD_prints(4, 2, "Camera Set");
	LCD_prints(5, 2, "Irda Set");
	LCD_prints(6, 2, "Keybord Set");
	LCD_prints(7, 2, "Mouse Set");
	for(i = 0; i <8; i++) LCD_prints(i, 0, "=>");
	i = 0;
	LCD_prints(0, 13, "1/8");
	LCD_underlines(tempi, 0, 13, 1);
	while(1)
	{	
		if(tempi != i)  
		{
			LCD_underlines(tempi, 0, 13, 0);
			LCD_underlines(i, 0, 13, 1);
			LCD_printc(0, 13, i+1+48);
			tempi = i;
		}
#if		 Menu_roll_repeat	//重复滚动
		if(!up)			{speaker(); i--; i &= 0x07;}
		if(!down)		{speaker(); i++; i &= 0x07;}	
#else						//不重复滚动
		if(!up)			{speaker(); i--; if(i < 1) i = 0;}
		if(!down)		{speaker(); i++; if(i > 6) i = 7;}
#endif	
	}
/*--------------------------------------------------------------*/
#endif
}


