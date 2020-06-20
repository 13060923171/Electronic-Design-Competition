#include <reg52.h>
#include <absacc.h>
#include <intrins.h>
#include <string.h>

#include "lcd.h"
#include "font.h"

sbit LCD_WR =P3^5;  //Data Write into T6963C,L��Ч
sbit LCD_RD =P3^6;  //Data Read from T6963C,L��Ч
sbit LCD_CE =P3^4;  //ʹ���źţ�L��Ч
sbit LCD_CD =P3^7;  //��wr=L,LCD_CD=H:д����,LCD_CD=L:д����;��rd=L,LCD_CD=H:��״̬,LCD_CD=L:������
sbit LCD_RST=P3^3;  //Lcm reset,����Ч
sbit LCD_FS =P3^2;  //����ѡ��,H:6*8����;L:8*8����

sbit bf0 = P1^0;
sbit bf1 = P1^1;
sbit bf3 = P1^3;

uchar code HexTable[]="0123456789ABCEDF";

uchar code Num_Tab[]=
{
	/* 48 0x30 '0' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00   000 */
	0x44, /* 0 000 00 */
	0x4c, /* 0 00  00 */
	0x54, /* 0 0 0 00 */
	0x64, /* 0  00 00 */
	0x44, /* 0 000 00 */
	0x38, /* 00   000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 49 0x31 '1' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x08, /* 0000 000 */
	0x18, /* 000  000 */
	0x08, /* 0000 000 */
	0x08, /* 0000 000 */
	0x08, /* 0000 000 */
	0x08, /* 0000 000 */
	0x1c, /* 000   00 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 50 0x32 '2' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00   000 */
	0x44, /* 0 000 00 */
	0x04, /* 00000 00 */
	0x08, /* 0000 000 */
	0x10, /* 000 0000 */
	0x20, /* 00 00000 */
	0x7c, /* 0     00 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 51 0x33 '3' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00   000 */
	0x44, /* 0 000 00 */
	0x04, /* 00000 00 */
	0x18, /* 000  000 */
	0x04, /* 00000 00 */
	0x44, /* 0 000 00 */
	0x38, /* 00   000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 52 0x34 '4' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x08, /* 0000 000 */
	0x18, /* 000  000 */
	0x28, /* 00 0 000 */
	0x48, /* 0 00 000 */
	0x7c, /* 0     00 */
	0x08, /* 0000 000 */
	0x1c, /* 000   00 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 53 0x35 '5' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 0     00 */
	0x40, /* 0 000000 */
	0x78, /* 0    000 */
	0x04, /* 00000 00 */
	0x04, /* 00000 00 */
	0x44, /* 0 000 00 */
	0x38, /* 00   000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 54 0x36 '6' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00   000 */
	0x40, /* 0 000000 */
	0x78, /* 0    000 */
	0x44, /* 0 000 00 */
	0x44, /* 0 000 00 */
	0x44, /* 0 000 00 */
	0x38, /* 00   000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 55 0x37 '7' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 0     00 */
	0x04, /* 00000 00 */
	0x04, /* 00000 00 */
	0x08, /* 0000 000 */
	0x10, /* 000 0000 */
	0x10, /* 000 0000 */
	0x10, /* 000 0000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 56 0x38 '8' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00   000 */
	0x44, /* 0 000 00 */
	0x44, /* 0 000 00 */
	0x38, /* 00   000 */
	0x44, /* 0 000 00 */
	0x44, /* 0 000 00 */
	0x38, /* 00   000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 57 0x39 '9' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00   000 */
	0x44, /* 0 000 00 */
	0x44, /* 0 000 00 */
	0x44, /* 0 000 00 */
	0x3c, /* 00    00 */
	0x04, /* 00000 00 */
	0x38, /* 00   000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

};
//6x10�ķ���
uchar code BlockTab[]={
	/* 57 0x39 '9' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7C, /* 0     00 */
	0x7C, /* 0     00 */
	0x7C, /* 0     00 */
	0x7C, /* 0     00 */
	0x7C, /* 0     00 */
	0x7C, /* 0     00 */
	0x7C, /* 0     00 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	//empty block
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
};






void LCD_Busy (uchar autowr)    //��״̬
{
  	LCD_CE = 0;
  	LCD_DATA_BUS=0xff;
  	LCD_CD=1;
  	LCD_WR=1;
  	LCD_RD=0;
  	if(autowr)
    	{
    		while(bf3==0)
    			;
    	}
  	else
    	{
    		while((bf0==0)||(bf1==0))
    			;
     	}
  	LCD_RD=1;
}
uchar LCD_BusyStatus ()    //��״̬
{
	uchar dat;
  	LCD_CE = 0;
	LCD_CD=1;
	LCD_RD=1;
	LCD_WR=1;
  	LCD_DATA_BUS=0xff;
	LCD_RD=0;
	dat=LCD_DATA_BUS;
	LCD_RD=1;
  	return dat;
}

/* ����,ָ���д�б� */
void LCD_CheckBusy1(void)  
{
	while ((LCD_BusyStatus()&3)!=3) {}
}
/* �����Զ����б� */
/*
void LCD_CheckBusy2(void) 
{				
	while ((LCD_BusyStatus()&4)!=4) {}
} 
*/
/* �����Զ�д�б� */
/*
void LCD_CheckBusy3(void) 
{
	while ((LCD_BusyStatus()&8)!=8) {}
}
*/
/*------------д��������ݵ�LCD--------------*/
void LCD_Write1 (uchar dat,uchar comm)       //дһ�����ݺ�һ������
{
  	LCD_Data(dat);
  	LCD_Comm(comm);
}
void LCD_Write2 (uchar datl,uchar dath,uchar comm)  //д�������ݺ�һ������
{
  	LCD_Data(datl);
  	LCD_Data(dath);
  	LCD_Comm(comm);
}
void LCD_WriteInt (uint dat,uchar comm)       //дһ��16�������ݺ�һ������
{
  	uchar datl,dath;
  	datl=dat;
  	dath=dat>>8;
  	LCD_Data(datl);
  	LCD_Data(dath);
  	LCD_Comm(comm);
}
void LCD_AutoWrite (uchar dat)               //�Զ�д����
{
  	LCD_CE = 0;
  	LCD_Busy (1);
  	LCD_CD=0;
  	LCD_RD=1;
  	LCD_DATA_BUS=dat;
  	LCD_WR=0;
  	LCD_WR=1;
}
/*	������	*/
uchar LCD_Read(void) 
{
	uchar dat;
  	LCD_CE = 0;
	LCD_CheckBusy1();
	LCD_CD=0;
	LCD_DATA_BUS = 0xff;
	LCD_RD=0;
	dat=LCD_DATA_BUS;
	LCD_RD=1;
	LCD_CD=1;
	return dat;
}
/*	�Զ�������	*/
/*
uchar LCD_AutoRead(void) 
{
	uchar dat;
  	LCD_CE = 0;
	LCD_CheckBusy2();
	LCD_CD=0;
	LCD_DATA_BUS = 0xff;
	LCD_RD=0;
	dat=LCD_DATA_BUS;
	LCD_RD=1;
	LCD_CD=1;
	return dat;
}
*/
void LCD_Comm (uchar comm)       //д����
{
  	LCD_CE = 0;
  	LCD_Busy (0);
  	LCD_CD=1;
  	LCD_RD=1;
  	LCD_DATA_BUS=comm;
  	LCD_WR=0;
  	LCD_WR=1;
}
void LCD_Data (uchar dat)       //д����
{
  	LCD_CE = 0;
  	LCD_Busy (0);
  	LCD_CD=0;
  	LCD_RD=1;
  	LCD_DATA_BUS=dat;
  	LCD_WR=0;
  	LCD_WR=1;
}

/*------------------��ʼ��-----------------*/
void LCD_Init (void) 
{
  	//LCD_RST=0;
  	//;
  	//LCD_RST=1;
  	//LCD_FS=0;
  	//LCD_CE=0;
  	
  	//LCD_CE = 0;
  	LCD_WR=1;
  	LCD_RD=1;
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR,0x40); 	//�ı���ʾ���׵�ַ
  	LCD_WriteInt(LCD_GRAPH_HOME_ADDR,0x42); //ͼ����ʾ���׵�ַ
  	LCD_Write2(LCD_WIDTH,0x00,0x41);        //�ı���ʾ�����
  	LCD_Write2(LCD_WIDTH,0x00,0x43);        //ͼ����ʾ�����
  	LCD_Comm(0x81);                        	//�߼�"���"
  	LCD_Write2(0x02,0x00,0x22);             //CGRAMƫ�õ�ַ����
  	LCD_Comm(0x9c);                        	//�����ı���ʾ,����ͼ����ʾ
  	//LCD_Comm(0x98);                        	//�����ı���ʾ,����ͼ����ʾ
  	
  	LCD_Cls();
}

