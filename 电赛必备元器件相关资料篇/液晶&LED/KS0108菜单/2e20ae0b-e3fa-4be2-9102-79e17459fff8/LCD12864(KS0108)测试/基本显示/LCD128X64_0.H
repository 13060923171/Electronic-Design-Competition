/*--------------------------------------------------------------*/
//File:	LCD128X64_0.H
//Date:	09/5/18
//Time: 21:16
//Note: FG12864E(KS0108)液晶显示
//CS2(左)-CS1(右), 是高电平有效!!!
//该液晶是行扫描!!!
//每8位显示数据是低位在上, 高位在下
//测试硬件
//单片机:  STC89C51RC
//模式:    12T
//晶振:	   12MHz


/*--------------------------------------------------------------*/
//防止重复引用
#ifndef	__LCD128X64_0_H__
#define __LCD128X64_0_H__


/*--------------------------------------------------------------*/
//指令宏定义
#define Display_On     0x3f		//显示开指令
#define Display_Off    0x3e		//显示关指令
#define Y_Col_Addr     0x40		//定位到第0列指令(列起始地址)(0-63)
#define X_Page_Addr    0xb8		//定位到第0页指令(页起始地址)(0-7)
#define Start_Line	   0xc0		//定位从DDROM中的第0行开始往屏幕上显示


/*--------------------------------------------------------------*/
//便于理解宏定义
//清屏参数
#define LSCR	0	//左屏
#define RSCR	1	//右屏
#define ASCR	2	//全屏


/*--------------------------------------------------------------*/
//功能宏定义
//屏幕选择
#define LCD_sel_left	{CS2 = 1; CS1 = 0;}		//左屏
#define LCD_sel_right	{CS2 = 0; CS1 = 1;}		//右屏
#define LCD_sel_all		{CS2 = 1; CS1 = 1;}		//全屏
//液晶复位
#define LCD_reset_hard	{RST = 0; RST = 1;}		//硬件复位
#define LCD_reset_soft	 LCD_reset_12864();		//软件复位



/*--------------------------------------------------------------*/
//数据接口定义
sbit RS  = P2^0;     //数据指令控制端	//高电平数据，低电平指令
sbit RW  = P2^1;     //读写控制端		//高电平写入，低电平读出
sbit E   = P2^2;     //使能端			//下降沿写入数据，高电平读出
sbit CS2 = P2^3;     //左边屏幕使能		//高电平有效
sbit CS1 = P2^4;     //右边屏幕使能		//高电平有效
sbit RST = P2^5;     //LCD复位端		//低电平复位
sfr	 Data_IO = 0x80; //P0 数据D0~D7		//P0=0x80; /P1=0x90; /P2=0xA0; /P3=0xB0;


/*--------------------------------------------------------------*/
//函数声明
unsigned char LCD_read_cmd(void);			//读出指令(状态)
unsigned char LCD_read_dat(void);			//读出数据
void LCD_check_busy(void);					//检查忙标志位
void LCD_write_cmd(unsigned char cmd);		//写入指令
void LCD_write_dat(unsigned char dat);		//写入数据
void LCD_clr_scr(unsigned char sel_dat);	//显示清屏
void Display_Img(unsigned char code *img);	//显示128X64图片
void Display_sz(unsigned char pag, unsigned char col, unsigned char code *sz);	//指定位置显示数字8*16
void Display_hz(unsigned char pag, unsigned char col, unsigned char code *hz);	//指定位置显示汉字16*16
void LCD_init_12864(void);					//初始化LCD128*64


