#include"spi.H"
#include <avr/interrupt.h>

#define delay_us  asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

unsigned int number,count,feedback,setspeed=500;
static int pulsewidth=500,pulsewidth1=500;
uchar RX_BUF[32];


/*void delay(unsigned int m)
{
    unsigned int i;
	for(i=0;i<m;i++)
	{
	    delay_us;
	}
}

SIGNAL(SIG_INTERRUPT0)
{
   SREG=0X00;
   delay(100);
   if((PIND&0X01)==0X01)
   {
      SetRX_Mode();
	 if(nRF24L01_RxPacket(RX_BUF))
	 {
		PORTA=0XFF;
		if(RX_BUF[0]==0X00)
		{
		   PORTA&=0x7F;
	       PORTC&=0x7F;
           delayms(300);
		}
		if(RX_BUF[0]==0X01)
		{
		   PORTA&=0x7F;
	       PORTC|=0x80;
           delayms(300);
		}
		if(RX_BUF[0]==0X02)
		{
		   PORTA|=0x80;
	       PORTC|=0x80;
           delayms(300);
		}
		if(RX_BUF[0]==0X03)
		{
		   PORTA|=0x80;
	       PORTC&=0x7F;
           delayms(300);
		}
	 }
   }
   SREG=0X80;
}*/

int main()
{
   uchar i,sta;
   DDRA=0XFF;
   DDRC=0XFF;
   PORTC=0XF0;
   PORTA=0XF0;
   
   DDRB=0X60;
   DDRE=0XFF;
   EICRA=0X02;//外部中断初始化
   EIMSK=0X01;

   TCCR1A=0XA3;//此频率为10KHz
   TCCR1B=0X19;
   OCR1A=1000;
   OCR1B=pulsewidth;

   TCCR3A=0XA3;
   TCCR3B=0X19;
   OCR3A=1000;
   OCR3B=pulsewidth1;
  SREG=0X80;
  init_NRF24L01();
  DDRB|=0X60;
 //while(1); 
  while(1)
   {    
	 SetRX_Mode();
	 if(nRF24L01_RxPacket(RX_BUF))
	 {
		if(RX_BUF[0]==0X00)
		{
//		   PORTA=0x01;
	       PORTC&=0x3F;

		}
		if(RX_BUF[0]==0X01)
		{
	//	   PORTA=0x02;
	       PORTC|=0xC0;
		}
		if(RX_BUF[0]==0X02)
		{
		   
	//	   PORTA=0x40;
	       PORTC|=(1<<7);
           PORTC&=0XBF;
		}
		if(RX_BUF[0]==0X03)
		{
		   
	//	   PORTA|=0x80;
	       PORTC|=(1<<6);
           PORTC&=0X7F;
		}
	 }
	 
   }
}