/********************************************************/
/* ����							*/
/********************************************************/
void LCD_Cls(void) 
{
	uint  i;

  	//LCD_CE = 0;
	//LCD_Write2(0x00,0x00,LC_ADD_POS);// �õ�ַָ��Ϊ���㿪ʼ
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR,LC_ADD_POS);
	LCD_Comm(LC_AUT_WR);		// �Զ�д
	for(i=0;i< 144*LCD_WIDTH;i++)	// ��һ��
	{
		LCD_Data(0x00);		// д����0x00
	}
	LCD_Comm(LC_AUT_OVR);		// �Զ�д����
	LCD_Write2(0x00,0x00,LC_ADD_POS);// ���õ�ַָ��
}

void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
{
	uint addr;
	uchar h,w;
  	uchar i,j;
	
	//x����Ҫ8�ֽڶ���
	x1 =(x1 / 8) * 8;
	x2 =(x2 / 8) * 8;
	
	h = y2 - y1;
	w = (x2 - x1)/8;
	
	addr = LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y1 + x1/8;
	
  	for(j=0;j<h;j++)
  	{
  		LCD_WriteInt(addr,0x24);
		LCD_Comm(0xb0);
    		for(i=0;i<w;i++)
    		{
	 		LCD_AutoWrite(0x00);
    		}
		LCD_Comm(0xb2);
		addr += LCD_WIDTH;
  	}
}


