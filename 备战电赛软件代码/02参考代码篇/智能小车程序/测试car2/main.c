//P3.0~1 ���ƺ��ⷢ��
//p4.2~5 ������룻 p4.6~7 ���ʹ��
//P6��0~1  ������룬��Ϊ����

#include "msp430x16x.h"
#define uint unsigned int
#define uchar unsigned char

#include "ADC.h"
/********************************************/
//��������
void Int_Clk();
void Init_PWM();
void delay(uint t);
void Turn();
uchar Switch();
void Init_Port();
/********************************************/

/********************************************/
//��������
extern uint Result[8];    //ADC�����Ϊ16���� FFFF
uchar Motor_Flag=0;   //�����־λ 0ǰ�� 1��ת 2��ת 3����
/********************************************/



/*******************************************
�������ƣ�Int_Clk
��    �ܣ�ʱ�ӳ�ʼ������ϵͳʱ�Ӻ���ϵͳʱ�Ӿ�Ϊ8M
��    ������
����ֵ  ����
********************************************/
void Int_Clk()
{
  uchar i;
  BCSCTL1&=~XT2OFF;
  BCSCTL2|=SELM1+SELS;     //MCLK & SMCLK 8M
  do
  {
    IFG1&=~OFIFG;
    for (i=0;i<100;i++)
      _NOP();  
  }
  while((IFG1&OFIFG)!=0);
  IFG1&=~OFIFG;
}


/*******************************************
�������ƣ�Init_PWM
��    �ܣ���ʼ��PWM
��    ������
����ֵ  ����
********************************************/
void Init_PWM()
{
  P4DIR|=BIT7;
  P4OUT|=BIT7;
  P4DIR|=BIT6;
  P4OUT|=BIT6;
  P4DIR|=BIT2+BIT3+BIT4+BIT5;
  P4SEL|=BIT2+BIT3+BIT4+BIT5;
  TBCCTL2 |= OUTMOD_7;//TBCCR2���Ϊģʽ7,RESET/SET
  TBCCR2 =0;  //P4.2�����TB1__L1
  TBCCTL3 |= OUTMOD_7; //TBCCR3���Ϊģʽ7,SET/RESET
  TBCCR3 =0;                 //P4.3�����TB2__L2
   TBCCTL4 |= OUTMOD_7;//TBCCR4���Ϊģʽ7,RESET/SET
  TBCCR4 =0;  //P4.4�����TB1__L1
  TBCCTL5 |= OUTMOD_7; //TBCCR5���Ϊģʽ7,SET/RESET
  TBCCR5 =0;                 //P4.5�����TB2__L2
  TBCTL |= TBSSEL_1+MC_1;  //TIMER_B��������������ʽ   TBSSEL HAVE TO
  TBCCR0=800;
}


/*******************************************
�������ƣ�delau
��    �ܣ���ʱ
��    ����t
����ֵ  ����
********************************************/
void delay(uint t)
{
  uint i,j;
  for (i=0;i<t;i++)
    for (j=0;j<t;j++)
      _NOP();
}



/*******************************************
�������ƣ�Turn
��    �ܣ�ǰ����ת��
��    ������
����ֵ  ����
 
TBCCR2 ����ǰ��
TBCCR3 ���ֺ���
TBCCR4 ����ǰ��
TBCCR5 ���ֺ���
********************************************/
void Turn()
{
  switch(Motor_Flag)
  {
  case 0:
    {
      TBCCR2=800;   //ȫ��ǰ��
      TBCCR3=0;
      TBCCR4=800;
      TBCCR5=0;
      break;
    }
  case 1:          //��ת
    {
      TBCCR2=0;
      TBCCR3=600;
      TBCCR4=800;
      TBCCR5=0;
      break;
    }
  case 2:          //��ת
    {
      TBCCR2=800;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=600;
      break;
    }
  case 3:          //���ٺ���
    {
      TBCCR2=0;
      TBCCR3=700;
      TBCCR4=0;
      TBCCR5=700;
      break;
    }
  default:         //ֹͣ
    {
      TBCCR2=0;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=0;
      break;
    }
  }
  delay(100);
}



/*******************************************
�������ƣ�Switch
��    �ܣ��жϺ���λ��
��    ������
����ֵ  ��0:�м�:ǰ��   1:����ƫ��,��ת   2:����ƫ�ң���ת  4:ֹͣ
********************************************/
uchar Switch()
{ 
  //3276 2V  1638 1V  
  P3OUT=0x03;   //�������������
  delay(10);
  ADC12_Deal();  //��ʼADCת��
  if (Result[0]>3500 && Result[1]<2000)
  {
    P3OUT=0;
    return 2;      //����ƫ��
    
  }
  if (Result[0]<2000 && Result[1]>3500)
  {
    P3OUT=0;
    return 1;    //����ƫ��
    
  }
    
  //if (Result[0]<1638 && Result[1]<1638)
    //return 0;
  P3OUT=0;
  return 0;
}



/*******************************************
�������ƣ�Init_Port
��    �ܣ���ʼ���˿�
��    ������
����ֵ  ����
********************************************/
void Init_Port()
{
  P3DIR=0xFF;   //���������
}



void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Int_Clk();
  Init_Port();
  ADC12_Init();
  Init_PWM();
  _EINT();
  while(1)
  {
    Motor_Flag=Switch();
    Turn();
  }
}
