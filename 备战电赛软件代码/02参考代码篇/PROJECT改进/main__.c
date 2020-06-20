/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.6c Professional
Automatic Program Generator
?Copyright 1998-2006 Pavel Haiduc, HP  vula_adcfoTech s.r.l.
http://www.hp vula_adcfotech.com

Project : 小车
Version :  0.00
Date    : 2007-8-30
Author  : momo
Company : zhong bei da xue
Comments:


Chip type           : ATmega16L
Program type        : Application
Clock frequency     : 1.000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/
#include "config.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif


#define   startCPA    TCCR1A|=0X40
#define   startCPB    TCCR1A|=0X10

#define   stopCPA     TCCR1A&=0XBF
#define   stopCPB     TCCR1A&=0XEF

#define   MBgo          PORTD.0=0
#define   MBback       PORTD.0=1

#define   MAgo          PORTD.1=0
#define   MAback       PORTD.1=1
/****************************************************
全局变量声明
*****************************************************/
volatile unsigned char  flage=0X00;
volatile unsigned char Timer[3]={0,0,0};
volatile unsigned char  write;
volatile unsigned char BalanceTime=0;
unsigned char  vula_adc;
 flash char *step[5]={ "stepA","stepB","stepC","back ","over "} ;
 flash char *mode[3]={"normal","advanc","demo  "};
 flash char *display[5]={"frist","sec   ","tree  ","four  ","five  "};
/******************************************************
系统中断    （显示，蜂鸣）
*******************************************************/
 interrupt [TIM0_COMP ] void tim0_comp_isr(void)
 {
    static unsigned char buzz;
    switch ( flage )
     {
    case 0X04:
                    Timer[0]=Read_sec( );
                   if( Timer[0]==Timer[2] )
                   ;
                   else
                   {
                       BalanceTime++;
                       LCD_write_char( 13 , 1 ,  ( BalanceTime%10)+'0' );
                       LCD_write_char( 12 , 1 ,  ( BalanceTime/10)+'0' );
                       Timer[2]=Timer[0]   ;
                    }
                    goto j1;
                    break;
    case 0X84:
                   Timer[0]=Read_sec( );
                   if( Timer[0]==Timer[2] )
                   ;
                   else
                   {
                       BalanceTime++;
                       LCD_write_char( 13 , 1 ,  ( BalanceTime%10)+'0' );
                       LCD_write_char( 12 , 1 ,  ( BalanceTime/10)+'0' );
                       Timer[2]=Timer[0]   ;
                    }
   case 0X80:
                    if( (++buzz) == 10 )
                      {
                            buzz=0;
                             PORTC.2=1;
                             flage -=0x80;
                            }
     j1:
    case 0X00:
                   Timer[0]=Read_sec( );
                   write=Timer[0];
                   LCD_write_char( 11 , 0 ,   (write& 0x0f)+ '0' );
                   LCD_write_char( 10 , 0 ,   ( (write>>4)&0x07) + '0' );

                   Timer[1]=Read_miu( );
                   write=Timer[1];
                   LCD_write_char(  4 , 0  ,  (write& 0x0f)+'0');
                   break;

    default:   break;
    }
  }
 /*****************************************************************************
 系统中断T2     (寻线校正)
 *****************************************************************************/
  interrupt [TIM2_COMP ] void tim2_comp_isr(void)
 {
     if(PORTD.1==1)
       {

          if(PIND.6==1)

              startCPA;

               else

                     stopCPA;


          if(PIND.7==1)

              startCPB;

                else

                     stopCPB;

       }
      else
        {
           if(PIND.2==1)

              startCPA;

                else

                      stopCPA;


          if(PIND.3==1)

              startCPB;

                else

                      stopCPB;
         }
 }
 /**********************************************************
 读取ad值
 ***********************************************************/
 unsigned char get_ad(void) {

	unsigned char i;
	 ADMUX = 0x60;			              /*基准AVCC、左对齐、通道0*/
                  ADCSRA = 0xC2;
                  			                               /*使能、开启、4分频*/
	while(!(ADCSRA & (1 << ADIF)));	                     /*等待*/
	i = ADCH;
	ADCSRA &= ~(1 << ADIF);		              /*清标志*/
	ADCSRA &= ~(1 << ADEN);		              /*关闭转换*/

	return i;
}
/*********************************************************************
蜂鸣器函数
*********************************************************************/
void beep( void )
{
   PORTC.2=0;
   delay_ms(500);
   PORTC.2=1;
}
 /**********************************************************************
 根据ad值选择平衡调整模式
 **********************************************************************/
