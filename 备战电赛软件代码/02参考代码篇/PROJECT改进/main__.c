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
 /**********************************************************
 ��ȡadֵ
 ***********************************************************/
 unsigned char get_ad(void) {

	unsigned char i;
	 ADMUX = 0x60;			              /*��׼AVCC������롢ͨ��0*/
                  ADCSRA = 0xC2;
                  			                               /*ʹ�ܡ�������4��Ƶ*/
	while(!(ADCSRA & (1 << ADIF)));	                     /*�ȴ�*/
	i = ADCH;
	ADCSRA &= ~(1 << ADIF);		              /*���־*/
	ADCSRA &= ~(1 << ADEN);		              /*�ر�ת��*/

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
    TCCR2  = 0x0A;                                   //�Զ�Ѱ�߿�

    SEI( );                                                  //ȫ���жϿ�
    /**********************************************
    ��һ�׶�
    **********************************************/
   while ( Timer[0] != 0X14 ) ;
   LCD_write_str( 0 ,  1 , step[1] );

   CLI( );
   beep(  );
   start_PCF( );
   SEI( );
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
   CLI( );
   start_PCF( );                                                     //��Ҫ�Ķ�
   LCD_write_str( 0 ,  1 , step[2] );
   LCD_write_str( 6 ,  1 , "Balan:"  );
   flage+=0x04;                                                      //��ʾƽ��
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
  ���Ӳ���
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

         CLI( );
         LCD_write_str( 0 ,  1 ,display[j] );
         LCD_write_str( 6 ,  1 , "Balan:"  );
         flage+=0x04;
         SEI( );
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

    MAgo ;
    MBback;
    startCPA  ;
    startCPB  ;

    SEI( );

     find=1;

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
    delay_ms(10000);

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
     delay_ms(8000);

//***********************************************************************
//����ת��
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
    delay_ms(9000);

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
/*********************************************************************************************/
/********************�����Ƕ�ATmega16���Դ�I2C�����ʹ��**************************************/
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
extern void Init_TWI(void) ;  															//TWI��ʼ��
extern uchar TWI_Send(uchar Slave,uchar Address,uchar Data);                                 				//��IIC��ַΪSlave�Ĵӻ���Address��ַ����(д)һ�ֽ�����Data
extern uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT) ;                                	//��IIC��ַΪSlave�Ĵӻ���Address��ַ��ʼ����N�ֽ�����Data
extern uchar TWI_Receive(uchar Slave,uchar Address) ;                                                                          //��IIC��ַΪSlave�Ĵӻ���Address��ַ��ȡһ�ֽ�����Data������ֵΪ��ȡ������
*/
//************TWI��ʼ��*************/




