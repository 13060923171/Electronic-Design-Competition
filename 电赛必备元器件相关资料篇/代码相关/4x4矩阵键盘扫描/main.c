#include "msp430x14x.h"
#include "keypad.h"
#include"1602.h"

uchar table[17]={"0123456789ABCDEFG"};

void main( void )
{                                                                           
  /*WDTCTL = WDT_ADLY_1_9;// 设置看门口为间隔定时器模式，定时间隔为1.9ms
  IE1 |=WDTIE;//打开看门口中断使能
  _EINT();*/
  
  Init_Keyboard();//初始化键盘
  LCD_Reset();//初始化液晶
  while(1)
  {
    Key_Event();
    if(key_flag==1)
    {
      key_flag=0;
      Write_Cmd(0x80);
      Write_Data(table[key_val]);  
    }
  }
}

/*#pragma vector=WDT_VECTOR
__interrupt void WDT_Timer(void)
{
  LPM3_EXIT;
}*/
  
  