unsigned char  output( void )
{
  if( vula_adc>=0X7D &&  vula_adc<=0X80 )
                return 0;
   if(  vula_adc>=0X7C && vula_adc<=0X81 )
                  return 1 ;
    if(  vula_adc>=0X7B &&  vula_adc<=0X82 )
                   return  2 ;
      if(  vula_adc>=0X7A &&  vula_adc<=0X83)
                    return 3 ;
       if(  vula_adc>=0X79 &&  vula_adc<=0X84)
                     return  4 ;
          if(  vula_adc>=0X78 &&  vula_adc<=0X85)
                      return  5 ;
            if(  vula_adc>=0X77 &&  vula_adc<=0X86)
                        return  6 ;
          if(  vula_adc>=0X76 &&  vula_adc<=0X87)
                          return  7  ;
       if(  vula_adc>=0X75 && vula_adc<=0X88)
                            return  8 ;
      if(vula_adc >=0X74 && vula_adc<= 0X89)
                           return 9;

}

/**************************************************************************************************
平衡驱动控制函数
**************************************************************************************************/
void  mortorgo( unsigned char high , unsigned char low, unsigned int  go_time, unsigned int  back_time )
 {
      OCR1AH=high;
      OCR1AL = low;                                                      //new
      OCR1BH=high;
      OCR1BL= low;

       if( vula_adc<0x7F)
      {
        MAgo;
        MBgo;
      }
      else
      {
       MAback;
       MBback;
      }

      startCPA ;
      startCPB ;

      delay_ms( go_time ) ;

      stopCPA ;
      stopCPB;

       if( vula_adc<0x7F)
      {
       MAback;
       MBback;
      }
      else
      {
        MAgo;
        MBgo;
      }

      startCPA ;
      startCPB ;

      delay_ms( back_time ) ;

      stopCPA ;
      stopCPB;
 }
void Findbenlen( void )
{
       unsigned char Flage_balan=1;
       unsigned char Flage_select;
       unsigned char count=0;
   do{

       vula_adc = get_ad ( );

       Flage_select = output(  ) ;

      switch ( output(  ) ) {
                      case 0 :

                                  stopCPA ;
                                  stopCPB;
                                  if(count<=9)
                                       count++;
                                     else
                                       Flage_balan=0;
                                 break;
                      case 1 :
                                  count=0;
                                  mortorgo( 0x03,0xFF , 400, 350 ) ;
                                 break;
                      case 2 :
                                  count=0;
                                  mortorgo( 0x02,0xFF , 800, 700 ) ;
                                 break;
                      case 3 :
                                 count=0;
                                 mortorgo( 0x02,0xFF , 900, 800 ) ;
                                 break;
                      case 4 :
                                 count=0;
                                 mortorgo( 0x02,0xFF , 900, 700 ) ;
                                 break;
                      case 5 :
                                  count=0;
                                mortorgo( 0x02,0xFF , 900, 650 ) ;
                                 break;
                      case 6 :
                                  count=0;
                                 mortorgo( 0x02,0xFF , 900,600 ) ;
                                 break;
                       case 7 :
                                  count=0;
                                  mortorgo( 0x02,0xFF , 1000, 700 ) ;
                                 break;
                       case 8 :
                                 count=0;
                                 mortorgo( 0x02,0xFF , 1000,600 ) ;
                                break;
                       case 9 :
                                  count=0;
                                  mortorgo( 0x02,0xFF , 1000,500) ;
                                  break;

                    default:
                                  count=0;
                                  mortorgo( 0x02,0xFF , 1000,500) ;
                                 break;
    };
       delay_ms(200);
  }

 while( Flage_balan );
}

unsigned char SelectMode ( void )
{
  unsigned char temp=0;
  LCD_write_str( 0 , 0 ,"press START key");
  LCD_write_str( 0 , 1 ,"zhong bei da xue ");

  while( PINC.6 ==1 );
  while(PINC.6 ==0);

  LCD_clear( );
  LCD_write_str( 0 , 0 ,"mode:");
  LCD_write_str( 0 , 1 ,"select mode");
  LCD_write_str( 6 , 0 ,mode[0]);
    delay_ms( 1000 );
  do
    {
      if( PINC.7==0 )
          {
             while(PINC.7==0);
				++temp;

             if(temp==3)
                temp=0;

               LCD_write_str( 6 , 0 ,mode[temp]);
          }
    }
     while(PINC.6==1);
     while(PINC.6==0);
      LCD_clear( );
   return(temp);
}

