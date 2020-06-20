#include"spi.H"
#include <avr/interrupt.h>

#define delay_us  asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

unsigned int number,count,feedback,setspeed=500;
unsigned int pulsewidth=0,pulsewidth1=0;
uchar RX_BUF[32];


void delay(unsigned int m)
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
 //  DDRD&=0XFE;
 //  PORTC&=0X3F;

   if(nRF24L01_RxPacket(RX_BUF))
   {
		 if(RX_BUF[0]==0X00)//正转
		 {
	        
			PORTC&=0x3F;
			pulsewidth=0;
			pulsewidth1=0;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;
      
            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
            delay(535);

			pulsewidth=1000;
			pulsewidth1=1000;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;

		 }
		 if(RX_BUF[0]==0X01)//反
		 {

			PORTC|=0xC0;

			pulsewidth=1000;
			pulsewidth1=1000;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
			delay(535);



			pulsewidth=0;
			pulsewidth1=0;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
			

		 }
		 if(RX_BUF[0]==0X02)//左
		 {	

	        PORTC|=(1<<7);
            PORTC&=0XBF;
			pulsewidth=0;
			pulsewidth1=1000;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
			delay(535);



			pulsewidth=1000;
			pulsewidth1=0;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;

		 }
		if(RX_BUF[0]==0X03)//右
		{
		   
	       PORTC|=(1<<6);
           PORTC&=0X7F;

		   	pulsewidth=1000;
			pulsewidth1=0;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
			delay(535);


		    pulsewidth=0;
		    pulsewidth1=1000;
		    TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
		}
		if(RX_BUF[0]==0X04)//右
		{
		   
	       PORTC|=0xC0;

		   	pulsewidth=1000;
			pulsewidth1=1000;
			TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
			delay(535);


		    pulsewidth=1000;
		    pulsewidth1=1000;
		    TCCR1A=0XA3;//此频率为10KHz  //PB6
            TCCR1B=0X19;
            OCR1A=1000;
            OCR1B=pulsewidth;

            TCCR3A=0XA3;
            TCCR3B=0X19;
            OCR3A=1000;
            OCR3B=pulsewidth1;
		}
        
	   if(RX_BUF[0]==0X05)
	   {
		    TCCR2=0X1A;
		    OCR2=0XF9;
	   }
	   if(RX_BUF[0]==0X06)
	   {
		    OCR2=0X00;

	   } 
	 	 
	}
   SPI_RW_Reg(WRITE_REG+STATUS,0xFF);
   SREG=0X80;
}

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

  SREG=0X80;
  init_NRF24L01();
  DDRB|=0XE0;

  pulsewidth=1000;
  pulsewidth1=1000;
  TCCR1A=0XA3;//此频率为10KHz  //PB6
  TCCR1B=0X19;
  OCR1A=1000;
  OCR1B=pulsewidth;

  TCCR3A=0XA3;
  TCCR3B=0X19;
  OCR3A=1000;
  OCR3B=pulsewidth1;
  delay(535);
  SetRX_Mode();
  while(1)
  {    
	  SetRX_Mode();
  }
}
