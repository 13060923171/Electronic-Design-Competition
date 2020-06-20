/*--------------------------------------------------------------*/
//包含文件
#include "LCD128X64_V4.H"


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
unsigned char LCD_read_dat(void)
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
//液晶字节定位(8*1)
//液晶规划:
//x: 0 - 7	(行)
//y: 0 -127	(列)
void LCD_byte_pos(unsigned char x, unsigned char y)
{
	if(y < 64)	   LCD_sel_left		//左半屏
	else {y -= 64; LCD_sel_right}	//右半屏
//	LCD_write_cmd(Start_Line);		//起始行地址
	LCD_write_cmd(X_Page_Addr + x);	//页地址0-7
	LCD_write_cmd(Y_Col_Addr + y);	//列地址0-63
}


/*--------------------------------------------------------------*/
//液晶字符定位(8*8)
//液晶规划:
//x: 0 - 7	(行)
//y: 0 -15	(列)
void LCD_char_pos(unsigned char x, unsigned char y)
{
	y <<= 3;
	LCD_byte_pos(x, y);
}


/*--------------------------------------------------------------*/
//显示清屏
void LCD_clr_scr(unsigned char sel_dat)
{
	unsigned char i, j;
	
	if(sel_dat == 0)		LCD_sel_left
	else if(sel_dat == 1)	LCD_sel_right
	else if(sel_dat == 2)	LCD_sel_all
	for(j = 0; j < 8; j++)
	{
		LCD_write_cmd(X_Page_Addr + j);
		LCD_write_cmd(Y_Col_Addr);
		for(i = 0; i < 64; i++) LCD_write_dat(0x00);
	}
}


/*--------------------------------------------------------------*/
//读出8bytes数据
//液晶规划:
//x: 0 - 7	(行)
//y: 0 -15	(列)
void LCD_read_8bytes(unsigned char x, unsigned char y, unsigned char *read_dat)
{
	unsigned char i;

	LCD_char_pos(x, y); LCD_read_dat();	//空读一次!!!
	for(i = 0; i < 8; i++) 
	{
		*read_dat = LCD_read_dat();
		read_dat++;	
	}
}


/*--------------------------------------------------------------*/
#if Font_type	//(8*16字体)
/*--------------------------------------------------------------*/
//液晶字符输出(8*16字体)
//x: 0 -3   (行)
//y: 0 -15	(列)
void LCD_printc(unsigned char x, unsigned char y, unsigned char c_dat)
{
	unsigned char i, j;

	for(j = 0; j < 2; j++)
	{
		LCD_char_pos((x * 2 + j), y);
		for(i = 0; i < 8; i++) LCD_write_dat(Font_code[c_dat - 32][8 * j + i]);		
	}
}
/*--------------------------------------------------------------*/
#else 			//(8*8字体)
/*--------------------------------------------------------------*/
//液晶字符输出(8*8字体)
//x: 0 - 7	(行)
//y: 0 -15	(列)
void LCD_printc(unsigned char x, unsigned char y, unsigned char c_dat)
{
	unsigned char i;

	LCD_char_pos(x, y);
	for(i = 0; i < 6; i++) LCD_write_dat(Font_code[c_dat - 32][i]);
}
#endif


/*--------------------------------------------------------------*/
//液晶字符串输出
//x: 0 - 3	(行)
//y: 0 -15	(列)
void LCD_prints(unsigned char x, unsigned char y, unsigned char *s_dat)
{
	while(*s_dat) LCD_printc(x, y++, *s_dat++);
}


/*--------------------------------------------------------------*/
//液晶字符反色区域(8*8字体)
//x: 0 - 7	(行)
//y: 0 -15	(列)
void LCD_inverse(unsigned char x, unsigned char y)
{
	unsigned char i;
	unsigned char read_buffer[8];

	LCD_read_8bytes(x, y, read_buffer);
	LCD_char_pos(x, y);
	for(i = 0; i < 8; i++) LCD_write_dat(~read_buffer[i]);
}


/*--------------------------------------------------------------*/
//液晶反色显示
void LCD_scr_inverse(void)
{
	unsigned char i, j;

	for(j = 0; j < 8; j++)
	{
		for(i = 0; i < 16; i++) LCD_inverse(j, i);
	}
}


