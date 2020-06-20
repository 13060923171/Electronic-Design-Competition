/*--------------------------------------------------------------*/
#include <AT89X52.H>
#include "LCD128X64_V5.H"
#include "LCD_Menu.c"


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
	unsigned char key_dat_tmp;

	LCD_init_12864();

	//汉字查表输出
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
		key_dat = P3;
		if(key_dat_tmp != key_dat)
		{
			Menu_Refresh(key_dat);
			key_dat_tmp = key_dat;
			delay50ms();
		}
	}
}
