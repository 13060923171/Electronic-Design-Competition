/****************************
�������ܣ�ɨ����̣�����ȡ��ֵ
****************************/
#include "msp430x14x.h"
typedef unsigned int uint;
typedef unsigned char uchar;

/********ȫ�ֱ���************/
uint key_pressed,key_flag;
uint key_val;

/*********************�ֲ�����***********************/
uchar key_map[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint row,col,tmp1,tmp2,position=0;
uint tmp=0x00;

/***********************
��������:Init_Keyboard
���ܣ���ɼ��̳�ʼ��
��������
����ֵ����
***********************/
void Init_Keyboard(void)
{
  P2DIR=0XF0;//��P2�ڵ���λ����Ϊ���룬����λ����Ϊ���
  P2OUT=0;//ʹP2�ڸ���λ���Ϊ0���ȴ���������
  key_pressed=0;
  key_flag=0;
  key_val=0;
}
                                                                                
/***********************
��������:delay
���ܣ���ʱԼ15ms�������������
��������
����ֵ����
***********************/
                                                                                
void Delay(void)
{
  uint delay_num;
  for(delay_num=5000;delay_num>0;delay_num--);
}

                                                  
/***********************
��������:Key_Check
���ܣ����ھ����������ɨ��
��������
����ֵ����
***********************/
void Key_Check(void)
{
  tmp1=0x80;
  for(row=4;row>=1;row--)//��ɨ��
    {
      P2OUT=0XF0;
      P2OUT -=0X80;
      tmp1 >>=1;
      if(tmp<0X0F)//����м�������
      {
        tmp2=0x01;
        for(col=1;col<=4;col++)//��ɨ��
          {
            if((tmp & tmp2)==0x00)//�����һ��Ϊ�ļ�������
            {
              position=(row-1)*4+col;//ȷ���󰴼���λ��
              key_val=key_map[position];//������ֵ
              return;//����ѭ��
            }
            tmp2 <<=1;
          }
      }
      
    }
  
  
}

                                                  
/***********************
��������:key_event
���ܣ�����Ƿ��м����£���������Ӧ��ֵ
��������
����ֵ����
***********************/
void Key_Event(void)
{
  tmp=P2IN&0x0f;
  if((key_pressed==0)&&((tmp&0x0f)<0x0f))//�ж��м�����
  {
    key_pressed=1;
    Delay();
    Key_Check();
  }
  else if((key_pressed==1)&&(tmp&0x0f==0x0f))//��������Ѿ����������ɿ�
  {
    key_pressed=0;
    key_flag=1;
  }
  else _NOP();//������
  
}

                                                                                    
                                                                                      
                                                                                  
                                                                                
