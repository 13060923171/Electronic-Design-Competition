/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.6c Professional
Automatic Program Generator
?Copyright 1998-2006 Pavel Haiduc, HP  vula_adcfoTech s.r.l.
http://www.hp vula_adcfotech.com

Project : С��
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


#define                  startCPA    TCCR1A|=0X40
#define   startCPB    TCCR1A|=0X10

#define   stopCPA     TCCR1A&=0XBF
#define   stopCPB     TCCR1A&=0XEF

#define   MBgo          PORTD.0=0
#define   MBback       PORTD.0=1

#define   MAgo          PORTD.1=0
#define   MAback       PORTD.1=1
/**************************************************** 
ȫ�ֱ�������
*****************************************************/
volatile unsigned char  flage=0X00;
volatile unsigned char Timer[3]={0,0,0};
volatile unsigned char  write;
volatile unsigned char BalanceTime=0;  
unsigned char balan =1;    
unsigned char  vula_adc;   
unsigned char  ad;
 flash char *step[4]={ "stepA","stepB","stepC","back " } ;
 flash char *mode[2]={"normal","advanc"};
/****************************************************** 
ϵͳ�ж�    ����ʾ��������
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
 ϵͳ�ж�T2     (Ѱ��У��)
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
 
 unsigned char get_ad(void) {

	unsigned char i;
	 ADMUX = 0x60;			              /*��׼AVCC������롢ͨ��0*/
                  ADCSRA = 0xC2;			                    /*ʹ�ܡ�������4��Ƶ*/
	while(!(ADCSRA & (1 << ADIF)));	                     /*�ȴ�*/
	i = ADCH;
	ADCSRA &= ~(1 << ADIF);		/*���־*/
	ADCSRA &= ~(1 << ADEN);		/*�ر�ת��*/

	return i;
}   

void beep( void )
{
   PORTC.2=0;
   delay_ms(500);
   PORTC.2=1;   
}

void Findbenlen( void )
{
    
    balan=1;
     do
       { 
        
          if( get_ad( ) <=0X79   )       //0x7d(yuan shi)  0X7E bux vula_adcg
               {
                        MBgo ;  
                         MAgo ; 
                        startCPA ;
                        startCPB ; 
                        delay_ms(200);  
                         stopCPA  ;
                        stopCPB  ;
                        }    
                         delay_ms(200);           
                if( get_ad( ) >=0X81   )       //0x7d(yuan shi)  0X7E bux vula_adcg
               {
                        MBback ;  
                         MAback ; 
                         startCPA ;
                        startCPB ; 
                        delay_ms(200);  
                        stopCPA  ;
                        stopCPB  ;
                        }     
                         delay_ms(400);          
            if( get_ad( ) >= 0X84)      //0x82
            {
                         MBback ;  
                         MAback ; 
                         startCPA ;
                        startCPB ; 
                        delay_ms(200);  
                        stopCPA  ;
                        stopCPB  ;
                        }        
                         delay_ms(400);  
             if( get_ad( ) <=0X82  )       //0x7d(yuan shi)  0X7E bux vula_adcg
               {
                        MBgo ;  
                         MAgo ; 
                        startCPA ;
                        startCPB ; 
                        delay_ms(200);  
                         stopCPA  ;
                        stopCPB  ;
                        }             
             delay_ms(400); 
                ad=get_ad( );
              if(  ad < 0X82 &&  ad >  0X79 )   
                balan=0;
                
        }    while(balan); 
        
        balan=1;                     
      
     do
       { 
        
          if( get_ad( ) <=0X7B   )       //0x7d(yuan shi)  0X7E bux vula_adcg
               {
                        MBgo ;  
                         MAgo ; 
                        startCPA ;
                        startCPB ; 
                        delay_ms(150);  
                         stopCPA  ;
                        stopCPB  ;
                        }             
    
            if( get_ad( ) >= 0X82)      //0x82
            {
                         MBback ;  
                         MAback ; 
                         startCPA ;
                        startCPB ; 
                        delay_ms(150);  
                        stopCPA  ;
                        stopCPB  ;
                        }   
       
             delay_ms(2000); 
                ad=get_ad( );
              if(  ad < 0X82 &&  ad >  0X7B )   
                balan=0;
                
        }    while(balan);                      
}

unsigned char SelectMode ( void )
{
  unsigned char temp=0;
  LCD_write_str( 0 , 0 ,"pass START key");  
  LCD_write_str( 0 , 1 ,"zhong bei da xue "); 
  //do
  //{
  //delay_ms( 250 );
  //}
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
          
             if(temp==2)
                temp=0;
                
               LCD_write_str( 6 , 0 ,mode[temp]);  
         }
         
        //delay_ms( 200 );  
      }  
     while(PINC.6==1);
     while(PINC.6==0);  
      LCD_clear( );
   return(temp);
}    

