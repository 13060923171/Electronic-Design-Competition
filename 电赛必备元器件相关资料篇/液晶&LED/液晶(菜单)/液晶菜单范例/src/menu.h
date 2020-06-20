#ifndef __MENU_H
#define __MENU_H

#include "typedef.h"

#define PAGE_ITEM_COUNT	3

struct MENU_ITEM{
	uchar id;	//菜单ＩＤ号
	uchar name[16];	//菜单项最大为8个汉字长度
	uchar x,y;	//坐标
	void (*func)(uchar id);	//功能
};


void MenuInit();
void SetMenu(struct MENU_ITEM *p);
void RefreshMenu();
void ReturnBackMenu();
void ReturnMainMenu();
void MenuEvent();


#endif