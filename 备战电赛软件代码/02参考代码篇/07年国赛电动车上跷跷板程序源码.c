/*****************************************************
This program was produced by the
CodeWizardAVR V2.03.4 Standard
Automatic Program Generator
© Copyright 1998-2008 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 2009/7/25
Author  : 
Company : 
Comments: 


Chip type           : ATmega16L
Program type        : Application
Clock frequency     : 14.745600 MHz
Memory model        : Small
External RAM size   : 0
Data Stack size     : 256
*****************************************************/
#include <mega16.h>
#include <delay.h>

// USART¿ØÖÆºÍ×´Ì¬¼Ä´æÆ÷µÄ±êÖ¾Î»¶¨Òå
#define RXC  7          // UCSRAÎ»¶¨Òå
#define TXC  6
#define UDRE 5
#define FE   4
#define DOR  3
#define PE   2
#define U2X  1
#define MPCM 0
#define RXCIE 7         // UCSRBÎ»¶¨Òå
#define TXCIE 6
#define UDRIE 5
#define RXEN  4
#define TXEN  3
#define UCSZ2 2
#define RXB8  1
#define TXB8  0
#define URSEL 7         // UCSRCÎ»¶¨Òå
#define UMSEL 6
#define UPM1  5
#define UPM0  4
#define USBS  3
#define UCSZ1 2
#define UCSZ0 1
#define UCPOL 0 

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<PE)
#define DATA_OVERRUN (1<<DOR)
#define DATA_REGISTER_EMPTY (1<<UDRE)

#define MOTOR_L1 PORTB.0       //µç»ú
#define MOTOR_L2 PORTB.1
#define MOTOR_R1 PORTB.2
#define MOTOR_R2 PORTB.3

#define BW_LEFT  PINA.0        //ºÚ°×Ïß
#define BW_RIGHT PINA.1

#define BUZZER   PORTA.3        //·äÃùÆ÷


#define NULL        0x00
#define LEFT        0x01
#define RIGHT       0x02
#define BAL_START   0x03
#define BAL         0x04
#define BAL_OK      0x05
#define GET_B       0x06
#define B_OK        0x07
#define A_OK        0x08
#define MODE_B      0x09
#define MODE_P      0x0A
#define BAL_AGAIN   0x0B
#define FULL        0xFF

unsigned char tran = 5; //USARTÊý¾Ý
bit receive_ok = 0;
bit dir = 0;
bit mode2 = 0;
unsigned char delay_dir = 0;

interrupt [USART_RXC] void usart_rx_isr(void)   // USART½ÓÊÕÖÐ¶Ï·þÎñ
{
    unsigned char status,data;
    status = UCSRA;
    data = UDR;
    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
        {
        tran = data;
        receive_ok = 1;
        }
}

void USART_Transmit(unsigned char data)
{
        while (!(UCSRA & DATA_REGISTER_EMPTY));    // µÈ´ý·¢ËÍ¼Ä´æÆ÷¿Õ                                        
        UDR = data;                                 // ·¢ËÍÊý¾Ý                                
}


void motor_direct(unsigned char direct)          //µç»ú»»Ïò
{
if(direct == 0)
    {
    MOTOR_L1 = 0;
    MOTOR_L2 = 1;
    MOTOR_R1 = 0;
    MOTOR_R2 = 1;
    }
if(direct == 1)
    {
    MOTOR_L1 = 1;
    MOTOR_L2 = 0;
    MOTOR_R1 = 1;
    MOTOR_R2 = 0;
    }
}

void motor_start(void)                            //µç»úÆô¶¯
{
if(dir == 0)
    {
    MOTOR_L1 = 0;
    MOTOR_L2 = 1;
    MOTOR_R1 = 0;
    MOTOR_R2 = 1;
    }
if(dir == 1)
    {
    MOTOR_L1 = 1;
    MOTOR_L2 = 0;
    MOTOR_R1 = 1;
    MOTOR_R2 = 0;
    }
}
                                                       
void motor_stop(void)                             //µç»úÍ£Ö¹
{
MOTOR_L1 = 1;
MOTOR_L2 = 1;
MOTOR_R1 = 1;
MOTOR_R2 = 1;
}

void motor_jerk(unsigned int time ,unsigned char direct_stop)
{
    motor_direct(direct_stop);
    delay_ms(time);
    motor_stop();
}