/*******************************************************
基本要求部分
*******************************************************/
void nomal ( void )
{
 /*****************************************************
 显示时钟，初始设置
 *****************************************************/
    LCD_write_str( 0 , 0 ,"minc:  sec: ");
    LCD_write_str( 0 ,  1 , step[0] );
    delay_ms(1000);
    beep(  );
    start_PCF( );
/********************************************************
  小车动作初始化
********************************************************/
    MBgo ;
    MAgo;
    OCR1AH=0x02;
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x02;
    OCR1BL=0xFF;
    startCPA ;
     startCPB ;
    delay_ms(2000);
    OCR1AH=0x01;
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x01;
    OCR1BL=0xFF;
    TCCR2  = 0x0A;                                   //自动寻线开

    SEI( );                                                  //全局中断开
    /**********************************************
    第一阶段
    **********************************************/
   while ( Timer[0] != 0X14 ) ;
   LCD_write_str( 0 ,  1 , step[1] );

   CLI( );
   beep(  );
   start_PCF( );
   SEI( );
   /************************************************
   寻找平衡
   ************************************************/
   TCCR2  = 0x00;                                      //寻线关
    Findbenlen(  );
    PORTC.2=0;
    flage+=0x80;                                        //开启蜂鸣，中断关闭

   /************************************************
   找到平衡
   ************************************************/
   CLI( );
   start_PCF( );                                                     //需要改动
   LCD_write_str( 0 ,  1 , step[2] );
   LCD_write_str( 6 ,  1 , "Balan:"  );
   flage+=0x04;                                                      //显示平衡
    SEI( );

    do
       {
        if( Timer[0]==0X05 )
            {
                         flage -=0x04;
                         CLI( );
                         start_PCF( );
                         SEI( );
            }
       }
   while ( (flage==0X04 )|| ( flage==0X84  ) );


   /***************************************************
   改变速度
   ***************************************************/
   CLI( );
   OCR1AH=0x01;
   OCR1AL =0xFF;                                                       //new
   OCR1BH=0x01;
   OCR1BL=0xFF;
   SEI( );

    MBgo;
    MAgo;
    startCPA ;
    startCPB ;
  /************************************************
   慢走，直到传感器感知木板落下
  ************************************************/
  TCCR2  = 0x0A;
   /**********************************************
   检测传感器状态，没黑线时停下
   ***********************************************/
  while(PIND.2||PIND.3);                                           //关寻线


  CLI( );
  stopCPA ;
  stopCPB;
  TCCR2  = 0x00;
  beep(  );
  start_PCF( );
  delay_us(10);
  SEI( );
  /***********************************************
   等待五秒，倒车返回
  ***********************************************/
  MBback;
  MAback;
  while( Timer[0]!=0X05 )  ;
  LCD_write_str( 0 ,  1 , step[3] );
  beep(  );

  OCR1AH=0x02;
  OCR1AL =0xFF;                                                       //new
  OCR1BH=0x02;
  OCR1BL=0xFF;

  startCPA ;
  startCPB ;

   TCCR2  = 0x0A;

  delay_ms(2000);

   OCR1AH=0x01;
  OCR1AL =0xFF;                                                       //new
  OCR1BH=0x01;
  OCR1BL=0xFF;


   while(PIND.6||PIND.7);
   beep( );
   LCD_write_str( 0 ,  1 , step[4] );

   CLI( );

   stopCPA ;
  stopCPB;
 }

  /******************************************************
  发挥部分
  ******************************************************/
