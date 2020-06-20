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
���룺
 	DataBuff	:Ϊ��д�������룯�������������ַ
 	ByteQuantity	:ΪҪ��д���ݵ��ֽ�����
	Address 	:ΪEEPROM��Ƭ�ڵ�ַ
 	ControlByte 	:ΪEEPROM�Ŀ����ֽڣ�������ʽΪ(1)(0)(1)(0)(A2)(A1)(A0)(R/W),����R/W=1,
 			 ��ʾ������,R/W=0Ϊд����,A2,A1,A0ΪEEPROM��ҳѡ��Ƭѡ��ַ;
 	EepromType	:Ϊö�ٱ���,��ΪM2401��M24256�е�һ��,�ֱ��Ӧ24C01��24C256;
����� 
 	��������ֵΪһ��λ������������1��ʾ�˴β���ʧЧ��0��ʾ�����ɹ�;
˵���� 	
 	ERRORCOUNTΪ������������������ERRORCOUNT�β���ʧЧ��������ֹ������������1
 	SDA��SCL���û��Զ��壬�����ݶ���ΪP0^0��P0^1;
 	������û����ùܣ�ֻҪ��ֻ�ӳ��������ĳ����в��������Ϳ�����;
***********************************************************************************/
bit IICReadWrite(uchar *DataBuff,	/*DataBuffΪ��д��������/���������*/
   uchar ByteQuantity,	/*ΪҪ��д���ݵ��ֽ�*/
   uint16 Address,	/*ΪEEPROM��Ƭ�ڵ�*/
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
		if(!(ControlByte&0x01))	//д����
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
		else			//������
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
	if(!(ControlByte&0x01)) //���д��������ʱ
	{
		IICDelay(255);
		IICDelay(255);
		IICDelay(255);
		IICDelay(255);
	}
	return(errorflag);
}


/*****************�����Ƕ�IIC���ߵĲ����ӳ���***/
/*****************��������**********************/
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

/*****************ֹͣIIC����****************/
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

/**************���Ӧ��λ*******************/
bit IICRecAck(void)
{
	SCL=0;
	SDA=1;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	CY=SDA; //��Ϊ����ֵ���Ƿ���CY�е�
	SCL=0;
	return(CY);
}

/***************��IIC���߲���Ӧ��*******************/
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

/*****************����IIC���߲���Ӧ��***************/
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

/*******************��IIC����д����*********************/
void IICSendByte(uchar sendbyte)
{
	uchar data j=8;
	for(;j>0;j--)
	{
		SCL=0;
		_nop_();
		_nop_();
		_nop_();
		sendbyte<<=1; //����C51����ʵ�����������ʼ�ջ�ʹCY=sendbyte^7;
		SDA=CY;
		SCL=1;
	}
	SCL=0;
}

/**********************��IIC�����϶������ӳ���**********/
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

/***************һ������ʱ����************************/
void IICDelay(uchar IICDelayCount)
{
	while(IICDelayCount--);
}


