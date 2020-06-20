#include <t89c51rd2.h>
#include "key.h"
#include "utili.h"

uchar cur_key;
uchar pre_key;

void (*onKeyUp)(uchar key);
void (*onKeyDown)(uchar key);

void KeyInit()
{
	cur_key = 0;
	pre_key = 0;
	
	onKeyUp = NULL;
	onKeyDown = NULL;
}

uchar ReadKey(void) 
{
	uchar key;
	
	//��������ڷ������ļ���ָ��,����������ȴ���ָ��
	//
	//
	
	//�����Ǽ�Ȿ�����̽ӿ�
	key = (~P0) & 0x1f; //
	if( key == 0)
		return 0;
	
	delay(250);

	key = (~P0) & 0x1f; //

	if( key == 0)
		return 0;

	return key;	
}

//����ɨ��,��������Ӧ���¼�
void KeyEvent()
{
	cur_key = ReadKey();
	
	if( cur_key != 0)	//�м��̰���
	{
		if( pre_key == cur_key)	//ǰһ����û�ͷ�
			return;
			
		pre_key = cur_key;
		
		if( onKeyDown != NULL) onKeyDown(cur_key);
	}
	else
	{
		if( pre_key != 0)	//�а����շſ�
		{
			if( onKeyUp != NULL) onKeyUp(pre_key);
		}
		pre_key = 0;
	}
}