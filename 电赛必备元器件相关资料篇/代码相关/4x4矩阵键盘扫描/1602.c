/***********************************
功能：实现1602液晶显示
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
函数名称：LCD_Reset
功能：液晶初始化
参数：无
返回值：无
************************************/

void LCD_Reset(void)
{
  DataDIR=0XFF;
  P4DIR |=BIT5+BIT6+BIT7;
  CLR_RW;
  
  Write_Cmd(0x38);//显示模式设置
  Write_Cmd(0x08);//显示关闭
  Write_Cmd(0x01);//显示清屏
  Write_Cmd(0x06);//显示光标移动设置
  Write_Cmd(0x0c);//显示开及光标设置

}

/***********************************
函数名称：Write_Cmd
功能：向液晶写命令
参数：cmd
返回值：无
************************************/
void Write_Cmd(uchar cmd)
{
  //Wait_Enable();
  CLR_RS; 
  _NOP();
  
  SET_EN;//置位EN位写入命令
  DataPort=cmd;//将命令写入数据端口
  delay();
  CLR_EN;
}
  
  
  
/***********************************
函数名称：Write_Data
功能：向液晶写数据
参数：无
返回值：无
************************************/  
void Write_Data(uchar data)
{
 // Wait_Enable();
  SET_RS;
  _NOP();
  
  SET_EN;//置位EN位写入数据  
  DataPort=data;//将命令写入数据端口
  delay();
  CLR_EN;
  
}
/***********************************
函数名称：delay
功能：延时
参数：无
返回值：无
************************************/
void delay(void)
{
  uint temp;
  for(temp=300;temp>0;temp--);
}