/*******************************************************
����Ҫ�󲿷�
*******************************************************/
void nomal ( void ) 
{
 /*****************************************************
 ��ʾʱ�ӣ���ʼ����
 *****************************************************/   
    LCD_write_str( 0 , 0 ,"minc:  sec: "); 
    LCD_write_str( 0 ,  1 , step[0] ); 
     delay_ms(1000);
    PORTC.2=0;
    delay_ms(500);
      PORTC.2=1;
   start_PCF( );
/********************************************************
  С��������ʼ��
********************************************************/  
    MBgo ;  
    MAgo;
    startCPA  ;
    startCPB  ;
    TCCR2  = 0x0A;                                   //�Զ�Ѱ�߿�
    
    SEI( );                                                  //ȫ���жϿ�   
    /**********************************************
    ��һ�׶�
    **********************************************/
   while ( Timer[0] != 0X15 ) ;
   LCD_write_str( 0 ,  1 , step[1] ); 
  
   CLI( ); 
   beep(  );
   start_PCF( ); 
   /***************************************************
   �ı��ٶ�
   ***************************************************/
   OCR1AH=0x06; 
   OCR1AL =0xFF;                                                       //new
   OCR1BH=0x06; 
   OCR1BL=0xFF; 
   SEI( ); 
   /************************************************
   Ѱ��ƽ��
   ************************************************/    
      TCCR2  = 0x00;   
   /********************************************************** 
      do
       {  
        startCPA ;
        startCPB ; 
        delay_ms(700); 
        stopCPA  ;
        stopCPB  ;  
         delay_ms(200); 
         if( get_ad( )>= 0X75 ) 
           balan=0; 
           
       }  while(balan); 
     
      balan=1;
      
     do
       { 
        
          if( get_ad( ) <=0X7B   )       //0x7d(yuan shi)  0X7E bux vula_adcg
               {
                        MBgo ;  
                         MAgo ; 
                        startCPA ;
                        startCPB ; 
                        delay_ms(300);  
                         stopCPA  ;
                        stopCPB  ;
                        }             
    
            if( get_ad( ) >= 0X82)      //0x82
            {
                         MBback ;  
                         MAback ; 
                         startCPA ;
                        startCPB ; 
                        delay_ms(300);  
                        stopCPA  ;
                        stopCPB  ;
                        }   
       
             delay_ms(1700); 
                ad=get_ad( );
              if(  ad < 0X82 &&  ad >  0X7B )   
                balan=0;
                
        }    while(balan);                      
    *******************************************************/      
    Findbenlen(  );
    PORTC.2=0;
    flage+=0x80;                                        //�����������жϹر�   
    
   /************************************************
   �ҵ�ƽ��
   ************************************************/
   CLI( ); 
   start_PCF( );                                                     //��Ҫ�Ķ�
   //Timer[2]= Read_sec( );
   LCD_write_str( 0 ,  1 , step[2] );
   LCD_write_str( 6 ,  1 , "Balan:"  ); 
   flage+=0x04;
    SEI( );  
    
    do  
       {   
       //����ƽ��
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
   �ı��ٶ�
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
   ���ߣ�ֱ����������֪ľ������
  ************************************************/ 
  TCCR2  = 0x0A; 
   /********************************************** 
   ��⴫����״̬��û����ʱͣ��
   ***********************************************/ 
  while(PIND.2||PIND.3);                                           //��Ѱ��   
          
   
  CLI( );  
  stopCPA ;
  stopCPB; 
  TCCR2  = 0x00;  
  beep(  ); 
  start_PCF( ); 
  delay_us(10);
  SEI( );
  /***********************************************
   �ȴ����룬��������
  ***********************************************/  
  MBback;
  MAback;
  while( Timer[0]!=0X05 )  ; 
  LCD_write_str( 0 ,  1 , step[3] );
  beep(  ); 
  startCPA ;
  startCPB ;  
  TCCR2  = 0x0A;  
   while(PIND.6||PIND.7);  
    beep(  ); 
 }        
 
  /******************************************************
  ���Ӳ���
  ******************************************************/    
void advance ( void )
{
   unsigned char find=1;
   
   
   LCD_write_str( 0 , 0 ,"min:  sec: "); 
   start_PCF( );
     
   MBgo ;  
   MAgo;
   startCPA  ;
   startCPB  ;
    
     
     SEI( );  
 /*****************************************************
����������������������ʱ����Ѱ�߹���
 *****************************************************/
   while(find)  
   {
     if( PIND.2==1 )  
        find=0;  
        
      if(PIND.3==1)  
         find=0;  
   } ;
    //while( (P vula_adcD.6|P vula_adcD.7)==0 );
 TCCR2  = 0x0A;
 
 /***************************************************** 
 һ��ʱ��󣬼�����ƽ��
 *****************************************************/    
    //while( Timer[0]!=0X40  );
   while ( get_ad( ) < 0X75 ) ;  //xiu gai
       TCCR2  = 0x00; 
 /**************************************************** 
 �ҵ��󣬸���ƽ��ָʾ��
 *****************************************************/   
   CLI( );
   OCR1AH=0x06; 
   OCR1AL =0xFF;                                                       //new
   OCR1BH=0x06; 
   OCR1BL=0xFF; 
   SEI( );  
   Findbenlen(  );
   PORTC.2=0;
   flage+=0x80;
     
        
   CLI( );
   LCD_write_str( 6 ,  1 , "Balan:"  ); 
   flage+=0x04;
   SEI( );
/****************************************************** 
���ƽ��״̬����ƽ��ʱ����Ѱ��
*******************************************************/    
    while( get_ad( ) < 0X82 &&  get_ad( )>  0X7B );
/*******************************************************
�ҵ��󣬸���ƽ��ָʾ
*******************************************************/     
   Findbenlen(  ); 
   PORTC.2=0;
   flage+=0x80;  
        
   CLI( );
   LCD_write_str( 6 ,  1 , "Balan:"  ); 
   flage+=0x04;
   SEI( );  
     
    MBgo ;
    MAback ;
    OCR1AH=0x01; 
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x01; 
    OCR1BL=0xFF; 
    startCPA  ;
    startCPB  ;
    delay_ms( 1000 );   
     find=1;
     
     while(find)  
   {
     if( PIND.2==1 )  
        find=0;  
        
      if(PIND.3==1)  
         find=0;  
   } ;   
    MBgo ;
    MAgo;
}          
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
      if(vula_adc <=0X74 )                      
                           return 9;  
     if( vula_adc >= 0X89 )       
                          return 10;                                                                                                         
}   