/****************************************************
*****************************************************
��IIC��ַΪSlave�Ĵӻ���Address��ַ����(д)һ�ֽ�����Data
����0:д�ɹ�
���ط�0:дʧ��
******************************************************
******************************************************/
uchar TWI_Send(uchar Slave,uchar Address,uchar Data)
{

	TWCR=0x80|0x20|0x04;                                             //��:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);��TWINTд1�����ʹ��TWI������Start�ź�
	while((TWCR&0x80)!=0x80) ;                                       //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ������Start�ź��ѷ���
	if((TWSR&0xf8)!=0x08)                                            //��:if ((TWSR & 0xF8) != START),���״̬�Ĵ���,����Ԥ��Ƶλ,���״̬�ֲ���STARTת������
	return(1);                                                       //����ֵ1�������ӻ�û�ж�Start�ź���Ӧ��

                                                                 //Slave��SLA+W,���Ǵӻ���ַ ,������0x18��0x20,�˴�ֻ��0x18
                                                                 //��ν��Ӧ���ǡ������������յ���ַ�͡�д���󣬽�SDA��ѹ���ͣ��ɡ�����������ȡ
                                                                 //�AATmge16�������������͡���ѹ�򷵻�0x18,���򷵻�0x20��
	TWDR=Slave;                                                      //���µ�ַ�Ĵ���,װSlave�뵽TWDR�Ĵ���
	TWCR=0x80|0x04;                                                  //��:TWCR = (1<<TWINT) | (1<<TWEN);��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ
	while((TWCR&0x80)!=0x80) ;                                       //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ,����Slave�ź��ѷ���
	if((TWSR&0xf8)!=0x18)                                            //��:if ((TWSR & 0xF8) != MT_SLA_ACK),���״̬�Ĵ���,
	return(2);                                                       //����ֵ2�������ӻ�û�ж�Slave�ź���Ӧ��

                                                                 //Address��MT_SLA_ACK�Ƿ�ط�������ַ,������0x28��0x30, �˴�ֻ��0x28
	TWDR=Address;
	TWCR=0x80|0x04;                                                  //��:TWCR = (1<<TWINT) | (1<<TWEN);��TWINTд1�����ʹ��TWI�������Ĵ�����ַ��Ϣ
	while((TWCR&0x80)!=0x80) ;                                      //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ������Address�ź��ѷ���
	if((TWSR&0xf8)!=0x28)                                           //���״̬�Ĵ���(MT_DATA_ACK)
	return(3);                                                      //����ֵ3�������ӻ�û�ж�Address�ź���Ӧ��

	TWDR=Data;                                                      //д���ݵ�ZLG7290
	TWCR=0x80|0x04;                                                 //��:TWCR = (1<<TWINT) | (1<<TWEN);��TWINTд1�����ʹ��TWI������������Ϣ
	while((TWCR&0x80)!=0x80) ;                                       //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ������Data�ź��ѷ���
	if((TWSR&0xf8)!=0x28)                                           //if ((TWSR & 0xF8) != MT_DATA_ACK);���״̬�Ĵ���
	return(4);                                                      //����ֵ3�������ӻ�û�ж�Data�ź���Ӧ��

	TWCR=0x80|0x04|0x10;                                            //��:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);��TWINTд1�����ʹ��TWI������Stop�ź�
	return(0);

}





/******************************************************************
*********************************************************************
��IIC��ַΪSlave�Ĵӻ���Address��ַ��ʼ����N�ֽ�����Data
******************************************************************
*******************************************************************/

/*uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT)
 {
    uchar Count;
    TWCR=0x80|0x20|0x04;                                                      //��TWINTд1�����ʹ��TWI������Start�ź�
    while((TWCR&0x80)!=0x80) ;                                                //�ȴ�TWINT��λ������Start�ź��ѷ���
    if((TWSR&0xf8)!=0x08)                                                     //���״̬�Ĵ���
    return(1);                                                                //����ֵ1�������ӻ�û�ж�Start�ź���Ӧ��

    TWDR=Slave;                                                               //���µ�ַ�Ĵ���
    TWCR=0x80|0x04;                                                           //��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ
    while((TWCR&0x80)!=0x80) ;                                                //�ȴ�TWINT��λ������Slave�ź��ѷ���
    if((TWSR&0xf8)!=0x18)                                                     //���״̬�Ĵ���
    return(2);                                                                //����ֵ2�������ӻ�û�ж�Slave�ź���Ӧ��

    TWDR=Address;
    TWCR=0x80|0x04;                                                            //��TWINTд1�����ʹ��TWI�������Ĵ�����ַ��Ϣ
    while((TWCR&0x80)!=0x80) ;                                                 //�ȴ�TWINT��λ������Address�ź��ѷ���
    if((TWSR&0xf8)!=0x28)                                                      //���״̬�Ĵ���
    return(3);                                                                 //����ֵ3�������ӻ�û�ж�Address�ź���Ӧ��

    for(Count=0;Count<CNT;Count++)                                             //����дN���ֽ� ,ʵ�ַ���N�ֽ�����Data
       {
		 TWDR=Array[Count];                                                    //��������װ��TWDR
		 TWCR=0x80|0x04;                                                       //��(1<<TWINT)|(1<<TWEN);��TWINTд1�����ʹ��TWI������������Ϣ
		 while((TWCR&0x80)!=0x80) ;                                            //�ȴ�TWINT��λ������Data�ź��ѷ���
		 if((TWSR&0xf8)!=0x28)                                                 //���״̬�Ĵ���
		 return(4);
	   }                                                                                 //����ֵ4�������ӻ�û�ж�Data�ź���Ӧ��

		TWCR=0x80|0x04|0x10;                                                  //��TWINTд1�����ʹ��TWI������Stop�ź�
		return(0);
 }

 */
