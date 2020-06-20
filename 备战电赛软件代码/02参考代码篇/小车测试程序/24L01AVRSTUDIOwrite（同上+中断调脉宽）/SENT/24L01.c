#include "spi.H"
#include <avr/interrupt.h>
#define delay_us  asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
unsigned char TX_BUF[32];

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
  // delay(1000);
   if((PIND&0X01)==0X01)
   {
      init_NRF24L01();
	  TX_BUF[0]=0X00;
      nRF24L01_TxPacket(TX_BUF); 
      delayms(30);
      SPI_RW_Reg(WRITE_REG+STATUS,0xFF);
      PORTA=0X01;
   }
   SREG=0X80;
}

SIGNAL(SIG_INTERRUPT1)
{
   SREG=0X00;
  // delay(1000);
   if((PIND&0X02)==0X02)
   {
      init_NRF24L01(); 
	  TX_BUF[0]=0X01;	  
      nRF24L01_TxPacket(TX_BUF); 
      delayms(30);
      SPI_RW_Reg(WRITE_REG+STATUS,0xFF);
      PORTA=0X02;
   }
   SREG=0X80;
}

SIGNAL(SIG_INTERRUPT2)
{
   SREG=0X00;
  // delay(1000);
   if((PIND&0X04)==0X04)
   {
      init_NRF24L01();
	  TX_BUF[0]=0X02;	  
      nRF24L01_TxPacket(TX_BUF); 
      delayms(30);
      SPI_RW_Reg(WRITE_REG+STATUS,0xFF);
      PORTA=0X04;
   }
   SREG=0X80;
}

SIGNAL(SIG_INTERRUPT3)
{
   unsigned char j;
   SREG=0X00;
 //  delay(1000);
   if((PIND&0X08)==0X08)
   {
      init_NRF24L01(); 
	  TX_BUF[0]=0X03;	  
      nRF24L01_TxPacket(TX_BUF); 
      delayms(30);
      j=SPI_Read(STATUS);
	  PORTA=j;
	  delay(50000);
      SPI_RW_Reg(WRITE_REG+STATUS,0xFF);   
	  PORTA=0X08;   
   }
   SREG=0X80;   
}

int main(void)
{
  DDRA=0XFF;
  DDRD=0XF0;
  DDRC=0XFF;
  PORTC=0XF0;
  PORTA=0XF0;
  DDRE=0XFF;
  
  EICRA=0X55;//外部中断初始化
  EIMSK=0X0F;
  SREG=0X80;

  delayms(22);
  init_NRF24L01();
  DDRA=0XFF;
  PORTA=0X0F;
  while(1);
}




