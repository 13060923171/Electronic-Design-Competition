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
	
	//这里读串口发过来的键盘指令,如果有则优先串口指令
	//
	//
	
	//以下是检测本机键盘接口
	key = (~P0) & 0x1f; //
	if( key == 0)
		return 0;
	
	delay(250);

	key = (~P0) & 0x1f; //

	if( key == 0)
		return 0;

	return key;	
}

//键盘扫描,并产生相应的事件
void KeyEvent()
{
	cur_key = ReadKey();
	
	if( cur_key != 0)	//有键盘按下
	{
		if( pre_key == cur_key)	//前一个键没释放
			return;
			
		pre_key = cur_key;
		
		if( onKeyDown != NULL) onKeyDown(cur_key);
	}
	else
	{
		if( pre_key != 0)	//有按键刚放开
		{
			if( onKeyUp != NULL) onKeyUp(pre_key);
		}
		pre_key = 0;
	}
}