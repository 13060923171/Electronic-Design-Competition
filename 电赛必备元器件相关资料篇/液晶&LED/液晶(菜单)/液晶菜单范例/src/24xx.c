#include <t89c51rd2.h>
#include "24xx.h"

sbit SDA = P0^6;
sbit SCL = P0^7;

enum eepromtype EepromType;

void  IICDelay(uchar DelayCount);
void  IICStart(void);
void  IICStop(void);
bit   IICRecAck(void);
void  IICNoAck(void);
void  IICAck(void);
uchar IICReceiveByte(void);
void  IICSendByte(uchar sendbyte);

/***********************************************************************************
输入：
 	DataBuff	:为读写数据输入／输出缓冲区的首址
 	ByteQuantity	:为要读写数据的字节数量
	Address 	:为EEPROM的片内地址
 	ControlByte 	:为EEPROM的控制字节，具体形式为(1)(0)(1)(0)(A2)(A1)(A0)(R/W),其中R/W=1,
 			 表示读操作,R/W=0为写操作,A2,A1,A0为EEPROM的页选或片选地址;
 	EepromType	:为枚举变量,需为M2401至M24256中的一种,分别对应24C01至24C256;
输出： 
 	函数返回值为一个位变量，若返回1表示此次操作失效，0表示操作成功;
说明： 	
 	ERRORCOUNT为允许最大次数，若出现ERRORCOUNT次操作失效后，则函数中止操作，并返回1
 	SDA和SCL由用户自定义，这里暂定义为P0^0和P0^1;
 	其余的用户不用管，只要把只子程序放在你的程序中并调用它就可以了;
***********************************************************************************/
bit IICReadWrite(uchar *DataBuff,	/*DataBuff为读写数据输入/输出缓冲区*/
   uchar ByteQuantity,	/*为要读写数据的字节*/
   uint16 Address,	/*为EEPROM的片内地*/
   uchar ControlByte,	/**/
   enum eepromtype EepromType)	
{
	uchar data j,i=ERRORCOUNT;
	bit errorflag=1;
	while(i--)
	{
		IICStart();
		IICSendByte(ControlByte&0xfe);
		if(IICRecAck())
		{
			IICStop();
			continue;
		}
		if(EepromType>M2416)
		{
			IICSendByte((uchar)(Address>>8));
			if(IICRecAck())
			{
				IICStop();
				continue;
			}
		}
		IICSendByte((uchar)Address);
		if(IICRecAck())
		{
			IICStop();
			continue;
		}
		if(!(ControlByte&0x01))	//写数据
		{
			j=ByteQuantity;
			errorflag=0; //********clr errorflag
			while(j--)
			{
				IICSendByte(*DataBuff++);
				if(!IICRecAck())
					continue;
				errorflag=1;
				break;
			}
			if(errorflag==1) continue;
			break;
		}
		else			//读数据
		{
			IICStart();
			IICSendByte(ControlByte);
			if(IICRecAck())
			{
				IICStop();
				continue;
			}
			while(--ByteQuantity)
			{
				*DataBuff++=IICReceiveByte();
				IICAck();
			}
			*DataBuff=IICReceiveByte(); //read last byte data
			IICNoAck();
			errorflag=0;
			break;
		}
	}
	IICStop();
	if(!(ControlByte&0x01)) //如果写数据则延时
	{
		IICDelay(255);
		IICDelay(255);
		IICDelay(255);
		IICDelay(255);
	}
	return(errorflag);
}


/*****************以下是对IIC总线的操作子程序***/
/*****************启动总线**********************/
void IICStart(void)
{
	SCL=0; //
	SDA=1;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	SDA=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCL=0;
	SDA=1; //
}

/*****************停止IIC总线****************/
void IICStop(void)
{
	SCL=0;
	SDA=0;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	SDA=1;
	_nop_();
	_nop_();
	_nop_();
	SCL=0;
}

/**************检查应答位*******************/
bit IICRecAck(void)
{
	SCL=0;
	SDA=1;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	CY=SDA; //因为返回值总是放在CY中的
	SCL=0;
	return(CY);
}

/***************对IIC总线产生应答*******************/
void IICACK(void)
{
	SDA=0;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCL=0;
	_nop_();
	SDA=1;
}

/*****************不对IIC总线产生应答***************/
void IICNoAck(void)
{
	SDA=1;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCL=0;
}

/*******************向IIC总线写数据*********************/
void IICSendByte(uchar sendbyte)
{
	uchar data j=8;
	for(;j>0;j--)
	{
		SCL=0;
		_nop_();
		_nop_();
		_nop_();
		sendbyte<<=1; //无论C51怎样实现这个操作，始终会使CY=sendbyte^7;
		SDA=CY;
		SCL=1;
	}
	SCL=0;
}

/**********************从IIC总线上读数据子程序**********/
uchar IICReceiveByte(void)
{
	register receivebyte,i=8;
	SCL=0;
	while(i--)
	{
		SCL=1;
		receivebyte=(receivebyte<<1)|SDA;
		SCL=0;
	}
	return(receivebyte);
}

/***************一个简单延时程序************************/
void IICDelay(uchar IICDelayCount)
{
	while(IICDelayCount--);
}


