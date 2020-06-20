/**************************************************************************************
 程序名称：
           时间 温度控制系统（128*64LCD+ds1302+ds18b20）
 主要功能：
           温度计 时钟 闹铃 密码锁 篮球器 计算器 温度控制 键盘锁 系统设置等。
 主要说明：
           程序全部定义在一个C文件中。
 作者：    
           LiJH
 时间：   
           2008.6.20
 版本：   
           LiJH-V1.0
**************************************************************************************/
#include    <AT89S53.h>
#include    <intrins.h>		
#define     uchar        unsigned char
#define     uint         unsigned int
#define     time_count   0x3cb0
#define     close        1
#define     open         0

/*============ 状态控制及温感器总线定义 ==============*/ 
 
#define     b20_io       P3_4		// DS18B20总线	  
#define     pw_oc        P3_7		// 密码锁开启控制(黄灯)
#define     buzzer       P3_3 		// 蜂鸣器
#define     lcd_bl       P0_6		// 背光控制口
#define     als_th       P0_4		// 上限温度警报(红灯)
#define     als_tl       P3_6		// 下限温度警报(绿灯)
#define	    keyboard     P1			// 键盘数据口


/*================= ST7920LCD 定义 ===================*/

#define     comm         0			// lcd指令
#define     dat          1			// lcd数据
#define     lcd_rst      P0_7		// lcd复位
#define     lcd_e        P3_0		// lcd使能
#define     lcd_rw       P3_1		// lcd读写
#define     lcd_rs       P3_2		// lcd数据命令
#define     busy         P2_7       // 忙碌位
#define     lcd_data     P2			// lcd(8bit)数据传输 

  
/*=============== DS1302 寄存器地址定义 ===============*/

#define		d02_rst      P0_1		// DS1302使能
#define		d02_data     P0_2		// DS1302数据线
#define		d02_sclk     P0_3		// DS1302脉冲
#define     d02_wrc      0x8e		// 1302读写RAM控制(1允许 0禁止)
#define     d02_year	 0x8c		// 年
#define     d02_weeks	 0x8a		// 星期
#define     d02_month	 0x88		// 月
#define     d02_day		 0x86		// 日
#define     d02_hour	 0x84		// 时
#define     d02_minute	 0x82		// 分
#define     d02_second	 0x80		// 秒
#define     d02_signs	 0xc0		// 系统状态(记录设置标记)
#define     d02_passw	 0xc2		// 密码寄存器首位（有3位下位地址加2，初始值为：741456）
#define     d02_alm		 0xc8		// 闹铃时间小时位（下位为分钟位加2）
#define     d02_temp	 0xcc		// 温度上限警报（有两位下位位下限加2）
#define     d02_initsi   0xd4		// 1302初始化标记（为0xb4时，说明已经初始化）
#define     ret_ds1302()    d02_rst=0;d02_sclk=0;d02_rst=1	// 1302读写复位
#define     end_ds1302()    d02_sclk=1;d02_rst=0			// 1302读写结束



/*================================= 全局变量定义 ==================================*/


typedef	struct			// 定义时间类型 
{
  uchar	hour;
  uchar	minute;
  uchar	second;
}time;

typedef	struct			// 定义日期类型 
{
  uchar	year;
  uchar	month;
  uchar	day;
  uchar	week;
}date;

typedef union 		    // 定义温度类型
{
  int temp_data;
  uchar t_data[2];
}trdata;

trdata  bdata  temp;             // 温度警报值暂存	
date    idata  today={'8',1,1,1};	 // 日期暂存
time    idata  now={0x12,0,0} , save_time={0x12,0,0}; 
              // 时间暂存(now是读出时间暂存 save_time上一时间暂存)
uchar   idata  time_tick=20,time_tick1=20,alarm_time[2],als_temp[2], 
              // T0中断数,     T1中断数,     闹铃时间,   警报温度
               bl_delay=15,temp_delay=5,opera[2][12],resu[12], 
			  // 背光时间,  温度更新时间, 计算器数据寄存, 结果寄存
			   i,j,save1,temp_min=0; 
			   // 计算器i/j下标，中间暂存,温度更新（分）



uchar  bdata   signs=0;		 // 系统标记寄存器
  sbit bl1=signs^7; 		 // bl1,bl0背光选择位(00 延时,01 常开,10 关闭) 
  sbit bl0=signs^6;			
  sbit di1=signs^5;			 // di1,di0显示模式(00模式1,01模式2,10模式3)
  sbit di0=signs^4;			
  sbit meu=signs^3;			 // 菜单显示方式(0 图片,1 文字)
  sbit keb=signs^2;			 // 键盘锁(0 无锁,1加锁)
  sbit alm=signs^1;			 // 闹钟启动位(0 关闭,1开启)
  sbit als=signs^0;			 // 报警启动位(0 启动,1关闭)



uint   bdata   run_signs=0;     // 运行标记寄存器
  sbit ds18b20=run_signs^0;		// 温感器存在标记(0不存在，1存在)
  sbit temp_pn=run_signs^1;		// 温度正负号标记(0正，1负)
  sbit temprun=run_signs^2;		// 温感器运行标记(0否，1启动)
  sbit nba_tim=run_signs^3;   	// 篮球计时器标记(0关闭，1启动)
  sbit adju_si=run_signs^4;		// 数字更改连续标记(1连续)
  sbit cal_del=run_signs^5;     // 计算器显示点标记（0没有显示）
  sbit cal_run=run_signs^6;	    // 计算完毕标记（1有效）
  sbit oth_run=run_signs^7;     // 其他标记
  sbit alm_run=run_signs^8;     // 闹钟时间到达标记（1有效）
  sbit dis_sig=run_signs^9;     // 显示时间标记（1有效）
  sbit lock_op=run_signs^10;    // 关锁时间到标记（1有效）
  sbit b20time=run_signs^11;    // 温感器重新读温度标记(1有效)
  sbit t0_isr =run_signs^12;    // TO中断标记（1有效(1s)
  sbit thl_als=run_signs^13;	// 温度警报标记
  sbit init_d2=run_signs^14;	// 模式2初始显示
  sbit buzstop=run_signs^15;	// 蜂鸣器停止标记




/*================================= 字体显示代码 ==================================*/ 

uchar code weeks[7][2]={"一","二","三","四","五","六","日"}; // 星期				//图片中文
uchar code menu_cn[6][8]={"调    整","工    具","设    置","保    密","版    本","退    出"};
uchar code menu_cn_2[6][6]={"调  整","工  具","设  置","保  密","版  本","退  出"};// 菜单文字
uchar code selected[2][16]={{"进入【      】\x04 "},{"设置【    】\x10 \x04 "}}; // 选框
uchar code secrets_menu[4][6]={"开  锁","更  改","初始化","退  出"};           // 保密菜单中文
uchar code tool_menu[5][6]={"计算器","温度计","闹  铃","篮球器","退  出"};	   // 工具菜单中文
uchar code char_num[10]={"0123456789"};	 // 数字显示代码							 // 每月天数
uchar code dayofmonth[12]={0x31,0x28,0x31,0x30,0x31,0x30,0x31,0x31,0x30,0x31,0x30,0x31}; 
uchar code key_code[]={0xD7,0x7E,0x7D,0x7B,0xBE,0xBD,0xBB,0xDE,0xDD,0xDB,0xB7,0x77,0xEE,0xED,0xEB,0xE7};
uchar code ksp[4]={0x7F,0xBF,0xDF,0xEF};	 // 按键扫描代码
uchar code tool_cal[4][2]={"+ ","- ","* ","/ "}; // 运算符号
uchar code alm_sign[2][2]={"关","开"};			 // 闹铃开关
uchar code set_bl_dm[2][3][4]={{"30秒","常开","关闭"},{"模01","模02","模03"}};
uchar code set_mka[3][2][4]={{"图片","文字"},{"解锁","加锁"},{"开启","关闭"}};
uchar code nba_oppo[8][2]={"红","白","蓝","绿","紫","灰","黄","黑"};
uchar code set_menu[6][4]={"背光","显示","菜单","键盘","警报","退出"};


uchar code version[10][14]={   // 版本信息
                            {"清远职业技术学"},
							{"  信息科技学院"},
					        {"08届毕业设计  "},
					        {"  —05电子02班"},
			                {"作者：李锦华  "},
					        {"指导：李祖明  "},
					        {"版本:LIJH-V1.0"},
					        {"    2008/06/19"},
					        {"谢谢你的使用！"},
					        {"         (ok)\x10"}};


uchar code timer_tips[12][16]={ // 篮球器使用说明
                            {"使用方法：      "},
							{"  (+)(-)与(*)(/)"},
							{"分别是两方的分数"},
							{"加与减,(7)键回退"},
							{"一秒,(8)键24秒复"},
							{"位,(9)键暂停与开"},
							{"始；暂停状态下："},
							{"按两次(OK)键可返"},
							{"回时钟模式-2界面"},
							{"，(OK)键返回程序"},
							{"界面,(./S)键两次"},
							{"退出程序.      \x1f"}};



/*================================= 操作导航代码 ==================================*/ 

 
typedef struct			// 定义主菜单操作索引 
{
  uchar shaft_num;
  uchar left_num;
  uchar right_num;
}menu_index;

typedef struct			// 定义导航更改索引 
{
  uchar lnum;
  uchar rnum;
}index;

menu_index code index_m[6]={{3,5,1},{4,0,2},{5,1,3},{0,2,4},{1,3,5},{2,4,0}}; // 菜单导航
index code index_t[6]={{4,1},{0,2},{1,3},{2,4},{3,0}};       // 工具导航
index code index_p[5]={{3,1},{0,2},{1,3},{2,0}};             // 保密导航
index code index_s[6]={{5,1},{0,2},{1,3},{2,4},{3,5},{4,0}}; // 设置导航
index code index_a[7]={{0x81,0x91},{0x83,0x91},{0x85,0x91},{0x89,0x81},
		                {0x8b,0x81},{0x8d,0x81},{0x8f,0x91}};       // 调整下的坐标
index code index_temp_set[3]={{0x8b,0x81},{0x8b,0x91},{0x8f,0x91}}; // 温度设定下的坐标
index code index_alm[3]={{0x8c,0x81},{0x8e,0x81},{0x8f,0x91}};	    // 闹铃更改坐标	
index code index_cal[3]={{0x86,0x90},{0x8e,0x80},{0x8e,0x90}};	    // 计算器操作坐标
uchar code index_nba[4]={0x93,0x97,0x8b,0x8f};					    // 篮球器设置导航





/*========================== 菜单图片（宽度x高度=32*24）=============================*/

uchar code menu_pic[6][96]={ 
	{// 调整 
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x00,0x00,0x02,
	 0x40,0x00,0x00,0x82,0x47,0xFF,0xE1,0xC2,0x47,0xFF,0xF3,0xE2,0x47,0x38,0x68,0x02,
	 0x46,0xDF,0x68,0x82,0x46,0xDF,0x69,0xC2,0x46,0xDE,0xE8,0x82,0x46,0xDD,0xE8,0x02,
	 0x46,0xDD,0xE8,0x02,0x47,0x3D,0xE8,0x02,0x47,0xFF,0xE8,0x02,0x47,0xFF,0xE9,0xC2,
	 0x42,0x00,0x18,0x02,0x41,0xFF,0xF3,0xE2,0x40,0x00,0x01,0xC2,0x40,0x00,0x00,0x82,
	 0x40,0x00,0x00,0x02,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// 工具					
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x00,0x03,0x82,
	 0x41,0xF0,0x07,0xC2,0x43,0xFF,0xCF,0xE2,0x47,0x3F,0xFE,0x02,0x47,0x3F,0xFE,0x02,
	 0x43,0xFF,0xCF,0xE2,0x41,0xF0,0x0F,0xC2,0x40,0x00,0x03,0x82,0x40,0x00,0x00,0x02,
	 0x40,0x00,0x03,0xC2,0x40,0x00,0x03,0xC2,0x41,0x80,0x01,0x82,0x43,0xFF,0xF3,0xC2,
	 0x47,0xFF,0xFF,0xE2,0x47,0xFF,0xFF,0xE2,0x43,0xFF,0xF3,0xC2,0x41,0x80,0x01,0x82,
	 0x40,0x00,0x01,0x82,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// 设置
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x43,0x18,0x00,0x02,
	 0x44,0xA4,0x0E,0x02,0x44,0xA4,0x1F,0x02,0x43,0x24,0x3F,0x02,0x40,0x00,0x7F,0x02,
	 0x40,0xF0,0xFC,0x02,0x43,0xF9,0xF0,0x02,0x43,0x03,0xE0,0x02,0x46,0x77,0x80,0x02,
	 0x46,0x7F,0x00,0x02,0x46,0x7E,0x00,0x02,0x46,0x7C,0x00,0x02,0x47,0x3D,0x00,0x02,
	 0x43,0x1D,0x81,0x22,0x43,0x81,0xB2,0x42,0x41,0xC3,0x4F,0xE2,0x40,0xFE,0x4A,0x42,
	 0x40,0x3C,0x32,0x42,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// 保密
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0xFC,0x03,0x02,
	 0x41,0xFE,0x1F,0x02,0x43,0x87,0x03,0x82,0x43,0x03,0x03,0x02,0x43,0x03,0x03,0x02,
	 0x43,0x03,0x0F,0x02,0x43,0x03,0x03,0x02,0x43,0x03,0x03,0x02,0x47,0xFF,0x83,0x02,
	 0x47,0xFF,0x83,0x02,0x47,0xCF,0x83,0x02,0x47,0x87,0x83,0x02,0x47,0xCF,0x83,0x02,
	 0x47,0xCF,0x87,0x82,0x47,0xCF,0x8F,0xC2,0x47,0xCF,0x9C,0xE2,0x47,0xFF,0x8F,0xC2,
	 0x43,0xFF,0x07,0x82,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// 版本
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x00,0x00,0x02,
	 0x40,0x0F,0x00,0x62,0x40,0x1D,0x80,0xF2,0x40,0x38,0x41,0xF2,0x40,0x70,0x23,0xE2,
	 0x40,0xE2,0x17,0xC2,0x41,0xE4,0x4F,0x82,0x41,0xF0,0x9F,0x02,0x41,0x59,0x16,0x02,
	 0x42,0x4C,0x1D,0x02,0x42,0x46,0x20,0x82,0x42,0x47,0x00,0x42,0x44,0x43,0x80,0xC2,
	 0x44,0x44,0xC1,0xC2,0x44,0xC8,0x63,0x82,0x45,0x50,0x37,0x02,0x46,0x60,0x1E,0x02,
	 0x40,0x40,0x0C,0x02,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// 退出
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x40,0xFF,0xE2,
	 0x40,0x60,0x80,0x62,0x43,0xF0,0x80,0xA2,0x43,0xF0,0x81,0x22,0x40,0x60,0x82,0x22,
	 0x40,0x40,0x84,0x22,0x40,0x00,0x88,0x22,0x40,0x00,0x88,0x22,0x47,0xFF,0xF8,0x22,
	 0x47,0xFF,0xF8,0x22,0x46,0x2A,0x2A,0x22,0x46,0xE3,0x78,0x22,0x46,0x77,0x78,0x22,
	 0x46,0xE3,0x78,0x62,0x46,0x2A,0x38,0x82,0x47,0xFF,0xF9,0x02,0x47,0xFF,0xFA,0x02,
	 0x40,0x00,0x0C,0x02,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,}};




/*================ 显示模式2数字及温度符号图片（宽度x高度=16*32）=====================*/