void buzzer_voice(void)
{
 BUZZER = 1;                      
 delay_ms(500);
 BUZZER = 0;
}

void mode_pro(void)
{
unsigned char cache = 0;
bit once = 0,balance_again = 0;

//buzzer_voice();
motor_start();                //µç»ú¿ªÆô

delay_ms(500);
motor_stop();

while(1)
     {
     if(BW_RIGHT == 1 && BW_LEFT == 0)
         {
         OCR1AH=0xef;
         OCR1AL=0xFF;
         OCR1BH=0xaf;
         OCR1BL=0xFF;
         delay_ms(300);
         motor_jerk(150,0);
         OCR1BH=0xef;
         OCR1BL=0xFF;
         OCR1AH=0x00;
         OCR1AL=0xFF;
         delay_ms(300);
         motor_jerk(200,1);
         delay_dir = 1;
         }
     else if(BW_RIGHT == 0 && BW_LEFT == 1)
         {
         OCR1BH=0xef;
         OCR1BL=0xFF;
         OCR1AH=0xaf;
         OCR1AL=0xFF;
         delay_ms(300);
         motor_jerk(150,0);
         OCR1AH=0xef;
         OCR1AL=0xFF;
         OCR1BH=0x00;
         OCR1BL=0xFF;
         delay_ms(300);
         motor_jerk(200,1);
         delay_dir = 2;
         }
     else if(BW_RIGHT == 0 && BW_LEFT == 0)
         {
         OCR1AH=0xc0;
         OCR1AL=0xFF;
         OCR1BH=0xf0;
         OCR1BL=0xFF;
         motor_jerk(30,0);
         }
     else if(BW_RIGHT == 1 && BW_LEFT == 1)
         {
         /*if(delay_dir == 1)
              {
              OCR1AH=0xef;
              OCR1AL=0xFF;
              OCR1BH=0xaf;
              OCR1BL=0xFF;
              delay_ms(300);
              motor_jerk(150,0);
              OCR1BH=0xef;
              OCR1BL=0xFF;
              OCR1AH=0x00;
              OCR1AL=0xFF;
              delay_ms(300);
              motor_jerk(200,1);
              }
         else if(delay_dir == 2)
              {
              OCR1BH=0xef;
              OCR1BL=0xFF;
              OCR1AH=0xaf;
              OCR1AL=0xFF;
              delay_ms(300);
              motor_jerk(150,0);
              OCR1AH=0xef;
              OCR1AL=0xFF;
              OCR1BH=0x00;
              OCR1BL=0xFF;
              delay_ms(300);
              motor_jerk(200,1);
              }*/
         break;
         }
     delay_ms(400);
     };

OCR1AH=0xef;
OCR1AL=0xFF;
OCR1BH=0xef;
OCR1BL=0xFF;

buzzer_voice();
motor_start();                //µç»ú¿ªÆô

delay_ms(1000);
motor_stop();

again:

delay_ms(1000);

USART_Transmit(BAL_START);
USART_Transmit(BAL_START);
USART_Transmit(BAL_START);
USART_Transmit(BAL_START);
USART_Transmit(BAL_START);

while (1)                       //ÕÒÆ½ºâ
{
 cache = tran;
 if(receive_ok == 1 && cache == LEFT)
    {
    motor_jerk(30,1);
    delay_ms(800);
    }
 else if(receive_ok == 1 && cache == RIGHT)
    {
    motor_jerk(30,0);
    delay_ms(800);
    }
 else 
    {
    motor_stop();
    if(cache == BAL && once == 0)
        {
        buzzer_voice();
        once = 1;
        }
    if(cache == BAL_OK)
        {
        buzzer_voice();
        break;
        }
    }
}

if(balance_again == 0)
   {
   while(1)
       {
       if(tran == BAL_AGAIN)
           break;
       delay_ms(100); 
       }
   balance_again = 1;
   once = 0;
   receive_ok = 0;
   goto again;
   }

motor_jerk(150,0); 
                                                                       
while (1)
      {
      if(BW_LEFT == 1 && BW_RIGHT == 1)
          {
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          break;
          }
      else 
          {
          motor_jerk(30,0);
          delay_ms(800);
          }
      }

while (1)
      {
       if(tran == B_OK)
           break;                                       
      };

motor_jerk(1200,1);
      
while (1)
      {
       if(BW_LEFT == 1 && BW_RIGHT == 1)
          {
          motor_jerk(30,1);
          delay_ms(500);
          motor_jerk(30,1);
          delay_ms(500);
          motor_jerk(30,1);
          break;
          }
      else 
          {
          motor_jerk(40,1);
          delay_ms(800);
          }                                        
      };
      
USART_Transmit(A_OK);
USART_Transmit(A_OK);
USART_Transmit(A_OK);
USART_Transmit(A_OK);
USART_Transmit(A_OK);
      
while (1)
      {
      
      };      
}