/*--------------------------------------------------------------*/
//代码区
unsigned char code kb2[]={    //   '空白'  16*16 //
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char code kb1[]={    //   '空白'  8*16 //
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char code sz0[]={    //数字  0  8*16 //
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,
};

unsigned char code sz1[]={    //数字  1  8*16 //
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
};

unsigned char code sz2[]={    //数字  2  8*16 //
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
};

unsigned char code sz3[]={    // 数字 3  8*16 //
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,
};

unsigned char code sz4[]={    // 数字 4  8*16 //
0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,
};

unsigned char code sz5[]={    // 数字 5  8*16 //
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,
};

unsigned char code sz6[]={    // 数字 6  8*16 //
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,
};

unsigned char code sz7[]={    // 数字 7  8*16 //
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,
};

unsigned char code sz8[]={    // 数字 8  8*16 //
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,
};

unsigned char code sz9[]={    // 数字 9  8*16 //
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,
};

unsigned char code *shuzi[]={sz0, sz1, sz2, sz3, sz4, sz5, sz6, sz7, sz8, sz9};


/*--------------------------------------------------------------*/
//读出指令(状态)
unsigned char LCD_read_cmd(void)
{
	unsigned char read_cmd;

	Data_IO = 0xff;		//输入
	RS = 0;				//指令
	RW = 1;				//读出
	E  = 1;				//高电平读出
	read_cmd = Data_IO;	//指令
	E  = 0;

	return (read_cmd);
}


/*--------------------------------------------------------------*/
//读出数据
//先只须空读一次, 以后接着就无须空读
//空读之后读出的数据都是有效的!!!
/*unsigned char LCD_read_dat(void)
{
	unsigned char read_dat;

	Data_IO = 0xff;		//输入
	RS = 1;				//数据
	RW = 1;				//读出
	E  = 1;				//高电平读出
	read_dat = Data_IO;	//数据	
	E  = 0;	

	return (read_dat);
}


/*--------------------------------------------------------------*/
//检查忙标志位
void LCD_check_busy(void)
{
	unsigned char lcd_status;

	do{ 
		lcd_status = LCD_read_cmd(); 
	}while(lcd_status & 0x80);
}


/*--------------------------------------------------------------*/
//写入指令
void LCD_write_cmd(unsigned char cmd)
{
	LCD_check_busy();
	RS = 0;			//指令
	RW = 0;			//写入

	Data_IO = cmd;	//指令	
	E  = 1;
	E  = 0;			//下降沿写入
}


/*--------------------------------------------------------------*/
//写入数据
void LCD_write_dat(unsigned char dat)
{
	LCD_check_busy();
	RS = 1;			//数据
	RW = 0;			//写入

	Data_IO = dat;	//数据
	E  = 1;
	E  = 0;			//下降沿写入
}


/*--------------------------------------------------------------*/
//显示清屏
void LCD_clr_scr(unsigned char sel_dat)
{
	unsigned char i, j;
	
	if(sel_dat == 0)		LCD_sel_left
	else if(sel_dat == 1)	LCD_sel_right
	else if(sel_dat == 2)	LCD_sel_all
	LCD_write_cmd(Y_Col_Addr);
	LCD_write_cmd(X_Page_Addr);
	for(j = 0; j < 8; j++)
	{
		LCD_write_cmd(X_Page_Addr + j);
		for(i = 0; i < 64; i++) LCD_write_dat(0x00);
	}
}


/*--------------------------------------------------------------*/
//显示128X64图片
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
/*void Display_Img(unsigned char code *img_dat)
{
	unsigned char i, j;

	for(j = 0; j < 8; j++)
	{
		LCD_sel_left					//左半屏
		LCD_write_cmd(X_Page_Addr + j);
		LCD_write_cmd(Y_Col_Addr);
		for(i = 0; i < 64; i++) LCD_write_dat(img_dat[j * 128 + i]);

		LCD_sel_right					//右半屏
		LCD_write_cmd(X_Page_Addr + j);
		LCD_write_cmd(Y_Col_Addr);
		for(i = 64; i < 128; i++) LCD_write_dat(img_dat[j * 128 + i]);
	}
}


/*--------------------------------------------------------------*/
//显示128X64图片
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
void Display_Img0(unsigned char pag, unsigned char col, unsigned char x, unsigned char y, unsigned char code *img_dat)
{
	unsigned char i, j;

	y = (y + 4) >> 3;					//四舍五入

	for(j = 0; j < y; j++)
	{
		LCD_sel_left					//左半屏
		LCD_write_cmd(X_Page_Addr + pag + j);
		LCD_write_cmd(Y_Col_Addr + col);
		for(i = 0; i < 64; i++) LCD_write_dat(img_dat[j * x + i]);

		if(x > 63)
		{
			LCD_sel_right 				//右半屏
			LCD_write_cmd(X_Page_Addr + pag + j);
			LCD_write_cmd(Y_Col_Addr + col);
			for(i = 64; i < x; i++) LCD_write_dat(img_dat[j * x + i]);
		}
	}
}


/*--------------------------------------------------------------*/
//指定位置显示数字8*8
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
/*void Display_sz0(unsigned char pag, unsigned char col, unsigned char code *sz_dat)
{
	unsigned char i;

	LCD_write_cmd(X_Page_Addr + pag);
	LCD_write_cmd(Y_Col_Addr + col);
	for(i = 0; i < 8; i++) LCD_write_dat(sz_dat[i]);
}



/*--------------------------------------------------------------*/
//指定位置显示数字8*16
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
void Display_sz(unsigned char pag, unsigned char col, unsigned char code *sz_dat)
{
	unsigned char i, j;

	for(j = 0; j < 2; j++)
	{
		LCD_write_cmd(X_Page_Addr + pag + j);
		LCD_write_cmd(Y_Col_Addr + col);
		for(i = 0; i < 8; i++) LCD_write_dat(sz_dat[8 * j + i]);
	}
}


/*--------------------------------------------------------------*/
//指定位置显示汉字16*16
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
void Display_hz(unsigned char pag, unsigned char col, unsigned char code *hz_dat)
{
	unsigned char i, j;

	for(j = 0; j < 2; j++)
	{
		LCD_write_cmd(X_Page_Addr + pag + j);
		LCD_write_cmd(Y_Col_Addr + col);
		for(i = 0; i < 16; i++) LCD_write_dat(hz_dat[16 * j + i]);
	}
}


/*--------------------------------------------------------------*/
//复位LCD128*64
void LCD_reset_12864(void)
{
	LCD_clr_scr(ASCR);			//清全屏
	LCD_write_cmd(Display_Off);	//关闭显示
	LCD_write_cmd(X_Page_Addr);	//起始页地址
	LCD_write_cmd(Start_Line);	//起始行
	LCD_write_cmd(Y_Col_Addr);	//起始列地址
	LCD_write_cmd(Display_On);	//开启显示
}


/*--------------------------------------------------------------*/
//初始化LCD128*64
void LCD_init_12864(void)
{
	LCD_reset_hard		//硬件复位
	LCD_reset_soft		//软件复位
}


/*--------------------------------------------------------------*/
#endif


		



			



	




 
