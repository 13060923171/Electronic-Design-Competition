#include "coulometry.h"
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"

#include "passwd.h"

void CoulometryMenuHandle(uchar id);

// �������� �˵�
struct MENU_ITEM code CoulometryMenu[] = {
	{1,"ֱ������",32,2  ,CoulometryMenuHandle},//DCWatch
	{2,"��������",32,22 ,CoulometryMenuHandle},//ACWatch
	{3,"��    ��",32,42 ,CoulometryExit},
	{0,NULL,0,0,NULL}
};

//�����������������
void CoulometryEnter(uchar id)
{
	id = id;
	SetMenu(CoulometryMenu);
}
//�������˵�
void CoulometryExit(uchar id)
{
	id = id;
	ReturnMainMenu();
}

//ֱ������
void DCWatch()
{
	uchar key = 0;
	
	GetPassword();
	
	LCD_Cls();
	LCD_PrintHz16(1,2, "��ѹ1 60.000 Vdc");
	LCD_PrintHz16(1,22,"����1  20.00 Adc");
	LCD_PrintHz16(1,42,"����1 000020   W");

	//��LEFT��������һ��
	while(key != KEY_LEFT)
	{
		key = ReadKey();
	
		//ʹ��UP/DOWN����ҳ?	
		
	}
}
//��������
void ACWatch()
{
	LCD_Cls();
	LCD_PrintHz16(32,22,"��������");

	//��LEFT��������һ��
	while( ReadKey() != KEY_LEFT);
}

///////////////////////////////////////////////////////////////////
//�˵������¼�
void CoulometryMenuHandle(uchar id)
{
	onKeyUp = NULL;	//�Ѽ��̴������
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
	//RefreshMenu();//ˢ�²˵�
}
