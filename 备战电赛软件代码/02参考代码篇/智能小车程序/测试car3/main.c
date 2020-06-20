#include "msp430x16x.h"
#define uint unsigned int
#define uchar unsigned char

#include "ADC.h"
/********************************************/
//函数定义
void Int_Clk();
void Init_PWM();
void delay(uint t);
void Turn();
uchar Switch();
void Init_Port();
/********************************************/

/********************************************/
//变量定义
extern uint Result[8];    //ADC结果，为16进制 FFFF
uchar Motor_Flag=0;   //电机标志位 0前进 1左转 2右转 3后退
/********************************************/



/*******************************************
函数名称：Int_Clk
功    能：时钟初始化，主系统时钟和子系统时钟均为8M
参    数：无
返回值  ：无
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
函数名称：Init_PWM
功    能：初始化PWM
参    数：无
返回值  ：无
********************************************/
void Init_PWM()
{
  P4DIR|=BIT7;
  P4OUT|=BIT7;
  P4DIR|=BIT6;
  P4OUT|=BIT6;
  P4DIR|=BIT2+BIT3+BIT4+BIT5;
  P4SEL|=BIT2+BIT3+BIT4+BIT5;
  TBCCTL2 |= OUTMOD_7;//TBCCR2输出为模式7,RESET/SET
  TBCCR2 =0;  //P4.2口输出TB1__L1
  TBCCTL3 |= OUTMOD_7; //TBCCR3输出为模式7,SET/RESET
  TBCCR3 =0;                 //P4.3口输出TB2__L2
   TBCCTL4 |= OUTMOD_7;//TBCCR4输出为模式7,RESET/SET
  TBCCR4 =0;  //P4.4口输出TB1__L1
  TBCCTL5 |= OUTMOD_7; //TBCCR5输出为模式7,SET/RESET
  TBCCR5 =0;                 //P4.5口输出TB2__L2
  TBCTL |= TBSSEL_1+MC_1;  //TIMER_B工作于增计数方式   TBSSEL HAVE TO
  TBCCR0=800;
}


/*******************************************
函数名称：delau
功    能：延时
参    数：t
返回值  ：无
********************************************/
void delay(uint t)
{
  uint i,j;
  for (i=0;i<t;i++)
    for (j=0;j<t;j++)
      _NOP();
}



/*******************************************
函数名称：Turn
功    能：前进与转弯
参    数：无
返回值  ：无
 
TBCCR2 左轮前进
TBCCR3 左轮后退
TBCCR4 右轮前进
TBCCR5 右轮后退
********************************************/
void Turn()
{
  switch(Motor_Flag)
  {
  case 0:
    {
      TBCCR2=800;   //全速前进
      TBCCR3=0;
      TBCCR4=800;
      TBCCR5=0;
    //   delay(400);
      break;
    }
  case 1:          //左转
    {
      TBCCR2=0;
      TBCCR3=600;
      TBCCR4=800;
      TBCCR5=0;
      break;
    }
  case 2:          //右转
    {
      TBCCR2=800;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=600;
      break;
    }
  case 3:          //快速后退
    {
      TBCCR2=0;
      TBCCR3=700;
      TBCCR4=0;
      TBCCR5=700;
      break;
    }
  case 4:        //左转90度
    {
      TBCCR2=100;
      TBCCR3=0;
      TBCCR4=800;
      TBCCR5=0;
      delay(1350);
      
      TBCCR2=0;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=0;
      delay(350);
      break;
   
    }
  case 5:       //右转90度
    {
      TBCCR2=800;
      TBCCR3=0;
      TBCCR4=100;
      TBCCR5=0;
      delay(1350);
      
      TBCCR2=0;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=0;
      delay(350);
      break;
      
    }
  case 6:      //左转180度
    {
      TBCCR2=0;
      TBCCR3=800;
      TBCCR4=750;
      TBCCR5=0;
      delay(300);
      
      TBCCR2=0;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=0;
      delay(200);
      break;
    
    }
/* case 7:       //右转180度
    {
      TBCCR2=500;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=500;
    delay(3000);
      break;
    }
  */  
   case 8:         //停止
    {
      TBCCR2=0;
      TBCCR3=0;
      TBCCR4=0;
      TBCCR5=0;
      delay(3000);
      break;
    }
  }
  delay(100);
}


/*******************************************
函数名称：Init_Port
功    能：初始化端口
参    数：无
返回值  ：无
********************************************/
void Init_Port()
{
  P3DIR=0xFF;   //红外二极管
}


/*******************************************
函数名称：Switch
功    能：判断黑线位置
参    数：无
返回值  ：0:中间:前进   1:黑线偏左,左转   2:黑线偏右，右转  4:停止
********************************************/
uchar Switch()
{ 

  //3276 2V  1638 1V  
  P3OUT=0xff;   //开启红外二极管
  delay(10);
  ADC12_Deal();  //开始ADC转化
  if (Result[0]>3500 && Result[1]<2000)
  {
   P3OUT=0;
    return 2;      //黑线偏左
    
  }
  if (Result[0]<2000 && Result[1]>3500)
  {
    P3OUT=0;
    return 1;    //黑线偏右
    
  }
    
   if(Result[4]<2000 && Result[5]<2000 && Result[6]<2000)  //尽头180度回转
  {
    P3OUT=0;
   
   return 6;

   //return 7;
     
  }
  
  
  if (Result[0]<2000 && Result[1]<2000 && Result[2]<2000 &&
      Result[3]<2000)
  {                        //直走
    P3OUT=0;
    return 0;
  }  
  
  if(Result[2]>2700 && Result[3]<2000 && 
     Result[4]<2000 && Result[5]<2000)      //直角左转
  {
   P3OUT=0; 
   return 4;
 
  } 
  
 if(Result[2]<2000 && Result[3]>2700 && 
     Result[4]<2000 && Result[5]<2000)     //直角右转
  {
   P3OUT=0; 
   return 5;
    
  }
  
  if(Result[2]>3000 && Result[3]<2000 && 
     (Result[4]>3000 || Result[5]>3000))   //T型左转
  {
     delay(10);
   P3OUT=0;
   return 4;

  }
  
   if(Result[3]>3000 && Result[2]<2000 && 
     (Result[4]>3000 || Result[5]>3000))   //T型直走
   {
      delay(10);
    
   P3OUT=0; 
  
    return 0;
 
   }  
   
   if(Result[3]>3000 && Result[2]>3000 && 
     (Result[4]>3000 || Result[5]>3000))    //十字左转
   {
      delay(10);
    P3OUT=0;
    return 4;
 
   }
    
  if(Result[3]>3000 && Result[2]>3000 &&  
      Result[4]<2000 && Result[5]<2000)   //T型左转
  {
     delay(10);
    P3OUT=0;
    return 4;
   
  }
  
  
  if(Result[1]>3000 && Result[0]>3000 && Result[6]>3000)
      {
        P3OUT=0;
        return 8;
 
      }

  P3OUT=0;
  return 0;
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
