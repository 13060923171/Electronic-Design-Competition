#include "objdetect.h"
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"

void ObjDetectMenuHandle(uchar id);

// ������ �˵�
struct MENU_ITEM code ObjDetectMenu[] = {
	{1,"�㶨��ѹ���CV" ,8,1 ,ObjDetectMenuHandle},//
	{2,"�㶨�������CC" ,8,18,ObjDetectMenuHandle},//
	{3,"�㶨���ʼ��CP" ,8,35,ObjDetectMenuHandle},//
	{4,"���Ե�����CR" ,8,1 ,ObjDetectMenuHandle},//
	{5,"��ǿ��ѹ���CV2",8,18,ObjDetectMenuHandle},//
	{6,"�Զ�ѭ�����AT" ,8,35,ObjDetectMenuHandle},//
	{7,"��  ��"         ,40,22,ObjDetectExit},
	{0,NULL,0,0,NULL}
};

//������ ���������
void ObjDetectEnter(uchar id)
{
	id = id;
	SetMenu(ObjDetectMenu);
}
//�������˵�
void ObjDetectExit(uchar id)
{
	id = id;
	ReturnMainMenu();
}
///////////////////////////////////////////////////////////////////
//�㶨��ѹ���CV
void CheckCVHandle()
{
	uchar key = 0;

	LCD_Cls();
	LCD_PrintHz16(1,2, "�㶨��ѹ���CV");
	LCD_Print24X32(0,24,"60.000");

	//��LEFT��������һ��
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}

//�㶨�������CC
void CheckCCHandle()
{
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "�㶨�������CC");
	LCD_Print24X32(0,24,"0.001");
	//��LEFT��������һ��
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//�㶨���ʼ��CP
void CheckCPHandle()
{
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "�㶨���ʼ��CP");
	LCD_Print24X32(0,24,"0.000");
	//��LEFT��������һ��
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//���Ե�����CR
void CheckCRHandle()
{
	//
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "���Ե�����CR");
	LCD_Print24X32(0,24,"0.000");
	//��LEFT��������һ��
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//��ǿ��ѹ���CV2
void CheckCV2Handle()
{
	//
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "��ǿ��ѹ���CV2");
	LCD_Print24X32(0,24,"0.000");
	//��LEFT��������һ��
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}
//�Զ�ѭ�����AT
void CheckATHandle()
{
	//
	uchar key = 0;
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "�Զ�ѭ�����AT");
	//��LEFT��������һ��
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	}
}


////////////////////////////////////////////////////////////////
void ObjDetectMenuHandle(uchar id)
{
	onKeyUp = NULL;	//�Ѽ��̴������
	
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
