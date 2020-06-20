#include <iom16v.h>
#include <macros.h>
#define uchar unsigned char
#define uint unsigned int
#define LcdBus PORTC
	
const uchar rs=3;
const uchar rw=4;
const uchar en=5;
const uchar psb=6;
const uchar rst=7;


/*------------------us��ʱ�ӳ���-----------------------------*/ 
void delayus(uint us) 
{  uint i; 
   us=us*5/4;
   for(i=0;i<us;i++); 
} 	


/*------------------ms��ʱ�ӳ���-----------------------------*/ 
void delayms(uint ms) 
{  uint i,j; 
   for(i=0;i<ms;i++) 
      for(j=0;j<1141;j++); 
} 


/*------------------æ���--------------------------*/ 
void ChkBusy(void)
{
	DDRC=0X00;		//����Ϊ����
	PORTC=0X00;
	PORTD &=~BIT(rs);
	PORTD |=BIT(rw);
	PORTD |=BIT(en);
	while(PINC & 0X80);
	PORTD &=~BIT(en);
	DDRC=0XFF;		//����Ϊ���
}

/*------------------��������--------------------------*/   
void clrscreen(void) 
{ 
    write_com(0x01); 
    delayms(2); 
} 


/*------------------��ʼ��LCD--------------------------*/   
void LcdIni(void)
{
	DDRC=0XFF;
	PORTC=0XFF;
	DDRD=0XFF;
	PORTD=0XFF;
	
	PORTD &= ~BIT(rst);
	NOP();
	PORTD |=BIT(rst);		     //��λ
	
	delayms(5);
	write_com(0x30);             //ѡ�����ָ�,ѡ��8bit������
	delayms(5);
	write_com(0x01);             //�����ʾ�������趨��ַָ��Ϊ00H
	delayms(5);
	write_com(0x06);             //ָ�������ϵĶ�ȡ��д��ʱ���趨�α���ƶ�����ָ����ʾ����λ
	delayms(5);	
	write_com(0x0c);             //����ʾ(���αꡢ������)
}


/*------------------дָ��--------------------------*/ 
void write_com(uchar com)
{
	ChkBusy();
	PORTD &=~BIT(rs);
	PORTD &=~BIT(rw);
	PORTD &=~BIT(en);
	PORTC =com;
	PORTD |=BIT(en);
    delayus(10);
	PORTD &=~BIT(en);
}


/*------------------д����--------------------------*/ 
void write_data(uchar dat)
{
	ChkBusy();
	PORTD |=BIT(rs);
	PORTD &=~BIT(rw);
	PORTD &=~BIT(en);
	PORTC=dat;
	PORTD |=BIT(en);
	delayus(10);
	PORTD &=~BIT(en);
}

