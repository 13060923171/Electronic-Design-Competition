/***********************************
���ܣ�ʵ��1602Һ����ʾ
************************************/
#include"msp430x14x.h"
#include"keypad.h"
#define CLR_RS P4OUT &=~BIT7
#define SET_RS P4OUT |=BIT7
#define CLR_RW P4OUT &=~BIT6
#define SET_RW P4OUT |=BIT6
#define SET_EN P4OUT |=BIT5
#define CLR_EN P4OUT &=~BIT5

#define DataDIR P5DIR
#define DataPort P5OUT

extern void Write_Cmd(uchar);
extern void Write_Data(uchar);
extern void delay(void);

/***********************************
�������ƣ�LCD_Reset
���ܣ�Һ����ʼ��
��������
����ֵ����
************************************/

void LCD_Reset(void)
{
  DataDIR=0XFF;
  P4DIR |=BIT5+BIT6+BIT7;
  CLR_RW;
  
  Write_Cmd(0x38);//��ʾģʽ����
  Write_Cmd(0x08);//��ʾ�ر�
  Write_Cmd(0x01);//��ʾ����
  Write_Cmd(0x06);//��ʾ����ƶ�����
  Write_Cmd(0x0c);//��ʾ�����������

}

/***********************************
�������ƣ�Write_Cmd
���ܣ���Һ��д����
������cmd
����ֵ����
************************************/
void Write_Cmd(uchar cmd)
{
  //Wait_Enable();
  CLR_RS; 
  _NOP();
  
  SET_EN;//��λENλд������
  DataPort=cmd;//������д�����ݶ˿�
  delay();
  CLR_EN;
}
  
  
  
/***********************************
�������ƣ�Write_Data
���ܣ���Һ��д����
��������
����ֵ����
************************************/  
void Write_Data(uchar data)
{
 // Wait_Enable();
  SET_RS;
  _NOP();
  
  SET_EN;//��λENλд������  
  DataPort=data;//������д�����ݶ˿�
  delay();
  CLR_EN;
  
}
/***********************************
�������ƣ�delay
���ܣ���ʱ
��������
����ֵ����
************************************/
void delay(void)
{
  uint temp;
  for(temp=300;temp>0;temp--);
}