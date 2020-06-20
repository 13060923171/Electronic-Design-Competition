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
