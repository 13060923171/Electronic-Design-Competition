/*****************************************************
This program was produced by the
CodeWizardAVR V2.03.4 Standard
Automatic Program Generator
© Copyright 1998-2008 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 2009-7-19
Author  : 
Company : 
Comments: 


Chip type           : ATmega16L
Program type        : Application
Clock frequency     : 16.000000 MHz
Memory model        : Small
External RAM size   : 0
Data Stack size     : 256
*****************************************************/

#include <mega16.h>
#include <delay.h>
#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART Receiver buffer
#define RX_BUFFER_SIZE 64
#define MyAddress 0x01      // ×Ô¼ºµÄµØÖ·
/**************±äÁ¿ÉùÃ÷***************/
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE<256
unsigned char rx_wr_index,rx_rd_index,rx_counter;
#else
unsigned int rx_wr_index,rx_rd_index,rx_counter;
#endif
// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;
// USART Receiver interrupt service routine
char str[] = "hello\n";
char frameData[16];
char state;
bit statechanged = 0, serFlag = 0, timerFlag = 0;
unsigned int timerCounter;
char baCounter;
/***************º¯ÊýÉùÃ÷****************/
char doFrameOk();
void doCommand();
void uartSend(char sendCmd, char sendData);
void balan();
void move(char mode);
void delay_1ms();
void delayForSer(unsigned int ms);
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
serFlag = 1;
status=UCSRA;
data=UDR;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer[rx_wr_index]=data;
   if (++rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
      rx_buffer_overflow=1;
      };
   };
}

