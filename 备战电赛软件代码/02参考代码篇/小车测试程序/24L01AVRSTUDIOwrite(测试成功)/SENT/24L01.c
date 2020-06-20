#include "spi.H"

int main(void)
{
//  unsigned char sta,i;
  unsigned char TX_BUF[32]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b};
  delayms(22);
  init_NRF24L01();
  DDRA=0XFF;
  PORTA=0X0f;
  
   
  while(1)
  {
	nRF24L01_TxPacket(TX_BUF); 
	// sta=SPI_Read(CONFIG);      // 读取状态寄存其来判断数据接收状况  
 	// PORTA=sta; 
	PORTA=~PORTA;
	delayms(50);
	SPI_RW_Reg(WRITE_REG+STATUS,0xff);	
  
  }
}




