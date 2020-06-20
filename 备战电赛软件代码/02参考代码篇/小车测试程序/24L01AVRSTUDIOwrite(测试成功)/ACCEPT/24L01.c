#include"spi.H"


int main()
{
  uchar i,sta;
  uchar RX_BUF[32];
  init_NRF24L01();
// LcdIni();
  DDRA=0XFF;
  PORTA=0X55;
  while(1)
   {
     SetRX_Mode();

	 // while(1);
	 // while(1);
	 if(nRF24L01_RxPacket(RX_BUF))
	  {

//	 sta=SPI_Read(STATUS);      // 读取状态寄存其来判断数据接收状况  
 //    PORTA=sta;
//	  while(1);
		for(i=0;i<5;i++)  
	    {
	       //write_com(0x80+i);
	       //write_data(RX_BUF[i]);
		   PORTA=RX_BUF[i];
		   delayms(50);
	    }
	  }
	 
   }
}
