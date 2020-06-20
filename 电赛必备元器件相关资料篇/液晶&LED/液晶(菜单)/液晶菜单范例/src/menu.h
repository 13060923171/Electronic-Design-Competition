#ifndef __MENU_H
#define __MENU_H

#include "typedef.h"

#define PAGE_ITEM_COUNT	3

struct MENU_ITEM{
	uchar id;	//�˵��ɣĺ�
	uchar name[16];	//�˵������Ϊ8�����ֳ���
	uchar x,y;	//����
	void (*func)(uchar id);	//����
};


void MenuInit();
void SetMenu(struct MENU_ITEM *p);
void RefreshMenu();
void ReturnBackMenu();
void ReturnMainMenu();
void MenuEvent();


#endif