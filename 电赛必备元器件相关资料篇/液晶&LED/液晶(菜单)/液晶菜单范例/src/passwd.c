#include "passwd.h"
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"

/*
//这里是处理5位密码的输入
bool GetPassword(uchar buf[5])
{
	//
	return false;
}
*/
/*
void PasswdExit(uchar id);
void PasswdMenuHandle(uchar id);

// 电量监视 菜单
struct MENU_ITEM code PasswdMenu[] = {
	{1 ,"1" ,0 ,2  ,PasswdMenuHandle},
	{2 ,"2" ,24,2  ,PasswdMenuHandle},
	{3 ,"3" ,48,2  ,PasswdMenuHandle},
	{4 ,"4" ,72,2  ,PasswdMenuHandle},
	{5 ,"5" ,96,2  ,PasswdMenuHandle},
	{6 ,"6" ,0 ,18 ,PasswdMenuHandle},
	{7 ,"7" ,24,18 ,PasswdMenuHandle},
	{8 ,"8" ,48,18 ,PasswdMenuHandle},
	{9 ,"9" ,72,18 ,PasswdMenuHandle},
	{10,"10",96,18 ,PasswdMenuHandle},
	
	{3,"返  回",32,42 ,PasswdExit},
	{0,NULL,0,0,NULL}
};

void PasswdEnter(uchar id)
{
	id = id;
	SetMenu(PasswdMenu);
}
//返回主菜单
void PasswdExit(uchar id)
{
	id = id;
	ReturnMainMenu();
}
void PasswdMenuHandle(uchar id)
{
}
*/

void GetPassword()
{
	uchar i;
	uchar key = 0;
	
	LCD_Cls();
	//画虚拟数字键盘
	LCD_PrintHz16(8  ,2,"0");
	LCD_PrintHz16(32 ,2,"1");
	LCD_PrintHz16(56 ,2,"2");
	LCD_PrintHz16(80 ,2,"3");
	LCD_PrintHz16(104,2,"4");
	LCD_PrintHz16(8  ,20,"5");
	LCD_PrintHz16(32 ,20,"6");
	LCD_PrintHz16(56 ,20,"7");
	LCD_PrintHz16(80 ,20,"8");
	LCD_PrintHz16(104,20,"9");
	LCD_LineXX(0,120,0);
	LCD_LineXX(0,120,19);
	LCD_LineXX(0,120,38);
	for(i = 0; i < 6; i++)
	{
		LCD_LineV(i * 24,0,38);
	}
	LCD_PrintHz16(0,42,"密码:");
	
	//LCD_Print6X8(40,42,"0123456789");	
	
	LCD_ReverseRect(0,1,3,18);

	while(key != KEY_LEFT)
	{
		key = ReadKey();
	
	}
}