/*--------------------------------------------------------------*/
#if Font_type	//(8*16字体)
/*--------------------------------------------------------------*/
//液晶字符反色输出(8*16字体)
//x: 0 - 3	(行)
//y: 0 -15	(列)
void LCD_inversec(unsigned char x, unsigned char y)
{
	x <<= 1;
	LCD_inverse(x, y);
	LCD_inverse(x+1, y);
}

/*--------------------------------------------------------------*/
#else 			//(8*8字体)
/*--------------------------------------------------------------*/
//液晶字符反色输出(8*8字体)
//x: 0 - 7	(行)
//y: 0 -15	(列)
void LCD_inversec(unsigned char x, unsigned char y)
{
	LCD_inverse(x, y);
}
#endif


/*--------------------------------------------------------------*/
//液晶字符串反色输出
//x: 0 -3/7 (行)
//y: 0 -15	(列)
//num: num < 16 - y
void LCD_inverses(unsigned char x, unsigned char y, unsigned char num)
{
	while(num--) LCD_inversec(x, y++);
}


/*--------------------------------------------------------------*/
//液晶汉字输出(16*16字体)
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
//x: 0 - 3	(行)
//y: 0 - 7	(列)
void LCD_printch(unsigned char x, unsigned char y, unsigned char code *h_dat)
{
	unsigned char i, j;

	x <<= 1; y <<= 1;
	for(j = 0; j < 2; j++)
	{
		LCD_char_pos((x + j), y);
		for(i = 0; i < 16; i++) LCD_write_dat(h_dat[16 * j + i]);		
	}
}


/*--------------------------------------------------------------*/
//液晶汉字反色输出(16*16字体)
//x: 0 - 3	(行)
//y: 0 - 7	(列)
void LCD_inversech(unsigned char x, unsigned char y)
{
	x <<= 1;
	y <<= 1;
	LCD_inverse(x, y);
	LCD_inverse(x, y+1);
	LCD_inverse(x+1, y);
	LCD_inverse(x+1, y+1);
}


/*--------------------------------------------------------------*/
//液晶汉字词组反色输出
//x: 0 - 3 (行)
//y: 0 - 8 (列)
//num: num < 8 - y	汉字个数
void LCD_inversesh(unsigned char x, unsigned char y, unsigned char num)
{
	while(num--) LCD_inversech(x, y++);
}


/*--------------------------------------------------------------*/
//液晶定位写入数据一个字节
//液晶规划:
//x: 0 - 7	(页)
//y: 0 -127	(列)
void LCD_write_dat_pos(unsigned char x, unsigned char y, unsigned char dat)
{
	LCD_byte_pos(x, y);
	LCD_write_dat(dat);
}


/*--------------------------------------------------------------*/
//液晶定位读出数据一个字节
//液晶规划:
//x: 0 - 7	(页)
//y: 0 -127	(列)
unsigned char LCD_read_dat_pos(unsigned char x, unsigned char y)
{
	unsigned char read_dat;

	LCD_byte_pos(x, y);
	read_dat = LCD_read_dat();		//dummy读
	read_dat = LCD_read_dat();		//数据有效
	
	return (read_dat);	
}


/*--------------------------------------------------------------*/
//显示128X64图片
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
void LCD_picture(unsigned char code *img_dat)
{
	unsigned char i, j;

	for(j = 0; j < 8; j++)
	{	
		for(i = 0; i < 128; i++) 
		{
			LCD_write_dat_pos(j, i, img_dat[j * 128 + i]);
		}
	}
}


/*--------------------------------------------------------------*/
//定位显示指定大小图片
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
//pag:	0 - 7			页坐标
//col:  0 - 127			列坐标
//x:	0 - (127-col)	图片宽
//y:	0 - (63-pag*8)	图片高
void LCD_pos_picture(unsigned char pag, unsigned char col, unsigned char x, unsigned char y, unsigned char code *img_dat)
{
	unsigned char i, j;

	y = (y + 4) >> 3;			//四舍五入
	for(j = 0; j < y; j++)
	{	
		for(i = 0; i < x; i++) 
		{
			LCD_write_dat_pos(pag + j, col + i, img_dat[j * x + i]);
		}
	}
}


