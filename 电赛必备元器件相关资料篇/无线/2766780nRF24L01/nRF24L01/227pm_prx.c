#include "API.h"
#include<reg52.h>

#define uchar unsigned char
//******************initializtion*******************//

//paramenters part******//

#define addr_wid 0x05//both,length of adress.
#define pld_wid	0x01//both,1~32,only ack canbe use 0 length
uchar ptx_addr[addr_wid]={0x7c,0x7c,0x7c,0x7c,0x7c};//PTX ,the free address ,the destination of the packet
uchar pipe0_addr[addr_wid]={0x7c,0x7c,0x7c,0x7c,0x7c};//PRX ,free address  which pipe0 canbe allow.
uchar static_rcv_buf[5]=0;//PTX,data buf to be transmitted.
uchar new_data=0;
uchar led[16]={0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b,0x77,0x1f,0x0d,0x3d,0x4f,0x47};


sbit CE=P3^0;
sbit CSN= P1^4;
sbit SCK=  P1^7;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P3^2;
sbit send_over=P0^0;
sbit recieve_over=P0^1;
sbit begin_tran=P0^2;
sbit init_over=P0^3;
//functions part********//


//****************************************************//
//function name: delay_11us();
//parameter: dull.
//function: to delay 11us,for transmiting.

 void delay_11us(void)
{uchar counter=0;
 for(;counter<5;counter++);
}	

//****************************************************//
//function name: delay_50us();
//parameter: dull.
//function: to delay 50us.

void delay_50us(void)
{
uchar counter=0;
 for(;counter<15;counter++);
}	

//****************************************************//
//function name: delay_200us();
//parameter: dull.
//function: to delay 50us.

void delay_200us(void)
{
uchar counter=0;
 for(;counter<60;counter++);
}

//****************************************************//
//function name: delay_1.5ms();
//parameter: dull.
//function: to delay 1.5ms.

void delay_1p5ms(void)
{
 uchar counter;

 for(counter=0;counter<0x2e;counter++)
    delay_50us();

}	

//****************************************************//
//function name: delay_0.5s();
//parameter: dull.
//function: to delay 1s.

void delay_0p5s(void)
{
 uchar counter1;
 uchar counter2;
 for(counter1=0;counter1<0x20;counter1++)//80*250*50=1*10^6 us
{
 for(counter2=0;counter2<0xfa;counter2++)
    delay_50us();
}
}	

//****************************************************//
//function name: delay_1s();
//parameter: dull.
//function: to delay 1s.

void delay_1s(void)
{
 uchar counter1;
 uchar counter2;
 for(counter1=0;counter1<0x50;counter1++)//80*250*50=1*10^6 us
{
 for(counter2=0;counter2<0xfa;counter2++)
    delay_50us();
}
}	

//****************************************************//
//function name: mcu_init().
//parameter: dull.
//function: to set initial condition of MCU.

void mcu_init(void)	//IO，SPI_init,CE线电平0，各灯齐闪，设定SPI工作模式，CSN线电平,CLK频率
{
 P0=0x0f;		// led close
 CE=0;			// chip enable
 CSN=1;			// Spi disable	
 SCK=0;			// Spi clock line init high
 MOSI=1;
 MISO=1;		//NUST TO WRITE 1,THEN YOU CAN USE IT AS INPUT PIN.
 delay_0p5s();
 P0=0x00;		// led close

}

//****************************************************//
//function name: SPI_DEAL(data)
//parameter: data
//function: operation on SPI department.then return the walue in MISO reg.

uchar SPI_DEAL(uchar byte)
{
uchar bit_ctr;
for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit
{
 MOSI = (byte & 0x80);         // output 'byte', MSB to MOSI
 byte = (byte << 1);  			// shift next bit into MSB..
 SCK = 1; 						// Set SCK high..
 byte |=MISO;       		  // capture current MISO bit
 SCK = 0;            		  // ..then set SCK low again
}
 return (byte);
}
//****************************************************//
//function name: TRAN_COMMOND(commond)
//parameter: commond,the SPI commonds such as,NOP,FLUSH_TX,FLUSH_RX,REUSE_TX_PL.
//function: to read status reg,as the last commonds,return the value in the status reg

uchar TRAN_COMMOND(uchar commond)
{
 uchar temp=0;
 CSN=0;		//命令操作使能
 temp=SPI_DEAL(commond);//发送操作命令，同时返回stature寄存器的值
 CSN=1;		//命令操作结束
 return temp;   //返回值
}

//****************************************************//
//function name: READ_REG(commond)
//parameter: reg,the register whoes value is to be read.
//function:read single byte value  of reg

uchar READ_REG(uchar commond)		//读寄存器的值
{
 uchar temp=0;
 CSN=0;		//命令操作使能
 SPI_DEAL(commond);//发送命令，同时返回stature寄存器的值
 temp=SPI_DEAL(0);//读取相应寄存器的值
 CSN=1;		//命令操作结束
 return temp;//返回值
}

//****************************************************//
//function name: READ_BUF(commond,buf,wid)
//parameter: commond,a special commond to read many bytes from a register.
//           buf, where is the read out bytes to be storeged.
//           wid,the length of bytes
//function: read multi values from a register.

