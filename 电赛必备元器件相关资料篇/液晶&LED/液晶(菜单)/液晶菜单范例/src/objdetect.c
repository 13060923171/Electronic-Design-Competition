#include "objdetect.h"
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"

void ObjDetectMenuHandle(uchar id);

// 对象检测 菜单
struct MENU_ITEM code ObjDetectMenu[] = {
	{1,"恒定电压检测CV" ,8,1 ,ObjDetectMenuHandle},//
	{2,"恒定电流检测CC" ,8,18,ObjDetectMenuHandle},//
	{3,"恒定功率检测CP" ,8,35,ObjDetectMenuHandle},//
	{4,"线性电阻检测CR" ,8,1 ,ObjDetectMenuHandle},//
	{5,"加强电压检测CV2",8,18,ObjDetectMenuHandle},//
	{6,"自动循环检测AT" ,8,35,ObjDetectMenuHandle},//
	{7,"返  回"         ,40,22,ObjDetectExit},
	{0,NULL,0,0,NULL}
};

//对象检测 入口主函数
void ObjDetectEnter(uchar id)
{
	id = id;
	SetMenu(ObjDetectMenu);
}
//返回主菜单
void ObjDetectExit(uchar id)
{
	id = id;
	ReturnMainMenu();
}
///////////////////////////////////////////////////////////////////
//恒定电压检测CV
void CheckCVHandle()
{
	uchar key = 0;

	LCD_Cls();
	LCD_PrintHz16(1,2, "恒定电压检测CV");
	LCD_Print24X32(0,24,"60.000");

	//按LEFT键返回上一层
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}

//恒定电流检测CC
void CheckCCHandle()
{
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "恒定电流检测CC");
	LCD_Print24X32(0,24,"0.001");
	//按LEFT键返回上一层
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//恒定功率检测CP
void CheckCPHandle()
{
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "恒定功率检测CP");
	LCD_Print24X32(0,24,"0.000");
	//按LEFT键返回上一层
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//线性电阻检测CR
void CheckCRHandle()
{
	//
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "线性电阻检测CR");
	LCD_Print24X32(0,24,"0.000");
	//按LEFT键返回上一层
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//加强电压检测CV2
void CheckCV2Handle()
{
	//
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "加强电压检测CV2");
	LCD_Print24X32(0,24,"0.000");
	//按LEFT键返回上一层
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//自动循环检测AT
void CheckATHandle()
{
	//
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "自动循环检测AT");
	//按LEFT键返回上一层
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}


////////////////////////////////////////////////////////////////
void ObjDetectMenuHandle(uchar id)
{
	onKeyUp = NULL;	//把键盘处理忽略
	
	switch(id)
	{
	case 1:
		CheckCVHandle();
		break;
	case 2:
		CheckCCHandle();
		break;
	case 3:
		CheckCPHandle();
		break;
	case 4:
		CheckCRHandle();
		break;
	case 5:
		CheckCV2Handle();
		break;
	case 6:
		CheckATHandle();
		break;
	}
	
	ObjDetectEnter(-1);	//
	//RefreshMenu();
}
