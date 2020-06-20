#include <avr/io.h>
//#include <avr/macros.h>
//#include "12864.h"
#define uchar unsigned char
#define uint unsigned int

/*------------------us延时子程序-----------------------------*/ 
void delayus(unsigned int us) 
{  unsigned int i; 
   us=us*5/4;
   for(i=0;i<us;i++); 
} 	


/*------------------ms延时子程序-----------------------------*/ 
void delayms(unsigned int ms) 
{  unsigned int i,j; 
   for(i=0;i<ms;i++) 
      for(j=0;j<1141;j++); 
} 



//------------------------------------------------------------------------------
//spi标志
#define DDR_SPI DDRB
#define DD_MOSI 2
#define DD_MISO 3
#define DD_SCK 1
#define DD_SS 0
#define CE PB4
#define IRQ PB5
//--------------------------------NRF24L01接口定义--------------------------------
//------------------------------------------------------------------------------
//#define NRF24L01_MISO          PB3  //输入0 
#define   Hign_24L01_MISO    PORTB|=(1 << PB3)
#define   Low_24L01_MISO     PORTB &= ~(1 << PB3)
#define   Read_24L01_MISO    PINB & (1 << PB3)

//#define NRF24L01_MOSI         PB2       //输出1
#define   Hign_24L01_MOSI    PORTB |= (1 << PB2) 
#define   Low_24L01_MOSI     PORTB &= ~(1 << PB2)
#define   Read_24L01_MOSI    PINB & (1 << PB2)

//#define NRF24L01_SCK    PB1      //输出1
#define   Hign_24L01_SCK    PORTB |= (1 << PB1)  
#define   Low_24L01_SCK     PORTB &= ~(1 << PB1)   
#define   Read_24L01_SCK    PINB & (1 << PB1);

//#define NRF24L01_CSN       PB0       //输出1
#define   Low_24L01_CSN     PORTB &= ~(1 << PB0)
#define   Hign_24L01_CSN    PORTB |= (1 << PB0)

//#define NRF24L01_CE         PB3       //输出1
#define   Hign_24L01_CE    PORTB |= (1 << PB4) 
#define   Low_24L01_CE     PORTB &= ~(1 << PB4)  
#define   Read_24L01_CE    PINB & (1 << PB4)



//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5   	// 发送地址长度，最大长度为5 5*8=40 bit
#define RX_ADR_WIDTH    5   	// 接收地址长度
#define TX_PLOAD_WIDTH  32    	// 发送字节长度，
#define RX_PLOAD_WIDTH  32  	// 接收字节长度
 uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};  //发送地址
 uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //接收地址
//***************************************NRF24L01寄存器指令*******************************************************
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
//*************************************************************


//*************************************************
//*******  void spi_init()  初始化SPI
//**************************************************
void spi_init()
{
 Hign_24L01_CSN;
 DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS)|(1 << CE);//设置MOSI,SCK,SS.CE 为OUTPUT,其它为INPUT
 DDR_SPI&=~((1<<DD_MISO)|(1<<IRQ));
 SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);//使能SPI接口,主机模式,MSB低位在前,模式0,16分频,SPI时钟1MHZ
 SPSR=0;
}


//**************************************************
//*******  uchar SPI_RW(uchar date)  读写SPI
//**************************************************
uchar SPI_RW(uchar date)
{
 SPDR=date;
    while(!(SPSR&(1<<SPIF)));
    return SPDR;
}
 
//**************************************************
//*******uchar SPI_Read(uchar reg)  读24L01寄存器
//**************************************************
uchar SPI_Read(uchar reg)
{
 uchar reg_val;
 
 Low_24L01_CSN;               
 SPI_RW(reg);            
 reg_val = SPI_RW(0);    
 Hign_24L01_CSN;               
 
 return(reg_val);       
}

//**************************************************
//*******  uchar SPI_RW_Reg(uchar reg, uchar value) 
//          写24L01寄存器
//**************************************************

uchar SPI_RW_Reg(uchar reg, uchar value)
{
 uchar status;
 
 Low_24L01_CSN;                 
 status = SPI_RW(reg);     
 SPI_RW(value);            
 Hign_24L01_CSN;                   
 
 return(status);           
}

//**************************************************
//*******  uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bites) 
//          读24L01 寄存器BUFF
//**************************************************
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bites)
{
 uint status1,uchar_ctr;
 
 Low_24L01_CSN;                     
 status1 = SPI_RW(reg);         
 
 for(uchar_ctr=0;uchar_ctr<bites;uchar_ctr++)
  pBuf[uchar_ctr] = SPI_RW(0);   
 
 Hign_24L01_CSN;                          
 
 return(status1);                    
}

//**************************************************
//*******  uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bites)
//          写24L01 寄存器BUFF
//**************************************************
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bites)
{
 uchar status1,uchar_ctr;
 
 Low_24L01_CSN;             //SPI使能      
 status1 = SPI_RW(reg);  
 for(uchar_ctr=0; uchar_ctr<bites; uchar_ctr++) //
 SPI_RW(*pBuf++);
 Hign_24L01_CSN;           //关闭SPI
 return(status1);        //
}

//**************************************************
//*******  void SetRX_Mode(void)
//          接收模式设置
//**************************************************
void SetRX_Mode(void)
{
 Low_24L01_CE;


 SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //写接收地址
 SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //0通道自动应答
 SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //数据通道0
 SPI_RW_Reg(WRITE_REG + RF_CH,0);        //射频通道
 SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);//写通道0 接受数据长度
 SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); //0db 1M 
 SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); //接收模式
 Hign_24L01_CE;        //开始接收
 delayus(200);//注意不能太小
}

//**************************************************
//*******  uchar nRF24L01_RxPacket(uchar* rx_buf)
//          接收数据包
//**************************************************
uchar nRF24L01_RxPacket(uchar* rx_buf)
{
    uchar sta,flag=0;
    sta=SPI_Read(STATUS);      // 读取状态寄存其来判断数据接收状况  
    if(sta&0x40)                 // 判断是否接收到数据RX_DR==1?
    {
       Low_24L01_CE;   // StandBy I模式
       SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
       flag =1;   //读取数据完成标志
    }
    SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
    return (flag);
}

//**************************************************
//*******  void nRF24L01_TxPacket(uchar * tx_buf)
//          发送数据包
//**************************************************
void nRF24L01_TxPacket(  uchar * tx_buf)
{ 
//   unsigned char sta=0;
//   unsigned char flag=0;
   Low_24L01_CE;   //StandBy I模式 
   SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); 
   SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
   SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);     // 装载数据 
   SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //
   SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //
   SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1f); //500+86us
   SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //
   SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   //
   SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);      // IRQ收发完成中断响应，16位CRC，主发送
   delayms(4);
   Hign_24L01_CE;   //置高CE，激发数据发送
   delayus(20);
   Low_24L01_CE;
}

void init_NRF24L01(void)
{
  spi_init();
  Low_24L01_CE;    // 空闲模式
  Hign_24L01_CSN;   // SPI 关闭
  Low_24L01_SCK;   // 关闭时钟
}




