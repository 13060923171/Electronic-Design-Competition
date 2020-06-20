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
