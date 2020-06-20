/*--------------------------------------------------------------*/
//File:	LCD128X64_V3.H
//Date:	09/5/24
//Time: 19:16
//Note: FG12864E(KS0108)液晶显示
//CS2(左)-CS1(右), 是高电平有效!!!
//该液晶是行扫描!!!
//每8位显示数据是低位在上, 高位在下
//测试硬件
//单片机:  STC89C52RC
//模式:    12T
//晶振:	   12MHz


/*--------------------------------------------------------------*/
//防止重复引用
#ifndef	__LCD128X64_V3_H__
#define __LCD128X64_V3_H__


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
//函数声明(私有)
/*--------------------------------------------------------------*/
unsigned char LCD_read_cmd(void);				//读出指令(状态)
unsigned char LCD_read_dat(void);				//读出数据
void LCD_check_busy(void);						//检查忙标志位
void LCD_write_cmd(unsigned char cmd);			//写入指令
void LCD_write_dat(unsigned char dat);			//写入数据
void LCD_byte_pos(unsigned char x, unsigned char y);							//液晶字节定位(8*1)
void LCD_char_pos(unsigned char x, unsigned char y);							//液晶字符定位(8*8)
void LCD_read_8bytes(unsigned char x, unsigned char y, unsigned char *read_dat);//读出8bytes数据
void LCD_inverse(unsigned char x, unsigned char y);								//液晶字符反色区域(8*8字体)
void LCD_write_dat_pos(unsigned char x, unsigned char y, unsigned char dat);	//液晶定位写入数据一个字节
unsigned char LCD_read_dat_pos(unsigned char x, unsigned char y);				//液晶定位读出数据一个字节
void LCD_reset_12864(void);						//复位LCD128*64


/*--------------------------------------------------------------*/
//函数声明(公有)
/*--------------------------------------------------------------*/
//清屏参数
// LSCR	0	//左屏
// RSCR	1	//右屏
// ASCR	2	//全屏
void LCD_clr_scr(unsigned char sel_dat);


/*--------------------------------------------------------------*/
//液晶字符输出(8*16字体/8*8字体)
//x: 0 -3/7 (行)
//y: 0 -15	(列)
void LCD_printc(unsigned char x, unsigned char y, unsigned char c_dat);


/*--------------------------------------------------------------*/
//液晶字符输出(8*16字体/8*8字体)
//x: 0 -3/7 (行)
//y: 0 -15	(列)
void LCD_prints(unsigned char x, unsigned char y, unsigned char *s_dat);


/*--------------------------------------------------------------*/
//液晶字符输出(8*16字体/8*8字体)
//x: 0 -3/7 (行)
//y: 0 -15	(列)
void LCD_inversec(unsigned char x, unsigned char y);


/*--------------------------------------------------------------*/
//液晶字符串反色输出
//x: 0 -3/7 (行)
//y: 0 -15	(列)
//num: num < 16 - y
void LCD_inverses(unsigned char x, unsigned char y, unsigned char num);


/*--------------------------------------------------------------*/
//液晶汉字输出(16*16字体)
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
//x: 0 - 3	(行)
//y: 0 - 7	(列)
void LCD_printch(unsigned char x, unsigned char y, unsigned char code *h_dat);


/*--------------------------------------------------------------*/
//液晶汉字反色输出(16*16字体)
//x: 0 - 3	(行)
//y: 0 - 7	(列)
void LCD_inversech(unsigned char x, unsigned char y);


/*--------------------------------------------------------------*/
//液晶汉字词组反色输出
//x: 0 - 3 (行)
//y: 0 - 8 (列)
//num: num < 8 - y	汉字个数
void LCD_inversesh(unsigned char x, unsigned char y, unsigned char num);


/*--------------------------------------------------------------*/
//显示128X64图片
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
void LCD_picture(unsigned char code *img_dat);


/*--------------------------------------------------------------*/
//定位显示指定大小图片
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
//pag:	0 - 7			页坐标
//col:  0 - 127			列坐标
//x:	0 - (127-col)	图片宽
//y:	0 - (63-pag*8)	图片高
void LCD_pos_picture(unsigned char pag, unsigned char col, unsigned char x, unsigned char y, unsigned char code *img_dat);


/*--------------------------------------------------------------*/
//液晶反色显示
void LCD_scr_inverse(void);


/*--------------------------------------------------------------*/
//画点函数
//x:	0 - 127		横坐标
//y:	0 - 63		列坐标	
//attr:		=1,		画点	
//			=0,		消点
void LCD_pixel(unsigned char x, unsigned char y, unsigned char attr);


/*--------------------------------------------------------------*/
//画横线
//y:	0 - 63		列坐标	
//attr:		=1,		画线
//			=0,		消线
void LCD_line_h(unsigned char y, unsigned char attr);


/*--------------------------------------------------------------*/
//画竖线
//x:	0 - 128		横坐标	
//attr:		=1,		画线
//			=0,		消线
void LCD_line_v(unsigned char x, unsigned char attr);


/*--------------------------------------------------------------*/
//显示直线
//x:	0 - 128		横坐标
//y:	0 - 63		纵坐标
//attr:		=1,		画线
//			=0,		消线
void LCD_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char attr);


/*--------------------------------------------------------------*/
//显示方形
//x:	0 - 128		横坐标
//y:	0 - 63		纵坐标
//attr:		=1,		画方
//			=0,		删方
void LCD_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char attr);


/*--------------------------------------------------------------*/
//显示圆
//x:	0 - 128		横坐标
//y:	0 - 63		纵坐标
//r：	0 - 63		半径
//attr:		=1,		画圆
//			=0,		删圆
void LCD_circle(unsigned char x, unsigned char y, unsigned char r, unsigned char attr);


/*--------------------------------------------------------------*/
//初始化LCD128*64
//清除所有显示DDRAM, 清除画面内容, 所有地址指针回0
void LCD_init_12864(void);	


/*--------------------------------------------------------------*/
#endif

