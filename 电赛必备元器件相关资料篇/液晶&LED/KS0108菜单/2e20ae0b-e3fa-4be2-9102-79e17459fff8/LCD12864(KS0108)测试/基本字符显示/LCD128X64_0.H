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
#define LCD_sel_left	{CS2 = 1; CS1 = 0;}			//左屏
#define LCD_sel_right	{CS2 = 0; CS1 = 1;}			//右屏
#define LCD_sel_all		{CS2 = 1; CS1 = 1;}			//全屏
//液晶复位
#define LCD_reset_hard	{RST = 0; RST = 1;}			//硬件复位
#define LCD_reset_soft	 LCD_reset_12864();			//软件复位
//液晶开关(不影响DDRAM)
#define LCD_show_on		LCD_write_cmd(Display_On);	//开启
#define LCD_show_off	LCD_write_cmd(Display_Off);	//关闭


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
unsigned char LCD_read_cmd(void);				//读出指令(状态)
unsigned char LCD_read_dat(void);				//读出数据
void LCD_check_busy(void);						//检查忙标志位
void LCD_write_cmd(unsigned char cmd);			//写入指令
void LCD_write_dat(unsigned char dat);			//写入数据
//unsigned char LCD_read_dat(void);				//读出数据
void LCD_clr_scr(unsigned char sel_dat);		//显示清屏
void LCD_pos(unsigned char x, unsigned char y);	//液晶定位(8*8)
void LCD_printc(unsigned char x, unsigned char y, unsigned char c_dat);	//液晶字符输出(8*16字体)
void LCD_prints(unsigned char x, unsigned char y, unsigned char *s_dat);//液晶字符串输出(8*16字体)
void LCD_reset_12864(void);						//复位LCD128*64
void LCD_init_12864(void);						//初始化LCD128*64


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
//液晶定位(8*8)
//液晶规划:
//x: 0 - 7	(行)
//y: 0 -15	(列)
void LCD_pos(unsigned char x, unsigned char y)
{
	if(y < 8)	  LCD_sel_left			//左半屏
	else {y -= 8; LCD_sel_right}		//右半屏
	LCD_write_cmd(X_Page_Addr + x);		//页地址
	LCD_write_cmd(Y_Col_Addr + (y * 8));//列地址
}


/*--------------------------------------------------------------*/
//液晶字符输出(8*16字体)
//x: 0 - 3	(行)
//y: 0 -15	(列)
void LCD_printc(unsigned char x, unsigned char y, unsigned char c_dat)
{
	unsigned char i, j;

	for(j = 0; j < 2; j++)
	{
		LCD_pos((x * 2 + j), y);
		for(i = 0; i < 8; i++) LCD_write_dat(Font_code[c_dat - 32][8 * j + i]);		
	}
}
	

/*--------------------------------------------------------------*/
//液晶字符串输出(8*16字体)
//x: 0 - 3	(行)
//y: 0 -15	(列)
void LCD_prints(unsigned char x, unsigned char y, unsigned char *s_dat)
{
	while(*s_dat) LCD_printc(x, y++, *s_dat++);
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


		



			



	




 