void main(void)
{
unsigned char cache = 0;
bit once = 0;

//unsigned char middle,balance,B,A;
//unsigned char time_h[10] = {0},time_m[10] = {0},time_s[10] = {0};
//unsigned char *mid = ""

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=0 State2=P State1=P State0=P 
PORTA=0x07;
DDRA=0x08;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=T State6=T State5=T State4=T State3=0 State2=0 State1=0 State0=0 
PORTB = 0x00;
DDRB=0x0F;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=1 State5=0 State4=0 State3=T State2=T State1=T State0=T 
PORTD=0x40;
DDRD=0x70;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 14.400 kHz
// Mode: Normal top=FFh
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 14745.600 kHz
// Mode: Fast PWM top=ICR1
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0xA2;
TCCR1B=0x19;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0xFF;
ICR1L=0xFF;
OCR1AH=0xef;
OCR1AL=0xFF;
OCR1BH=0xef;
OCR1BL=0xFF;


// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 19200
UCSRA=0x00;
UCSRB=0x98;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x2F;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x01;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// Global enable interrupts
#asm("sei")

if(BW_LEFT == 1 && BW_RIGHT == 1)
    {
    mode2 = 1;
    USART_Transmit(MODE_P);
    USART_Transmit(MODE_P);
    delay_ms(400);
    USART_Transmit(MODE_P);
    USART_Transmit(MODE_P);
    delay_ms(400);
    USART_Transmit(MODE_P);
    mode_pro();
    }
     
USART_Transmit(MODE_B);
USART_Transmit(MODE_B);
delay_ms(400);
USART_Transmit(MODE_B);
USART_Transmit(MODE_B);
delay_ms(400);
USART_Transmit(MODE_B); 

buzzer_voice();
motor_start();                //µç»ú¿ªÆô

delay_ms(1000);
motor_stop();

delay_ms(1000);

USART_Transmit(BAL_START);
USART_Transmit(BAL_START);
USART_Transmit(BAL_START);
USART_Transmit(BAL_START);
USART_Transmit(BAL_START);

while (1)                       //ÕÒÆ½ºâ
{
 cache = tran;
 if(receive_ok == 1 && cache == LEFT)
    {
    motor_jerk(30,1);
    delay_ms(800);
    }
 else if(receive_ok == 1 && cache == RIGHT)
    {
    motor_jerk(30,0);
    delay_ms(800);
    }
 else 
    {
    motor_stop();
    if(cache == BAL && once == 0)
        {
        buzzer_voice();
        once = 1;
        }
    if(cache == BAL_OK)
        {
        buzzer_voice();
        break;
        }
    }
}

motor_jerk(200,0); 
                                                                       
while (1)
      {
      if(BW_LEFT == 1 && BW_RIGHT == 1)
          {
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          USART_Transmit(GET_B);
          break;
          }
      else 
          {
          motor_jerk(30,0);
          delay_ms(800);
          }
      }

while (1)
      {
       if(tran == B_OK)
           break;                                       
      };

motor_jerk(1200,1);
      
while (1)
      {
       if(BW_LEFT == 1 && BW_RIGHT == 1)
          {
          motor_jerk(30,1);
          delay_ms(500);
          motor_jerk(30,1);
          delay_ms(500);
          motor_jerk(30,1);
          break;
          }
      else 
          {
          motor_jerk(40,1);
          delay_ms(800);
          }                                        
      };
      
USART_Transmit(A_OK);
USART_Transmit(A_OK);
USART_Transmit(A_OK);
USART_Transmit(A_OK);
USART_Transmit(A_OK);
      
while (1)
      {
      
      };      
}