/**********************************************************************
***********************************************************************
��IIC��ַΪSlave�Ĵӻ���Address��ַ��ȡһ�ֽ�����Data������ֵΪ��ȡ������
//����Ϊ��������ģʽ
/*����������ģʽ���������ԴӴӻ��������ݣ�Ϊ��������ģʽ�����뷢��START�źš�
�����ŵĵ�ַ����ʽ��������MT��MRģʽ�� ������� SLA+W ����MTģʽ���������SLA+R�����MRģʽ��

************************************************************************
***********************************************************************/
uchar TWI_Receive(uchar Slave,uchar Address)
{//����Start

	uchar Temp;
	TWCR=0x80|0x20|0x04;                                                         //��:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);��TWINTд1�����ʹ��TWI������Start�ź�
	while((TWCR&0x80)!=0x80) ;													 //�ȴ�TWINT��λ������Start�ź��ѷ���
	if((TWSR&0xf8)!=0x08) 														 //���״̬�Ĵ���,{0x08��ʾ:������TWSR״̬��(��Ԥ��ƵλΪ"0"�����)}
	return(1); 																	 //����ֵ1�������ӻ�û�ж�Start�ź���Ӧ��
//����Slave��W
	TWDR=Slave; 																//���µ�ַ�Ĵ���
	TWCR=0x80|0x04;																 //��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Slave�ź��ѷ���
	if((TWSR&0xf8)!=0x18) 														//���״̬�Ĵ���,(0x18��ʾ:SLA+W�ӻ��ѷ���,���յ�ACK)
	return(2); 																	//����ֵ2�������ӻ�û�ж�Slave�ź���Ӧ��
//����Address
	TWDR=Address;
	TWCR=0x80|0x04; 															//��TWINTд1�����ʹ��TWI�������Ĵ�����ַ��Ϣ
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Address�ź��ѷ���
	if((TWSR&0xf8)!=0x28) 														//���״̬�Ĵ���,(0x28��ʾ:���ػ���ַ�ѷ���,���յ�ACK)
	return(3); 																	//����ֵ3�������ӻ�û�ж�Address�ź���Ӧ��

//�������γ�������������ʽ��,���¾��ǽ����źŵĶ���

//����Start,��������TWI
	TWCR=0x80|0x20|0x04; 														//��TWINTд1�����ʹ��TWI������Start�ź�
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Start�ź��ѷ���
	if((TWSR&0xf8)!=0x10)
	return(4);																			/*���״̬�Ĵ���,��Repeat Start (״̬0x10) �����߽ӿڿ����ٴη���
																				��ͬ�Ĵӻ����򲻷���STOP�ź��������µĴӻ���REPEATED STARTʹ������
																				�����ڲ���ʧ���߿��Ƶ��������ڴӻ�������������������������ģʽ������л���*/
																				//����ֵ4�������ӻ�û�ж�Repeat Start�ź���Ӧ��
//����Slave��R
	TWDR=Slave+1; 																//���µ�ַ�Ĵ���
	TWCR=0x80|0x04; 															//��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Slave�ź��ѷ���
	if((TWSR&0xf8)!=0x40) 														//���״̬�Ĵ���,(0x40��ʾ:SLA+R(MR_SLA_ACK)�ѷ���,���յ�ACK)
	return(5); 																	//����ֵ5�������ӻ�û�ж�Slave+R�ź���Ӧ��

	TWCR=0x80|0x04;																 //��TWINTд1�����ʹ��TWI����������
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ�����������Ѿ����ս���
	if((TWSR&0xf8)!=0x50) ; 													//���״̬�Ĵ���,(0x50��ʾ:���յ�����,ACK�ѷ���)
// return(6); 																		//����ֵ3�������ӻ�û�ж�Address�ź���Ӧ��

	Temp=TWDR; //����������

	TWCR=0x80|0x04; 															//����NACK�ź�

	TWCR=0x80|0x04|0x10; 														//����Stop�ź�,��:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	delay_ms(1);
	TWCR=0x80;																	 //���TWINT�ͽ�ֹTWI(������������ֻ�ܶ�һ��)
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
IO��ʼ��
*****************************************************/
void Init_IO ( void )
{

// PA��Һ���ã�״̬���룬������������7����,ʣPA0 ��ȷ�ϼ���

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
PD0 (B�������ת)   PD1(A�������ת )      PD2(������D)  PD3(������C)
PD4(CPB)       PD5(CPA)     PD6(������B)       PD7(������A)
********************************************************************************************************/
//PORTD=0XCC;
 PORTD=0X00;
DDRD=0X33;

}
/*******************************************************************
T1��ʼ���������·���塣δ���жϣ�CTCģʽ
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
T0��ʼ��
*********************************************************************/
void Init_T0( void )
{
  TCCR0  = 0x00;//ֹͣ��ʱ��
  TCNT0  = 0x00;//��ʼֵ
  OCR0   = 0x79;//ƥ��ֵ
  TIMSK |= 0x02;//�ж�����
  TCCR0  = 0x0D;//������ʱ��
}
/***********************************************************************
T2��ʼ��
***********************************************************************/
 void Init_T2( void )
 {
   TCCR2  = 0x00;//ֹͣ��ʱ��
   ASSR   = 0x00;//�첽ʱ��ģʽ
   TCNT2  = 0x00;//��ʼֵ
   OCR2   = 0x3E;//ƥ��ֵ
   TIMSK |= 0x80;//�ж�����
   TCCR2  = 0x00;//������ʱ��
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
	TWCR= 0x00;                                   //��ֹTWI
	TWBR= 0x15;                                      //������15
                                               //����ѹΪ5Vʱ, ��������Ϊ10Kʱ, SCL��SDA��һ��ʱ������Ϊ10us; ��������Ϊ1Kʱ, SCL��SDA��һ��ʱ������Ϊ2.5us;
                                               //TWBRֵ��Ҫ��ϸ���������ZLG7290��IICƵ��Ҫ��
	TWSR= 0x01;                                    //Ԥ��Ƶϵ��1,��(0<<TWPS1)|(0<<TWSP0);
	                                                                   //IIC�ӻ���ַSLAVE=0xa0
	TWCR= 0x04;                                    //ʹ��TWI����ֹ�ж�,��:TWCR =(1<<TWEN),
                                               //��ֹ�жϼ�TWINT�����㣬TWI������ʼ����,��ˣ���
                                               //����TWINT֮ǰһ��Ҫ������ɶԵ�ַ�Ĵ���TWAR��
                                               //״̬�Ĵ���TWSR���Լ����ݼĴ���TWDR�ķ��ʡ�
 }
/*
	LCD1602Һ������������ģ��
	1����ֱ��Ƕ�뵽��Ŀ��ʹ��
	2������Ƶ�ʣ�1M
	3���羧����ߵͲ�������ʱҪ����Ӧ�޸�
	4��������ԭ��������ֻʹ����PB�ڵĸ���λ����PB4��PB7��PB0��PB3����������

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

/*��ʾ������д�뺯��*/
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

/*��ʾ������д�뺯��*/
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

/*��ʾ�������ʾ*/
void LCD_clear(void) {

	LCD_write_com(0x01);
	delay_ms( 15);
}

/*��ʾ���ַ���д�뺯��*/
void LCD_write_str(unsigned char x,unsigned char y,unsigned char flash *s) {

    uchar i=0;
    LCDsetxy( x ,  y);
    while (*s) {
    	LCD_write_data(*s);
    	i++;
    	s ++;
    	}
}

/*��ʾ�����ַ�д�뺯��*/
void LCD_write_char(unsigned char x,unsigned char y,unsigned char data) {

    LCDsetxy( x ,  y);

    LCD_write_data( data);
}
/*void CursorScroll(void)
{

    LCD_write_com(0x10);	//�������

}
void ScreenScroll(void)
{

	LCD_write_com(0x18);		//�������
	delay_ms(255);

} */

/*��ʾ����ʼ������*/
void LCD_init(void)
 {

							/*I/O�ڷ�������*/
	LCD_write_com(0x28);				/*��ʾģʽ����*/
	delay_ms( 15);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);
	delay_ms( 5);
	LCD_write_com(0x28);

	LCD_write_com(0x08);				/*��ʾ�ر�*/
	LCD_write_com(0x01);				/*��ʾ����*/
	LCD_write_com(0x06);				/*��ʾ����ƶ�����*/
	delay_ms( 5);
	LCD_write_com(0x0c);				/*��ʾ�����������*/
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
