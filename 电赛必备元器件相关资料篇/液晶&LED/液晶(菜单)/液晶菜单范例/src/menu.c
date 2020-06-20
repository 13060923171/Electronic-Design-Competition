#include "menu.h"
#include "key.h"
#include "utili.h"
#include "lcd.h"
#include "param.h"
#include "query.h"
#include "time.h"

#include "coulometry.h"
#include "objdetect.h"

//��һ��˵���
struct MENU_ITEM code MainMenu[] = {
	{1,"�ֶ�����",32,2  ,NULL},//coulometry.c
	{2,"�Զ����",32,22 ,NULL},//objdetect.c
	{3,"�����趨",32,42 ,NULL},//param.c
	{4,"��ֵ��ѯ",32,2  ,NULL},//query.c
	{5,"����ʱ��",32,22 ,NULL},//time.c
	{6,"�����궨",32,42 ,NULL},//
	{0,NULL,0,0,NULL}
};


//�˵�����
uchar MenuItemCount;	//�˵�����, �ڳ�ʼ��һ���˵�ʱ�������
uchar MenuItemIndex;	//��ǰָ��(����Ա�ҳ)
uchar MenuPageCount;	//�˵�ҳ��, ÿҳ��ʾ3��
uchar MenuCurPage;	//��ǰҳ����. 
uchar MenuCurPageItemCount;//��ǰҳ�м����˵���
int8  MenuSelectItem;

//ָ��ǰ�˵���ָ��
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

//��ѡһ���˵���
void FocusMenuItem(struct MENU_ITEM *item)
{
	uchar len;
	uchar x,w;
	
	//��Ŀ���ֳ���
	len = strlen(item->name);
	
	//������С�߽�
	if( item->x < 8) 
		x = 0;
	else
		x = item->x -8;
	//�������߽�
	if( len + 2 > 16)
		w = 16;
	else 
		w = len + 2;
	//��ѡһ������		
	LCD_ReverseRect(x,item->y-1,w,18);
}

//��ʼ���˵�
void InitMenuItem()
{
	MenuItemCount = 0;
	MenuItemIndex = 0;
	MenuPageCount = 0;
	MenuCurPage = 0;
	MenuCurPageItemCount = 0;
	
	if( PMenu == NULL)
		return;
		
	//�����м����˵���
	while( PMenu[MenuItemCount++].id != 0);

	if( MenuItemCount > 0) MenuItemCount --; //ȥ�����һ�в���ȷ

	//����һ����Ҫ����ҳ
	MenuPageCount = MenuItemCount / PAGE_ITEM_COUNT;
	
	if( MenuPageCount * PAGE_ITEM_COUNT < MenuItemCount) 
		MenuPageCount ++;
	
	RefreshMenu(); //ˢ�²˵�
	
}
//���¼��ز˵�
void RefreshMenu()
{
	uchar index;
	uchar count;
	
	
	if( PMenu == NULL) 
		return;
	
	LCD_Cls();
	
	MenuCurPageItemCount = 0;
	
	index = MenuCurPage * PAGE_ITEM_COUNT;	//ҳ����ʼ���������
	
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
		//���������һҳ����ʾ">>"
		LCD_PrintHz12(112,52,">>");
	}
	
	if( MenuCurPage > 0)
	{
		//������ǵ�һҳ����ʾ"<<"
		LCD_PrintHz12(0,52,"<<");
	}
	else	//û�� << ��Ϊ�˳��˵�
	{
		//
		if( PMenu != MainMenu)	//���˵���û���˳���
		{
			LCD_PrintHz12(0,52,"<-");
		}
	}
	
	MenuItemIndex = 0;
	
	//��ѡ��һ��
	FocusMenuItem(&PMenu[MenuCurPage * PAGE_ITEM_COUNT]);

	//LCD_PrintWord(0,0,MenuCurPage,0,0);
}

//ѡ��ҳ��
void SelectMenuPage(bool dir)
{
	if( dir )	//��һҳ
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
		
	//���ǵ�ǰҳ���׸��˵����������
	FirstItemIndex = MenuCurPage * PAGE_ITEM_COUNT;
	
	//ȡ��ԭ��ѡ��Ĳ˵���
	FocusMenuItem(&PMenu[FirstItemIndex + MenuItemIndex]);
	
	//�ı䵽��һ��
	if( dir )
		MenuItemIndex = (MenuItemIndex + 1 ) % MenuCurPageItemCount;
	else
	{
		if( MenuItemIndex == 0)
			MenuItemIndex = MenuCurPageItemCount-1;
		else
			MenuItemIndex = (MenuItemIndex - 1 ) % MenuCurPageItemCount;
	}

	//ѡ��ǰ��
	FocusMenuItem(&PMenu[FirstItemIndex + MenuItemIndex]);
}
void SelectMenuItem()
{
	if( PMenu == NULL)
		return;
		
	//���ǵ�ǰҳ���׸��˵����������
	MenuSelectItem = MenuCurPage * PAGE_ITEM_COUNT + MenuItemIndex;
}

void SetMenu(struct MENU_ITEM *p)
{
	//if( p == PMenu)
	//	return;
		
	PrevMenu = PMenu;	//����һ����һ��Ĳ˵�
	
	PMenu = p;

	InitMenuItem();

	onKeyUp = onMenuKeyDown;
}
void ReturnBackMenu()	//ֻ�ܷ��ص�ǰһ��˵�
{
	if( PrevMenu == NULL)
		return;
	PMenu = PrevMenu;

	PrevMenu = NULL;	
	InitMenuItem();
	
	//���ü���ӳ��
	onKeyUp = onMenuKeyDown;
}

//����Ƿ������˵�
void ReturnMainMenu()
{
	SetMenu(MainMenu);

	//�Ѱ����¼�����Ϊ�˵������¼�������
	//onKeyUp = onMenuKeyDown;
}

//����˵������¼�
void onMenuKeyDown(uchar key)
{
	switch(key)
	{
	case KEY_UP:
		MoveMenuItem(false); //ѡ����һ��
		break;
	case KEY_DOWN:
		MoveMenuItem(true);   //ѡ����һ��
		break;
	case KEY_LEFT:
		SelectMenuPage(false);//��ǰһҳ
		break;
	case KEY_RIGHT:
		SelectMenuPage(true); //���һҳ
		break;
	case KEY_ENTER:
		SelectMenuItem();
		break;
	}
	
} 
//����˵�����
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
			LCD_PrintHz16(24,24,"��ӭʹ�ã�");
		}
	}
		
	MenuSelectItem = -1;	
	
}


//