/*	�趨ͼ��x,yֵ*/
/*
void LCD_GraphSetAddr(uchar x,uchar y) 
{
	uint xy;
	xy=y;
	xy=xy*16+x+256;
  	//LCD_CE = 0;
	LCD_Write2(xy&0xff,xy/256,0x24);
}
*/
/*	�趨�ı�x,yֵ	*/
/*
void LCD_TextSetAddr(uchar x,uchar y) 
{
  	//LCD_CE = 0;
	LCD_Write2(y*16+x,0,0x24);
}
*/
/*	���һ��*/
/*
void LCD_ClrPixel(uchar x,uchar y) 
{
	uchar b;
	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;

	b = 7 - (x % 8);
	
	LCD_WriteInt(addr,0x24);
	LCD_Comm(0xf0|b);
}
*/
/*	����һ��	*/

void LCD_Putpixel(uchar x,uchar y) 
{
	uchar b;
	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;

	b = 7 - (x % 8);
	
	LCD_WriteInt(addr,0x24);
	LCD_Comm(0xf8|b);
}

/*	x,y����ʾ���	*/
/*
void LCD_ShowCursor(uchar x,uchar y) 
{
  	//LCD_CE = 0;
	LCD_Comm(0x97);	//��꿪
	LCD_Write2(x,y,0x21);
}
*/
/*	ȡ�����	*/
/*
void LCD_HideCursor(void)  
{	
  	//LCD_CE = 0;
	LCD_Comm(0x9c);	
}
*/
void LCD_PutImg(uchar x,uchar y,uchar w,uchar h,uchar *img) 
{
	uint data addr;
  	uchar data i,j;
  	uchar data c;
	
	addr = LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y + (x >> 3);
	
  	//LCD_CE = 0;
  	for(j=0;j<h;j++)
  	{
  		LCD_WriteInt(addr,0x24);
		LCD_Comm(0xb0);
    		for(i=0;i<w;i++)
    		{
    			c = img[j*w+i] ;
	 		LCD_AutoWrite(c);
   				
	  		//LCD_WriteInt(addr +i,0x24);	//+ LCD_WIDTH *j
      			//LCD_Write1(c,0xc0);
    		}
		LCD_Comm(0xb2);
		addr += LCD_WIDTH;
  	}
}
/*
void LCD_PrintNumStr(uchar x,uchar y,uchar *s) 
{
	x =(x / 8) * 8;

	while(*s)
	{
		LCD_PutImg(x,y,1,11,Num_Tab + (*s - '0') * 11);
		
		x = x + 8;
		
		s++;
	}
}


void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty)
{
	x =(x / 8) * 8;
	LCD_PutImg(x,y,1,11,BlockTab + (not_empty?0:1) * 11);
}
*/
void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h)
{
	uint data addr;
  	uchar data i,j;
  	uchar data c;
	
	x =(x / 8) * 8;
	addr = LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y + x/8;
	
  	for(j=0;j<h;j++)
  	{
    		for(i=0;i<w;i++)
    		{
	  		LCD_WriteInt(addr +i,0x24);	
	  		LCD_Comm(LC_NOC_RD);
	  		c = LCD_Read();
	  		c = ~c;

      			LCD_Write1(c,LC_NOC_WR);
    		}
		addr += LCD_WIDTH;
  	}
}
/*
void LCD_PrintWord(uchar x,uchar y,uint16 n,uchar start) 
{
	uchar buf[4];
	int i;
	
	if(start > 3) start = 0;
	
	for(i = 0; i < 4; i++)
	{
		buf[i] = n % 10;   
		n /= 10;
	}
	x =(x / 8) * 8;
	
	for(i = 3-start; i >= 0; i--)
	{
		LCD_PutImg(x,y,1,11,Num_Tab + buf[i] * 11);
		
		x = x + 8;
	}
}
void LCD_PrintHex(uchar x,uchar y,uchar hex) 
{
	uchar c1,c2;
	//��4λ
	c1 = hex & 0x0f;	//lo
	//��4λ
	c2 = (hex >> 4) & 0x0f; //hi

	LCD_PutImg(x,y,1,11,Num_Tab + c2 * 11);
	LCD_PutImg(x+8,y,1,11,Num_Tab + c1 * 11);
}

void LCD_GrapPutchar(uchar x,uchar y,uchar num) 
{
	uchar i,j;
	uchar dot;
	num = (num- '0')*9;
  	//LCD_CE = 0;
	for(i=0; i< 12;i++)
	{
		dot = Num_Tab[num + i];
		for(j = 0; j < 7; j++)
		{
			if( dot & 0x80)
				LCD_Putpixel(x+j,y);
			else
				LCD_ClrPixel(x+j,y);
				
			dot <<= 1;
		}
		y++;
	}
}

void LCD_GrapPrint(uchar x,uchar y,uchar code *s) 
{
	while(*s)
	{
		LCD_GrapPutchar(x,y,*s);
		x += 8;
		s++;
	}
}
*/
/*--------------��ʾ�ַ�------------------*/
/*
void LCD_TextPutchar(uchar x,uchar y,uchar c) 
{
  	//LCD_CE = 0;
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
  	LCD_Comm(0xb0);
	LCD_AutoWrite(c - 0x20);
  	LCD_Comm(0xb2);
}

void LCD_TextPrint(uchar x,uchar y,char *s) 
{
  	//LCD_CE = 0;
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
  	LCD_Comm(0xb0);
  	while(*s)
  	{
		LCD_AutoWrite(*s - 0x20);
		s++;
	}
  	LCD_Comm(0xb2);
}
void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start) 
{
	uchar buf[4];
	int i;
	
	if(start > 3) start = 0;
	
	for(i = 0; i < 4; i++)
	{
		buf[i] = n % 10;   
		n /= 10;
	}
	
	for(i = 3-start; i >= 0; i--)
	{
		LCD_TextPutchar(x,y,'0' + buf[i]);
		x ++;
	}
}

void LCD_TextPrintHex(uchar x,uchar y,uchar hex) 
{
	uchar c1,c2;
	//��4λ
	c1 = hex & 0x0f;	//lo
	//��4λ
	c2 = (hex >> 4) & 0x0f; //hi
	
	LCD_TextPutchar(x,y,HexTable[c2]);
	LCD_TextPutchar(x+1,y,HexTable[c1]);
}
*/
/************************************************/
/*���ߡ����ⷽ���б��,ֱ����ѧ���� aX+bY=1	*/
/************************************************/
/*
void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt) 
{
	register uchar t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x = xt-x0;				//������������
	delta_y = yt-y0;
	uRow = x0;
	uCol = y0;
	if(delta_x>0) incx=1;				//���õ�������
	else if( delta_x==0 ) incx=0;			//��ֱ��
	else {incx=-1;delta_x=-delta_x;}

	if(delta_y>0) incy=1;
	else if( delta_y==0 ) incy=0;			//ˮƽ��
	else {incy=-1;delta_y=-delta_y;}

	if( delta_x > delta_y )	distance=delta_x;	//ѡȡ��������������
	else distance=delta_y;

  	//LCD_CE = 0;
	for( t=0;t <= distance+1; t++ )
        {					//�������
		LCD_Putpixel(uRow,uCol);			//����
		xerr +=	delta_x	;
		yerr +=	delta_y	;
		
		if( xerr > distance )
               	{
			xerr-=distance;
			uRow+=incx;
		}
		if( yerr > distance )
               	{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
*/

