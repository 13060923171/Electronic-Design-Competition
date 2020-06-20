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
//δʹ�õĺ�������ע�͵�,ͬʱ��ͷ�ļ���ĺ�������ע�͵�
  /* ADC */
/* Analog Comparator */
/* USART */
/* SPI */
/* EEPROM */
/* Watchdog Timer Control Register */
/* Timer/Counter 2 */
/* Timer/Counter 1 */
/* Special Function I/O register */
/* Timer/Counter 0 */
/* MCU general */
/* SPM Conrol Register */
/* Timer/Counter Interrupts */
/* General Interrupts */
/* Port A bits */
/* Port B bits */
/* Port C bits */
/* Port D bits */
/* Lock and Fuse Bits with LPM/SPM instructions */
/* lock bits */
/* fuses low bits */
/* fuses high bits */
   typedef unsigned char  uchar;			/* �������ֲ���޷���8λ�����ؼ���    */
typedef signed   char  schar;			/* �������ֲ���з���8λ�����ؼ���    */
typedef unsigned int   uint;			/* �������ֲ���޷���16λ�����ؼ���   */
typedef signed   int   sint;			/* �������ֲ���з���16λ�����ؼ���   */
typedef unsigned long  ulong;			/* �������ֲ���޷���32λ�����ؼ���   */
typedef signed   long  slong;			/* �������ֲ���з���32λ�����ؼ���   */
// CodeVisionAVR C Compiler
// (C) 1998-2001 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega16
#pragma used+
sfrb TWBR=0;
sfrb TWSR=1;
sfrb TWAR=2;
sfrb TWDR=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      // 16 bit access
sfrb ADCSRA=6;
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRRL=9;
sfrb UCSRB=0xa;
sfrb UCSRA=0xb;
sfrb UDR=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   // 16 bit access
sfrb UBRRH=0x20;
sfrb UCSRC=0X20;
sfrb WDTCR=0x21;
sfrb ASSR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  // 16 bit access
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  // 16 bit access
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  // 16 bit access
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb SFIOR=0x30;
sfrb OSCCAL=0x31;
sfrb OCDR=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TWCR=0x36;
sfrb SPMCR=0x37;
sfrb TIFR=0x38;
sfrb TIMSK=0x39;
sfrb GIFR=0x3a;
sfrb GICR=0x3b;
sfrb OCR0=0X3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-
// Interrupt vectors definitions
// Needed by the power management functions (sleep.h)
#asm
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
#endasm
extern unsigned char TWI_Send(uchar Slave,uchar Address,uchar Data);
//extern unsigned char TWI_SendStr(unsigned char Slave,unsigned char Address,unsigned char *Array,unsigned char CNT);
extern unsigned char TWI_Receive(unsigned char Slave,unsigned char Address);
// CodeVisionAVR C Compiler
// (C) 1998-2000 Pavel Haiduc, HP InfoTech S.R.L.
#pragma used+
void delay_us(unsigned int n);
void delay_ms(unsigned int n);
#pragma used-
extern  void Init_IO ( void );
extern  void  Init_T1(  void );
extern  void Init_T0( void );
extern  void Init_T2( void );
extern  void Init_device( void ) ;
extern  void Init_TWI(void); 
//extern    unsigned char Read_PCF8563T(unsigned char Address);
extern    void Write_PCF8563T(unsigned char Address , char date);
extern   void start_PCF( void );
extern    char Read_sec( void);
extern   char Read_miu( void );
void LCD_init(void);
void LCD_clear(void);
void LCD_write_str(unsigned char X,unsigned char Y,unsigned char flash *s);
void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data);
void LCD_write_com(unsigned char com);
//void CursorScroll(void);  
//void ScreenScroll(void);
/**************************************************** 
ȫ�ֱ�������
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
ϵͳ�ж�    ����ʾ��������
*******************************************************/
 interrupt [20 ] void tim0_comp_isr(void)  
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
  interrupt [4 ] void tim2_comp_isr(void)  
 { 
     if(PORTD.1==1) 
       { 
                   if(PIND.6==1) 
                         TCCR1A|=0X40;  
                               else 
                                      TCCR1A&=0XBF;
                                             if(PIND.7==1)
                             TCCR1A|=0X10;
                                else        
                                    TCCR1A&=0XEF;     
                              }   
      else
        {
           if(PIND.2==1) 
                         TCCR1A|=0X40;  
                                    else 
                                        TCCR1A&=0XBF;
                                              if(PIND.3==1)
                          TCCR1A|=0X10;
                             else        
                                      TCCR1A&=0XEF;     
         }      
 }  
 /**********************************************************  
 ��ȡadֵ
 ***********************************************************/
 unsigned char get_ad(void) {
	unsigned char i;
	 ADMUX = 0x60;			              /*��׼AVCC������롢ͨ��0*/
                  ADCSRA = 0xC2; 
                  			                               /*ʹ�ܡ�������4��Ƶ*/
	while(!(ADCSRA & (1 << 4)));	                     /*�ȴ�*/
	i = ADCH;
	ADCSRA &= ~(1 << 4);		              /*���־*/
	ADCSRA &= ~(1 << 7);		              /*�ر�ת��*/
	return i;
}   
/*********************************************************************
����������
*********************************************************************/
void beep( void )
{
   PORTC.2=0;
   delay_ms(500);
   PORTC.2=1;   
}
 /**********************************************************************
 ����adֵѡ��ƽ�����ģʽ
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
        PORTD.1=0;
        PORTD.0=0;
      }
      else
      {
       PORTD.1=1;
       PORTD.0=1;
      }
           TCCR1A|=0X40 ;
      TCCR1A|=0X10 ; 
            delay_ms( go_time ) ;
           TCCR1A&=0XBF ;
      TCCR1A&=0XEF;
              if( vula_adc<0x7F)
      {
       PORTD.1=1;
       PORTD.0=1;
      }
      else
      {
        PORTD.1=0;
        PORTD.0=0;
      }
            TCCR1A|=0X40 ;
      TCCR1A|=0X10 ;
            delay_ms( back_time ) ;   
             TCCR1A&=0XBF ;
      TCCR1A&=0XEF;
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
                                                                      TCCR1A&=0XBF ;
                                  TCCR1A&=0XEF;
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
    beep(  );
    start_PCF( );
/********************************************************
  С��������ʼ��
********************************************************/  
    PORTD.0=0 ;  
    PORTD.1=0;   
    OCR1AH=0x02; 
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x02; 
    OCR1BL=0xFF;
    TCCR1A|=0X40 ;
     TCCR1A|=0X10 ;  
    delay_ms(2000);  
    OCR1AH=0x01; 
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x01; 
    OCR1BL=0xFF; 
    TCCR2  = 0x0A;                                   //�Զ�Ѱ�߿�
        #asm("sei");                                                  //ȫ���жϿ�   
    /**********************************************
    ��һ�׶�
    **********************************************/
   while ( Timer[0] != 0X14 ) ;
   LCD_write_str( 0 ,  1 , step[1] ); 
     #asm("cli"); 
   beep(  );
   start_PCF( ); 
   #asm("sei");
   /************************************************
   Ѱ��ƽ��
   ************************************************/    
   TCCR2  = 0x00;                                      //Ѱ�߹�
    Findbenlen(  );
    PORTC.2=0;
    flage+=0x80;                                        //�����������жϹر�   
      /************************************************
   �ҵ�ƽ��
   ************************************************/
   #asm("cli"); 
   start_PCF( );                                                     //��Ҫ�Ķ�
   LCD_write_str( 0 ,  1 , step[2] );
   LCD_write_str( 6 ,  1 , "Balan:"  ); 
   flage+=0x04;                                                      //��ʾƽ�� 
    #asm("sei");  
        do  
       {   
        if( Timer[0]==0X05 )
            {
                         flage -=0x04; 
                         #asm("cli");
                         start_PCF( ); 
                         #asm("sei");
            }
       }
   while ( (flage==0X04 )|| ( flage==0X84  ) );  
          /***************************************************
   �ı��ٶ�
   ***************************************************/
   #asm("cli");
   OCR1AH=0x01; 
   OCR1AL =0xFF;                                                       //new
   OCR1BH=0x01; 
   OCR1BL=0xFF; 
   #asm("sei"); 
       PORTD.0=0;
    PORTD.1=0;     
    TCCR1A|=0X40 ;
    TCCR1A|=0X10 ; 
  /************************************************
   ���ߣ�ֱ����������֪ľ������
  ************************************************/ 
  TCCR2  = 0x0A; 
   /********************************************** 
   ��⴫����״̬��û����ʱͣ��
   ***********************************************/ 
  while(PIND.2||PIND.3);                                           //��Ѱ��   
               #asm("cli");  
  TCCR1A&=0XBF ;
  TCCR1A&=0XEF; 
  TCCR2  = 0x00;  
  beep(  ); 
  start_PCF( ); 
  delay_us(10);
  #asm("sei");
  /***********************************************
   �ȴ����룬��������
  ***********************************************/  
  PORTD.0=1;
  PORTD.1=1;
  while( Timer[0]!=0X05 )  ; 
  LCD_write_str( 0 ,  1 , step[3] );
  beep(  );  
   OCR1AH=0x02; 
  OCR1AL =0xFF;                                                       //new
  OCR1BH=0x02; 
  OCR1BL=0xFF;
    TCCR1A|=0X40 ;
  TCCR1A|=0X10 ;
    TCCR2  = 0x0A;  
   delay_ms(2000);  
     OCR1AH=0x01; 
  OCR1AL =0xFF;                                                       //new
  OCR1BH=0x01; 
  OCR1BL=0xFF; 
    while(PIND.6||PIND.7);  
   beep( );  
   LCD_write_str( 0 ,  1 , step[4] );   
       #asm("cli"); 
    TCCR1A&=0XBF ;
  TCCR1A&=0XEF;  
 }        
   /******************************************************
  ���Ӳ���
  ******************************************************/    
