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
	LCD_PrintHz16(24,24,"��ӭʹ�ã�");
	delay1(100);

	//���ò˵�,��һ������ʱָ�����˵�
	ReturnMainMenu();
		
	//���ز���
	//���Ͳ���
	
	while(1)
	{
		KeyEvent();	//�������¼�
		
		MenuEvent();	//����˵�����
	}
	
}