void advance ( void )
{
   unsigned char find=1;
   unsigned char j=0;

   LCD_write_str( 0 , 0 ,"min:  sec: ");

   start_PCF( );

   MBgo ;
   MAgo;
   startCPA  ;
   startCPB  ;

    SEI( );
 /*****************************************************
　当两个传感器都在线上时开启寻线功能
 *****************************************************/
   while(find)
   {
     if( PIND.2==1 )
        find=0;

      if(PIND.3==1)
         find=0;
   } ;

 TCCR2  = 0x0A;

 /*****************************************************
 一定时间后，减速找平衡
 *****************************************************/
    while( Timer[0]<=0X30  );
    while ( get_ad( ) < 0X7F ) ;
       TCCR2  = 0x00;
 /****************************************************
 找到后，给出平衡指示，
 *****************************************************/
  do{
        Findbenlen(  );
        PORTC.2=0;
        flage+=0x80;

         CLI( );
         LCD_write_str( 0 ,  1 ,display[j] );
         LCD_write_str( 6 ,  1 , "Balan:"  );
         flage+=0x04;
         SEI( );
/******************************************************
检测平衡状态，不平衡时继续寻找
*******************************************************/
          while( get_ad( ) < 0X81 &&  get_ad( )>  0X7B );
          flage -=0x04;
          BalanceTime=0;                                                  //平衡显示清零
          LCD_write_str( 0,  1 , "                                   "  );
         if(j<5)
          j++;
          else
          j=0;
     }
    while(1);

/*******************************************************
找到后，给出平衡指示
*******************************************************/
/*******************************************************
   Findbenlen(  );
   PORTC.2=0;
   flage+=0x80;

   CLI( );
   LCD_write_str( 0 ,  1 , display[1] );
   LCD_write_str( 6 ,  1 , "Balan:"  );
   flage+=0x04;
   SEI( );

    delay_ms( 5000 );
    flage -=0x04;                                                         //关闭平衡显示
    TCCR0  = 0x00;                                                     //关闭显示

    MBgo ;
    MAback ;
    OCR1AH=0x02;
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x02;
    OCR1BL=0xFF;
    startCPA  ;
    startCPB  ;
    delay_ms( 4000 );                                                  //速度减慢一倍，延时增加一倍
     find=1;
      LCD_write_str( 0 ,  1 ,display[2] );

     while(find)                                                //当两个传感器有一个在线上时
   {
     if( PIND.2==1 )
        find=0;

      if(PIND.3==1)
         find=0;
   } ;

    MBgo ;
    MAgo;

    TCCR2  = 0x0A;                                               //开寻线

    delay_ms(5000);
                                                                          //5秒后加速
    OCR1AH=0x01;
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x01;
    OCR1BL=0xFF;

    delay_ms(5000);

                                                                           //关寻线
    TCCR2  = 0x00;

    stopCPA ;
     stopCPB;
    MBgo ;                                                              //梦幻舞步
    MAback;
    startCPA  ;
    startCPB  ;

    find=1;
    delay_ms(6000);

       while(find)                                                //当两个传感器有一个在线上时
   {
     if( PIND.2==1 )
        find=0;

      if(PIND.3==1)
         find=0;
  } ;

    MBgo ;                                                              //梦幻舞步
    MAgo;
     TCCR2  = 0x0A;
                                                    //开寻线
     delay_ms(5000);

//***********************************************************************
反向转弯
//***********************************************************************
      TCCR2  = 0x00;

     stopCPA ;
     stopCPB;
    MBback ;                                                              //梦幻舞步
    MAgo;
    delay_ms(1000);
    startCPA  ;
    startCPB  ;

    find=1;
    delay_ms(6000);

       while(find)                                                //当两个传感器有一个在线上时
   {
     if( PIND.2==1 )
        find=0;

      if(PIND.3==1)
         find=0;
  } ;

    MBgo ;                                                              //梦幻舞步
    MAgo;
     TCCR2  = 0x0A;

  **************************************************************************/
}

void demo( void )
{

    unsigned char find=1;

    LCD_write_str( 0 , 0 ,"min:  sec: ");

    start_PCF( );

    MAgo ;
    MBback;
    startCPA  ;
    startCPB  ;

    SEI( );

     find=1;

     while(find)                                                //当两个传感器有一个在线上时
   {
     if( PIND.2==1 )
        find=0;

      if(PIND.3==1)
         find=0;
   } ;

    MBgo ;
    MAgo;

    TCCR2  = 0x0A;                                               //开寻线

    delay_ms(5000);
                                                                          //5秒后加速


    delay_ms(5000);

                                                                           //关寻线
    TCCR2  = 0x00;

    stopCPA ;
     stopCPB;
    MBgo ;                                                              //梦幻舞步
    MAback;
    startCPA  ;
    startCPB  ;

    find=1;
    delay_ms(10000);

       while(find)                                                //当两个传感器有一个在线上时
   {
     if( PIND.2==1 )
        find=0;

      if(PIND.3==1)
         find=0;
  } ;

    MBgo ;                                                              //梦幻舞步
    MAgo;
     TCCR2  = 0x0A;
                                                    //开寻线
     delay_ms(8000);

//***********************************************************************
//反向转弯
//***********************************************************************
      TCCR2  = 0x00;

     stopCPA ;
     stopCPB;
    MBback ;                                                              //梦幻舞步
    MAgo;
    delay_ms(1000);
    startCPA  ;
    startCPB  ;

    find=1;
    delay_ms(9000);

       while(find)                                                //当两个传感器有一个在线上时
   {
     if( PIND.2==1 )
        find=0;

      if(PIND.3==1)
         find=0;
  } ;

    MBgo ;                                                              //梦幻舞步
    MAgo;
     TCCR2  = 0x0A;


}

