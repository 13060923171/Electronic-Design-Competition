#ifndef __LCD_H
#define __LCD_H

#include "typedef.h"

#define LCD_WIDTH      		16      //显示区宽度 16 * 8 = 128 dot
#define LCD_HEIGHT		64	//64 dot
#define LCD_TEXT_HOME_ADDR     	0x0000  //文本显示区首地址
#define LCD_GRAPH_HOME_ADDR     ((LCD_HEIGHT/8)*30)  //图形显示区首地址
#define LCD_DATA_BUS   		P1      //MCU P1<------> LCM

// ASCII字符控制代码解释定义
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


// T6963C 命令定义
#define LC_CUR_POS	0x21		// 光标位置设置
#define LC_CGR_POS	0x22		// CGRAM偏置地址设置
#define LC_ADD_POS	0x24		// 地址指针位置
#define LC_TXT_STP	0x40		// 文本区首址
#define LC_TXT_WID	0x41		// 文本区宽度
#define LC_GRH_STP	0x42		// 图形区首址
#define LC_GRH_WID	0x43		// 图形区宽度
#define LC_MOD_OR	0x80		// 显示方式：逻辑“或”
#define LC_MOD_XOR	0x81		// 显示方式：逻辑“异或”
#define LC_MOD_AND	0x82		// 显示方式：逻辑“与”
#define LC_MOD_TCH	0x83		// 显示方式：文本特征
#define LC_DIS_SW	0x90		// 显示开关：D0=1/0:光标闪烁启用/禁用；
					// D1=1/0:光标显示启用/禁用；
					// D2=1/0:文本显示启用/禁用；
					// D3=1/0:图形显示启用/禁用；
#define LC_CUR_SHP	0xA0		// 光标形状选择：0xA0-0xA7表示光标占的行数
#define LC_AUT_WR	0xB0		// 自动写设置
#define LC_AUT_RD	0xB1		// 自动读设置
#define LC_AUT_OVR	0xB2		// 自动读/写结束
#define LC_INC_WR	0xC0		// 数据一次写，地址加1
#define LC_INC_RD	0xC1		// 数据一次读，地址加1
#define LC_DEC_WR	0xC2		// 数据一次写，地址减1
#define LC_DEC_RD	0xC3		// 数据一次读，地址减1
#define LC_NOC_WR	0xC4		// 数据一次写，地址不变
#define LC_NOC_RD	0xC5		// 数据一次读，地址不变
#define LC_SCN_RD	0xE0		// 屏读
#define LC_SCN_CP	0xE8		// 屏拷贝
#define LC_BIT_OP	0xF0		// 位操作：
					// D0-D2：定义D0-D7位；D3：1置位；0：清除


void LCD_Busy (uchar autowr) ;    //测状态
uchar LCD_BusyStatus () ;    //测状态
void LCD_CheckBusy1(void) ;
//void LCD_CheckBusy2(void) ;
//void LCD_CheckBusy3(void) ;
void LCD_Write1 (uchar dat,uchar comm) ;       //写一个数据和一个命令
void LCD_Write2 (uchar datl,uchar dath,uchar comm) ;  //写两个数据和一个命令
void LCD_WriteInt (uint dat,uchar comm) ;       //写一个16进制数据和一个命令
void LCD_AutoWrite (uchar dat) ;               //自动写数据
uchar LCD_Read(void) ;
//uchar LCD_AutoRead(void) ;
void LCD_Comm (uchar comm) ;       //写命令
void LCD_Data (uchar dat) ;       //写数据
void LCD_Init (void) ;
void LCD_Cls (void) ;
void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2);
//void LCD_GraphSetAddr(uchar x,uchar y) ; /*地址*/
//void LCD_TextSetAddr(uchar x,uchar y) ; /*地址*/
//void LCD_ClrPixel(uchar x,uchar y) ; 
//void LCD_Putpixel(uchar x,uchar y) ;
//void LCD_ShowCursor(uchar x,uchar y) ;/*光标*/
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