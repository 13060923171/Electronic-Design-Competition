/*--------------------------------------------------------------*/
#include <AT89X52.H>
#include "LCD128X64_V5.H"
#include "LCD_Menu.c"
#include "Beep.H"
#include "IRDA.C"


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
	unsigned char key_dat;

	IRDA_init();					//遥控接收初始化
	LCD_init_12864();				//液晶初始化

	//汉字查表输出
mm:	LCD_clr_scr(ASCR);		//液晶清屏
	LCD_printss(0, 1, "welcome to wherever ");
	LCD_printss(1, 5, "you are ^_^");
	LCD_showsh(1, 3, "欢迎");
	delay500ms();
	LCD_showsh(2, 2, "使用菜单");
	delay500ms();
	LCD_showsh(3, 2, "演示系统");
	delay500ms(); delay500ms();
	LCD_clr_scr(ASCR);		//液晶清屏

	Menu_Refresh(key_dat);
	while(1)
	{
		if(IRDA_ok) 					//有按键按下
		{
			Close_IRDA					//关闭红外,等待以下任务执行完毕	
			IRDA_ok = 0;				//清接收有效标志位
			key_dat = IRDA_get_key();	//取得键值
			speaker();
			if(!key_dat) goto mm;
			Menu_Refresh(key_dat);
		}
		Open_IRDA						//开启红外,接收遥控按键
		delay50ms();
	}
}