uchar code mode_2num[11][64]={
	{// 图片数字0
	 0x00,0x00,0x3F,0xFC,0x5F,0xFA,0x6F,0xF6,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x40,0x02,0x00,0x00,
	 0x40,0x02,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x6F,0xF6,0x5F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
	
	{// 图片数字1
	 0x00,0x00,0x00,0x20,0x00,0x60,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,
     0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0x60,0x00,0x20,0x00,0x00,0x00,0x20,
	 0x00,0x60,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,
	 0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0x60,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00},
    
	{// 图片数字2
	 0x00,0x00,0x3F,0xFC,0x1F,0xFA,0x0F,0xF6,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x1F,0xFA,0x3F,0xFC,
	 0x5F,0xF8,0x60,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,
	 0x70,0x00,0x70,0x00,0x70,0x00,0x6F,0xF8,0x5F,0xFC,0x3F,0xFE,0x00,0x00,0x00,0x00},
	
	{// 图片数字3
	 0x00,0x00,0x7F,0xFC,0x3F,0xFA,0x1F,0xF6,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
     0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x1F,0xFA,0x3F,0xFC,
     0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
     0x00,0x0E,0x00,0x0E,0x00,0x0E,0x1F,0xF6,0x3F,0xFA,0x7F,0xFC,0x00,0x00,0x00,0x00},
	
	{// 图片数字4
	 0x00,0x00,0x40,0x02,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
 	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x5F,0xFA,0x3F,0xFC,
	 0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x02,0x00,0x00,0x00,0x00},
	
	{// 图片数字5
	 0x00,0x00,0x3F,0xFC,0x5F,0xF8,0x6F,0xF0,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,
	 0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x60,0x00,0x5F,0xF8,0x3F,0xFC,
	 0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x0F,0xF6,0x1F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
 	
	{// 图片数字6
	 0x00,0x00,0x3F,0xFC,0x5F,0xF8,0x6F,0xF0,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,
	 0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x60,0x00,0x5F,0xF8,0x3F,0xFC,
	 0x5F,0xFA,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x6F,0xF6,0x5F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
	
	{// 图片数字7
	 0x00,0x00,0x7F,0xFC,0x3F,0xFA,0x1F,0xF6,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x02,0x00,0x00,
	 0x00,0x02,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x02,0x00,0x00,0x00,0x00},
	
	{// 图片数字8
	 0x00,0x00,0x3F,0xFC,0x5F,0xFA,0x6F,0xF6,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x5F,0xFA,0x3F,0xFC,
	 0x5F,0xFA,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x6F,0xF6,0x5F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
	
	{// 图片数字9
	 0x00,0x00,0x3F,0xFC,0x5F,0xFA,0x6F,0xF6,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x5F,0xFA,0x3F,0xFC,
	 0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x1F,0xF6,0x3F,0xFA,0x7F,0xFC,0x00,0x00,0x00,0x00},
	
	{// 温度符号
     0x00,0x00,0x00,0x00,0x70,0xE0,0x53,0xF0,0x77,0xFA,0x0E,0x0E,0x0C,0x06,0x18,0x06,
     0x18,0x02,0x38,0x02,0x30,0x00,0x30,0x00,0x70,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
     0x60,0x00,0x60,0x00,0x70,0x00,0x30,0x00,0x30,0x00,0x38,0x00,0x18,0x02,0x18,0x02,
     0x0C,0x06,0x0E,0x06,0x07,0x0E,0x03,0xFA,0x00,0xF0,0x00,0x60,0x00,0x00,0x00,0x00}};





/*===================== 显示模式3数字图片（宽度x高度=24*32）=========================*/

uchar code mode_3num[10][96]={
	{// 图片数字0
	 0x00,0x7E,0x00,0x01,0xE7,0x80,0x03,0xC3,0xC0,0x07,0x81,0xE0,0x0F,0x80,0xF0,0x0F,
	 0x00,0xF0,0x1F,0x00,0xF8,0x1E,0x00,0xF8,0x3E,0x00,0x78,0x3E,0x00,0x7C,0x3E,0x00,
	 0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,
	 0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,
	 0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x78,0x1E,0x00,0xF8,0x1F,0x00,0xF8,0x0F,0x00,
	 0xF0,0x0F,0x81,0xF0,0x07,0x81,0xE0,0x03,0xC3,0xC0,0x01,0xE7,0x80,0x00,0x7E,0x00},

	{// 图片数字1
	 0x00,0x0C,0x00,0x00,0x1C,0x00,0x00,0x7C,0x00,0x07,0xFC,0x00,0x00,0x3C,0x00,0x00,
	 0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,
	 0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,
	 0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,
	 0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,
	 0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3E,0x00,0x00,0x7F,0x00,0x07,0xFF,0xF0},
			   
	{// 图片数字2
	 0x00,0xFF,0x00,0x03,0xC7,0xC0,0x07,0x01,0xE0,0x0E,0x00,0xF0,0x1E,0x00,0xF8,0x1E,
	 0x00,0xF8,0x3E,0x00,0x78,0x3E,0x00,0x78,0x3F,0x00,0x78,0x3F,0x00,0x78,0x1F,0x00,
     0xF8,0x00,0x00,0xF8,0x00,0x00,0xF0,0x00,0x01,0xF0,0x00,0x03,0xE0,0x00,0x03,0xC0,
	 0x00,0x07,0x80,0x00,0x0F,0x00,0x00,0x1E,0x00,0x00,0x3C,0x00,0x00,0x78,0x00,0x00,
	 0xF0,0x00,0x01,0xE0,0x00,0x03,0xC0,0x00,0x07,0x80,0x1C,0x07,0x00,0x1C,0x0E,0x00,
	 0x38,0x1C,0x00,0x38,0x3C,0x00,0x78,0x3F,0xFF,0xF8,0x3F,0xFF,0xF8,0x3F,0xFF,0xF8},

	{// 图片数字3
	 0x00,0xFE,0x00,0x07,0x87,0x80,0x0F,0x03,0xC0,0x1E,0x01,0xE0,0x1E,0x01,0xF0,0x1E,
	 0x01,0xF0,0x1F,0x00,0xF0,0x1F,0x00,0xF0,0x1E,0x00,0xF0,0x00,0x00,0xF0,0x00,0x01,
	 0xF0,0x00,0x01,0xF0,0x00,0x03,0xE0,0x00,0x03,0xC0,0x00,0x0F,0x00,0x00,0xFE,0x00,
	 0x00,0x07,0x80,0x00,0x01,0xE0,0x00,0x00,0xF0,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,
	 0x00,0x78,0x00,0x00,0x7C,0x1E,0x00,0x7C,0x3F,0x00,0x7C,0x3F,0x00,0x7C,0x3F,0x00,
	 0x78,0x3E,0x00,0xF8,0x1E,0x00,0xF0,0x0F,0x01,0xE0,0x07,0x87,0xC0,0x01,0xFF,0x00},

	{// 图片数字4
	 0x00,0x03,0xC0,0x00,0x03,0xC0,0x00,0x07,0xC0,0x00,0x0F,0xC0,0x00,0x0F,0xC0,0x00,
	 0x1F,0xC0,0x00,0x3F,0xC0,0x00,0x3F,0xC0,0x00,0x77,0xC0,0x00,0x77,0xC0,0x00,0xE7,
	 0xC0,0x01,0xC7,0xC0,0x01,0xC7,0xC0,0x03,0x87,0xC0,0x07,0x07,0xC0,0x07,0x07,0xC0,
	 0x0E,0x07,0xC0,0x1E,0x07,0xC0,0x1C,0x07,0xC0,0x38,0x07,0xC0,0x38,0x07,0xC0,0x7F,
	 0xFF,0xFE,0x7F,0xFF,0xFE,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,
	 0xC0,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,0xE0,0x00,0x7F,0xFE},

	{// 图片数字5
	 0x0F,0xFF,0xF8,0x0F,0xFF,0xF8,0x0F,0xFF,0xF8,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,
	 0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,
	 0x00,0x0E,0x7F,0x00,0x0D,0xFF,0xC0,0x0F,0xC3,0xE0,0x1F,0x01,0xF0,0x1E,0x00,0xF8,
	 0x1E,0x00,0xF8,0x00,0x00,0x78,0x00,0x00,0x7C,0x00,0x00,0x7C,0x00,0x00,0x7C,0x00,
	 0x00,0x7C,0x1E,0x00,0x7C,0x3F,0x00,0x7C,0x3F,0x00,0x78,0x3F,0x00,0x78,0x3E,0x00,
	 0xF8,0x1E,0x00,0xF0,0x1E,0x01,0xF0,0x0E,0x01,0xE0,0x07,0x87,0xC0,0x00,0xFF,0x00},

	{// 图片数字6
	 0x00,0x3F,0xC0,0x00,0xF1,0xE0,0x03,0xC1,0xF0,0x07,0x81,0xF8,0x07,0x01,0xF8,0x0F,
	 0x00,0xF0,0x1F,0x00,0x00,0x1E,0x00,0x00,0x1E,0x00,0x00,0x3E,0x00,0x00,0x3E,0x00,
	 0x00,0x3E,0x00,0x00,0x3E,0x3F,0x80,0x3E,0xFF,0xE0,0x3F,0xE3,0xF0,0x3F,0x80,0xF8,
	 0x3F,0x00,0xF8,0x3F,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x3C,0x3E,
	 0x00,0x3C,0x3E,0x00,0x3C,0x3E,0x00,0x3C,0x3E,0x00,0x7C,0x1E,0x00,0x7C,0x1F,0x00,
	 0x78,0x0F,0x00,0x78,0x0F,0x80,0xF0,0x07,0xC0,0xE0,0x03,0xE3,0xC0,0x00,0xFF,0x00},

	{// 图片数字7
	 0x1F,0xFF,0xFC,0x1F,0xFF,0xFC,0x1F,0xFF,0xF8,0x1F,0x00,0x38,0x1C,0x00,0x70,0x1C,
	 0x00,0xE0,0x38,0x00,0xE0,0x38,0x01,0xC0,0x00,0x01,0xC0,0x00,0x03,0x80,0x00,0x03,
	 0x80,0x00,0x07,0x80,0x00,0x07,0x00,0x00,0x0F,0x00,0x00,0x0E,0x00,0x00,0x1E,0x00,
	 0x00,0x1E,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x7C,0x00,0x00,
	 0x78,0x00,0x00,0x78,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,
	 0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0x78,0x00},

	{// 图片数字8
	 0x00,0xFF,0x00,0x07,0xC3,0xC0,0x0F,0x00,0xE0,0x1E,0x00,0xF0,0x1E,0x00,0x78,0x3C,
	 0x00,0x78,0x3C,0x00,0x78,0x3C,0x00,0x7C,0x3E,0x00,0x78,0x3E,0x00,0x78,0x1F,0x00,
	 0x78,0x1F,0x80,0xF0,0x0F,0xE1,0xE0,0x07,0xFB,0xC0,0x01,0xFF,0x80,0x01,0xFF,0x80,
	 0x07,0xBF,0xC0,0x0F,0x0F,0xE0,0x1E,0x03,0xF0,0x3E,0x01,0xF8,0x3C,0x00,0xF8,0x3C,
	 0x00,0x7C,0x7C,0x00,0x7C,0x78,0x00,0x3C,0x78,0x00,0x3C,0x7C,0x00,0x3C,0x3C,0x00,
	 0x78,0x3C,0x00,0x78,0x1E,0x00,0x70,0x0F,0x00,0xE0,0x07,0xC3,0xC0,0x01,0xFF,0x00},

	{// 图片数字9
	 0x01,0xFE,0x00,0x07,0xC7,0x80,0x0F,0x01,0xE0,0x1E,0x01,0xE0,0x1E,0x00,0xF0,0x3E,
	 0x00,0xF8,0x3C,0x00,0x78,0x3C,0x00,0x78,0x7C,0x00,0x7C,0x7C,0x00,0x7C,0x7C,0x00,
	 0x7C,0x7C,0x00,0x7C,0x7C,0x00,0x7C,0x3C,0x00,0xFC,0x3E,0x00,0xFC,0x3E,0x01,0xFC,
     0x1F,0x03,0xFC,0x1F,0x8F,0xFC,0x0F,0xFF,0x7C,0x03,0xFC,0x7C,0x00,0x00,0x7C,0x00,
     0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF0,0x00,0x01,0xF0,0x0F,0x01,
	 0xE0,0x1F,0x01,0xE0,0x1F,0x03,0xC0,0x1F,0x07,0x80,0x0F,0x9F,0x00,0x03,0xFC,0x00}};




/*========================== 标记及点图片（宽度x高度=16*16）===========================*/


uchar code running[7][32]={
     {// 警报标记16*16
	  0x00,0x00,0x00,0x39,0x00,0x7B,0x00,0xFE,0x01,0xDC,0x0F,0x98,0x0F,0x18,0x0C,0x1F,
      0x0C,0x1F,0x0F,0x18,0x0F,0x98,0x01,0xDC,0x00,0xFE,0x00,0x7B,0x00,0x39,0x00,0x00,},
	 {// 闹铃开启标记16*16
	  0x00,0x00,0x00,0x40,0x00,0xE0,0x01,0xB0,0x01,0xB0,0x03,0x18,0x03,0x18,0x06,0x0C,
      0x0C,0x06,0x0C,0xE6,0x1B,0x1B,0x0C,0x06,0x06,0x0C,0x03,0xF8,0x00,0xE0,0x00,0x00},
	 {// 键盘锁标记16*16
      0x00,0x00,0x01,0xF8,0x03,0xFC,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0xFC,0x06,0x96,
      0x0E,0xA7,0x0E,0xC7,0x0E,0xA7,0x0E,0x97,0x07,0xFE,0x03,0xFC,0x01,0xF8,0x00,0x00,},
	 {// 温度计标记16*16
	  0x00,0x00,0x1F,0xF8,0x1F,0xF8,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
      0x01,0x90,0x01,0x98,0x01,0x9C,0x01,0x9E,0x01,0x9C,0x00,0x18,0x00,0x10,0x00,0x00},
	 {// 模式3点16*16
	  0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
      0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	 {// 模式2点16*16
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
      0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	  
	 {// 篮球器运行标记16*16
	  0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x1F,0xF8,0x01,0x80,0x01,0x80,0x01,0x90,
      0x01,0x98,0x01,0x9C,0x01,0x9E,0x01,0x9C,0x01,0x98,0x01,0x90,0x00,0x00,0x00,0x00}};


/*========================== 菜单按钮图片（宽度x高度=16*8）============================*/

uchar code menu_key[]=
	 {//菜单按钮
	  0x7F,0xFE,0x81,0x81,0x83,0xC1,0x87,0xE1,0x8F,0xF1,0x9F,0xF9,0x80,0x01,0x7F,0xFE};





/*=====================================================================================
 函数名称：定时器中断处理与初始化，延时
 功能描述：时钟更新、篮球器计时
 全局变量：time_tick、time_tick1、resu[2]、resu[3]、resu[4]、temp_delay、temp_min
 参数说明：无
 返回说明：t0_isr(中断标记)、b20time(温度更新标记)
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


/*延时（一次10个机器周期不等） 参数格式：延时数*/
void delay(unsigned int det)
{
  while(det--);
}



/*1s延时*/
void delay_pw()
{
  delay(60000);
  delay(60000);
}


/*6个周期  6us */
void  delay_b20(uchar n)
{  
  while(n--);
}



/*主时钟中断处理*/
void time0_isr(void) interrupt  TF0_VECTOR  using 1
{
  TR0=0;
  TL0=(time_count&0x00FF);
  TH0=(time_count>>8);
  TR0=1;
  if(--time_tick)  return;
  time_tick=20;
  t0_isr=1;
  if(nba_tim&!oth_run)	   // 当在篮球器界面时 nba_tim==1、oth_run==0 
  {
    if(resu[3]==0)
	{
	  resu[3]=59;
	  if(resu[2]>0)	resu[2]--;
	}
	else resu[3]--;
	if(resu[2]==0&resu[3]==0)  // 比赛时间到后停止
	{
	  TR0=0;TR1=0;EA=0;
	}
	return;
  }
  if((++temp_min)==60)	  // 实现温度的更新
  {
    temp_min=0;
    temp_delay--;
	if(temp_delay==0)
    {
	  b20time=1;temp_delay=5;
	}
  }	
}


/*24时钟中断处理*/
void time1_isr(void) interrupt  TF1_VECTOR  using 2
{
  TR1=0;
  TL1=(time_count&0x00FF);
  TH1=(time_count>>8);
  TR1=1;
  if(--time_tick1) return;
  time_tick1=20;
  if(resu[4]>0)resu[4]--;	  //  篮球器24秒中断
  t0_isr=1;
  if(resu[4]==0)
  { TR0=0;TR1=0;EA=0;}
}


/*定时器初始化*/
void time_init()			
{
  EA=0;
  TR0=0;
  TR1=0;
  TMOD=0x11;
  TL0=(time_count&0x00FF);
  TH0=(time_count>>8);
  TL1=(time_count&0x00FF);
  TH1=(time_count>>8);
  IE=0;
  ET0=1;
  ET1=1;
}



/*=====================================================================================
 函数名称：LCD驱动
 功能描述：基本驱动（写1个字节、清屏、初始化）
 全局变量：无
 参数说明：dat_comm（0：命令 1：数据）,content（数值）
 返回说明：无返回值
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


/*检查忙碌位*/
void chk_busy (void)
{
  lcd_data = 0xff;
  lcd_rs = 0;    // 命令
  lcd_rw = 1;	 // 读
  lcd_e = 1;
  while(busy==1);
  lcd_e = 0;
}


/*写LCD 参数格式：0:命令,1:数值 ，数据*/
void wr_lcd (uchar dat_comm,uchar content)
{
  chk_busy ();
  lcd_rs=dat_comm;
  lcd_rw = 0;         // 写
  lcd_data=content;   // 输出数据或命令
  lcd_e=1;             
  lcd_e=0;
}


/*清绘图*/
void clrgdram(void)
{
  uchar i,j;
  for(j=0;j<32;j++)
  {
    wr_lcd(comm,0x34);
    wr_lcd(comm,0x80+j);
    wr_lcd(comm,0x80);
    wr_lcd(comm,0x30);
    for(i=0;i<32;i++)
      wr_lcd(dat,0x00);  // 写入0x00
   }
  wr_lcd(comm,0x36);
}


/*清除显示*/
void clear_dis(void)
{
  wr_lcd(comm,0x30);
  wr_lcd(comm,0x01);  // 清屏，地址指针指向00H
  clrgdram();		  // 请绘图RAM
}



/*开机LCD初始化*/
void init_lcd (void)
{
  lcd_rst=1;
  wr_lcd(comm,0x30);  // 30---基本指令动作  
  wr_lcd(comm,0x06);  // 光标的移动方向
  wr_lcd(comm,0x0c);  // 开显示，关游标
  clear_dis(); 
}





/*=====================================================================================
 函数名称：DS1302驱动
 功能描述：基本驱动（读写数据、读写日期，时间、初始化）
 全局变量：无
 参数说明：w_adder（写地址）,r_adder（读地址），w_data（写入数据）
 返回说明：返回读取的数据
 设 计 人：LIJH
 版    本：1.0
 说    明：上升沿写入数值，下降沿读出数值，没用设立后背电池充电
 ======================================================================================*/


/*向1302写数据 参数格式：地址，数据*/
void w1302(uchar w_adder,uchar w_data)
{
  uchar d;
  ret_ds1302();
  for(d=0;d<16;d++)
  {
    if(d==8) w_adder=w_data;
	d02_data=(bit)(w_adder&0x01);
    d02_sclk=0;		 // 上升沿
    w_adder>>=1;
    d02_sclk=1;
  }
  end_ds1302();    
}



/*接收1302一个字节的数据 参数格式：读地址*/
uchar r1302(uchar r_adder)
{
  uchar d=0;
  ret_ds1302();
  for(d=0;d<8;d++)	 // 写地址
  {
    d02_data=(bit)(r_adder&0x01);
    d02_sclk=0;
    r_adder>>=1;
    d02_sclk=1;
  }
  r_adder=0;
  for(d=0;d<8;d++)	 // 读数据 
  {
	d02_data=1;
	d02_sclk=1;		 // 下降沿
	r_adder>>=1;
	d02_sclk=0;
	if(d02_data) r_adder|=0x80;
  }
  end_ds1302();
  return(r_adder);
}



/*写入时间与日期*/
void wds1302_time_date(void)
{
  w1302(d02_wrc,0x00);   			 // 写允许
  w1302(0x90,0xa5);                  // 单二极管2K电阻充电
  w1302(d02_year,today.year);   	 // 年
  w1302(d02_weeks,today.week);  	 // 星期
  w1302(d02_month,today.month);   	 // 月
  w1302(d02_day,today.day);   		 // 日
  w1302(d02_hour,0x7f&now.hour);   	 // 时
  w1302(d02_minute,now.minute);   	 // 分
  w1302(d02_second,0x7f&now.second); // 秒
  w1302(d02_wrc,0x80); 				 // 写禁止
}




/*初始化写DS1302 RAM*/
void init_wds1302_ram(void)
{
  w1302(d02_wrc,0x00); 
  w1302(0x90,0xa5);         // 单二极管2K电阻充电
  w1302(d02_signs,0x00);   	// 状态寄存
  w1302(d02_passw,0x74);   	// 密码储存首位
  w1302(d02_passw+2,0x14);  // 初始值为:741456
  w1302(d02_passw+4,0x56);  // 密码最后一位
  w1302(d02_alm,0x12);   	// 闹铃时(12小时)
  w1302(d02_alm+2,0x00);   	// 闹铃分(0分)
  w1302(d02_temp,0xff);   	// 温度上限(255为关闭) 	 
  w1302(d02_temp+2,0xff);   // 温度下限(关闭)
  w1302(d02_initsi,0xb4);   // 是否要初始化标记(0xb4为已经初始化)
  w1302(d02_wrc,0x80); 
}

/*写DS1302一字节的RAM*/
void wds1302_data(uchar adder,uchar _data)
{
  w1302(d02_wrc,0x00); 
  w1302(adder,_data);
  w1302(d02_wrc,0x80);
} 


/*读取日期*/
void rds1302_date(void)
{
  
  today.year=r1302(d02_year+1);	  
  today.week=r1302(d02_weeks+1);
  today.month=r1302(d02_month+1);
  today.day=r1302(d02_day+1);  
}


/*读取时间*/
void rds1302_time(void)
{
  now.hour=(r1302(d02_hour+1));
  now.minute=r1302(d02_minute+1);
  now.second=r1302(d02_second+1);
  if(now.hour==alarm_time[0]&&now.minute==alarm_time[1]) alm_run=1;
  else 										// 闹铃时间比较
  {
    alm_run=0;buzstop=0;buzzer=close;
  } 
}					

/*开机读数据*/
void rds1302_data(void)
{
  signs=r1302(d02_signs+1);
  alarm_time[0]=r1302(d02_alm+1);
  alarm_time[1]=r1302(d02_alm+3);
  als_temp[0]=r1302(d02_temp+1);
  als_temp[1]=r1302(d02_temp+3);		   
}
 

/*DS1302初始化*/
void init_ds1302(void)
{
  wds1302_time_date();
  init_wds1302_ram();
}


/*=====================================================================================
 函数名称：DS18b20驱动
 功能描述：基本驱动（读写数据、数据转换、初始化）
 全局变量：无
 参数说明：无
 返回说明：返回读取的数据
 设 计 人：LIJH
 版    本：1.0
 说    明：调试中。。。
 ======================================================================================*/



/*18B20初始化*/
void init_ds18b20()
{
  b20_io=1;
  b20_io=0;
  delay_b20(120);		// 延时≥480us
  b20_io=1;
  delay_b20(12);	    // 拉高为高电平后在15—60us出现低电平，并维持60—240us
  ds18b20=~b20_io;
  delay_b20(40);	
}


/*向18B20发送一个字节*/
void send18b20_byte(uchar data_send)
{
  uchar d=0;
  for (d=0;d<8;d++)
  {
	b20_io=0;					// 拉低电平后向总线写入一个位，15us后18B20接收  
    b20_io=data_send&0x01;	    
    delay_b20(10);				// 延时60us后完成一次
    b20_io=1;
    data_send>>=1;			    // 下位开始
  }
}


/*从18B20读出温度值 12位分辨率*/  
void receive18b20(void)
{
  uchar n=1,d;
  for (d=0;d<16;d++)
  {
	if(d==8) n=0;
	b20_io=0;					  // 拉低总线
	temp.t_data[n]>>=1;            
	b20_io=1;                     // 拉高总线
	if(b20_io)					  // 读入总线数值
      temp.t_data[n]|=0x80;
    delay_b20(10);				  // 等待60us
  }
}


/*数据转化*/
void conversion_t(void)
{
  uchar save=0,save1=0;
  if(temp.temp_data<0) 	         // 判断数值符号
    {temp_pn=1;temp.temp_data=~temp.temp_data+1;}
  else temp_pn=0;
  if(temp.t_data[1]&0x01==1) save+=6;	    // 小数点后的值转变
  if((temp.t_data[1]>>1)&0x01==1) save+=12;
  if((temp.t_data[1]>>2)&0x01==1) save+=25;
  if((temp.t_data[1]>>3)&0x01==1) save+=50;
  temp.temp_data>>=4;
  save1=temp.temp_data&0x00ff;				// 整数部分数值
  temp.t_data[0]=save1;
  temp.t_data[1]=save;
  thl_als=1;
  if(save1>=(als_temp[0]&0x7f))      // 温度比较
  {
	if(!temp_pn||(temp_pn&(als_temp[0]&0x80)!=0)){ als_th=open;als_tl=close;}
  }
  else if(save1<(als_temp[1]&0x7f)&&als_temp[1]!=255)
  {
    if(!temp_pn||(temp_pn&(als_temp[1]&0x80)!=0)) {als_tl=open;als_th=close;}
  }
  else {als_th=als_tl=close;thl_als=0;buzstop=0;buzzer=close;} 
}



/*18b20功能*/
void ds18b20_func(void)
{
  EA=0;
  TR0=0;
  temp.temp_data=0;
  init_ds18b20(); 
  send18b20_byte(0xcc); // 跳过ROM
  send18b20_byte(0x44); // 温度转换
  delay_pw();
  init_ds18b20();
  send18b20_byte(0xcc); 
  send18b20_byte(0xbe); // 读RAM
  receive18b20();		// 读取温度值
  conversion_t();		// 数值转换
  EA=1;
  TR0=1;
}



/*=====================================================================================
 函数名称：画面构造
 功能描述：界面1、2、3，图片菜单，反白，标头，16*16（32）图片
 全局变量：无
 参数说明：函数说明
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：X坐标每一格每次16点
 ======================================================================================*/


/*反白或显示图片 参数格式：(H)8bit数据，(L)8bit数据,X0坐标(16bit)，Y0坐标(1bit),X坐标(16bit)，Y坐标(1bit)*/
void con_disp (uchar data1,uchar data2,uchar x0,uchar y0,uchar xl,uchar yl)
{
  uchar i,j;
  for(j=0;j<yl;j++)
  {
    for(i=0;i<xl;i++)
    {
      wr_lcd(comm,0x34);     // 扩展功能
      wr_lcd(comm,y0+j);	 // 写Y坐标
      wr_lcd(comm,x0+i);	 // 写X坐标
      wr_lcd(comm,0x30);	 // 普通功能
      wr_lcd(dat,data1);
      wr_lcd(dat,data2);
    }
  }
  wr_lcd(comm,0x36);		 // 开绘图显示
}



/*显示字体 参数格式: 显示位置(0x80-0x9f),显示个数，显示内容指针*/
void display_cnasc(uchar dis_bit,uchar num,uchar *cnasc)
{
  wr_lcd(comm,0x30);	     // 普通功能
  wr_lcd(comm,dis_bit);	     // 写地址
  while(num>0)
  {
    wr_lcd(dat,*(cnasc++));
    wr_lcd(dat,*(cnasc++));
	num--; 
  }
}



/*导航标头显示*/
void dis_title(void)
{
  display_cnasc(0x80,1,"\x15 ");
  display_cnasc(0x90,1,"┏");
  display_cnasc(0x97,1,"┓");
  display_cnasc(0x98,1,"┗");
  display_cnasc(0x9f,1,"┛");
  display_cnasc(0x92,1,"\x1e ");
  display_cnasc(0x9a,1,"\x1f ");
  con_disp(0xff,0xff,0x80,0x80,3,16);
  con_disp(0xef,0xef,0x83,0x80,1,16);
  con_disp(0xde,0xf7,0x84,0x80,1,16);
  con_disp(0x76,0xdb,0x85,0x80,1,16);
  con_disp(0x55,0x55,0x86,0x80,1,16);
  con_disp(0x55,0x55,0x87,0x80,1,16);	
}


/*图片显示标头*/
void dis_title_pic(void)
{
  display_cnasc(0x80,2,"〓〓");
  display_cnasc(0x86,2,"〓〓");
}


/*功能实现显示标头*/
void dis_title_e(void)
{
  display_cnasc(0x80,1," \x15");
  con_disp(0xff,0xff,0x80,0x80,1,16);
  con_disp(0xff,0xc7,0x81,0x80,1,16);
  con_disp(0xff,0xff,0x82,0x80,3,16);
  con_disp(0xe3,0xff,0x85,0x80,1,16);
  con_disp(0xff,0xff,0x86,0x80,2,16);
}



/*显示菜单图片  格式：图片反显(1)或正常显示(0)，显示图片的号码*/
void anti_nor_pic (uchar cpl_sign,uchar menu_num)
{
  uchar i,j,x,y;
  x=menu_num*3;	   // 计算图片显示坐标
  if(x>6) x--;
  if(menu_num<3) y=16;	// 上下半屏显示
  else y=8;
  if(cpl_sign==1) display_cnasc(0x82,4,menu_cn[menu_num]);
  for(j=0;j<24;j++)
  {
   for(i=0;i<2;i++)
   {
     wr_lcd(comm,0x34);
     wr_lcd(comm,0x80+y);
     wr_lcd(comm,0x80+x+i);  // 分两次显示 16bit
     wr_lcd(comm,0x30);
     if(cpl_sign==1)	      // 图片反选
     {
	   wr_lcd(dat,~menu_pic[menu_num][j*4+i*2]);
       wr_lcd(dat,~menu_pic[menu_num][j*4+i*2+1]);
	 }
     else					  // 正常显示
     {
	   wr_lcd(dat,menu_pic[menu_num][j*4+i*2]);
       wr_lcd(dat,menu_pic[menu_num][j*4+i*2+1]);
	 }
   }
   y++;
   if(y==32&&menu_num<3) {x+=8;y=0;}  // 上下屏切换
  }
  wr_lcd(comm,0x36);
}




/*状态16*16图片  格式：反色（1）或正常0，X坐标，Y坐标，状态编码（16*16）*/
void dis_run_state(uchar cpl,uchar x0,uchar y0,uchar state_num)
{
  uchar j;
  for(j=0;j<16;j++)
  {
    wr_lcd(comm,0x34);  // 扩展功能
    wr_lcd(comm,y0++);	// 写Y地址
    wr_lcd(comm,x0);	// 写X地址
    wr_lcd(comm,0x30);	// 普通功能
	if(cpl==1)			// 写数据
    {
	  wr_lcd(dat,~running[state_num][j*2]);
      wr_lcd(dat,~running[state_num][j*2+1]);
	}
	else
	{
	  wr_lcd(dat,running[state_num][j*2]);
      wr_lcd(dat,running[state_num][j*2+1]);
	}
  }
  wr_lcd(comm,0x36);
}

 

 /*显示菜单按钮  参数格式：反色(1)或正常(0)*/
void dis_menu_key(uchar d)
{
  if(d==1)
  {
    for(d=0;d<8;d++)								  // 显示菜单按钮
      con_disp(~menu_key[d*2],~menu_key[d*2+1],0x9f,0x97+d,1,1);
  }
  else
  {
    for(d=0;d<8;d++)								  // 显示菜单按钮
      con_disp(menu_key[d*2],menu_key[d*2+1],0x9f,0x97+d,1,1);
  }
}




/*显示模式3的界面*/
void dis_mode3(void)
{
  uchar i,j,h,l,x_0=0x80,y_0=0x90;
  h=now.hour>>4;       // 分离十位 个位
  l=now.hour&0x0f;
  for(j=0;j<2;j++)
  {
    for(i=0;i<32;i++)  // 十位个位一起读入
    { 
      wr_lcd(comm,0x34);
      wr_lcd(comm,y_0++);
      wr_lcd(comm,x_0);
      wr_lcd(comm,0x30);
      wr_lcd(dat,mode_3num[h][i*3]);  // 连续写数据，X坐标自动加1
      wr_lcd(dat,mode_3num[h][i*3+1]);
      wr_lcd(dat,mode_3num[h][i*3+2]);
      wr_lcd(dat,mode_3num[l][i*3]);
      wr_lcd(dat,mode_3num[l][i*3+1]);
      wr_lcd(dat,mode_3num[l][i*3+2]); 
      if(y_0==0xa0){x_0=x_0+8;y_0=0x80;}
    }
    x_0=0x84,y_0=0x90;
    h=now.minute>>4;   // 分离十位 个位
    l=now.minute&0x0f;
  }
  dis_run_state(0,0x83,0x90,4); // 显示点
  dis_run_state(0,0x8b,0x80,4);					   
  y_0=0x80;			   // 显示状态标记
  if(keb) {dis_run_state(0,0x87,y_0,2);y_0+=16;}
  if(alm) {dis_run_state(0,0x87,y_0,1);y_0+=16;}
  if(y_0<0xa0&&!als){dis_run_state(0,0x87,y_0,0);}
  if(y_0==0xa0&&!als){dis_run_state(0,0x8f,0x80,0);}
  dis_menu_key(0);
}




/*显示模式1和2的共用部分*/
void mode1_2_union(void)
{
  uchar x=0x9b;
  display_cnasc(0x98,2,"星期");
  if(nba_tim) dis_run_state(0,x,0x90,6);
  if(keb) {dis_run_state(0,x,0x90,2);x++;}    // 显示标记
  if(alm) {dis_run_state(0,x,0x90,1);x++;}
  if(!als){dis_run_state(0,x,0x90,0);}
  dis_menu_key(0);                           // 显示菜单按钮
}


/*显示模式2的日期*/
void dis_date_mode2(uchar d)
{
  uchar save[2];
  display_cnasc(d+2,1,"年");
  display_cnasc(d+4,1,"月");
  display_cnasc(d+6,1,"日");
  display_cnasc(0x9a,1,weeks[today.week]);  // 显示星期
  save[0]=char_num[today.year>>4];	        // 将显示的数值存到数组中
  save[1]=char_num[today.year&0x0f];
  display_cnasc(d+1,1,save);
  save[0]=char_num[today.month>>4];
  save[1]=char_num[today.month&0x0f];
  display_cnasc(d+3,1,save);
  save[0]=char_num[today.day>>4];
  save[1]=char_num[today.day&0x0f];
  display_cnasc(d+5,1,save);
}

/*显示模式2的时间数字显示  16*32*/
void num2_asc_dis(uchar num,uchar x_0)
{
  uchar i,y_0=0x90;
  for(i=0;i<32;i++)  // 十位个位一起读入
  { 
    wr_lcd(comm,0x36);
    wr_lcd(comm,y_0++);
    wr_lcd(comm,x_0);
    wr_lcd(comm,0x30);
    wr_lcd(dat,mode_2num[num][i*2]);
    wr_lcd(dat,mode_2num[num][i*2+1]);
	if(i==15){x_0+=8;y_0=0x80;}
  }
  wr_lcd(comm,0x36);
}

/*显示模式2的时间*/
void dis_time_mode2(void)
{
  uchar h,l;
  if(now.hour!=save_time.hour||init_d2)
  {
    h=now.hour>>4;    // 分离十位 个位
    l=now.hour&0x0f;
    num2_asc_dis(h,0x80);
	num2_asc_dis(l,0x80+1);
  }
  if(now.minute!=save_time.minute||init_d2)
  {
    h=now.minute>>4;    // 分离十位 个位
    l=now.minute&0x0f;
    num2_asc_dis(h,0x83); 
	num2_asc_dis(l,0x83+1);
  }
  if(now.second!=save_time.second||init_d2)
  {
    h=now.second>>4;    // 分离十位 个位
    l=now.second&0x0f;
    num2_asc_dis(h,0x86);
	num2_asc_dis(l,0x86+1);
  }
  if(init_d2)
  {
    dis_run_state(0,0x82,0x90,5); // 显示点
    dis_run_state(0,0x8a,0x80,5);
    dis_run_state(0,0x85,0x90,5); // 显示点
    dis_run_state(0,0x8d,0x80,5);
  }	
}


/*显示模式2的界面*/
void dis_mode2(void)
{
  dis_date_mode2(0x81);	      // 显示日期
  display_cnasc(0x81,1,"20");
  dis_time_mode2();			  // 显示时间
  mode1_2_union();
}


/*显示模式1的日期*/
void dis_date_mode1(void)
{
  uchar save[2];
  display_cnasc(0x80,3,"日期："); 
  display_cnasc(0x90,3,"时间："); 
  display_cnasc(0x88,3,"温度：");
  display_cnasc(0x9a,1,weeks[today.week]);  // 显示星期
  display_cnasc(0x83,1,"20");
  save[0]=char_num[today.year>>4];			  
  save[1]=char_num[today.year&0x0f];
  display_cnasc(0x84,1,save);
  save[0]='/';
  save[1]=char_num[today.month>>4];
  display_cnasc(0x85,1,save);
  save[0]=char_num[today.month&0x0f];
  save[1]='/';
  display_cnasc(0x86,1,save);
  save[0]=char_num[today.day>>4];
  save[1]=char_num[today.day&0x0f];
  display_cnasc(0x87,1,save);
  mode1_2_union();
}


/*显示模式1的时间*/
void dis_time_mode1(void)
{
  uchar save[2];
  save[0]=char_num[now.hour>>4];
  save[1]=char_num[now.hour&0x0f];
  display_cnasc(0x93,1,save);
  save[0]=':';
  save[1]=char_num[now.minute>>4];
  display_cnasc(0x94,1,save);
  save[0]=char_num[now.minute&0x0f];
  save[1]=':';
  display_cnasc(0x95,1,save);
  save[0]=char_num[now.second>>4];
  save[1]=char_num[now.second&0x0f];
  display_cnasc(0x96,1,save);
}


/*显示模式1的温度*/
void dis_temp_mode1(void)
{
  uchar save[2];
  if(ds18b20)
  {
	 if(temp.t_data[0]>99)
	 {
	    save[0]=char_num[temp.t_data[0]/100];
		save[1]=char_num[(temp.t_data[0]/10)%10];
		display_cnasc(0x8b,1,save);
		save[0]=char_num[temp.t_data[0]%10];
		save[1]='.';
		display_cnasc(0x8c,1,save);
		save[0]=char_num[temp.t_data[1]/10];
		save[1]=' ';
		display_cnasc(0x8d,1,save);
		display_cnasc(0x8e,1,"℃");
	  }
	  else
	  {
        if(temp_pn) display_cnasc(0x8a,1,":-");
	    else  display_cnasc(0x8a,1,"：");
		save[0]=char_num[temp.t_data[0]/10];
		save[1]=char_num[temp.t_data[0]%10];
		display_cnasc(0x8b,1,save);
		save[0]='.';
		save[1]=char_num[temp.t_data[1]/10];
		display_cnasc(0x8c,1,save);
		display_cnasc(0x8d,1,"℃");
	  }
	}
  else display_cnasc(0x8b,3,"FF.F℃");
}



/*显示模式1的界面*/
void dis_mode1(void)
{
  dis_date_mode1(); 
  dis_time_mode1();
  dis_temp_mode1();
}



/*篮球器时间显示*/
void nba_dis_time(void)
{
  uchar d,dis[2];
  for(d=2;d<4;d++)
  {
	dis[0]=char_num[resu[d]/10];
	dis[1]=char_num[resu[d]%10];
	display_cnasc(0x81+d*2,1,dis);
  }
  display_cnasc(0x86,1,": ");
  if(resu[0]==1)
  {
	dis[0]='\x07';
	dis[1]=char_num[resu[4]/10];
	display_cnasc(0x93,1,dis);
	dis[1]='\x07';
	dis[0]=char_num[resu[4]%10];
	display_cnasc(0x94,1,dis);
  }
}


/*=====================================================================================
 函数名称：键盘扫描
 功能描述：读取键盘操作数据及部分状态的实现
 全局变量：dis_sig  t0_isr  temprun  b20time  alm_run als_temp	 buzstop  als
           lock_op  nba_tim 
 参数说明：无
 返回说明：键盘锁与密码锁时间到达后返回 255
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/

/*扫描键盘*/
uchar  keypad_scan()
{
  uchar key,dis[2];
  keyboard=0xf0;
  while (keyboard==0xf0)     // 等待按键按下
  {	 
 
  /*--------------------- 背光灯关闭延时 -----------------------*/
	
	delay(1000);	 			     
	if((++dis[0])==0&&bl_delay!=0) bl_delay--;  // 当有按键按下bl_delay重新赋值 	
	if(bl_delay==0&&!bl0) lcd_bl=close;         // 背光时间到且不是常开模式时关闭背光

	  
  /*----------------------- 键盘锁延时 -------------------------*/	

	if(keb&&dis_sig&&bl_delay==0&&resu[11]<2) resu[11]++; // 当有按键按下j清零
	if(bl_delay==0) bl_delay=15; 


  /*--------------------- 密码锁30秒锁延时 ---------------------*/		

	if(lock_op)				         
	{
	  if((++i)==0) save1++;	  	
	  if(save1==15) return(255);
	}
	

  /*---------------------- 时钟界面时间显示 ---------------------*/
	
	if(dis_sig&&t0_isr||nba_tim&&oth_run)     // 正常时钟界面与篮球器返回时钟界面显示
    {    
	  rds1302_date();	    // 读出日期 
      rds1302_time();		// 读出时间
	  if(di0) 	            // 模式2
	  {
	    dis_time_mode2(); 
		if(now.hour==0&&now.minute==0&&now.second==0)	dis_date_mode2(0x81); 
	  }
      else if(di1) 			// 模式3
	  { 
	    if(now.hour!=save_time.hour||now.minute!=save_time.minute)   // 时,分变化后显示
	      dis_mode3();	   
      }
      else 
	  {
	    dis_time_mode1();   // 模式1
	    if(now.hour==0&&now.minute==0&&now.second==0) dis_date_mode1();
	  }
	  save_time=now;        // 重新记录作下次比较
	  t0_isr=0;
	}
  
  
  /*-------------------- 温度计提示按键操作 ----------------------*/
  	
	if(temprun)
	{
	  if(!oth_run)
	  {
	    display_cnasc(0x96,2," key");    // 温度计时提示按键操作
		for(dis[1]=0;dis[1]<250;dis[1]++)
	    {
	      delay(1000);
		  if(keyboard!=0xf0)
		  {
		    keyboard=0x7f;
			if(keyboard==0x7d) return 0x7d;
			else  keyboard=0xf0;
		  }
	    }
		oth_run=1;
		display_cnasc(0x96,2,"    ");
	  }
	}
  
  
   /*------------------------ 蜂鸣器警报 --------------------------*/

	
	if(((alm_run&&alm)||thl_als)&&!buzstop)	// 温度，闹铃蜂鸣器警报
	{									    // 有按键按下时停止
	  if(now.second==0)	lcd_bl=close;
	  delay(15000);							// 读出温度时间比较
	  if(keyboard!=0xf0) break;
	  delay(15000);
	  if(keyboard!=0xf0) break;
	  delay(15000);
	  if(keyboard!=0xf0) break;
	  delay(15000);
	  if(alm_run)  lcd_bl=!lcd_bl;			// 只在闹铃功能闪
	  if(!als)buzzer=!buzzer;               // 警报开启后蜂鸣器开启			    
	} 


  /*----------------------- 温度刷新显示 -------------------------*/

  	
	if((temprun&&oth_run)||(b20time&&!di0&&!di1&&dis_sig))    // 温度计功能于模式1界面 
    {  
	 
	    ds18b20_func();					          // 温度检测功能
		oth_run=0;                          	  // 作交替标记用
	    if(temprun) return 20;		              // 温度计开启后返回
	    if(b20time){ dis_temp_mode1();b20time=0;} // 时钟界面5分钟更新一次，T0计时
    } 

  
   
   /*------------------------ 篮球器显示时间 ------------------------*/
	
	if(nba_tim&!oth_run)	    // 当篮球器模式下 oth_run==0为篮球器计时状态 
	{
	  if(EA) display_cnasc(0x8b,2," R\x10 ");  // 显示运行状态
	  else  display_cnasc(0x8b,2," S! ");
	  if(t0_isr)
	  {										   // 更新时间
	    nba_dis_time();
		if(resu[2]==0&&resu[3]==0)			   // 判断比赛情况 
		{
		  resu[6]++;
		  if(resu[6]<resu[1])  display_cnasc(0x85,3,"休息  ");
		  else  display_cnasc(0x85,3,"完结  ");
		}
		if(resu[4]==0||(resu[2]==0&&resu[3]==0))  // 时间到达警报
	    {
	       buzzer=open;
	       delay_pw();
		   if(resu[2]==0&&resu[3]==0)  // 24秒 总时间警报
		   {
		     delay_pw();
		   }
	       buzzer=close;
		   if(resu[4]==0) resu[4]=24;  // 重新赋值
	    }
	    t0_isr=0;					   // 清零等下次中断产生
	  }
	}
  }

			  
			        /*===== 以上等待按键 =====*/



  /*------------------------- 按键按下后 --------------------------*/

  
  delay(8000); 	             // 消抖振
  if(!lock_op)bl_delay=15;   // 背光时间赋值 
  if(bl1) lcd_bl=close;	     // 不在背光灯关闭时亮起
  else lcd_bl=open;
  if(alm_run|thl_als) {buzstop=1;buzzer=close;}
  if(keb&&dis_sig) 
  {
    if(resu[11]==2) return 255;  // 要求输入密码	
  	else resu[11]=0;			 // 时间不到时重新计时
  }
  do
  {    	
	for(dis[0]=0;dis[0]<=3;dis[0]++)
    {
      keyboard=ksp[dis[0]];
	  if(keyboard!=ksp[dis[0]])
      {	
        key=keyboard;
        if(key!=ksp[dis[0]]) return(key);
      }
    }
  }while(1);
} 
 

/*读键盘数据*/
uchar  gotkey()
{
  uchar  save,n;
  keyboard=0xf0;
  while(keyboard!=0xf0);    // 等待键盘松开
  delay(5000);	            // 消抖阵
  if(adju_si) 	            // 连写输入
  {
	adju_si=0;
	for(n=0;n<60;n++)
  	{
	  delay(2000);
	  if(keyboard!=0xf0)
	  { 
	    adju_si=1;          // 在更改时,在此时间内可连成多位数
 	    break;
	  }
    }
  }			
  save=keypad_scan();	    // 扫描键盘
  if(save==255)	return 255;	// 要求输入键盘锁密码或延时时间到达
  for (n=0;n<16;n++)
  {  
	if(save==key_code[n]) 
	   return(n);
  }
  return(20);
}



/*=====================================================================================
 函数名称：导航
 功能描述：主菜单 工具导航  保存提示
 全局变量：无
 参数说明：
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


/*按键按下显示 参数格式：k 按钮的显示地址*/
void key_dis(uchar k)
{
  display_cnasc(k,1,"\x04 ");
  delay(10000);
  if(k==0x92||k==0x96) display_cnasc(k,1,"\x1e ");
  else display_cnasc(k,1,"\x1f ");
}



/*确定按键按下显示*/
void enter_dis(void)
{
  display_cnasc(0x8f,1," \x09");
  delay(15000);
}



/*主菜单导航*/
uchar dis_menu(void)
{
  uchar d,save,key;
  if(meu)
  {  
    display_cnasc(0x81,2,"菜单");
	dis_title();
	display_cnasc(0x88,8,selected[0]);
	con_disp(0xff,0xff,0x8a,0x80,5,16);
  } 
  else	  // 显示图片菜单
  {
    dis_title_pic();
	for(d=0;d<6;d++)  
      anti_nor_pic(0,d);
	anti_nor_pic(1,0);
  }      
  save=d=0;
  do
  {
	if(meu)	  // 操作指示更新
	{	  
	  display_cnasc(0x93,3,menu_cn_2[index_m[save].left_num]);
	  display_cnasc(0x8b,3,menu_cn_2[save]);
	  display_cnasc(0x9b,3,menu_cn_2[index_m[save].right_num]); 
    }
	else
	{
	  anti_nor_pic(0,d);     
	  anti_nor_pic(1,save);
	}
	d=save;			      // 保存上一索引
	while(1)
	{
	  key=gotkey();
	  if(key==11||(key==3&&meu))
	  { 
	    if(meu) enter_dis();
		break;
	  }
	  else if(meu)
	  { 
	    if(key==2)
		{
		  save=index_m[d].right_num;
		  key_dis(0x9a);
		  break;
		}
	    if(key==5)
		{
		  save=index_m[d].left_num;
		  key_dis(0x92);
		  break;
		}
		if((key==1&&meu))
	    {
	      d=5;
		  break;
	    } 
	  }
	  else 
	  {
	    if(key==3){save=index_m[d].right_num; break;} // 根据导航键寻找索引
	    if(key==1){save=index_m[d].left_num; break;}
		if(key==2||key==5){save=index_m[d].shaft_num; break;}  
      }
	}
	if(key==11||((key==1|key==3)&&meu)) break;
  }while(1);
  return d;
}



/*工具导航*/
uchar tool_func(void)
{
  uchar save,d,key;
  display_cnasc(0x81,2,"工具");
  dis_title();
  display_cnasc(0x88,8,selected[0]);
  con_disp(0xff,0xff,0x8a,0x80,5,16); 
  d=0;
  save=0;
  do
  {	  
	display_cnasc(0x93,3,tool_menu[index_t[save].lnum]);
	display_cnasc(0x8b,3,tool_menu[save]);
	display_cnasc(0x9b,3,tool_menu[index_t[save].rnum]); 
	d=save;
	while(1)
	{
	  key=gotkey();
	  if(key==1){ d=4;break;}				// 根据按键寻找索引
	  if(key==11||key==3){enter_dis();break;}
	  if(key==2){save=index_t[d].rnum; key_dis(0x9a);break;}
	  if(key==5){save=index_t[d].lnum; key_dis(0x92);break;}
    }
	if(key==11||key==3||key==1) break;
  }while(1);
  return d;
}




/*更改后提示保存*/
void save_y_or_n(void)
{
  uchar key;
  clear_dis();
  oth_run=0;	  
  display_cnasc(0x92,5,"是否保存? ");
  display_cnasc(0x89,6,"Y(OK)\\N(./S)");
  while(1)
  {
	key=gotkey();
	if(key==11) 
	{
	  display_cnasc(0x9a,5,"保存成功！ ");
	  delay(50000);
	  oth_run=1;	 // 要求保存时oth_run标记为1
	  return;
	}
	if(key==10) return;
  }
} 


/*=====================================================================================
 函数名称：调整功能
 功能描述：时间日期调整
 全局变量：today  now
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


/*调整功能显示时间*/
void asjust_time()
{ 
  uchar save[2];
  display_cnasc(0x8a,1,"时");
  display_cnasc(0x8c,1,"分");
  display_cnasc(0x8e,1,"秒");
  save[0]=char_num[now.hour>>4];
  save[1]=char_num[now.hour&0x0f];
  display_cnasc(0x89,1,save);
  save[0]=char_num[now.minute>>4];
  save[1]=char_num[now.minute&0x0f];
  display_cnasc(0x8b,1,save);
  save[0]=char_num[now.second>>4];
  save[1]=char_num[now.second&0x0f];
  display_cnasc(0x8d,1,save);
}


/*调整功能月天数比较*/
uchar monthday(uchar year,uchar month)
{
  if(month==2&&year%4==0)
    return(29);
  else
  {	year=month&0x0f;
    month>>=4;
	month=month*10+year;	
    return(dayofmonth[month-1]);}
}



/*星期推算*/
void weeks_day()
{ 
  uchar d,buffer,day=4; 
  if(today.month>1)
  {
    buffer=(today.month>>4);   // 将BCD码转为十进制
	buffer=buffer*10;
	buffer+=(today.month&0x0f);
	for(d=0;d<buffer-1;d++)
	{
	  today.week=(dayofmonth[d]>>4);
	  today.week=today.week*10;
	  today.week+=(dayofmonth[d]&0x0f);
      day+=(today.week%7);
	}
  }
  buffer=(today.year>>4);
  buffer*=10;
  buffer+=(today.year&0x0f);
  if(today.year!=0)
  {
	day+=((buffer-1)/4)+buffer+1;
  }
  if(buffer%4==0&&today.month>2) day++;
  buffer=today.day>>4;
  buffer*=10;
  buffer+=(today.day&0x0f);
  today.week=(day+buffer)%7;
}



/*调整功能*/
void adjust_func(void)
{
  uchar d,save,b=0;
  dis_title_e();
  display_cnasc(0x82,3,menu_cn_2[0]);
  rds1302_date();
  rds1302_time();
  dis_date_mode2(0x90);
  display_cnasc(0x9a,1,"  "); 
  asjust_time();
  display_cnasc(0x9f,1,"→");
  display_cnasc(0x90,1,"『");
  display_cnasc(0x8f,1,"』");
  d=0;save=0;
  adju_si=0;
  do
  {
    con_disp(0x00,0x00,index_a[d].lnum,index_a[d].rnum,1,13);
    con_disp(0xff,0xff,index_a[save].lnum,index_a[save].rnum,1,13);
	d=save;
	while(1)
	{
	  save=gotkey();
	  if(save>=0&&save<10)
	  {
	    switch(d)
	    {
	      case 0: if(adju_si) // adju_si==1时两次输入连成一个数
		          { 
				    today.year<<=4;
			        today.year+=save;adju_si=0;
				  } 
	              else 
			      {
			        today.year=save;adju_si=1;
			      }
		          break;
		  case 1: if(adju_si)
		          {
				    today.month<<=4;today.month+=save;
		            if(today.month>0x12) today.month=save;
                    else adju_si=0;
				  }
				  else  
				  {
				    today.month=save;adju_si=1;
				  }
				  break;
		  case 2: if(adju_si)
		          {
				    today.day<<=4;today.day+=save;  // 月天数比较
					if(today.year%4==0&&today.month==2&&today.day==29) ;
		            else if(today.day>monthday(today.year,today.month)) 
					  today.day=save;
					else adju_si=0;
				  }
				  else  {today.day=save;adju_si=1;}
				  break;
		  case 3: if(adju_si)
		          {
				    now.hour<<=4;now.hour+=save;
		  			if(now.hour>0x23) now.hour=save;
					else adju_si=0;
				  }
				  else  {now.hour=save;adju_si=1;}
				  break;
		  case 4: if(adju_si)
		          {
				    now.minute<<=4;now.minute+=save;
		  			if(now.minute>0x59) now.minute=save;
					else adju_si=0;
				  }
				  else  {now.minute=save;adju_si=1;}
				  break;
		  case 5: if(adju_si)
		          {
				    now.second<<=4;now.second+=save;
		  			if(now.second>0x59) now.second=save;
					else adju_si=0;
				  }
				  else  {now.second=save;adju_si=1;}
				  break;
		}
		if(today.day>monthday(today.year,today.month)) today.day=1;	// 更改月份时判断天数
		if(today.month==0) today.month=1;
		if(today.day==0) today.day=1;
		if(d<3) dis_date_mode2(0x90); 	// 更新显示
        else asjust_time();
		display_cnasc(0x9a,1,"  ");		// 消去星期显示
      }
	  if(save==11){save=d+1;adju_si=0;break;}
	  if(save==10&&d!=0){save=d-1;adju_si=0;break;}
	}
    if(save==7&d==6) break;
  }while(1);
  weeks_day();	  // 计算星期
  save_y_or_n();  // 需要保存时oth_run设为1
  if(oth_run) wds1302_time_date();
}




/*=====================================================================================
 函数名称：计算器功能
 功能描述：10位带符号数的加减乘除运算
 全局变量：opera  resu  i  j
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/



/*清寄存器  参数格式：m=2时结果寄存器，当m=1时操作数1，当m=0时操作数0(1),
                        n=1时,只清寄存器不清显示, 0两清*/
void removal_data(uchar m,uchar n)
{
  uchar l;		// 当n=1时,只清寄存器不清显示, 0两清
  if(m==2)		// 当m=2时结果寄存器
  {			    // 当m=1时操作数1
    if(n==0)	// 当m=0时操作数0 ,1
	  for(l=0;l<6;l++)
	    display_cnasc(0x98+l,1,"  ");
    for(l=0;l<12;l++)
      resu[l]=0;
  }
  while(m<2)
  { 
    if(n==0)
	{
	  if(m==0) 
	  {  
	    for(l=0;l<6;l++)
	      display_cnasc(0x90+l,1,"  ");
		  display_cnasc(0x95,1," 0");
	  }	
	  else
	     for(l=0;l<6;l++)
	        display_cnasc(0x88+l,1,"  ");
	}
    for(l=0;l<12;l++)
      opera[m][l]=0;
	m++;
  }
}




/*显示数值  参数格式：显示首地址*/
void dis_cdata(uchar ass)
{
  uchar p,d,save[2];
  if(i==0&&opera[0][0]==1) 		// 显示符号
  {
    if(cal_run)  display_cnasc(0x98,1,"=-");
    else  display_cnasc(0x90,1," -");
  }
  d=opera[i][11];				// 计算位数
  if(opera[i][10]!=0&&opera[i][10]!=opera[i][11]+1) d++;
  save[0]=d/2;ass=ass-save[0]+5;	// 计算显示的开始位置
  p=1;
  cal_del=0;					// 显示点标记 （0为没有显示）
  if(d%2==1)					// 对位数位奇数先处理首位
  { 
    save[0]=' ';
    save[1]=char_num[opera[i][1]];
	display_cnasc(ass,1,save);
	p++;
  }
  ass++;						// 显示地址加一
  while(p<=opera[i][11])
  {									
    if(opera[i][10]==p&&(!cal_del)) {   // 显示点且作标记
	  save[0]='.';cal_del=1;}
    else { 
	  save[0]=char_num[opera[i][p]]; p++;}
    if(opera[i][10]==p&&(!cal_del)) { 
	  save[1]='.';cal_del=1;}
    else{ 
	  save[1]=char_num[opera[i][p]];p++;}
    display_cnasc(ass,1,save);
    ass++;
  }
}   


/*从键盘中写入数据*/   
char input_data()
{
  uchar save=20,c;
  for(;i<2;i++)
  {
    while(1)
    {
pp:   save=gotkey();	// 读键盘
	  if(save==20) continue;
	  if(save==11)		// 按键为确定时,选择计算或删除
	  {
	    c=save=0;
		while(1)		// 反色选择
	    { 
		  con_disp (0x00,0x00,index_cal[save].lnum,index_cal[save].rnum,2,16);
		  con_disp (0xff,0xff,index_cal[c].lnum,index_cal[c].rnum,2,16);	
		  save=gotkey();
		  if(save==10)
		  { 
		    save=c;
			if(++c==3) c=0;
		  }
		  else if(save==11) 
		  { 
            if(c==0) save=11;
		    else if(c==1) save=16;
			else  return 0;
		    break;
		  }
		  else break;
        }
		con_disp (0x00,0x00,index_cal[c].lnum,index_cal[c].rnum,2,16);	    
	  }
      if(save>=0&&save<=9&&j<10) 
	  {
	    if(i==1&&opera[1][0]==15&&j>8) goto pp;
        if(j==2&&opera[i][1]==0&&opera[i][10]==0)
          opera[i][1]=save;		// 防止小数位不在时首位为零
	    else{
		  opera[i][j]=save;opera[i][11]=j;j++;}
		if(i==0)				// 显示
		  dis_cdata(0x90);
		else
		  dis_cdata(0x88);
	  }
      else if(save==10&&j<10)	
      {
	    if(opera[i][10]==0)		// 小数点空时保存
	    {
		  if(opera[i][11]==0) {opera[i][10]=2;j++;}
	      else  opera[i][10]=j;
	    }
	  }
	  else if(save==16)	
	  {                           // 删除数据
        removal_data(i,0);j=1;
		if(i!=0){i--;j=10;}
      }
	  else if(i==1&&opera[1][11]!=0&&save>11&&save<16)
      { save1=save;break; }   		// 连续计算时保存运算符
      else if(save==11&&opera[1][11]==0) continue;// 操作数1为0时不理会
	  else if(save==11&&opera[1][11]!=0)  break; 
	  else if((i==0||i==1&&j==1)&&save>11&&save<16){i=0; break;}    
       // 当操作数0由运算符结束输入,再按符号键更改
	  else continue;
    }
    if(opera[i][10]>opera[i][11]||opera[i][10]==0)
      opera[i][10]=opera[i][11]+1;	// 整数时小数位保存到最后
    while(opera[i][11]>=opera[i][10]&&opera[i][opera[i][11]]==0)
      opera[i][11]--;				// 去除小数点后多余的0
    if(i==0)
    {
      opera[1][0]=save;			// 保存运算符并显示
      display_cnasc(0x88,1,tool_cal[save-12]);
    }
    if(opera[0][11]==0) display_cnasc(0x95,1," 0");
    j=1;
    while(j<=opera[i][11]&&opera[i][j]==0) j++;//判断是否操作数是否全为0
    j--;
    if(j==opera[i][11]) opera[i][11]=0;
    j=1;
  }
  return 1;
}


/****************************************************************************************
功能:  乘法运算
描述:  结果由右到左保存
参数:  i , j , opera[i][j],p,q,r,k,save1
返回值：1(成功),0(结果位0),-1(溢出)
/****************************************************************************************/

char multiplication()
{
  uchar p,q,r,k,save1=0; // p指向操作数0 ,q指向操作数1,r指向结果
  if(opera[0][11]==0||opera[1][11]==0) return 0;
  resu[10]=opera[0][11]+opera[1][11]-opera[0][10]-opera[1][10]+2; // RESU记录小数点后的位数
  q=opera[1][11];					// 记录操1的位数
  r=9;
  opera[1][11]=1;
  k=1;
  while(opera[1][k]==0) {opera[1][11]=opera[1][11]+1;k++;}	// 去除前端的0
  k=1;
  while(opera[0][k]==0) k++;			// 去除前端的0
  resu[11]=9;
  for(resu[0]=9;q>=opera[1][11];q--,resu[0]--)
  {
    if(opera[1][q]==0) continue;		// 操1此位数为0时直接比较
    for(p=opera[0][11],r=resu[0];p>=k;p--)
    {
      save1=opera[0][p]*opera[1][q];		// 将相乘后的数保存
      resu[r]+=(save1%10);//将缓存中的数取余后加到结果中R指向的位
      save1/=10;
      save1=save1+(resu[r]/10);			// 保存进位的数
      resu[r]%=10;					// 重新写入
	  if(r==1)	//结果有溢出时移位
	  {
	    if(save1==0&&p==k) break;	// 计算到最后位且没进位跳出
	    else if(resu[10]>0)		// 要求有小数存在
        { 
	      for(r=9;r>1;r--) resu[r]=resu[r-1]; 
	      r=1;
          resu[r]=save1;
	      resu[11]=1;
	      resu[10]--;
	      resu[0]++;
        }   
	    else   return -1;	 		// 否则溢出
	  }
	  if(r>1&&(r-1)<resu[11])			// 保存结果位的最高位
	  {
	    if(save1!=0) resu[11]=r-1;
	    else resu[11]=r;
	  }
	  if(r>1){ r--;resu[r]+=save1;}// 当R为1时只需要移位记录结果
    }
  }
  while(resu[resu[11]]==0) resu[11]++; 	// 去除前端的0
  resu[0]=opera[0][0];				// 保存符号位
  removal_data(0,1);					// 清数据不清显示
  if(resu[10]>8)   //如果小数点后的数有9 位或以上时，需后移到使小数点放到2的位置  
  {
    save1=resu[10]-10+resu[11]; 		// 记录后移位数
    if(save1>7)  return 0; 	 //当后移的数大于有效数字的位数时结果为0 
    else 	// 否则按需移位
    {
      for(p=save1+2,r=resu[11];p<10;p++,r++) opera[0][p]=resu[r];
	  opera[0][10]=2;opera[0][11]=9;opera[0][0]=resu[0];
    }		// 因有效数小于小数点后的数,所以首位为0  
  }	
  else  	//如果小数点后的数在8位以内时分两种情况
  {	
    opera[0][0]=resu[0];  
    save1=10-resu[11]; 	// 记录有效位数
    if(resu[10]>=save1) 	// 小数的在有效数字的左边
    {	
	  for(p=resu[10]+1,r=9;r>=resu[11];p--,r--)
        opera[0][p]=resu[r];	// 从小数点的最后一位开始
      opera[0][10]=2;opera[0][11]=resu[10]+1;
    }
    else  				// 小数点在有效数字内
    {  
	  for(p=1,r=resu[11];r<10;p++,r++) opera[0][p]=resu[r];
	    opera[0][11]=10-resu[11];
	  if(resu[10]==0) opera[0][10]=opera[0][11]+1;
	  else opera[0][10]=opera[0][11]-resu[10]+1;
    }
  }
  while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0) 
    opera[0][11]--;  	// 去除小数点后多余的0
  return 1;
}


/****************************************************************************************
功能:  除法运算
描述:  结果由左到右保存
参数:  i , j , opera[i][j]
p,q,r, remainde, divisor, decimal
返回值：1(成功),0(结果位0),-1(溢出)
*****************************************************************************************/

char divider()
{
  unsigned long int divisor=0,remainder=0;
  uchar p,q,r;
  char decimal=0;
  if(opera[1][11]==0) return -1;
  if(opera[0][11]==0) return 0;
  for(r=1;r<12;r++)		// 判断两数是否相等
  {
    if(opera[0][r]!=opera[1][r]) break;
  }
  if(r==12)
  { 
    p=opera[0][0];
    removal_data(0,1);
    opera[0][0]=r;
    opera[0][1]=1;
    opera[0][10]=2;
    opera[0][11]=1;
    return 1;
  }  
  decimal=(opera[0][11]-opera[0][10])-(opera[1][11]-opera[1][10]);
  // 记录两操作数小数点后位数的差 
  if(decimal>0)		// 若操0大于操1时重新记录小数点的位置
  {
    opera[0][10]=opera[0][11]-decimal+1;
    resu[10]=opera[0][10];
  }
  else  	// 否则小数点后没有数，记录有效数的位数
  {	
    opera[0][11]-=decimal;
    opera[0][10]=opera[0][11]+1;
    resu[10]=opera[0][10];
  }
  q=1;
  divisor=0;
  while(q<=opera[1][11]) // 将操1的数整形化
  {
    divisor=divisor*10+opera[1][q];q++;
  }
  if(divisor==1)   
  {
    if(resu[10]>10)	return -1;
	else return 1;
  }
  r=1;p=1;
  remainder=(unsigned long int)opera[0][1]; // 先对余数寄存器附初值
  do
  {   
	if(r>9)  // 结果位已经到达最后1位
	{  
  	  if(resu[1]==0&&resu[10]!=2) // 判断是否有得移位
	  { 	
	    for(r=1;r<9;r++) resu[r]=resu[r+1];	
	    resu[9]=0;
	    if(resu[10]!=0) resu[10]--;
	    r=9;
	  }
      else if(p>=opera[0][11]||p>=opera[0][10])  break; // 如果计算到最后一位，或者有小数点存在时结束
      else return -1; // 否则错误
	}
	resu[r]=(uchar)(remainder/divisor);	// 保存结果
    remainder=(remainder%divisor);		// 重新写入余数
	p++;r++;	 
    if(p>opera[0][11]&&remainder==0) break; // 除尽后跳出
    else if(p<=opera[0][11]&&p<10)		// 余数*10后加后一位
      {remainder=remainder*10+opera[0][p];}
    else   remainder=remainder*10;	 //超出后余数*10
  }while(1);
  resu[11]=r-1;
  resu[0]=opera[0][0];
  removal_data(0,1);
  r=1;   
  while(resu[r]==0&&resu[10]!=(r+1)&&r<resu[11]) r++; 
//去除结果前端的0
  if(r==resu[11]) return 0;
  resu[10]=resu[10]-r+1;
  p=1;
  while(r<=resu[11]){opera[0][p]=resu[r];r++;p++;}
  opera[0][0]=resu[0];opera[0][10]=resu[10];opera[0][11]=p-1;
  while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0) 
    opera[0][11]--;
  return 1;
}


/****************************************************************************************
功能:  加法运算
描述:  以小数点位中心两边相加
参数:  i , j , opera[i][j],p,q,r
返回值：1(成功),0(结果位0),-1(溢出)
/****************************************************************************************/

char adder()
{
  uchar p,q,r;
  if(opera[0][11]==0&&opera[1][11]==0)   return 0;
  else if(opera[0][11]==0)
    {for(q=0;q<12;q++) opera[0][q]=opera[1][q];return 1;}
  else if(opera[1][11]==0) return 1; 
  else
  {
    p=opera[0][10]-1;
    q=opera[1][10]-1;
    p>q?(r=p):(r=q);
    resu[10]=r+1;		// 小数点位保存
    resu[0]=opera[0][0];	// 保存符号位
    while(r>0)			// 将小数点前的数相应相加后放到结果中
    {
      if(p>0&&q>0){resu[r]=opera[0][p]+opera[1][q];p--;q--;}
      else if(p>0&&q==0){resu[r]=opera[0][p];p--;}
      else	{resu[r]=opera[1][q];q--;}
      r--;
    }
    r=resu[10];
    p=opera[0][10];
    q=opera[1][10];
    while(r<10)			// 将小数点后的数放到结果中
    {
      if(p>opera[0][11]&&q>opera[1][11]) break;
      else if(p<=opera[0][11]&&q<=opera[1][11])
        {resu[r]=opera[0][p]+opera[1][q];p++;q++;}
      else if(p<=opera[0][11]&&q>opera[1][11])
        {resu[r]=opera[0][p];p++;}
      else	{resu[r]=opera[1][q];q++;}
      r++;
    }
    r--;
    resu[11]=r;
    p=0;
    while(r>0)			// 由右到左将大于9的数进位
    {
      resu[r]+=p;			// p为进位标记
      p=0;
      if(resu[r]>9) {resu[r]-=10;p=1;}
      r--;
    }
    if(p==1)				// 溢出移位
    {
      if(resu[10]==10) return -1;
      else {
        for(r=9;r>1;r--) resu[r]=resu[r-1];
        resu[1]=1;resu[10]++;}
    }
   removal_data(0,1);
   r=0; 
   while(r<12){opera[0][r]=resu[r];r++;}
   while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0)
     opera[0][11]--;
   return 1;
  }
}

/****************************************************************************************
 功能:  减法运算
 描述:  先比较两操作数的大小用one ,two互换
参数:  i , j , opera[i][j],p,q,r，one，two
返回值：1(成功),0(结果位0),-1(溢出)
/****************************************************************************************/

char subtraction()
{
  uchar p,q,r,one,two;
  if(opera[0][11]==0&&opera[1][11]==0)   return 0;
  else if(opera[0][11]==0)
  {
    for(q=1;q<12;q++) 
    opera[0][q]=opera[1][q];
	opera[0][0]=!opera[0][0];return 1;
  }
  else if(opera[1][11]==0) return 1; 
  else
  { // 比较大小(大放到one)
    if(opera[0][10]>opera[1][10]){one=0;two=1;}
    else if(opera[1][10]>opera[0][10]) {one=1;two=0;}
    else
    {
      r=1;
	  while(r<10&&opera[0][r]==opera[1][r]) r++;
	  if(r==10) {removal_data(0,1);return 0;}
      opera[0][r]>opera[1][r]?(one=0,two=1):(one=1,two=0);
    }
    p=opera[one][10]-1;
    q=opera[two][10]-1;
    r=p;			// 保存小数点前的数
    resu[10]=r+1;	// 保存小数点位
    resu[0]=0;		// 用作借位标记
    while(r>0)		// 小数点前相减
    {
      if(p>0&&q>0)
	  {
	    opera[two][q]+=resu[0];
        resu[0]=0;
        if(opera[one][p]>=opera[two][q])
          resu[r]=opera[one][p]-opera[two][q];
	    else
	    {
		  resu[0]=1;
		  resu[r]=10+opera[one][p]-opera[two][q];
	    }
	    p--;q--;
	  }
	  else 
	  {	    
        if(opera[one][p]<resu[0])
          {resu[r]=10+opera[one][p]-resu[0];}
	    else
		  {resu[r]=opera[one][p]-resu[0];resu[0]=0;p--;}
	  }
      r--;
    }
    r=resu[10];
    p=opera[one][10];
    q=opera[two][10];
    while(r<10)		// 将小数点后的数放到结果中
    {
      if(p>opera[one][11]&&q>opera[two][11]) break;
      else if(p<=opera[one][11]&&q<=opera[two][11])
	  {
        if(opera[one][p]>=opera[two][q])  resu[r]=opera[one][p]-opera[two][q];
	    else
	    {
	      resu[r]=10+opera[one][p]-opera[two][q];
	      resu[0]=r;
	      for(r-=1;r>0;r--) // 向前借位
		  {
	        if(resu[r]==0) resu[r]=9;
		    else{resu[r]-=1;break;}
		  }
		  r=resu[0];	
	    }
	    p++;q++;
	  }				            
      else if(p<=opera[one][11]&&q>opera[two][11])
        {resu[r]=opera[one][p];p++;}
      else	
	  {
	    resu[r]=10-opera[two][q];  
	    resu[0]=r;
	    for(r-=1;r>0;r--)
        {
		  if(resu[r]==0) resu[r]=9;
	      else{ resu[r]-=1;break;}
		}
	    r=resu[0];
	    q++;	
	  }
      if(r==9) 		// 结果溢出移位
	  {
	    if(resu[1]==0&&resu[10]!=2) 
        {
          for(r=1;r<9;r++) resu[r]=resu[r+1];
		  resu[10]--;resu[9]=0;r=8;
        }
	  }
      r++;
    }
    r--;
    resu[11]=r;
    if(one==0)resu[0]=opera[0][0];	// 符号位写入
    else resu[0]=!opera[0][0];
    removal_data(0,1);
    r=1;
    while(resu[r]==0&&resu[10]!=(r+1)) r++; // 去除前端0
    resu[10]=resu[10]-r+1;			// 重新写入小数点
    p=1;
    while(r<=resu[11]){opera[0][p]=resu[r];r++;p++;}
    opera[0][0]=resu[0];opera[0][10]=resu[10];opera[0][11]=p-1;
    while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0) opera[0][11]--;
    return 1;
  }
}


/****************************************************************************************
功能:  计算器主函数
描述:  根据符号位,运算符调用函数
参数:  state,save
返回值：无
****************************************************************************************/


void calculator(void)
{
  char data state;
  uchar data save;
  save1=0;
  dis_title_e();
  display_cnasc(0x82,3,tool_menu[0]); 
  display_cnasc(0x96,2,"等于");
  display_cnasc(0x8e,2,"删除");
  display_cnasc(0x9e,2,"退出");
  removal_data(2,0); 	// 0为清除显示
  removal_data(0,0);
  i=0;j=1;
  while(1)
  {	
    cal_run=0;
    state=input_data();
    if(state==1)  // 根据opera[1][0]的数值调用程序
    {
      if(opera[1][0]==12&&opera[0][0]==0||opera[1][0]==13&&opera[0][0]==1) 
	    state=adder();
      else if(opera[1][0]==12&&opera[0][0]==1||opera[1][0]==13&&opera[0][0]==0)
	    state=subtraction();
      else if(opera[1][0]==14) state=multiplication();
      else state=divider();
    }
	if(state==0)  return;
    i=0;j=1;
    cal_run=1;
    switch(state)
    {
      case -1: 						// 结果溢出
	          removal_data(2,0);
              removal_data(0,0);  
		      display_cnasc(0x8a,3,"溢出!!");
		      save1=0;
			  break;
      case  0: 	       				// 结果为零
			  if(save1==0)
			  { 
			    removal_data(2,1);
				removal_data(0,1);
			    display_cnasc(0x98,1,"= ");
			    display_cnasc(0x9d,1," 0");
			  }
			  break;				// 结果显示
      case  1: 
			  if(save1==0) 
			  { 
			    cal_run=1;
			    display_cnasc(0x98,1,"= ");
                dis_cdata(0x98);
			  }
			  break;
    }
	if(save1==0) save=gotkey();	 // save1记录连续计算符
	cal_run=0;	
	if((save>11&&save<16&&state==1)||save1!=0) 
	{
	  removal_data(2,0);
	  removal_data(1,0);
	  display_cnasc(0x90,6,"               ");
	  if(opera[0][11]==0) display_cnasc(0x95,1," 0");
	  else dis_cdata(0x90);
	  if(save1!=0)save=save1;
	  save1=0;
	  display_cnasc(0x88,1,tool_cal[save-12]);
	  opera[1][0]=save;
	  i=1;j=1;
	}
	else if(save>=0&&save<10&&state==1)
	{
	  removal_data(2,0);
	  removal_data(0,0);
	  opera[0][1]=save;
	  opera[0][11]++;
	  dis_cdata(0x90);
	  i=0;j=2;
	}
	else if(save==10)
	{  
	  removal_data(2,0);
	  removal_data(0,0);
      opera[i][10]=2;
	  opera[i][11]=1;
      j=2;
	}
	else 
	{ 
	  removal_data(2,0);
	  removal_data(0,0);
	}
  }											
}
  

/*=====================================================================================
 函数名称：温度功能
 功能描述：连续检查温度
 全局变量：temprun  oth_run  als_temp  adju_si 
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


/*温度显示*/
uchar temp_func_dis(void)
{
  uchar save[2],k;
  clear_dis();
  dis_title_e();
  display_cnasc(0x82,3,"温度计");  
  display_cnasc(0x98,3,"设定");
  display_cnasc(0x9e,3,"退出");
  display_cnasc(0x8e,1," \x1f");
  num2_asc_dis(10,0x85);
  temprun=1;
  while(1)
  {
	if(ds18b20)
    {
      if(temp.t_data[0]>99)	  // 温度值大于99时
	  {
	    save[0]=temp.t_data[0]/100;
        save[1]=(temp.t_data[0]/10)%10;
        num2_asc_dis(save[0],0x80);
	    num2_asc_dis(save[1],0x81);
	    save[0]=char_num[temp.t_data[0]%10];
	    num2_asc_dis(save[0],0x82);
      }
	  else					 // 温度小于100 
	  {
        con_disp(0x00,0x00,0x80,0x90,1,16);
		con_disp(0x00,0x00,0x88,0x90,1,16);
		if(temp_pn) con_disp(0x3f,0xfc,0x80,0x9d,1,3);
	    else  con_disp(0x00,0x00,0x80,0x9d,1,3);
	    save[0]=temp.t_data[0]/10;
	    save[1]=temp.t_data[0]%10;
	    num2_asc_dis(save[0],0x81);
	    num2_asc_dis(save[1],0x82);
	  }
	  save[0]='.';
	  save[1]=char_num[temp.t_data[1]/10];
	  display_cnasc(0x8b,1,save);
	  save[0]=char_num[temp.t_data[1]%10];
	  if(save[0]=='1') save[1]='2';		   // 补偿精度
	  else if(save[0]=='7') save[1]='5';
	  else if(save[0]=='8'||save[0]=='1') save[1]='7';
	  else save[1]='0';
	  display_cnasc(0x8c,1,save);
    }
	else 
	{
      clear_dis();
      display_cnasc(0x92,4,"没有发现");
      display_cnasc(0x8c,4,"温感器！");
	  delay_pw();
	  temprun=0;
	  return 0;
	}
	k=gotkey();
	if(k==2)	 // 进入设置菜单
	{
	  display_cnasc(0x8e,1," \x04");
	  delay(35000);
	  display_cnasc(0x8e,1," \x1e");
	  display_cnasc(0x9a,4,"\x11 (ok) \x10");
	  return 1;
	}
  }
}

/*警报温度设置*/
uchar temp_func_set(void)
{
  uchar save[2];
  temprun=0;
  oth_run=0;
  rds1302_data();
  while(1)
  {  
	if(oth_run)
	{
	  con_disp (0x00,0x00,0x88,0x90,2,16);
	  con_disp (0xff,0xff,0x8e,0x90,2,16);
	}
	else
	{
	  con_disp (0xff,0xff,0x88,0x90,2,16);
	  con_disp (0x00,0x00,0x8e,0x90,2,16);
	}	
	save[0]=gotkey();
	if(save[0]==1||save[0]==3) oth_run=!oth_run; 
	if(save[0]==5) return 1;
	if(save[0]==11) 
	{  
	  if(oth_run) return 0;
	  else  
	  { 
		clear_dis();
		dis_title_e();
		save[1]=save[0]=0;
		oth_run=0;
	    dis_title_e();
        display_cnasc(0x82,3,"设  定"); 
	    display_cnasc(0x90,5,"警报温度：");
		display_cnasc(0x88,7,"高于【    】℃"); 
		display_cnasc(0x98,8,"低于【    】℃ \x10"); 
		while(1)
		{
		  if(save[1]==2) con_disp(0x00,0x00,index_temp_set[save[1]].lnum,index_temp_set[save[1]].rnum,2,13);
		  else con_disp(0x00,0x00,index_temp_set[save[1]].lnum,index_temp_set[save[1]].rnum,2,13);
		  if(save[0]==2) con_disp(0x0f,0xff,index_temp_set[save[0]].lnum,index_temp_set[save[0]].rnum,1,13);
          else con_disp(0xff,0xff,index_temp_set[save[0]].lnum,index_temp_set[save[0]].rnum,2,13);
	      save[1]=save[0];
          adju_si=0;
		  oth_run=0;
		  while(1)
		  {
		    for(i=0;i<2;i++)
		    {
		      j=0x8b+i*16;
			  if(als_temp[i]==255) display_cnasc(j,2," off"); // 255值为关闭值
		      else 
			  {
			    save1=save[1];
			    if(als_temp[i]>127)	 // 当数值大于127时温度值为负数
			    { 
				  display_cnasc(j,1,"- ");
				  save[0]=char_num[(als_temp[i]-128)/10];
				  save[1]=char_num[(als_temp[i]-128)%10];
				  display_cnasc(j+1,1,save);
			    }
			    else
			    {
				  save[0]=' ';
				  save[1]=char_num[als_temp[i]/100];
				  if(save[1]=='0') save[1]=' ';
				  display_cnasc(j,1,save);
				  save[0]=char_num[als_temp[i]/10%10];
				  save[1]=char_num[als_temp[i]%10];
				  display_cnasc(j+1,1,save);
			    }
			    save[1]=save1;
			  }
		    }		
			save[0]=gotkey();
			do		   // 输入符号等待数值输入或下一位
			{
			  if(save[0]==13){ oth_run=1;display_cnasc(0x8b+save[1]*16,2,"-   ");}
			  else if(save[0]==12){ oth_run=0;display_cnasc(0x8b+save[1]*16,2,"    ");}
			  else break;
			  save[0]=gotkey();
			  if(save[0]>=0&&save[0]<10||save[0]==11) break;
			}while(1);
			if(save[0]==14){ als_temp[save[1]]=255;}  // 乘号直接将警报温度关闭
			if(save[0]>=0&&save[0]<10&&save[1]<2)
		    {
			  if(adju_si)
		      { 
			    adju_si=0;
			    if(oth_run) als_temp[save[1]]&=0x7f;  // 消去符号位
				if(als_temp[save[1]]>24&&!oth_run) als_temp[save[1]]=240;
        		else als_temp[save[1]]=als_temp[save[1]]*10+save[0];
				if(oth_run&&als_temp[save[1]]>50) als_temp[save[1]]=50; 		
				if(!oth_run&&als_temp[save[1]]>120) als_temp[save[1]]=120; 
				if(!oth_run&&als_temp[save[1]]<100) adju_si=1;	// 根据数值打开连写
				if(oth_run&&als_temp[save[1]]<10)  adju_si=1;
			  }
			  else
			  { 
			    als_temp[save[1]]=save[0];
				adju_si=1;
			  }
			  if(oth_run) als_temp[save[1]]=als_temp[save[1]]|0x80;	 // 写入符号
			  else als_temp[save[1]]=als_temp[save[1]]&0x7f;
			}			
			if(save[0]==11)
			{
			  if(als_temp[1]!=255&&als_temp[0]!=255)	   // 上下限温度调整
			  {
     			if(save[1]==0&&als_temp[0]<als_temp[1]){als_temp[0]=als_temp[1];}
			    if(save[1]==1&&als_temp[1]>als_temp[0]){als_temp[1]=als_temp[0];}
			  }
			  save[0]=save[1]+1;adju_si=0;break;}
	        if(save[0]==10&&save[1]!=0) {save[0]=save[1]-1;adju_si=0;break;}
		  }
		  oth_run=0;
		  if(save[0]==3&&save[1]==2) break;
		}
		save_y_or_n();
		if(oth_run)
		{ 
		  wds1302_data(d02_temp,als_temp[0]);
		  wds1302_data(d02_temp+2,als_temp[1]);
		}
		return 1;
	  }
	}
  }
}		   
			          
		 
/*温度计主函数*/		 
void temp_func(void)
{		 
  uchar t;	
  while(1)
  {
    oth_run=1;
	t=temp_func_dis();
    if(t==1)
	{
	  t=temp_func_set();
	  if(t==0) return;
	}
    else return;
  }
} 
/*=====================================================================================
 函数名称：闹铃功能
 功能描述：设定闹铃
 全局变量：alarm_time adju_si
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


void alarm_func(void)
{
  uchar save[2],a,d;
  clear_dis();
  dis_title_e();
  display_cnasc(0x82,3,"闹  钟");  
  display_cnasc(0x91,6,"状态：\x11    \x10");
  display_cnasc(0x89,3,"闹铃："); 
  display_cnasc(0x9f,1,"→");
  display_cnasc(0x90,1,"『");
  display_cnasc(0x8f,1,"』"); 
  adju_si=0; 
  a=0;d=0;
  save[0]=char_num[alarm_time[0]>>4];
  save[1]=char_num[alarm_time[0]&0x0f];
  display_cnasc(0x8c,1,save);
  display_cnasc(0x8d,1,": ");
  save[0]=char_num[alarm_time[1]>>4];
  save[1]=char_num[alarm_time[1]&0x0f];	
  display_cnasc(0x8e,1,save);
  do 
  {
    if(a==0)
	{    
	  display_cnasc(0x95,1,alm_sign[alm]);
      display_cnasc(0x94,1,"\x11 ");
      display_cnasc(0x96,1," \x10");
	  con_disp(0x00,0x00,index_alm[d-1].lnum,index_alm[d-1].rnum,1,14);
	}
    if(a>0&&a<4)
	{
	  if(d==0)
	  {
		display_cnasc(0x94,1,"  ");
        display_cnasc(0x96,1,"  ");
	  }
	  else 
	   con_disp(0x00,0x00,index_alm[d-1].lnum,index_alm[d-1].rnum,1,14);
	 con_disp(0xff,0xff,index_alm[a-1].lnum,index_alm[a-1].rnum,1,14);	
    }	 	  
	d=a;
	while(1)
	{
	  a=gotkey();
	  if((a==1||a==3)&&d==0)
	  { 
	    alm=!alm;
		if(a==1) display_cnasc(0x94,1,"\x09 ");
		else display_cnasc(0x96,1," \x09");
		delay(35000);
		if(a==1) display_cnasc(0x94,1,"\x11 ");
        else display_cnasc(0x96,1," \x10");
		display_cnasc(0x95,1,alm_sign[alm]);
	  }
	  if(d>0&&d<3&&a>=0&&a<10)
	  {
	    if(adju_si)
		{
		  alarm_time[d-1]<<=4;alarm_time[d-1]+=a;
		  if(d==1&&alarm_time[0]>0x23) alarm_time[0]=a;
		  else if(d==2&&alarm_time[1]>0x59) alarm_time[1]=a;
		  else adju_si=0;
		}
	    else  
		{
		  alarm_time[d-1]=a;adju_si=1;
		}
		save[0]=char_num[alarm_time[d-1]>>4];
	    save[1]=char_num[alarm_time[d-1]&0x0f];
	    display_cnasc(0x8c+(d-1)*2,1,save);
	  }
	  if(a==11){a=d+1;break;}
	  if(a==10&&d!=0){a=d-1;break;}
	}
	adju_si=0;
	if(a==4) 
	{ 
	  save_y_or_n();
	  wds1302_data(d02_signs,signs);
	  wds1302_data(d02_alm,alarm_time[0]);
	  wds1302_data(d02_alm+2,alarm_time[1]);
	  return;
	}
  }while(1);
}



/*=====================================================================================
 函数名称：篮球器功能
 功能描述：分数  时间  24秒  
 全局变量：resu  opera
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/

/*比赛设置*/
void nba_set(void)
{
  uchar nba,save1,key,save[2];
  clear_dis();
  dis_title_e(); 
  if(resu[7]==1) display_cnasc(0x82,3,"加  时");
  else  
  {
    display_cnasc(0x82,3,"设  定");
    opera[0][0]=0;
    opera[1][0]=1;
  }
  resu[6]=0;	// 赋初值
  resu[0]=1;
  resu[1]=4;
  resu[2]=12;
  resu[3]=0;
  resu[4]=24;
  display_cnasc(0x90,7,"队1:红  队2:白");
  display_cnasc(0x88,7,"24秒:Y  节数:4");
  display_cnasc(0x98,8,"每节时间: 12分钟");
  save1=0;nba=0;
  while(1)
  {
    if(save1==4) display_cnasc(0x9c,1,":");
	else display_cnasc(index_nba[save1],1,"  ");
	if(nba==4)  display_cnasc(0x9c,1,":\x10");
	else display_cnasc(index_nba[nba],1,"\x11 "); 
	save1=nba;
	key=gotkey();	
	if(key==1||key==3)
	{	  
	  switch(nba)     // 导航更改数据
	  {
	    case 0:			    
		case 1:
		        if(key==1){if(opera[nba][0]>0) opera[nba][0]--;
				           else opera[nba][0]=7;}
				if(key==3){if(opera[nba][0]<7) opera[nba][0]++;
				           else opera[nba][0]=0;}
				display_cnasc(index_nba[nba]-1,1,nba_oppo[opera[nba][0]]);
				break;
		case 2:
				if(resu[0]>0) resu[0]=0;
				else resu[0]=1;
				if(resu[0]>0) 	display_cnasc(0x8a,1,":Y");
				else display_cnasc(0x8a,1,":N");
				break;
		case 3:
				if(key==1){if(resu[1]>1) resu[1]--;
				            else  resu[1]=4;}
			    if(key==3){if(resu[1]<4) resu[1]++;
			               else resu[1]=1;}
				save[0]=':';
				save[1]=char_num[resu[1]];
		       	display_cnasc(0x8e,1,save);
				break;
		case 4:
				if(key==1){if(resu[2]>0) resu[2]--;
				            else  resu[2]=60;}
			    if(key==3){if(resu[2]<60) resu[2]++;
			               else resu[2]=0;}
				save[0]=char_num[resu[2]/10];
				save[1]=char_num[resu[2]%10];
		       	display_cnasc(0x9d,1,save);
				resu[5]=resu[2];
				break;
      }
	}
	if(key==11) 
	{
	  nba++; if(nba==5) return;			   
	}
	if(key==10&&nba!=0)  nba--;
  }
}



/*界面其他元素*/
void nba_oth_dis(void)
{
  uchar save[2];
  if(resu[7]==1) display_cnasc(0x80,5,"加时时间：");
  else display_cnasc(0x80,5,"比赛时间：");
  display_cnasc(0x98,1,nba_oppo[opera[0][0]]);
  display_cnasc(0x9e,1,nba_oppo[opera[1][0]]);
  display_cnasc(0x99,1,"队");
  display_cnasc(0x9f,1,"队");
  save[0]='0';
  save[1]=char_num[resu[6]+1];
  display_cnasc(0x9b,1,save);
  display_cnasc(0x9c,1,"st");
  con_disp(0xff,0xff,0x83,0x90,2,1); // 画正方框
  con_disp(0xff,0xff,0x8b,0x8f,2,1);
  con_disp(0x80,0x00,0x83,0x91,1,15);
  con_disp(0x00,0x01,0x84,0x91,1,15);
  con_disp(0x80,0x00,0x8b,0x80,1,15);
  con_disp(0x00,0x01,0x8c,0x80,1,15);
  nba_dis_time();
  if(resu[0]==0) display_cnasc(0x93,2,"\x07--\x07");
  display_cnasc(0x8b,2," S! ");
}



/*篮球器主函数*/
void nba_timer(void)
{
  uchar save,key,nba=0;
  als=di0=1;alm=keb=di1=oth_run=0;
  time_init();  
  display_cnasc(0x82,3,"篮球器");
  dis_title_e(); 
  removal_data(0,1); 
  while(1)	  // 使用说明
  {
	display_cnasc(0x90,8,timer_tips[nba]);
	display_cnasc(0x88,8,timer_tips[nba+1]);
	display_cnasc(0x98,8,timer_tips[nba+2]);
	key=gotkey();
	if(key==11)
	{
	  nba+=3;
	  if(nba>9) break;
	}
	if(key==10)
	{
	  if(nba>0) nba-=3 ;
	}
  }
nn:
  nba_set();
  clear_dis();
  nba_oth_dis();
  nba_tim=1;
  while(1)
  {	
	for(save=0x80,nba=0;nba<2;nba++)
	{
	  if(opera[nba][1]>99)	// 分数显示
	  {
	    num2_asc_dis(opera[nba][1]/100,save);
		num2_asc_dis(opera[nba][1]/10%10,save+1);
		num2_asc_dis(opera[nba][1]%10,save+2);
	  }
	  else
	  {
		num2_asc_dis(opera[nba][1]/10,save);
		num2_asc_dis(opera[nba][1]%10,save+1);
      }
	  if(opera[1][1]>99) save=0x85;
	  else save=0x86;
	}
	
	do
	{
	key=gotkey();
  ss:
	switch(key)
	{
	  case 12:
	  case 13:
	           if(key==12)
			     {if(opera[0][1]<250) opera[0][1]++;}
	           else 
			     {if(opera[0][1]>0) opera[0][1]--;}
			   save=110;
	 		   break;
	  case 14:
	  case 15:
	           if(key==14)
			     {if(opera[1][1]<250) opera[1][1]++;}
	           else 
			     {if(opera[1][1]>0) opera[1][1]--;}
			   save=110;
			   break;
	  case  7:
	           TR1=TR0=EA=0;
	           if((++resu[3])>59)
	           {
	             resu[3]=0;
		         if((++resu[2])>59)
				 { 
				   resu[3]=0;
				   if(++resu[2]>60) resu[2]=0;
				 }
			   }
		       if(resu[4]<24) resu[4]++;
			   nba_dis_time();
			   break;
	  case  8:
	           if(resu[0]==1)
			   { 
			     TL1=(time_count&0x00FF);
                 TH1=(time_count>>8);
			     resu[4]=24; nba_dis_time();
				 TR0=TR1=EA=1;
			   }
			   break;
	  case  9: 
	           if(resu[2]==0&&resu[3]==0)
	           {
	             resu[2]=resu[5];resu[4]=24;
				 if(resu[6]==resu[1])
				 {							  // 判断比分进行加时赛
				   if(opera[0][1]==opera[1][1]){nba_tim=0;resu[7]=1; goto nn;}
				   while(gotkey()!=10);		  // 否则只能退出
				   signs=r1302(d02_signs+1); 
				   nba_tim=0;
				   return;
				 }
				 else nba_oth_dis();
				 break;
	           } 
			   if(EA) {TR0=TR1=EA=0;break;}
	           if(!EA&resu[4]!=0){ EA=TR0=1;if(resu[0]==1) TR1=1;}
			   break;														 
	  case 10:
	  case 11:
	           if(!EA)			// 暂停状态下 ok键两次返回时钟界面
			   {                //  ./s 键两次退出
			     delay(6000);
				 save=gotkey();
				 if(save==key)
				 {
				   if(key==11)
				   {
				     clear_dis();oth_run=1;
				     rds1302_time();
					 init_d2=1;
				     dis_mode2();
					 init_d2=0;
				     while(gotkey()!=11);
				     oth_run=0;
					 save=110;
				     clear_dis();
				     nba_oth_dis();
				   }
				   else{ signs=r1302(d02_signs+1); nba_tim=0;return;}
			     }
				 else{ key=save;goto ss;}
			   }
			   break;	  
	}
	if(save==110)  break;
    }while(1);	
  }  
}

	 
/*=====================================================================================
 函数名称：设置功能
 功能描述：系统状态设置  
 全局变量：signs
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


/*设置调整内容 参数格式：选中设置项，设置项子值*/
uchar set_dis(uchar menu,uchar sub)
{
  uchar save;
  con_disp(0xff,0xff,0x8e,0x80,2,16);
  con_disp(0x00,0x00,0x8a,0x80,4,16);
  display_cnasc(0x96,1,"\x1e ");
  display_cnasc(0x9e,1,"\x1f ");
  display_cnasc(0x92,1,"  ");
  display_cnasc(0x9a,1,"  ");
  display_cnasc(0x8d,1,"\x11 ");
  display_cnasc(0x8a,1,"  ");
  while(1)
  {
    
    if(menu<2)
      display_cnasc(0x8e,2,set_bl_dm[menu][sub]);
    else
	  display_cnasc(0x8e,2,set_mka[menu-2][sub]);
	save=gotkey();
	if(save==5)
	{
	  key_dis(0x96);
	  if(sub==0&&menu<2) sub=2;
	  else if(sub==0&&menu>1) sub=1;
	  else sub--;
    }
	if(save==2)
	{
	  key_dis(0x9e);
	  if(sub==2&&menu<2) sub=0;
	  else if(sub==1&&menu>1) sub=0;
	  else sub++;
	}
	if(save==1||save==11)
	{
	  display_cnasc(0x8d,1,"\x04 ");
        delay(30000);
	  con_disp(0x00,0x00,0x8e,0x80,2,16);
      con_disp(0xff,0xff,0x8a,0x80,4,16);
      display_cnasc(0x92,1,"\x1e ");
      display_cnasc(0x9a,1,"\x1f ");
      display_cnasc(0x96,1,"  ");
      display_cnasc(0x9e,1,"  ");
	  display_cnasc(0x8a,1,"【");
	  display_cnasc(0x8d,3,"】\x10 \x04 ");
	  return(sub);	  // 返回设置值
	}
  }
}


/*设置调整*/
void set_func(void)
{
  uchar d,key,save;
  display_cnasc(0x81,2,"设置");
  dis_title();
  display_cnasc(0x88,8,selected[1]);
  con_disp(0xff,0xff,0x8a,0x80,4,16);
  save=d=0;
  do
  {	  
	display_cnasc(0x93,2,set_menu[index_s[save].lnum]);
	display_cnasc(0x8b,2,set_menu[save]);
	display_cnasc(0x9b,2,set_menu[index_s[save].rnum]); 
	d=save;
	while(1)
	{
	  key=gotkey();
	  if(key==11||key==3){ enter_dis();break;}
	  if(key==5){save=index_s[d].lnum;key_dis(0x92); break;}
	  if(key==2){save=index_s[d].rnum;key_dis(0x9a); break;}
	  if(key==1){ wds1302_data(d02_signs,signs); return;}
    }
	if(key==11||key==3)
	{
      save=0;
      for(key=0;key<2;key++)  //  进行两次比较 首次为现时值 其次为设定置更改 
      {
        switch(d)
        {
          case 0: 
		          if(key==1)
		          {
				    bl0=bl1=0;
					if(save==1) bl0=1;
					if(save==2)	bl1=1;
		          }
	              else 
				  {if(bl0) save=1; if(bl1) save=2;}
				  if(bl1) lcd_bl=close;
				  else lcd_bl=open;
			      break;
	      case 1: 
		          if(key==1)
		          {
				    di0=di1=0;
					if(save==1) di0=1;
					if(save==2)	di1=1;
		          }
			      else {if(di0) save=1; if(di1) save=2;}
			      break;
	      case 2: 
		          if(key==1) meu=(bit)save;
	              else {if(meu) save++;}	      
			      break;
          case 3: 
		          if(key==1) keb=(bit)save;
	              else{if(keb) save++;}	      
			      break;
          case 4: 
		          if(key==1) als=(bit)save;
	              else{if(als) save++;}	      
			      break;
          case 5: wds1302_data(d02_signs,signs);
	              return;
        }
        if(key==0) save=set_dis(d,save);
      }
	  save=d;
	}
  }while(1);
}


/*=====================================================================================
 函数名称：保密功能
 功能描述：系统重设  密码锁  密码更改  
 全局变量：resu  
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/


/*密码整理*/
void passwork_finish()
{
  uchar i;
  for(i=0;i<6;i+=2)
  {
    resu[i]<<=4;
	resu[i]+=resu[i+1];
  }
}



/*读取密码*/
uchar input_passwork()
{
  uchar i,save[2];
  save[0]=' ';
  display_cnasc(0x9d,3,"(./S)\x10");
  for(i=0;i<6;i++)
  {
    while(1)
	{
	  resu[i]=gotkey();
	  if(resu[i]>=0&&resu[i]<10) break;
	  if(resu[i]==10) return 10;
	}
    save[1]=char_num[resu[i]];
	display_cnasc(0x89+i,1,save); 
	delay(20000);
	display_cnasc(0x89+i,1," *");
  }
  return 11;
}



/*密码确认*/
void input_second_passwork()
{
  uchar i,save[2];
  save[0]=' ';
  display_cnasc(0x89,6,"            ");
  for(i=0;i<6;i++)
  {    
	if(resu[i]!=gotkey())	break;
	save[1]=char_num[resu[i]];
	display_cnasc(0x89+i,1,save); 
	delay(20000);
	display_cnasc(0x89+i,1," *");
  }
  if(i!=6)
  {
    display_cnasc(0x89,6,"密码不相同！");
	delay(50000);
	display_cnasc(0x9a,6,"任意键 \x10    ");
	while(gotkey()==0xf0);
	return;
  }
  else
  {
	passwork_finish();
	for(i=0;i<6;i+=2)
	  wds1302_data(d02_passw+i,resu[i]);
	for(i=0;i<6;i+=2)
	  if(resu[i]!=r1302(d02_passw+i+1)) break;
	if(i==6)
	 {display_cnasc(0x98,8,"\x10 更改成功!(OK) ");
	  while(gotkey()!=11);return;}
	else
	  display_cnasc(0x9a,6,"更改失败！  ");
  }
  delay_pw();
}				



/*密码比较*/
uchar passwork_comp()
{
  uchar i;
  passwork_finish();
  for(i=0;i<6;i+=2)
	if(resu[i]!=r1302(d02_passw+i+1)) break;
  if(i==6)	 return 1;
  else
  { 
    display_cnasc(0x89,6,"  密码错误！");
	delay_pw();
	display_cnasc(0x89,6,"            ");
	return 110;
  }
}



/*保密导航*/
void passw_func(void)
{
  uchar save,d,key;
  d=0;save=0;
  do
  {
    clear_dis();
	display_cnasc(0x81,2,"保密");
    dis_title();
    display_cnasc(0x88,8,selected[0]);
    con_disp(0xff,0xff,0x8a,0x80,5,16); 
    do
    {	  
	  display_cnasc(0x93,3,secrets_menu[index_p[save].lnum]);
	  display_cnasc(0x8b,3,secrets_menu[save]);
	  display_cnasc(0x9b,3,secrets_menu[index_p[save].rnum]);  
	  d=save;
	  while(1)
	  {
	    key=gotkey();
	    if(key==11||key==3){enter_dis();break;}
	    if(key==2){save=index_p[d].rnum; key_dis(0x9a);break;}
	    if(key==5){save=index_p[d].lnum; key_dis(0x92);break;}
		if(key==1){return;}
      }
      if(key==11||key==3)
	  {	
		if(d==3) return;		
		clear_dis();
		dis_title_e();
        switch(d)
        {
          case 0: 
		          display_cnasc(0x82,3,"密码锁");
				  display_cnasc(0x90,5,"输入密码：");
				  while(1)
				  {
				    key=input_passwork();
				    if(key==10) break;
				    key=passwork_comp();
				    if(key==1) 
		            {
				      display_cnasc(0x90,8,"处开启状态，按键");
		              display_cnasc(0x88,8,"□1-常开□2-关闭");
				      display_cnasc(0x98,8,"■自动延时30秒  ");
					  pw_oc=open;
				      save1=i=0;   
				      lock_op=1;   // 开启标记等待延时
				      while(1)
				      {
					    key=gotkey(); // 返回255为延时时间到达
					    if(key==255){ lock_op=0;pw_oc=close;break;}
					    if(key==1) 
					    {
					      display_cnasc(0x88,1,"■");
					      display_cnasc(0x98,1,"□");
					      lock_op=0;
					    }
					    if(key==2)
					    {
					      display_cnasc(0x88,1,"□");
						  display_cnasc(0x98,1,"□");
						  display_cnasc(0x8c,1,"■");
						  delay(60000);
						  clear_dis();
					      dis_title_e();
		                  display_cnasc(0x82,3,"密码锁");
					      display_cnasc(0x90,5,"密码锁将在");
						  display_cnasc(0x8a,6," 5秒后关闭：");
					      for(save=0;save<5;save++)
					      { 
					        display_cnasc(0x99+save,1,"→"); 
						    delay_pw();
					      }
						  display_cnasc(0x9e,1,"√");
						  pw_oc=open;
						  delay(30000);
						  pw_oc=close;
					      lock_op=0;
					      break;
					    }
				      }
					  break;
				    }
				  } 
		 	      break;
	      case 1: 
		          display_cnasc(0x82,3,"更  改");
				  display_cnasc(0x90,6,"输入旧密码：");
				  while(1)
				  {
				    key=input_passwork();
				    if(key==10) break;
				    key=passwork_comp();
				    if(key==1)
				    {
				      display_cnasc(0x92,1,"新");
					  display_cnasc(0x89,6,"            ");
				      key=input_passwork();
					  if(key==10) break;
					  display_cnasc(0x90,6,"确认密码：  ");
					  input_second_passwork();
					  break;		
				    }
				  }
				  break;
	      case 2: 
		          display_cnasc(0x82,3,"初始化");
          		  display_cnasc(0x91,6,"是否初始化? ");
                  display_cnasc(0x89,6,"Y(OK)\\N(./S)");
				  while(1)
				  {
				    key=gotkey();
					if(key==11)
					{
					  clear_dis();
					  dis_title_e();
					  display_cnasc(0x82,3,"初始化");
					  display_cnasc(0x90,5,"输入密码：");
					  do
					  {
					    key=input_passwork();
					    if(key==10) break;
					    key=passwork_comp();
					    if(key==1)
					    {
						  display_cnasc(0x90,5,"密码正确！");
						  display_cnasc(0x89,7,"\x10 初始化中... ");
						  init_ds1302();
						  rds1302_data();
						  delay_pw();
						  display_cnasc(0x9a,6,"\x10 完成！(OK)");
						  while(gotkey()!=11);
						  key=10;
						  break;
					    }
					  }while(1);
					}
					if(key==10) break;
				  }
				  break;
		}
		key=99;save=d;
	  }
	  if(key==99) break;
	}while(1);
  }while(1);
}


/*=====================================================================================
 函数名称：版本信息
 功能描述：  
 全局变量：
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/

void version_func(void)
{
  uchar save,v=0;
  display_cnasc(0x82,3,"版  本");
  dis_title_e();
  do
  {
	display_cnasc(0x90,7,version[v]);
	display_cnasc(0x88,7,version[v+1]);
	display_cnasc(0x98,7,version[v+2]);
	if(v!=0) display_cnasc(0x97,1," \x1e");
	else if(v==0)
	  display_cnasc(0x97,1,"院"); 
	else display_cnasc(0x97,1,"  ");
	if(v!=7) display_cnasc(0x9f,1," \x1f");
	else display_cnasc(0x9f,1,"  ");
	save=gotkey();
	if(save==2&&v<7)
	{
	  display_cnasc(0x9f,1," \x04");
      delay(30000);
	  v++;
	}
	if(save==5&&v>0)
	{
	  display_cnasc(0x97,1," \x04");
      delay(30000);
	  v--;
	}
	if(save==1) return;
	if((save==11||save==3)&&v==7) 
	{
	  display_cnasc(0x9e,2,") \x09 ");
      delay(30000);
	  return;	  
	}
  }while(1);
}



/*=====================================================================================
 函数名称：系统主函数
 功能描述：  
 全局变量：
 参数说明：见函数
 返回说明：无
 设 计 人：LIJH
 版    本：1.0
 说    明：
 ======================================================================================*/
main()
{
  uchar key; 							 
  init_lcd();						  // 初始化lcd
  lcd_bl=als_tl=als_th=buzzer=pw_oc=close;
  key=r1302(d02_initsi+1);            // 读出1302初始化标记位
  if(bl1) lcd_bl=close;
  else lcd_bl=open;
  display_cnasc(0x92,4,"欢迎使用");   // 显示开机画面“欢迎使用”
  display_cnasc(0x9d,3,"—LiJH");
  if(key!=0xb4)  init_ds1302();       // DS1302初始化
  rds1302_data();					  // 开机读回设置数据
  rds1302_date();
  rds1302_time();                     // 读日期时间   
  if(!ds18b20) init_ds18b20();		  // 初始化18b20
  if(ds18b20) 
  {  			                     
	ds18b20_func();					   // 读出温度
  } 
  time_init();                         // 定时器初始化
  delay_pw();resu[11]=0; 
  while(1)
  {  
	EA=1;
	TR0=1;
	dis_sig=1;	
	clear_dis();
	if(di0){init_d2=1;dis_mode2();init_d2=0;}
    else if(di1) dis_mode3();
    else dis_mode1();	
	do
	{
	  key=gotkey();
	  if(key==255)			// 键盘密码输入
	  {
		TR0=0;
		dis_sig=0;
		clear_dis();
		dis_title_e();
		display_cnasc(0x82,3,"键盘锁");
		display_cnasc(0x90,5,"输入密码：");
		while(1)
		{
		  keb=0;
		  key=input_passwork();
		  if(key==10) {keb=1;resu[11]=2;break;}
		  key=passwork_comp();
		  if(key==1) {keb=1;resu[11]=0;break;} 	  
		}
		rds1302_date();
        rds1302_time();
	    save_time=now;
		break;
	  }
	  if(key==11)
	  {
	    dis_menu_key(1);
		delay(35000);
		do
		{
		  dis_sig=0;
		  TR0=0;
		  clear_dis();
		  key=dis_menu();
		  clear_dis();
		  if(key==0) adjust_func();
		  else if(key==1) // 调用相应程序功能S
		  {
		    while(1)
			{
			  clear_dis();
		      key=tool_func();
			  if(key==4) break;
			  clear_dis(); 
			  switch(key)
			  {
			    case 0:	calculator();
				        break;
				case 1: temp_func();
				    	break;
				case 2: alarm_func();
				        break;
				case 3: nba_timer();
				        break;
			  }
			}
		  }
		  else if(key==2)	set_func();
		  else if(key==3)	passw_func();
		  else if(key==4) version_func();
		  else
		  {
		    rds1302_date();
            rds1302_time();
			save_time=now;
			resu[11]=0;
			dis_sig=1;
			break;
		  }
		}while(1);
	  }
	  if(dis_sig) break;
	}while(1);
  }
}