void main ( void )
{
 /*******************************************************
变量定义
 *******************************************************/
  unsigned char  key;


/********************************************************
初始化设备
*********************************************************/
  Init_IO( );
  Init_T0(  );
  Init_T1(  );
  Init_T2(  );
  Init_TWI( );
  Init_device( ) ;
  LCD_init( );

  delay_ms( 10 );
  LCD_clear( );
  delay_ms(1);

  /********************************************************
  模式选择
  ********************************************************/
  key=SelectMode( );

switch (key)
    {
    case 0:
                 nomal (  );
                 break;
     case 1:
                 advance ( );
                   break;
      case 2:
                  demo( );
                  break;
    default:
                   break;
    };


  while(1);                                             //结束
}
/*********************************************************************************************/
/********************以下是对ATmega16的自带I2C定义和使用**************************************/
/*********************************************************************************************/
#include "config.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
#include "TWI.h"

/*
extern void Init_TWI(void) ;  															//TWI初始化
extern uchar TWI_Send(uchar Slave,uchar Address,uchar Data);                                 				//向IIC地址为Slave的从机的Address地址发送(写)一字节数据Data
extern uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT) ;                                	//向IIC地址为Slave的从机的Address地址开始发送N字节数据Data
extern uchar TWI_Receive(uchar Slave,uchar Address) ;                                                                          //从IIC地址为Slave的从机的Address地址读取一字节数据Data，返回值为读取的数据
*/
//************TWI初始化*************/




/****************************************************
*****************************************************
向IIC地址为Slave的从机的Address地址发送(写)一字节数据Data
返回0:写成功
返回非0:写失败
******************************************************
******************************************************/
uchar TWI_Send(uchar Slave,uchar Address,uchar Data)
{

	TWCR=0x80|0x20|0x04;                                             //或:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);对TWINT写1清除，使能TWI，发出Start信号
	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Start信号已发出
	if((TWSR&0xf8)!=0x08)                                            //或:if ((TWSR & 0xF8) != START),检测状态寄存器,屏蔽预分频位,如果状态字不是START转出错处理
	return(1);                                                       //返回值1，表明从机没有对Start信号作应答

                                                                 //Slave即SLA+W,即是从机地址 ,可以是0x18或0x20,此处只用0x18
                                                                 //所谓的应答是“从器件”在收到地址和“写”后，将SDA电压拉低，由“主器件”读取
                                                                 //ATmge16如果读到这个“低”电压则返回0x18,否则返回0x20。
	TWDR=Slave;                                                      //更新地址寄存器,装Slave入到TWDR寄存器
	TWCR=0x80|0x04;                                                  //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出从机地址信息
	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位,表明Slave信号已发出
	if((TWSR&0xf8)!=0x18)                                            //或:if ((TWSR & 0xF8) != MT_SLA_ACK),检测状态寄存器,
	return(2);                                                       //返回值2，表明从机没有对Slave信号作应答

                                                                 //Address是MT_SLA_ACK是否控发送器地址,可以是0x28或0x30, 此处只用0x28
	TWDR=Address;
	TWCR=0x80|0x04;                                                  //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出寄存器地址信息
	while((TWCR&0x80)!=0x80) ;                                      //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Address信号已发出
	if((TWSR&0xf8)!=0x28)                                           //检测状态寄存器(MT_DATA_ACK)
	return(3);                                                      //返回值3，表明从机没有对Address信号作应答

	TWDR=Data;                                                      //写数据到ZLG7290
	TWCR=0x80|0x04;                                                 //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出数据信息
	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Data信号已发出
	if((TWSR&0xf8)!=0x28)                                           //if ((TWSR & 0xF8) != MT_DATA_ACK);检测状态寄存器
	return(4);                                                      //返回值3，表明从机没有对Data信号作应答

	TWCR=0x80|0x04|0x10;                                            //或:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);对TWINT写1清除，使能TWI，发出Stop信号
	return(0);

}





