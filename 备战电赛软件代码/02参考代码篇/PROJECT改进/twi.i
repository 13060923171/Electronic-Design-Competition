/*********************************************************************************************/ 
/********************�����Ƕ�ATmega16���Դ�I2C�����ʹ��**************************************/ 
/*********************************************************************************************/ 
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
