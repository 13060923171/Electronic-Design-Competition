#include "menu.h"
#include "key.h"
#include "utili.h"
#include "lcd.h"
#include "param.h"
#include "query.h"
#include "time.h"

#include "coulometry.h"
#include "objdetect.h"

//第一层菜单：
struct MENU_ITEM code MainMenu[] = {
	{1,"手动测试",32,2  ,NULL},//coulometry.c
	{2,"自动检测",32,22 ,NULL},//objdetect.c
	{3,"参数设定",32,42 ,NULL},//param.c
	{4,"数值查询",32,2  ,NULL},//query.c
	{5,"调整时间",32,22 ,NULL},//time.c
	{6,"仪器标定",32,42 ,NULL},//
	{0,NULL,0,0,NULL}
};


//菜单控制
uchar MenuItemCount;	//菜单项数, 在初始化一个菜单时计算出来
uchar MenuItemIndex;	//当前指向(是相对本页)
uchar MenuPageCount;	//菜单页数, 每页显示3项
uchar MenuCurPage;	//当前页面数. 
uchar MenuCurPageItemCount;//当前页有几个菜单项
int8  MenuSelectItem;

//指向当前菜单的指针
struct MENU_ITEM *PrevMenu,*PMenu;

void onMenuKeyDown(uchar key);

void MenuInit()
{
	MenuItemCount = 0;
	MenuItemIndex = 0;
	MenuPageCount = 0;
	MenuCurPage = 0;
	MenuCurPageItemCount = 0;
	MenuSelectItem = -1;
	
	PMenu = NULL;
	PrevMenu = NULL;
}

//反选一个菜单项
void FocusMenuItem(struct MENU_ITEM *item)
{
	uchar len;
	uchar x,w;
	
	//项目名字长度
	len = strlen(item->name);
	
	//控制最小边界
	if( item->x < 8) 
		x = 0;
	else
		x = item->x -8;
	//控制最大边界
	if( len + 2 > 16)
		w = 16;
	else 
		w = len + 2;
	//反选一个区域		
	LCD_ReverseRect(x,item->y-1,w,18);
}

//初始化菜单
void InitMenuItem()
{
	MenuItemCount = 0;
	MenuItemIndex = 0;
	MenuPageCount = 0;
	MenuCurPage = 0;
	MenuCurPageItemCount = 0;
	
	if( PMenu == NULL)
		return;
		
	//计算有几个菜单项
	while( PMenu[MenuItemCount++].id != 0);

	if( MenuItemCount > 0) MenuItemCount --; //去掉最后一行才正确

	//计算一下需要多少页
	MenuPageCount = MenuItemCount / PAGE_ITEM_COUNT;
	
	if( MenuPageCount * PAGE_ITEM_COUNT < MenuItemCount) 
		MenuPageCount ++;
	
	RefreshMenu(); //刷新菜单
	
}
//重新加载菜单
void RefreshMenu()
{
	uchar index;
	uchar count;
	
	
	if( PMenu == NULL) 
		return;
	
	LCD_Cls();
	
	MenuCurPageItemCount = 0;
	
	index = MenuCurPage * PAGE_ITEM_COUNT;	//页面起始项的索引号
	
	count = 0;
	while( PMenu[index].id != 0 && count < PAGE_ITEM_COUNT)
	{
		LCD_PrintHz16(PMenu[index].x,PMenu[index].y,PMenu[index].name);
		MenuCurPageItemCount ++;
		index++;
		count ++;
	}
	
	if( MenuCurPage < (MenuPageCount - 1))
	{
		//如果还有下一页则显示">>"
		LCD_PrintHz12(112,52,">>");
	}
	
	if( MenuCurPage > 0)
	{
		//如果不是第一页则显示"<<"
		LCD_PrintHz12(0,52,"<<");
	}
	else	//没有 << 是为退出菜单
	{
		//
		if( PMenu != MainMenu)	//主菜单就没有退出了
		{
			LCD_PrintHz12(0,52,"<-");
		}
	}
	
	MenuItemIndex = 0;
	
	//反选第一项
	FocusMenuItem(&PMenu[MenuCurPage * PAGE_ITEM_COUNT]);

	//LCD_PrintWord(0,0,MenuCurPage,0,0);
}

//选择页面
void SelectMenuPage(bool dir)
{
	if( dir )	//下一页
	{
		if( MenuCurPage < (MenuPageCount - 1)) 
		{	
			MenuCurPage ++;
			RefreshMenu();
		}
	}
	else
	{
		if( MenuCurPage > 0) 
		{	
			MenuCurPage --;
			RefreshMenu();
		}
		else
		{
			if( PMenu != MainMenu)
			{
				ReturnMainMenu();
			}
		}
	}
}
void MoveMenuItem(bool dir)
{
	uchar FirstItemIndex;
	
	if( PMenu == NULL) 
		return;
		
	//这是当前页面首个菜单项的索引号
	FirstItemIndex = MenuCurPage * PAGE_ITEM_COUNT;
	
	//取消原来选择的菜单项
	FocusMenuItem(&PMenu[FirstItemIndex + MenuItemIndex]);
	
	//改变到下一项
	if( dir )
		MenuItemIndex = (MenuItemIndex + 1 ) % MenuCurPageItemCount;
	else
	{
		if( MenuItemIndex == 0)
			MenuItemIndex = MenuCurPageItemCount-1;
		else
			MenuItemIndex = (MenuItemIndex - 1 ) % MenuCurPageItemCount;
	}

	//选择当前项
	FocusMenuItem(&PMenu[FirstItemIndex + MenuItemIndex]);
}
void SelectMenuItem()
{
	if( PMenu == NULL)
		return;
		
	//这是当前页面首个菜单项的索引号
	MenuSelectItem = MenuCurPage * PAGE_ITEM_COUNT + MenuItemIndex;
}

void SetMenu(struct MENU_ITEM *p)
{
	//if( p == PMenu)
	//	return;
		
	PrevMenu = PMenu;	//保存一下上一层的菜单
	
	PMenu = p;

	InitMenuItem();

	onKeyUp = onMenuKeyDown;
}
void ReturnBackMenu()	//只能返回到前一层菜单
{
	if( PrevMenu == NULL)
		return;
	PMenu = PrevMenu;

	PrevMenu = NULL;	
	InitMenuItem();
	
	//重置键盘映射
	onKeyUp = onMenuKeyDown;
}

//这个是反回主菜单
void ReturnMainMenu()
{
	SetMenu(MainMenu);

	//把按键事件设置为菜单按键事件处理函数
	//onKeyUp = onMenuKeyDown;
}

//处理菜单键盘事件
void onMenuKeyDown(uchar key)
{
	switch(key)
	{
	case KEY_UP:
		MoveMenuItem(false); //选择上一项
		break;
	case KEY_DOWN:
		MoveMenuItem(true);   //选择下一项
		break;
	case KEY_LEFT:
		SelectMenuPage(false);//向前一页
		break;
	case KEY_RIGHT:
		SelectMenuPage(true); //向后一页
		break;
	case KEY_ENTER:
		SelectMenuItem();
		break;
	}
	
} 
//处理菜单命令
void MenuEvent()
{
	if( MenuSelectItem != -1 && PMenu != NULL)
	{		
		if(PMenu[MenuSelectItem].func != NULL)
		{
			PMenu[MenuSelectItem].func(PMenu[MenuSelectItem].id);
		}
		else
		{
			LCD_PrintHz16(24,24,"欢迎使用！");
		}
	}
		
	MenuSelectItem = -1;	
	
}


//