/******************************************************************
*********************************************************************
向IIC地址为Slave的从机的Address地址开始发送N字节数据Data
******************************************************************
*******************************************************************/

/*uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT)
 {
    uchar Count;
    TWCR=0x80|0x20|0x04;                                                      //对TWINT写1清除，使能TWI，发出Start信号
    while((TWCR&0x80)!=0x80) ;                                                //等待TWINT置位，表明Start信号已发出
    if((TWSR&0xf8)!=0x08)                                                     //检测状态寄存器
    return(1);                                                                //返回值1，表明从机没有对Start信号作应答

    TWDR=Slave;                                                               //更新地址寄存器
    TWCR=0x80|0x04;                                                           //对TWINT写1清除，使能TWI；发出从机地址信息
    while((TWCR&0x80)!=0x80) ;                                                //等待TWINT置位，表明Slave信号已发出
    if((TWSR&0xf8)!=0x18)                                                     //检测状态寄存器
    return(2);                                                                //返回值2，表明从机没有对Slave信号作应答

    TWDR=Address;
    TWCR=0x80|0x04;                                                            //对TWINT写1清除，使能TWI；发出寄存器地址信息
    while((TWCR&0x80)!=0x80) ;                                                 //等待TWINT置位，表明Address信号已发出
    if((TWSR&0xf8)!=0x28)                                                      //检测状态寄存器
    return(3);                                                                 //返回值3，表明从机没有对Address信号作应答

    for(Count=0;Count<CNT;Count++)                                             //连续写N个字节 ,实现发送N字节数据Data
       {
		 TWDR=Array[Count];                                                    //建立数组装载TWDR
		 TWCR=0x80|0x04;                                                       //或(1<<TWINT)|(1<<TWEN);对TWINT写1清除，使能TWI；发出数据信息
		 while((TWCR&0x80)!=0x80) ;                                            //等待TWINT置位，表明Data信号已发出
		 if((TWSR&0xf8)!=0x28)                                                 //检测状态寄存器
		 return(4);
	   }                                                                                 //返回值4，表明从机没有对Data信号作应答

		TWCR=0x80|0x04|0x10;                                                  //对TWINT写1清除，使能TWI，发出Stop信号
		return(0);
 }

 */
/**********************************************************************
***********************************************************************
从IIC地址为Slave的从机的Address地址读取一字节数据Data，返回值为读取的数据
//以下为主机接收模式
/*在主机接收模式，主机可以从从机接收数据，为进入主机模式，必须发送START信号。
紧接着的地址包格式决定进入MT或MR模式。 如果发送 SLA+W 进入MT模式；如果发送SLA+R则进入MR模式。

************************************************************************
***********************************************************************/
uchar TWI_Receive(uchar Slave,uchar Address)
{//发送Start

	uchar Temp;
	TWCR=0x80|0x20|0x04;                                                         //或:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);对TWINT写1清除；使能TWI；发出Start信号
	while((TWCR&0x80)!=0x80) ;													 //等待TWINT置位，表明Start信号已发出
	if((TWSR&0xf8)!=0x08) 														 //检测状态寄存器,{0x08表示:主机的TWSR状态码(在预分频位为"0"情况下)}
	return(1); 																	 //返回值1，表明从机没有对Start信号作应答
//发送Slave＋W
	TWDR=Slave; 																//更新地址寄存器
	TWCR=0x80|0x04;																 //对TWINT写1清除，使能TWI；发出从机地址信息
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Slave信号已发出
	if((TWSR&0xf8)!=0x18) 														//检测状态寄存器,(0x18表示:SLA+W从机已发送,接收到ACK)
	return(2); 																	//返回值2，表明从机没有对Slave信号作应答
//发送Address
	TWDR=Address;
	TWCR=0x80|0x04; 															//对TWINT写1清除，使能TWI；发出寄存器地址信息
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Address信号已发出
	if((TWSR&0xf8)!=0x28) 														//检测状态寄存器,(0x28表示:主控机地址已发送,接收到ACK)
	return(3); 																	//返回值3，表明从机没有对Address信号作应答

//上面三段程序定义是主机方式后,以下就是接收信号的定义

//发送Start,重新启动TWI
	TWCR=0x80|0x20|0x04; 														//对TWINT写1清除；使能TWI；发出Start信号
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Start信号已发出
	if((TWSR&0xf8)!=0x10)
	return(4);																			/*检测状态寄存器,在Repeat Start (状态0x10) 后，两线接口可以再次访问
																				相同的从机，或不发送STOP信号来访问新的从机。REPEATED START使得主机
																				可以在不丢失总线控制的条件下在从机、主机发送器及主机接收器模式间进行切换。*/
																				//返回值4，表明从机没有对Repeat Start信号作应答
//发送Slave＋R
	TWDR=Slave+1; 																//更新地址寄存器
	TWCR=0x80|0x04; 															//对TWINT写1清除，使能TWI；发出从机地址信息
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Slave信号已发出
	if((TWSR&0xf8)!=0x40) 														//检测状态寄存器,(0x40表示:SLA+R(MR_SLA_ACK)已发送,接收到ACK)
	return(5); 																	//返回值5，表明从机没有对Slave+R信号作应答

	TWCR=0x80|0x04;																 //对TWINT写1清除，使能TWI；接受数据
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明数据已经接收接受
	if((TWSR&0xf8)!=0x50) ; 													//检测状态寄存器,(0x50表示:接收到数据,ACK已返回)
// return(6); 																		//返回值3，表明从机没有对Address信号作应答

	Temp=TWDR; //读接收数据

	TWCR=0x80|0x04; 															//发出NACK信号

	TWCR=0x80|0x04|0x10; 														//发出Stop信号,或:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	delay_ms(1);
	TWCR=0x80;																	 //清除TWINT和禁止TWI(不加上这句程序只能读一次)
	return(Temp);

}