void LCD_LineH(uchar y) 
{
	char i;
	
  	//LCD_CE = 0;
  	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
	LCD_Comm(0xb0);
	for(i=0;i<LCD_WIDTH;i++)
 		LCD_AutoWrite(0xff);
	LCD_Comm(0xb2);
}
void LCD_LineV(uchar x,uchar y1,uchar y2) 
{
	int i;
  	//LCD_CE = 0;
	for(i = y1; i < y2; i++)
	{
		LCD_Putpixel(x,i);		
	}
	
}


void LCD_LineXX(uchar x1,uchar x2,uchar y)
{
	int i;
	for(i = x1; i < x2; i++)
	{
		LCD_Putpixel(i,y);		
	}
}
/*
void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
{
	LCD_LineXX(x1,x2,y1);
	LCD_LineXX(x1,x2,y2);
	LCD_LineV(x1,y1,y2);
	LCD_LineV(x2,y1,y2);
}

*/

FNT_GB12 code *GetHzk12(uchar c1,uchar c2)
{
	int i = 0;

	while(HZK12[i].Index[0] != 0)
	{
		if(c1 == HZK12[i].Index[0] && c2 == HZK12[i].Index[1])
			return &HZK12[i];
		i ++;
	}
	return NULL;
}


FNT_GB16 code *GetHzk16(uchar c1,uchar c2)
{
	int i = 0;

	while(HZK16[i].Index[0] != 0)
	{
		if(c1 == HZK16[i].Index[0] && c2 == HZK16[i].Index[1])
			return &HZK16[i];
		i ++;
	}
	return NULL;
}

