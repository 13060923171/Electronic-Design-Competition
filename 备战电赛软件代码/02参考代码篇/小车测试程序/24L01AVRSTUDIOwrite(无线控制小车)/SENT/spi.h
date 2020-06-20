#include <avr/io.h>
//#include <avr/macros.h>
//#include "12864.h"
#define uchar unsigned char
#define uint unsigned int

/*------------------us��ʱ�ӳ���-----------------------------*/ 
void delayus(unsigned int us) 
{  unsigned int i; 
   us=us*5/4;
   for(i=0;i<us;i++); 
} 	


/*------------------ms��ʱ�ӳ���-----------------------------*/ 
void delayms(unsigned int ms) 
{  unsigned int i,j; 
   for(i=0;i<ms;i++) 
      for(j=0;j<1141;j++); 
} 



//------------------------------------------------------------------------------
//spi��־
#define DDR_SPI DDRB
#define DD_MOSI 2
#define DD_MISO 3
#define DD_SCK 1
#define DD_SS 0
#define CE PB4
#define IRQ PB5
//--------------------------------NRF24L01�ӿڶ���--------------------------------
//------------------------------------------------------------------------------
//#define NRF24L01_MISO          PB3  //����0 
#define   Hign_24L01_MISO    PORTB|=(1 << PB3)
#define   Low_24L01_MISO     PORTB &= ~(1 << PB3)
#define   Read_24L01_MISO    PINB & (1 << PB3)

//#define NRF24L01_MOSI         PB2       //���1
#define   Hign_24L01_MOSI    PORTB |= (1 << PB2) 
#define   Low_24L01_MOSI     PORTB &= ~(1 << PB2)
#define   Read_24L01_MOSI    PINB & (1 << PB2)

//#define NRF24L01_SCK    PB1      //���1
#define   Hign_24L01_SCK    PORTB |= (1 << PB1)  
#define   Low_24L01_SCK     PORTB &= ~(1 << PB1)   
#define   Read_24L01_SCK    PINB & (1 << PB1);

//#define NRF24L01_CSN       PB0       //���1
#define   Low_24L01_CSN     PORTB &= ~(1 << PB0)
#define   Hign_24L01_CSN    PORTB |= (1 << PB0)

//#define NRF24L01_CE         PB3       //���1
#define   Hign_24L01_CE    PORTB |= (1 << PB4) 
#define   Low_24L01_CE     PORTB &= ~(1 << PB4)  
#define   Read_24L01_CE    PINB & (1 << PB4)



//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5   	// ���͵�ַ���ȣ���󳤶�Ϊ5 5*8=40 bit
#define RX_ADR_WIDTH    5   	// ���յ�ַ����
#define TX_PLOAD_WIDTH  32    	// �����ֽڳ��ȣ�
#define RX_PLOAD_WIDTH  32  	// �����ֽڳ���
 uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};  //���͵�ַ
 uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���յ�ַ
//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//*************************************************************


//*************************************************
//*******  void spi_init()  ��ʼ��SPI
//**************************************************
void spi_init()
{
 Hign_24L01_CSN;
 DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS)|(1 << CE);//����MOSI,SCK,SS.CE ΪOUTPUT,����ΪINPUT
 DDR_SPI&=~((1<<DD_MISO)|(1<<IRQ));
 SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);//ʹ��SPI�ӿ�,����ģʽ,MSB��λ��ǰ,ģʽ0,16��Ƶ,SPIʱ��1MHZ
 SPSR=0;
}


//**************************************************
//*******  uchar SPI_RW(uchar date)  ��дSPI
//**************************************************
uchar SPI_RW(uchar date)
{
 SPDR=date;
    while(!(SPSR&(1<<SPIF)));
    return SPDR;
}
 
//**************************************************
//*******uchar SPI_Read(uchar reg)  ��24L01�Ĵ���
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
//          д24L01�Ĵ���
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
//          ��24L01 �Ĵ���BUFF
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
//          д24L01 �Ĵ���BUFF
//**************************************************
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bites)
{
 uchar status1,uchar_ctr;
 
 Low_24L01_CSN;             //SPIʹ��      
 status1 = SPI_RW(reg);  
 for(uchar_ctr=0; uchar_ctr<bites; uchar_ctr++) //
 SPI_RW(*pBuf++);
 Hign_24L01_CSN;           //�ر�SPI
 return(status1);        //
}

//**************************************************
//*******  void SetRX_Mode(void)
//          ����ģʽ����
//**************************************************
void SetRX_Mode(void)
{
 Low_24L01_CE;


 SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //д���յ�ַ
 SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //0ͨ���Զ�Ӧ��
 SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //����ͨ��0
 SPI_RW_Reg(WRITE_REG + RF_CH,0);        //��Ƶͨ��
 SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);//дͨ��0 �������ݳ���
 SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); //0db 1M 
 SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); //����ģʽ
 Hign_24L01_CE;        //��ʼ����
 delayus(200);//ע�ⲻ��̫С
}

//**************************************************
//*******  uchar nRF24L01_RxPacket(uchar* rx_buf)
//          �������ݰ�
//**************************************************
uchar nRF24L01_RxPacket(uchar* rx_buf)
{
    uchar sta,flag=0;
    sta=SPI_Read(STATUS);      // ��ȡ״̬�Ĵ������ж����ݽ���״��  
    if(sta&0x40)                 // �ж��Ƿ���յ�����RX_DR==1?
    {
       Low_24L01_CE;   // StandBy Iģʽ
       SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
       flag =1;   //��ȡ������ɱ�־
    }
    SPI_RW_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
    return (flag);
}

//**************************************************
//*******  void nRF24L01_TxPacket(uchar * tx_buf)
//          �������ݰ�
//**************************************************
void nRF24L01_TxPacket(  uchar * tx_buf)
{ 
//   unsigned char sta=0;
//   unsigned char flag=0;
   Low_24L01_CE;   //StandBy Iģʽ 
   SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); 
   SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
   SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);     // װ������ 
   SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //
   SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //
   SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1f); //500+86us
   SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //
   SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   //
   SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);      // IRQ�շ�����ж���Ӧ��16λCRC��������
   delayms(4);
   Hign_24L01_CE;   //�ø�CE���������ݷ���
   delayus(20);
   Low_24L01_CE;
}

void init_NRF24L01(void)
{
  spi_init();
  Low_24L01_CE;    // ����ģʽ
  Hign_24L01_CSN;   // SPI �ر�
  Low_24L01_SCK;   // �ر�ʱ��
}




