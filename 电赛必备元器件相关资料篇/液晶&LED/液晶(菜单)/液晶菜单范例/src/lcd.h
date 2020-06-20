#ifndef __LCD_H
#define __LCD_H

#include "typedef.h"

#define LCD_WIDTH      		16      //��ʾ����� 16 * 8 = 128 dot
#define LCD_HEIGHT		64	//64 dot
#define LCD_TEXT_HOME_ADDR     	0x0000  //�ı���ʾ���׵�ַ
#define LCD_GRAPH_HOME_ADDR     ((LCD_HEIGHT/8)*30)  //ͼ����ʾ���׵�ַ
#define LCD_DATA_BUS   		P1      //MCU P1<------> LCM

// ASCII�ַ����ƴ�����Ͷ���
#define STX	0x02
#define ETX	0x03
#define EOT	0x04
#define ENQ	0x05
#define BS	0x08
#define CR	0x0D
#define LF	0x0A
#define DLE	0x10
#define ETB	0x17
#define SPACE	0x20
#define COMMA	0x2C


// T6963C �����
#define LC_CUR_POS	0x21		// ���λ������
#define LC_CGR_POS	0x22		// CGRAMƫ�õ�ַ����
#define LC_ADD_POS	0x24		// ��ַָ��λ��
#define LC_TXT_STP	0x40		// �ı�����ַ
#define LC_TXT_WID	0x41		// �ı������
#define LC_GRH_STP	0x42		// ͼ������ַ
#define LC_GRH_WID	0x43		// ͼ�������
#define LC_MOD_OR	0x80		// ��ʾ��ʽ���߼�����
#define LC_MOD_XOR	0x81		// ��ʾ��ʽ���߼������
#define LC_MOD_AND	0x82		// ��ʾ��ʽ���߼����롱
#define LC_MOD_TCH	0x83		// ��ʾ��ʽ���ı�����
#define LC_DIS_SW	0x90		// ��ʾ���أ�D0=1/0:�����˸����/���ã�
					// D1=1/0:�����ʾ����/���ã�
					// D2=1/0:�ı���ʾ����/���ã�
					// D3=1/0:ͼ����ʾ����/���ã�
#define LC_CUR_SHP	0xA0		// �����״ѡ��0xA0-0xA7��ʾ���ռ������
#define LC_AUT_WR	0xB0		// �Զ�д����
#define LC_AUT_RD	0xB1		// �Զ�������
#define LC_AUT_OVR	0xB2		// �Զ���/д����
#define LC_INC_WR	0xC0		// ����һ��д����ַ��1
#define LC_INC_RD	0xC1		// ����һ�ζ�����ַ��1
#define LC_DEC_WR	0xC2		// ����һ��д����ַ��1
#define LC_DEC_RD	0xC3		// ����һ�ζ�����ַ��1
#define LC_NOC_WR	0xC4		// ����һ��д����ַ����
#define LC_NOC_RD	0xC5		// ����һ�ζ�����ַ����
#define LC_SCN_RD	0xE0		// ����
#define LC_SCN_CP	0xE8		// ������
#define LC_BIT_OP	0xF0		// λ������
					// D0-D2������D0-D7λ��D3��1��λ��0�����


void LCD_Busy (uchar autowr) ;    //��״̬
uchar LCD_BusyStatus () ;    //��״̬
void LCD_CheckBusy1(void) ;
//void LCD_CheckBusy2(void) ;
//void LCD_CheckBusy3(void) ;
void LCD_Write1 (uchar dat,uchar comm) ;       //дһ�����ݺ�һ������
void LCD_Write2 (uchar datl,uchar dath,uchar comm) ;  //д�������ݺ�һ������
void LCD_WriteInt (uint dat,uchar comm) ;       //дһ��16�������ݺ�һ������
void LCD_AutoWrite (uchar dat) ;               //�Զ�д����
uchar LCD_Read(void) ;
//uchar LCD_AutoRead(void) ;
void LCD_Comm (uchar comm) ;       //д����
void LCD_Data (uchar dat) ;       //д����
void LCD_Init (void) ;
void LCD_Cls (void) ;
void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2);
//void LCD_GraphSetAddr(uchar x,uchar y) ; /*��ַ*/
//void LCD_TextSetAddr(uchar x,uchar y) ; /*��ַ*/
//void LCD_ClrPixel(uchar x,uchar y) ; 
//void LCD_Putpixel(uchar x,uchar y) ;
//void LCD_ShowCursor(uchar x,uchar y) ;/*���*/
//void LCD_HideCursor(void) ;
void LCD_LineH(uchar y) ;
void LCD_LineV(uchar x,uchar y1,uchar y2) ;
void LCD_LineXX(uchar x1,uchar x2,uchar y);
//void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2);
//void LCD_Line(uchar x0,uchar y0,uchar x1,uchar y1) ;
void LCD_PutImg(uchar x,uchar y,uchar w,uchar h,uchar *img) ;
//void LCD_PrintNumStr(uchar x,uchar y,uchar *s) ;
//void LCD_TextPutchar(uchar x,uchar y,uchar c) ;
//void LCD_TextPrint(uchar x,uchar y,char *s) ;
//void LCD_TextPrintHex(uchar x,uchar y,uchar hex) ;
//void LCD_GrapPutchar(uchar x,uchar y,uchar num) ;
//void LCD_GrapPrint(uchar x,uchar y,uchar code *s) ;
//void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt) ;
void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty);
//void LCD_PrintWord(uchar x,uchar y,uint16 n,uchar start) ;
//void LCD_PrintHex(uchar x,uchar y,uchar hex) ;
//void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start) ;
void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h);



void LCD_PrintHz12(uchar x,uchar y,uchar *s);
void LCD_PrintHz16(uchar x,uchar y,uchar *s);
void LCD_Print24X32(uchar x, uchar y,uchar *s);
void LCD_Print6X8(uchar x, uchar y,uchar *s);

#endif