void LCD_PrintHz12(uchar x,uchar y,uchar *s)
{
	uchar c1,c2;
	//x���������8λ����
	x =(x / 8) * 8;
	while(*s)
	{
		c1 = *s++;
		if(c1 == ' ')
			x += 8;
		else if( c1 > 0x80)
		{
			//����
			c2 = *s++;
			LCD_PutImg(x,y+2,2,12,GetHzk12(c1,c2)->Msk);
			x += 16;	
		}
		else	//ASCII
		{
			LCD_PutImg(x,y,1,16,ASC8x16[c1]);	//-
			x += 8;
		}
	}
}


void LCD_PrintHz16(uchar x,uchar y,uchar *s)
{
	uchar c1,c2;
	//x���������8λ����
	x =(x / 8) * 8;
	while(*s)
	{
		c1 = *s++;
		if(c1 == ' ')
		{
			x += 8;
		}
		else if( c1 > 0x80)
		{
			//����
			c2=*s++;
			LCD_PutImg(x,y,2,16,GetHzk16(c1,c2)->Msk);
			x += 16;	
		}
		else	//ASCII
		{
			LCD_PutImg(x,y+2,1,16,ASC8x16[c1]);	//-
			x += 8;
		}
	}
}

//��ʾ6x8������
void LCD_Print6X8(uchar x, uchar y,uchar *s)
{
	//x���������8λ����
	x =(x / 8) * 8;
	while(*s)
	{
		LCD_PutImg(x,y,1,8,font6x8[*s - 32]);	//-
		x += 8;

		s ++;
	}
}

//��ʾ24x32������
void LCD_Print24X32(uchar x, uchar y,uchar *s)
{
	//x���������8λ����
	x =(x / 8) * 8;
	while(*s)
	{
		if( *s >= '0' && *s <= '9')	//��ʾ
		{
			LCD_PutImg(x,y,3,32,Font24X32[*s-'0']);
			x += 24;
		}
		else if( *s == ' ')
		{
			x += 8;
		}
		else
		{
			LCD_PutImg(x,y+16,1,16,ASC8x16[*s]);	//-
			x += 8;
		}
		s ++;
	}
}