// Timer 2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
// Reinitialize Timer 2 value
TCNT2=0xff;
// Place your code here
if(++timerCounter == 8000)
{
    timerCounter = 0;
    timerFlag = 1;
}
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index];
if (++rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>

// Declare your global variables here

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x30;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer 1 Stopped
// Mode: Normal top=FFFFh
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 15.625 kHz
// Mode: Normal top=FFh
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x07;
TCNT2=0xff;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

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
UBRRL=0x33;
PORTD=0x00;
DDRD=0x30;
DDRA=0xf0;
PORTA = 0xff;
DDRA &= 0b11111100;
PORTA |= 0b00000011;
// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// Global enable interrupts


// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: Ph. correct PWM top=01FFh
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0xa2;
TCCR1B=0x12;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
ACSR=0x80;
ICR1H = 0x01;
ICR1L = 0xf0;
#asm("sei")
puts(str);
//*****µÚÒ»½×¶Î*******
while(getchar() != 0x0c);
putchar(0x01);
move(1);
delay_ms(2000);
move(0);
putchar(0x02);
delay_ms(1000);
                                        /*******±à´íµÄ*******/
                                        //move(1);
                                        //delay_ms(500);
                                        //while(!PINA.0 || !PINA.1);
                                        //move(0);
                                        //putchar(0x02);
                                        //delay_ms(1000);
                                        //putchar(0x01);
                                        //move(2);
                                        //delay_ms(500);
                                        //while(!PINA.0 || !PINA.1);
                                        //move(0);
                                        //putchar(0x02);
                                        //delay_ms(1000);//ÔÝÍ£1sÐÝÏ¢Ò»ÏÂ
                                        
                                        
/********µÚ¶þ½×¶Î**********/                                        
//while(getchar() != 0x0c);
putchar(0x01);
TIMSK=0x40;
while (baCounter < 3)
{
    
//   Place your code here
    if(rx_counter>=5)
    {
        delay_ms(1);
        if(getchar() == 0x55)
        {        
            if(doFrameOk())
            {
                doCommand();
            }
        }
    }
    if(timerFlag && TIMSK)
    {
        timerFlag = 0;
        balan();
    }
};
putchar(0x02);
TIMSK=0x00;
UCSRB.7 = 0;
delay_ms(1000);
/******µÚÈý½×¶Î******/
//while(getchar() != 0x0c);
putchar(0x01);
move(1);
while(!PINA.0 || !PINA.1);
move(0);
delay_ms(1000);
putchar(0x02);
delay_ms(1000);
/******µÚËÄ½×¶Î*****/
putchar(0x01);
move(2);
delay_ms(1000);
while(!PINA.0 || !PINA.1);
move(0);
putchar(0x02);
while(1);
}
void balan()
{
    switch(state)
    {
        case 1:
                move(4);
                delayForSer(80);
                move(5);
            break;
        case 2:
                move(3);
                delayForSer(80);
                move(5);
            break;
        case 3:
            move(6);
            break;
        default:
            move(5);
            break;
    }
}
/*************ÒÆ¶¯º¯Êý(0:È«ËÙÍ£Ö¹, 1:È«ËÙÇ°ÐÐ,2:È«ËÙºóÍË, 3:ÂýËÙÇ°ÐÐ, 4:ÂýËÙºóÍË, 5:ÂýËÙÍ£Ö¹, 6:»ØÍËÉ²³µ(ÓÃÓÚÕÒµ½Æ½ºâ)) **********************/
void move(char mode)
{
    switch(mode)
    {
        case 0:
            //É²³µÃüÁî
            PORTA.4 = PORTA.6 = ~PORTA.4;
            PORTA.5 = PORTA.7 = ~PORTA.5;
            OCR1A = OCR1B = 0x01f0;
            delay_ms(250);
            //ÓÐ±ØÒªÊ±¼ÓÉÏÇã½Ç¼ì²âÃüÁî

            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 0;
            OCR1A = OCR1B = 0x01f0;
 //           putchar('4');
            break;
        case 1:
            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 1; 
            OCR1A = OCR1B = 0x0160;
//            putchar('2');
            break;
        case 2:
            PORTA.4 = PORTA.6 = 1;
            PORTA.5 = PORTA.7 = 0; 
            OCR1A = OCR1B = 0x0160;
//            putchar('3');
            break;
        case 3:
            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 1; 
            OCR1AH = OCR1BH = 0x01;
            OCR1AL = OCR1BL = 0xc0;
//            putchar('2');
            break;
        case 4:
            PORTA.4 = PORTA.6 = 1;
            PORTA.5 = PORTA.7 = 0; 
            OCR1AH = OCR1BH = 0x01;
            OCR1AL = OCR1BL = 0xc0;
//            putchar('3');
            break;
        case 5:
            //É²³µÃüÁî
            PORTA.4 = PORTA.6 = ~PORTA.4;
            PORTA.5 = PORTA.7 = ~PORTA.5;
            OCR1A = OCR1B = 0x00ee;
            delay_ms(100);
            //ÓÐ±ØÒªÊ±¼ÓÉÏÇã½Ç¼ì²âÃüÁî

            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 0;
            OCR1A = OCR1B = 0x01f0;
//            putchar('4');
            break;
        case 6:
            PORTA.4 = PORTA.6 = statechanged;
            PORTA.5 = PORTA.7 = ~statechanged;
            TIMSK = 0x00;
            OCR1A = OCR1B = 0x01f0;
            delay_ms(130);
            //ÓÐ±ØÒªÊ±¼ÓÉÏÇã½Ç¼ì²âÃüÁî
            PORTA.4 = PORTA.6 = ~PORTA.4;
            PORTA.5 = PORTA.7 = ~PORTA.5;
            OCR1A = OCR1B = 0x00ee;
            delay_ms(100);
            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 0;
            OCR1A = OCR1B = 0x0000;
//            putchar('4');
    }
}
char doFrameOk()
{
    char flag=0, k=0,i,frameLen;
    if(getchar() == 0xaa)
        k++;
    if(getchar() == MyAddress)
        k++;
    if(k == 2)
    {
        flag = 1;
        frameLen = getchar();
        for(i = 0;i < frameLen; i++)
        {
            frameData[i] = getchar();
        }
        
    }
    return flag;
}
void doCommand()
{
//    int i = 1;
    switch(frameData[0])
    {
        case 0x01: uartSend(0xff, 0x00);
        break;
        case 0x02:
            PORTA.4 = PORTA.6 = 1;
            PORTA.5 = PORTA.7 = 0; 
            OCR1AH = OCR1BH = frameData[1];
            OCR1AL = OCR1BL = frameData[2];
            putchar('2');
            break;
        case 0x03: 
            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 1;
            OCR1AH = OCR1BH = frameData[1];
            OCR1AL = OCR1BL = frameData[2];
            putchar('3');
            break;
        case 0x04:
//É²³µÃüÁî
            PORTA.4 = PORTA.6 = ~PORTA.4;
            PORTA.5 = PORTA.7 = ~PORTA.5;
            OCR1A = OCR1B = 0x00ee;
            delay_ms(250);
//Çã½Ç¼ì²âÃüÁî
//            if(state == 1)
//            {
//                PORTA.4 = PORTA.6 = 1;
//                PORTA.5 = PORTA.7 =  
//            }
//            if
            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 0;
            OCR1A = OCR1B = 0x01f0;
            putchar('4');
            break;
        case 0x05:
            PORTA.4 = PORTA.7 = 1;
            PORTA.5 = PORTA.6 = 0;
            OCR1AH = OCR1BH = frameData[1];
            OCR1AL = OCR1BL = frameData[2];
            putchar('5');
            break;
        case 0x06:
            PORTA.4 = PORTA.7 = 0;
            PORTA.5 = PORTA.6 = 1;
            OCR1AH = OCR1BH = frameData[1];
            OCR1AL = OCR1BL = frameData[2];
            putchar('6');
            break;
        case 0x07:
            PORTA.4 = PORTA.6 = 1;
            PORTA.5 = PORTA.7 = 0;
            OCR1AH = frameData[1];
            OCR1AL = frameData[2];
            OCR1BH = frameData[3];
            OCR1BL = frameData[4];
            putchar('7');
            break;
        case 0x08:
            PORTA.4 = PORTA.6 = 0;
            PORTA.5 = PORTA.7 = 1;
            OCR1AL = frameData[1];
            OCR1AL = frameData[2];
            OCR1BH = frameData[3];
            OCR1BL = frameData[4];
            putchar('8');
            break;
    //½ÓÊÜÆ½ºâ×´Ì¬Ö¸Ê¾        
        case 0x09:
            state = 1;
            statechanged = 0;
            TIMSK = 0x40;
            putchar('9');
            baCounter = 0;
            break;
        case 0x0a:
            state = 2;
            statechanged = 1;
            TIMSK = 0x40;
            putchar('a');
            baCounter = 0;
            break;
        case 0x0b:
            state = 3;
            timerFlag = 1;
            putchar('b');
            baCounter++;
            break;           
        default: uartSend(0xee, 0x00);
    }     
}
void uartSend(char sendCmd, char sendData)
{
        putchar(0x55);
        putchar(0xaa);
        putchar(0x02);
        putchar(sendCmd);
        putchar(sendData);
        putchar('\n');               
}
void delay_1ms(void)

{

  unsigned int i;

  for(i=1;i<2000 && rx_counter<5;i++)
 ;

}
void delayForSer(unsigned int ms)
{
    int i;
    for(i = 0; i < ms && rx_counter<5; i++)
        delay_1ms();
}