#include  "config.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

/*****************************************************
IO初始化
*****************************************************/
void Init_IO ( void )
{

// PA接液晶用，状态输入，开启上拉，用7个口,剩PA0 接确认键。

PORTA=0xFE;
DDRA=0xFE;

PORTB=0x00;
DDRB=0x00;

/******************************************************************************************************
 PC0(SCL)  PC1(SDA) PC7( )
******************************************************************************************************/
PORTC=0xF7;
DDRC=0x07;
/*******************************************************************************************************
PD0 (B电机正反转)   PD1(A电机正反转 )      PD2(传感器D)  PD3(传感器C)
PD4(CPB)       PD5(CPA)     PD6(传感器B)       PD7(传感器A)
********************************************************************************************************/
//PORTD=0XCC;
 PORTD=0X00;
DDRD=0X33;

}
/*******************************************************************
T1初始化，输出两路脉冲。未开中断，CTC模式
OCR1A=0x00FF;OCR1B=0x00FF;
*******************************************************************/
void  Init_T1(  void )
{
 TCCR1A=0x00;
 TCCR1B=0x0A;

 OCR1AH=0x01;
 OCR1AL=0xFF;

 OCR1BH=0x01;
 OCR1BL=0xFF;
}
/*********************************************************************
T0初始化
*********************************************************************/
void Init_T0( void )
{
  TCCR0  = 0x00;//停止定时器
  TCNT0  = 0x00;//初始值
  OCR0   = 0x79;//匹配值
  TIMSK |= 0x02;//中断允许
  TCCR0  = 0x0D;//启动定时器
}
/***********************************************************************
T2初始化
***********************************************************************/
 void Init_T2( void )
 {
   TCCR2  = 0x00;//停止定时器
   ASSR   = 0x00;//异步时钟模式
   TCNT2  = 0x00;//初始值
   OCR2   = 0x3E;//匹配值
   TIMSK |= 0x80;//中断允许
   TCCR2  = 0x00;//启动定时器
 }

void Init_device( void )
{

MCUCR=0x00;
MCUCSR=0x80;

// Timer(s)/Counter(s) Interrupt(s) initialization

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

}

void Init_TWI(void)
 {
	TWCR= 0x00;                                   //禁止TWI
	TWBR= 0x15;                                      //波特率15
                                               //当电压为5V时, 上拉电阻为10K时, SCL和SDA的一个时钟周期为10us; 上拉电阻为1K时, SCL和SDA的一个时钟周期为2.5us;
                                               //TWBR值需要仔细调节以配合ZLG7290的IIC频率要求
	TWSR= 0x01;                                    //预分频系数1,或(0<<TWPS1)|(0<<TWSP0);
	                                                                   //IIC从机地址SLAVE=0xa0
	TWCR= 0x04;                                    //使能TWI，禁止中断,即:TWCR =(1<<TWEN),
                                               //禁止中断即TWINT被清零，TWI立即开始工作,因此，在
                                               //清零TWINT之前一定要首先完成对地址寄存器TWAR，
                                               //状态寄存器TWSR，以及数据寄存器TWDR的访问。
 }