void advance ( void )
{
   unsigned char find=1;
   unsigned char j=0; 
         LCD_write_str( 0 , 0 ,"min:  sec: "); 
      start_PCF( );
        PORTD.0=0 ;  
   PORTD.1=0;
   TCCR1A|=0X40  ;
   TCCR1A|=0X10  ;
        #asm("sei");  
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
    TCCR2  = 0x0A;
  /***************************************************** 
 һ��ʱ��󣬼�����ƽ��
 *****************************************************/    
    while( Timer[0]<=0X30  );
    while ( get_ad( ) < 0X7F ) ; 
       TCCR2  = 0x00; 
 /**************************************************** 
 �ҵ��󣬸���ƽ��ָʾ��
 *****************************************************/   
  do{ 
        Findbenlen(  );
        PORTC.2=0;
        flage+=0x80;
                 #asm("cli");
         LCD_write_str( 0 ,  1 ,display[j] ); 
         LCD_write_str( 6 ,  1 , "Balan:"  ); 
         flage+=0x04;
         #asm("sei");
/****************************************************** 
���ƽ��״̬����ƽ��ʱ����Ѱ��
*******************************************************/    
          while( get_ad( ) < 0X81 &&  get_ad( )>  0X7B );
          flage -=0x04; 
          BalanceTime=0;                                                  //ƽ����ʾ����                                              
          LCD_write_str( 0,  1 , "                                   "  );  
         if(j<5)
          j++; 
          else 
          j=0;
     }
    while(1);      
    /*******************************************************
�ҵ��󣬸���ƽ��ָʾ
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
    flage -=0x04;                                                         //�ر�ƽ����ʾ                                       
    TCCR0  = 0x00;                                                     //�ر���ʾ     
  
    MBgo ;
    MAback ;
    OCR1AH=0x02; 
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x02; 
    OCR1BL=0xFF; 
    startCPA  ;
    startCPB  ;
    delay_ms( 4000 );                                                  //�ٶȼ���һ������ʱ����һ��        
     find=1;
      LCD_write_str( 0 ,  1 ,display[2] ); 
    
     while(find)                                                //��������������һ��������ʱ
   {
     if( PIND.2==1 )  
        find=0;  
        
      if(PIND.3==1)  
         find=0;  
   } ;   
   
    MBgo ;
    MAgo;  
    
    TCCR2  = 0x0A;                                               //��Ѱ��
                            
    delay_ms(5000);
                                                                          //5������
    OCR1AH=0x01; 
    OCR1AL =0xFF;                                                       //new
    OCR1BH=0x01; 
    OCR1BL=0xFF;  
     
    delay_ms(5000);  
     
                                                                           //��Ѱ��
    TCCR2  = 0x00;
    
    stopCPA ;
     stopCPB; 
    MBgo ;                                                              //�λ��貽
    MAback;    
    startCPA  ;
    startCPB  ;
   
    find=1;
    delay_ms(6000);   
     
       while(find)                                                //��������������һ��������ʱ
   {
     if( PIND.2==1 )  
        find=0;  
        
      if(PIND.3==1)  
         find=0;  
  } ;   
  
    MBgo ;                                                              //�λ��貽
    MAgo;  
     TCCR2  = 0x0A; 
                                                    //��Ѱ��  
     delay_ms(5000);   
      
//***********************************************************************
����ת��
//***********************************************************************    
      TCCR2  = 0x00;
    
     stopCPA ;
     stopCPB; 
    MBback ;                                                              //�λ��貽
    MAgo;   
    delay_ms(1000); 
    startCPA  ;
    startCPB  ;
   
    find=1;
    delay_ms(6000);   
     
       while(find)                                                //��������������һ��������ʱ
   {
     if( PIND.2==1 )  
        find=0;  
        
      if(PIND.3==1)  
         find=0;  
  } ;   
  
    MBgo ;                                                              //�λ��貽
    MAgo;  
     TCCR2  = 0x0A; 
     
  **************************************************************************/                                                   
}    
void demo( void )
{
       unsigned char find=1;
       LCD_write_str( 0 , 0 ,"min:  sec: "); 
       start_PCF( );
         PORTD.1=0 ;  
    PORTD.0=1;
    TCCR1A|=0X40  ;
    TCCR1A|=0X10  ;
        #asm("sei");  
     find=1;
         while(find)                                                //��������������һ��������ʱ
   {
     if( PIND.2==1 )  
        find=0;  
              if(PIND.3==1)  
         find=0;  
   } ;   
       PORTD.0=0 ;
    PORTD.1=0;  
        TCCR2  = 0x0A;                                               //��Ѱ��
                                delay_ms(5000);
                                                                          //5������
            delay_ms(5000);  
                                                                                //��Ѱ��
    TCCR2  = 0x00;
        TCCR1A&=0XBF ;
     TCCR1A&=0XEF; 
    PORTD.0=0 ;                                                              //�λ��貽
    PORTD.1=1;    
    TCCR1A|=0X40  ;
    TCCR1A|=0X10  ;
       find=1;
    delay_ms(10000);   
            while(find)                                                //��������������һ��������ʱ
   {
     if( PIND.2==1 )  
        find=0;  
              if(PIND.3==1)  
         find=0;  
  } ;   
      PORTD.0=0 ;                                                              //�λ��貽
    PORTD.1=0;  
     TCCR2  = 0x0A; 
                                                    //��Ѱ��  
     delay_ms(8000);   
      //***********************************************************************
//����ת��
//***********************************************************************    
      TCCR2  = 0x00;
         TCCR1A&=0XBF ;
     TCCR1A&=0XEF; 
    PORTD.0=1 ;                                                              //�λ��貽
    PORTD.1=0;   
    delay_ms(1000); 
    TCCR1A|=0X40  ;
    TCCR1A|=0X10  ;
       find=1;
    delay_ms(9000);   
            while(find)                                                //��������������һ��������ʱ
   {
     if( PIND.2==1 )  
        find=0;  
              if(PIND.3==1)  
         find=0;  
  } ;   
      PORTD.0=0 ;                                                              //�λ��貽
    PORTD.1=0;  
     TCCR2  = 0x0A; 
}      
void main ( void )
{
 /*******************************************************
��������
 *******************************************************/ 
  unsigned char  key;       
    /******************************************************** 
��ʼ���豸
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
  ģʽѡ��
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
       while(1);                                             //����
}
