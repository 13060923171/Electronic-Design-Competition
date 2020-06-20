
#pragma used+
sfrb TWBR=0;
sfrb TWSR=1;
sfrb TWAR=2;
sfrb TWDR=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      
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
sfrw EEAR=0x1e;   
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
sfrw OCR1B=0x28;  
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  
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

#pragma used+

void delay_us(unsigned int n);
void delay_ms(unsigned int n);

#pragma used-

char rx_buffer[64];

unsigned char rx_wr_index,rx_rd_index,rx_counter;

bit rx_buffer_overflow;

char str[] = "hello\n";
char frameData[16];
char state;
bit statechanged = 0;

char doFrameOk();
void doCommand();
void uartSend(char sendCmd, char sendData);
void balan();
void move(char mode);
interrupt [12] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
if ((status & ((1<<4) | (1<<2) | (1<<3)))==0)
{
rx_buffer[rx_wr_index]=data;
if (++rx_wr_index == 64) rx_wr_index=0;
if (++rx_counter == 64)
{
rx_counter=0;
rx_buffer_overflow=1;
};
};
}

#pragma used+
char getchar(void)
{
char data;

unsigned int i;
for(i=0;i<(unsigned int)(16*143-2);i++)
if(rx_counter!=0)
break;

data=rx_buffer[rx_rd_index];
if (++rx_rd_index == 64) rx_rd_index=0;
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-

typedef char *va_list;

#pragma used+

char getchar(void);
void putchar(char c);
void puts(char *str);
void putsf(char flash *str);

char *gets(char *str,unsigned int len);

void printf(char flash *fmtstr,...);
void sprintf(char *str, char flash *fmtstr,...);
void snprintf(char *str, unsigned int size, char flash *fmtstr,...);
void vprintf (char flash * fmtstr, va_list argptr);
void vsprintf (char *str, char flash * fmtstr, va_list argptr);
void vsnprintf (char *str, unsigned int size, char flash * fmtstr, va_list argptr);
signed char scanf(char flash *fmtstr,...);
signed char sscanf(char *str, char flash *fmtstr,...);

#pragma used-

#pragma library stdio.lib

void main(void)
{

PORTA=0x00;
DDRA=0x00;

PORTB=0x00;
DDRB=0x00;

PORTC=0x00;
DDRC=0x00;

PORTD=0x00;
DDRD=0x00;

TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

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

ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

MCUCR=0x00;
MCUCSR=0x00;

TIMSK=0x00;

UCSRA=0x00;
UCSRB=0x98;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x33;

ACSR=0x80;
SFIOR=0x00;

DDRA=0xf0;
PORTA = 0x00;
PORTD=0x00;
DDRD=0x30;

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
while (1)
{

if(getchar() == 0x55)
{        
if(doFrameOk())
{
doCommand();
}
}
balan();
};
}
void balan()
{
switch(state)
{
case 1:
move(4);
delay_ms(250);
move(0);
delay_ms(250);
break;
case 2:
move(3);
delay_ms(250);
move(0);
delay_ms(250);
break;
case 3:
move(0);
break;
default:
move(0);
break;
}
}

void move(char mode)
{
switch(mode)
{
case 0:

PORTA.4 = PORTA.6 = ~PORTA.4;
PORTA.5 = PORTA.7 = ~PORTA.5;
OCR1A = OCR1B = 0x00ee;
delay_ms(250);

PORTA.4 = PORTA.6 = 0;
PORTA.5 = PORTA.7 = 0;
OCR1A = OCR1B = 0x01f0;
putchar('4');
break;
case 1:
PORTA.4 = PORTA.6 = 0;
PORTA.5 = PORTA.7 = 1; 
OCR1AH = OCR1BH = 0x01;
OCR1AL = OCR1BL = 0xf0;
putchar('2');
break;
case 2:
PORTA.4 = PORTA.6 = 1;
PORTA.5 = PORTA.7 = 0; 
OCR1AH = OCR1BH = 0x01;
OCR1AL = OCR1BL = 0xf0;
putchar('3');
break;
case 3:
PORTA.4 = PORTA.6 = 0;
PORTA.5 = PORTA.7 = 1; 
OCR1AH = OCR1BH = 0x01;
OCR1AL = OCR1BL = 0x40;
putchar('2');
break;
case 4:
PORTA.4 = PORTA.6 = 1;
PORTA.5 = PORTA.7 = 0; 
OCR1AH = OCR1BH = 0x01;
OCR1AL = OCR1BL = 0x40;
putchar('3');
break;
}
}
char doFrameOk()
{
char flag=0, k=0,i,frameLen;
if(getchar() == 0xaa)
k++;
if(getchar() == 0x01      )
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

PORTA.4 = PORTA.6 = ~PORTA.4;
PORTA.5 = PORTA.7 = ~PORTA.5;
OCR1A = OCR1B = 0x00ee;
delay_ms(250);

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

case 0x09:
state = 1;
statechanged = 1;
putchar('9');
break;
case 0x0a:
state = 2;
statechanged = 1;
putchar('a');
break;
case 0x0b:
state = 3;
statechanged = 1;
putchar('b');
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
