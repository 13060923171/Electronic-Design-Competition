#include "coulometry.h"
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"

#include "passwd.h"

void CoulometryMenuHandle(uchar id);

// 电量监视 菜单
struct MENU_ITEM code CoulometryMenu[] = {
	{1,"直流监视",32,2  ,CoulometryMenuHandle},//DCWatch
	{2,"交流监视",32,22 ,CoulometryMenuHandle},//ACWatch
	{3,"返    回",32,42 ,CoulometryExit},
	{0,NULL,0,0,NULL}
};

//电量监视入口主函数
void CoulometryEnter(uchar id)
{
	id = id;
	SetMenu(CoulometryMenu);
}
//返回主菜单
void CoulometryExit(uchar id)
{
	id = id;
	ReturnMainMenu();
}

//直流监视
void DCWatch()
{
	uchar key = 0;
	
	GetPassword();
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "电压1 60.000 Vdc");
	LCD_PrintHz16(1,22,"电流1  20.00 Adc");
	LCD_PrintHz16(1,42,"功率1 000020   W");

	//按LEFT键返回上一层
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	
		//使用UP/DOWN键翻页?	
		
	}
}
//交流监视
void ACWatch()
{
	LCD_Cls();
	LCD_PrintHz16(32,22,"交流监视");

	//按LEFT键返回上一层
	while( ReadKey() != KEY_LEFT);
}

///////////////////////////////////////////////////////////////////
//菜单处理事件
void CoulometryMenuHandle(uchar id)
{
	onKeyUp = NULL;	//把键盘处理忽略
	switch(id)
	{
	case 1:
		DCWatch();
		break;
	case 2:
		ACWatch();
		break;
	}

	CoulometryEnter(-1);
	//RefreshMenu();//刷新菜单
}