void READ_BUF(uchar commond,uchar *buf,uchar wid)//读rx_payload或者pipe里的地址,多字节,wid 可通过读相应寄存器得出。
{
 uchar temp=0;
 CSN=0;//enable trans commond
 SPI_DEAL(commond);
 for(temp=0;temp<wid;temp++)//lsb first output
 {
 buf[temp]=SPI_DEAL(0);
 }
 CSN=1;//close trans
}

//****************************************************//
//function name: WRITE_REG(commond,data)
//parameter: commond,a special commond to write one byte to a register.
//           data, the value to be written.
//function: write sigle value to a register.

uchar WRITE_REG(uchar commond,uchar dat)
{
 uchar temp;
 CSN=0; 	//命令操作使能
 temp=SPI_DEAL(commond);//发送命令，同时返回stature寄存器的值
 SPI_DEAL(dat);	//write data
 CSN=1;		//命令操作结束
 return temp;   //返回值
}


//****************************************************//
//function name: WRITE_BUF(commond,buf,wid)
//parameter: commond,a special commond to write many bytes to a register.
//           buf, where is the bytes to be writen.
//           wid,the length of bytes
//function: write multi values to a register.

void WRITE_BUF(uchar commond,uchar *buf,uchar wid)//读rx_payload或者pipe里的地址,多字节,wid 可通过读相应寄存器得出。
{
 uchar temp=0;
 CSN=0;
 SPI_DEAL(commond);
 for(temp=0;temp<wid;temp++)//lsb first output,低字节的高位先传送
 {
 SPI_DEAL(buf[temp]);
 }
 CSN=1;
}


//****************************************************//
//function name: RX_INIT_REG().
//parameter: dull.
//function: to set initial condition of a PRX.
void RX_INIT_REG(void)
{
WRITE_REG(W_REG+EN_AA,0x00);//only pipe 0 enabled  Auto Acknowledgment
WRITE_REG(W_REG+EN_RXADDR,0x01);//only pipe 0 enabled rx and tx
WRITE_REG(W_REG+SETUP_AW,0x03);//set Address Widths of tx and rx to be 5 byte
WRITE_REG(W_REG+SETUP_RETR,0x10);//Auto Retransmit Delay time is 500us,disable .15 times retransmission
WRITE_REG(W_REG+RF_CH,0x10);//channel frequency is 2400+(16)RF_CH MHZ
WRITE_REG(W_REG+RF_SETUP,0x0f);//2Mbit/s,0dbm,LAN gain
WRITE_BUF(W_REG+RX_ADDR_P0,pipe0_addr,addr_wid);//pipe 0 adress reset 0xE7E7E7E7E7,maybe 0x7c7c7c7c7c
WRITE_BUF(W_REG+TX_ADDR,pipe0_addr,addr_wid);// ptx address set as pipe0,when ACK enable
WRITE_REG(W_REG+RX_PW_P0,pld_wid);//Number of bytes in RX payload in data pipe 0, value of 0 is indicated of pipe no use
WRITE_REG(W_REG+CONFIG,0x7b);//disable INT of all.enable CRC with 1 byte,power down,prx
}

//****************************************************//
//function name: PRX_INIT()
//parameter: dull
//function: the PRX set of RFIC.

void PRX_INIT(void)
{
 init_over=1;
 RX_INIT_REG();
 CE=1;
 delay_0p5s();
 init_over=0;

}

//****************************************************//
//function name: STATIC_RCV_PCK()
//parameter: static buf is "static_rcv_buf"
//function: receive bytes from RFIC.

void STATIC_RCV_PCK()
{
 READ_BUF(R_RX_PAYLOAD,static_rcv_buf,pld_wid); //read data
 WRITE_REG(W_REG+STATUS,0x40);					//clear rx_dr
}
//****************************************************//
//function name: PRX_HANDLE()
//parameter: dull
//function: the PRX work plan of RFIC.

void PRX_HANDLE(void)
{
	int status=0;
	while(new_data==0)	//wait for rcv over.
		{
			status=TRAN_COMMOND(NOP);
			if(status&0x40)
				{new_data=1;}	//data is received jump out wait.
		}
	if(new_data)//recieve 
		{
			STATIC_RCV_PCK();
			new_data=0;
		}
}

//
//****the PRX will display 0~f

void main()//prx
{
 uchar num=0;

 mcu_init();
 PRX_INIT();
 new_data=0;
 //start_display();//led 0~f
 while(1)
 {
  recieve_over=0;
  PRX_HANDLE();
  P2=led[static_rcv_buf[0]];
  recieve_over=1;
  delay_0p5s();
 /*
 while(new_data==0)	//wait for rcv over.
	{
	 num=TRAN_COMMOND(NOP);
	 if(num&0x40)
	 {new_data=1;}	//data is received.
	}
////
	num=TRAN_COMMOND(NOP);//recieve
  P2=led[num&0x0f];
  num=(num>>4);
  delay_0p5s();
 P2=led[num&0x0f];
 delay_0p5s();
/////
if(new_data)
{
STATIC_RCV_PCK();
new_data=0;
WRITE_REG(W_REG+STATUS,0x40);
*/
/*
  num=TRAN_COMMOND(NOP);
  P2=led[num&0x0f];
  num=(num>>4);
  delay_0p5s();
 P2=led[num&0x0f];
 delay_0p5s();
*/
//recieve_over=1;

//}
 ///
 
 
 // recieve_over=1;
  // recieve_over=0;
  }
}