/**************************************************************************************************
ƽ���������ƺ���
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
void main ( void )
{
 /*******************************************************
��������
 *******************************************************/ 
  unsigned char  key;       
  unsigned char  temp_key;
  
/******************************************************** 
��ʼ���豸
*********************************************************/
  Init_IO( );
 // Init_T0(  );  
  Init_T1(  );
  //Init_T2(  );
  Init_TWI( );
  Init_device( ) ;      
  LCD_init( ); 
  
  delay_ms( 10 );
  LCD_clear( );   
  delay_ms(1);
    MBgo ;  
    MAgo;
    startCPA  ;
    startCPB  ;
  /********************************************************
  ģʽѡ��
  ********************************************************/
  do{
       vula_adc = get_ad ( );
     
     
      temp_key=vula_adc/16;
      LCD_write_char( 1 , 0 ,  temp_key+'0' );   
      temp_key=vula_adc%16;
      LCD_write_char( 2 , 0 ,  temp_key+'0' );  
       temp_key= vula_adc;
         key = output(  ) ;
      switch ( output(  ) ) {
                      case 0 : 
                                    
                                  stopCPA ;
                                  stopCPB;
                                 break;
                      case 1 :  
                                 
                                  mortorgo( 0x03,0xFF , 400, 350 ) ;
                                 break; 
                      case 2 :   
                                   
                                  mortorgo( 0x02,0xFF , 800, 700 ) ;
                                 break;
                      case 3 : 
                                 
                                 mortorgo( 0x02,0xFF , 900, 800 ) ;
                                 break;
                      case 4 :   
                                
                                mortorgo( 0x02,0xFF , 900, 700 ) ;
                                 break; 
                      case 5 :
                               
                                mortorgo( 0x02,0xFF , 900, 650 ) ;
                                 break; 
                      case 6 :  
                              
                                mortorgo( 0x02,0xFF , 900,600 ) ;
                                 break;  
                       case 7 :
                                  
                                 mortorgo( 0x02,0xFF , 1000, 700 ) ;
                                 break;   
                       case 8 :
                             
                                mortorgo( 0x02,0xFF , 1000,600 ) ; 
                                break;   
                       case 9 :
                                   MAgo;
                                   MBgo;
                                   OCR1AH=0x02; 
                                   OCR1AL = 0XFF;                                                      //new
                                   OCR1BH=0x02;
                                   OCR1BL= 0XFF;  
                                   startCPA ;
                                   startCPB ; 
                                break;  
                       case 10:
                                   MAback;
                                   MBback;
                                   OCR1AH=0x02; 
                                   OCR1AL = 0XFF;                                                      //new
                                   OCR1BH=0x02;
                                   OCR1BL= 0XFF;  
                                   startCPA ;
                                   startCPB ;                                                                            
                    default:  LCD_clear();  
                                LCD_write_char( 10 , 1 , 'E' );  
                                 break;
    };
       delay_ms(200);
  }
 
 while( 1 ); 
   
   // SEI();    
  // TCCR2  = 0x00;
   
   

  /*do
    {
    MBgo ;  
    MAgo;
    startCPA  ;
    startCPB  ;
    
    delay_ms(500);
    MBback ;  
    MAback;   
     delay_ms(500);
    MBgo ;  
    MAgo;  
    }
    while(1);    */
}
