//#include <t89c51rd2.h>

#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"



void main()
{
	LCD_Init();
	KeyInit();
	MenuInit();
	
//	LCD_Rectange(0,0,127,63);
	LCD_PrintHz16(24,24,"欢迎使用！");
	delay1(100);

	//设置菜单,第一次运行时指向主菜单
	ReturnMainMenu();
		
	//加载参数
	//发送参数
	
	while(1)
	{
		KeyEvent();	//检测键盘事件
		
		MenuEvent();	//处理菜单命令
	}
	
}