/*
	LCD1602液晶屏四线驱动模块
	1、可直接嵌入到项目中使用
	2、晶振频率：1M
	3、如晶振提高低层驱动延时要作相应修改
	4、程序在原来基础上只使用了PB口的高四位，即PB4—PB7，PB0—PB3可以做它用

	SunnyTrip
	www.iccavr.com
*/

#include "config.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

#define RS_CLR	PORTA.1=0
#define RS_SET	PORTA.1=1

#define RW_CLR	PORTA.2=0
#define RW_SET	PORTA.2=1

#define EN_CLR	PORTA.3=0
#define EN_SET	PORTA.3=1


void LCDsetxy(unsigned char x , unsigned char y)
 {
  	 if (y == 0)
 	 	{
    	 LCD_write_com(0x80 + x);
    	}
    else
		{
    	 LCD_write_com(0xC0 + x);
    	}

 }

/*显示屏命令写入函数*/
void LCD_write_com(unsigned char com) {

	RS_CLR;
	RW_CLR;
	EN_SET;
	PORTA&=0x0f;
	PORTA|= com&0xf0;
	com = com<<4;
	EN_CLR;
	delay_us( 10);
	EN_SET;
	PORTA&=0x0f;
	PORTA|= com&0xf0;
	EN_CLR;
	delay_us(10);
}

/*显示屏命令写入函数*/
void LCD_write_data(unsigned char data) {

	RS_SET;
	RW_CLR;
	EN_SET;
	PORTA&=0x0f;
	PORTA|= data&0xf0;
	data = data<<4;
	EN_CLR;
	delay_us( 10);
	EN_SET;
	PORTA&=0x0f;
	PORTA|= data&0xf0;
	EN_CLR;
}

/*显示屏清空显示*/
void LCD_clear(void) {

	LCD_write_com(0x01);
	delay_ms( 15);
}

/*显示屏字符串写入函数*/
void LCD_write_str(unsigned char x,unsigned char y,unsigned char flash *s) {

    uchar i=0;
    LCDsetxy( x ,  y);
    while (*s) {
    	LCD_write_data(*s);
    	i++;
    	s ++;
    	}
}

/*显示屏单字符写入函数*/
void LCD_write_char(unsigned char x,unsigned char y,unsigned char data) {

    LCDsetxy( x ,  y);

    LCD_write_data( data);
}
/*void CursorScroll(void)
{

    LCD_write_com(0x10);	//光标左移

}
void ScreenScroll(void)
{

	LCD_write_com(0x18);		//画面左滚
	delay_ms(255);

} */

/*显示屏初始化函数*/
void LCD_init(void)
 {

							/*I/O口方向设置*/
	LCD_write_com(0x28);				/*显示模式设置*/
	delay_ms( 15);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);

	LCD_write_com(0x08);				/*显示关闭*/
	LCD_write_com(0x01);				/*显示清屏*/
	LCD_write_com(0x06);				/*显示光标移动设置*/
	delay_ms( 5);
	LCD_write_com(0x0c);				/*显示开及光标设置*/
}
#include  "config.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

#define    sec    0X02
#define   miu    0X03
#define   com   0X00

 /* uchar Read_PCF8563T(uchar Address)
 {
   uchar temp;
   temp=TWI_Receive(0xA2, Address);
   return temp;
 }  */

  void Write_PCF8563T(uchar Address , uchar date)
 {

  TWI_Send(0xA2, Address, date);

 }
void start_PCF( void )
{
 Write_PCF8563T( com , 0x00 );
 delay_ms(2);
 Write_PCF8563T( sec , 0x00 );
 delay_ms(2);
 Write_PCF8563T( miu , 0x00 );
}

uchar Read_sec( void )
{
   uchar temp;
   temp=TWI_Receive(0xA2, sec);
   return temp;
}
uchar Read_miu( void )
{
   uchar temp;
   temp=TWI_Receive(0xA2, miu);
   return temp;
}
