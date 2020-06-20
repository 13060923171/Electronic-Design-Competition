/****************************
函数功能：扫描键盘，并读取键值
****************************/
#include "msp430x14x.h"
typedef unsigned int uint;
typedef unsigned char uchar;

/********全局变量************/
uint key_pressed,key_flag;
uint key_val;

/*********************局部变量***********************/
uchar key_map[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint row,col,tmp1,tmp2,position=0;
uint tmp=0x00;

/***********************
函数名称:Init_Keyboard
功能：完成键盘初始化
参数：无
返回值：无
***********************/
void Init_Keyboard(void)
{
  P2DIR=0XF0;//将P2口底四位设置为输入，高四位设置为输出
  P2OUT=0;//使P2口高四位输出为0，等待按键按下
  key_pressed=0;
  key_flag=0;
  key_val=0;
}
                                                                                
/***********************
函数名称:delay
功能：延时约15ms，完成消斗功能
参数：无
返回值：无
***********************/
                                                                                
void Delay(void)
{
  uint delay_num;
  for(delay_num=5000;delay_num>0;delay_num--);
}

                                                  
/***********************
函数名称:Key_Check
功能：用于矩阵键盘行列扫描
参数：无
返回值：无
***********************/
void Key_Check(void)
{
  tmp1=0x80;
  for(row=4;row>=1;row--)//行扫描
    {
      P2OUT=0XF0;
      P2OUT -=0X80;
      tmp1 >>=1;
      if(tmp<0X0F)//检测有键被按下
      {
        tmp2=0x01;
        for(col=1;col<=4;col++)//列扫描
          {
            if((tmp & tmp2)==0x00)//检查那一列为的键被按下
            {
              position=(row-1)*4+col;//确定梭按键盘位置
              key_val=key_map[position];//读出键值
              return;//结束循环
            }
            tmp2 <<=1;
          }
      }
      
    }
  
  
}

                                                  
/***********************
函数名称:key_event
功能：检测是否有键摁下，并赋予相应键值
参数：无
返回值：无
***********************/
void Key_Event(void)
{
  tmp=P2IN&0x0f;
  if((key_pressed==0)&&((tmp&0x0f)<0x0f))//判断有键按下
  {
    key_pressed=1;
    Delay();
    Key_Check();
  }
  else if((key_pressed==1)&&(tmp&0x0f==0x0f))//如果按键已经被按下且松开
  {
    key_pressed=0;
    key_flag=1;
  }
  else _NOP();//不操作
  
}

                                                                                    
                                                                                      
                                                                                  
                                                                                