/*--------------------------------------------------------------*/
//画点函数
//x:	0 - 127		横坐标
//y:	0 - 63		列坐标	
//attr:		=1,		画点	
//			=0,		消点
void LCD_pixel(unsigned char x, unsigned char y, unsigned char attr)
{
	unsigned char pixel_dat, temp_dat, yy;
	
	yy = y >> 3;						//划分到页
	pixel_dat = LCD_read_dat_pos(yy, x);//读出数据
	temp_dat = 0x01 << (y & 0x07);		//页中的点
	if(attr) pixel_dat |= temp_dat;		//画点
	else	 pixel_dat &= ~temp_dat;	//消点
	LCD_write_dat_pos(yy, x, pixel_dat);//写入组合数据
}


/*--------------------------------------------------------------*/
//画横线
//y:	0 - 63		列坐标	
//attr:		=1,		画线
//			=0,		消线
void LCD_line_h(unsigned char y, unsigned char attr)
{
	unsigned char i;

	for(i = 0; i < 128; i++) LCD_pixel(i, y, attr);
}


/*--------------------------------------------------------------*/
//画竖线
//x:	0 - 128		横坐标
//attr:		=1,		画线
//			=0,		消线
void LCD_line_v(unsigned char x, unsigned char attr)
{
	unsigned char i;

	for(i = 0; i < 64; i++) LCD_pixel(x, i, attr);
}


/*--------------------------------------------------------------*/
//显示直线
//x:	0 - 128		横坐标
//y:	0 - 63		纵坐标
//attr:		=1,		画线
//			=0,		消线
void LCD_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char attr)
{
	char dx, dy;
	char inc_x, inc_y;
	int  xerr = 0, yerr = 0;				//初始化变量
	unsigned char i, ds;

    dx = x2 - x1;							//计算坐标增量
    dy = y2 - y1;

	if(dx > 0) inc_x = 1;					//设置单步方向
	else 
	{
		if(dx == 0) {inc_x = 0;			  } //垂直线
		else		{inc_x = -1; dx = -dx;}
	}

	if(dy > 0) inc_y = 1;					//设置单步方向
	else
    {
		if(dy == 0) {inc_y = 0;			  }	//水平线
		else		{inc_y = -1; dy = -dy;}
	}

	if(dx > dy) ds = dx;					//选取基本增量坐标轴
	else		ds = dy;

	for(i = 0; i <= ds+1; i++)				//画线输出
	{
		LCD_pixel(x1, y1, attr);			//画点
		xerr += dx;
		yerr += dy;
		if(xerr > ds)
		{
			xerr -= ds;
			x1   += inc_x;
		}
		if(yerr > ds)
		{
			yerr -= ds;
			y1   += inc_y;
		}
	}
}


/*--------------------------------------------------------------*/
//显示方形
//x:	0 - 128		横坐标
//y:	0 - 63		纵坐标
//attr:		=1,		画方
//			=0,		删方
void LCD_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char attr)
{
	LCD_line(x1, y1, x2, y1, attr);
	LCD_line(x1, y1, x1, y2, attr);
	LCD_line(x2, y1, x2, y2, attr);
	LCD_line(x1, y2, x2, y2, attr);
}


/*--------------------------------------------------------------*/
//显示圆
//x:	0 - 128		横坐标
//y:	0 - 63		纵坐标
//r：	0 - 63		半径
//attr:		=1,		画圆
//			=0,		删圆
void LCD_circle(unsigned char x, unsigned char y, unsigned char r, unsigned char attr)
{
	unsigned char dx, dy = r; 

	for(dx = 0; dx <= r; dx++)
	{
		while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
		LCD_pixel(x + dx, y - dy, attr);
		LCD_pixel(x - dx, y - dy, attr);
		LCD_pixel(x - dx, y + dy, attr);
		LCD_pixel(x + dx, y + dy, attr);

		LCD_pixel(x + dy, y - dx, attr);
		LCD_pixel(x - dy, y - dx, attr);
		LCD_pixel(x - dy, y + dx, attr);
		LCD_pixel(x + dy, y + dx, attr);
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


			



	




 
