#include "msp430x14x.h"
#include "keypad.h"
#include"1602.h"

uchar table[17]={"0123456789ABCDEFG"};

void main( void )
{                                                                           
  /*WDTCTL = WDT_ADLY_1_9;// ���ÿ��ſ�Ϊ�����ʱ��ģʽ����ʱ���Ϊ1.9ms
  IE1 |=WDTIE;//�򿪿��ſ��ж�ʹ��
  _EINT();*/
  
  Init_Keyboard();//��ʼ������
  LCD_Reset();//��ʼ��Һ��
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
  
  