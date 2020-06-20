/*--------------------------------------------------------------*/
//文件：	IRDA.C
//遥控器：	3010系列遥控器,目前只支持SAA3010T和H-3010X两种型号
//编码：	RC-5协议,
//			双相位编码发送,
//			(0->1)为1,(1->0)为0
//			周期T = 1.688ms,
//解码：	定时采样
//单片机：	STC89C51RC
//晶振：	12MHz
//红外接收：1838
//日期：	2009-5-9
//时间：	16:15	


/*--------------------------------------------------------------*/
#include "IRDA.H"



/*--------------------------------------------------------------*/
//红外接口定义
#if T3010_Int_Select		//外部中断1	
	sbit IRDA = P3^3;		//外部中断INT1
#else						//外部中断0
	sbit IRDA = P3^2;		//外部中断INT0
#endif


/*--------------------------------------------------------------*/
//变量定义
#if   (T3010_Type_Select == 0)	//SAA3010T
	#define  D_ERROR   -1		//容错值
#elif (T3010_Type_Select == 1)	//H-3010X
	#define  D_ERROR   0		//容错值
#elif (T3010_Type_Select == 2)	//HL-3010							
	#define  D_ERROR   -1		//容错值	
#endif

#define  T_1	80			//1.688ms
#define  T_1_4	(T_1/4)		//422
unsigned char T20_C;		//20us计数次数
unsigned int  IRDA_dat; 	//13位数据-解码数值(1个起始位、1个控制位、5个系统位、6个指令位)
unsigned char IRDA_dat_bit; //13bits
unsigned char IRDA_cmd;		//指令码
unsigned char IRDA_sys;		//系统码
unsigned char IRDA_con;		//长按/双击
bit IRDA_ok;				//接收数据有效


/*--------------------------------------------------------------*/
//按键键码
#if	T3010_Type_Select			//选择H-3010X - CHUNGHOP - 34个按键						
								//选择HL-3010 - 慧丽 - 34个按键
unsigned char code key_code[] = {
 		          		0x0c,	//					 0,
	0x26,	0x23,	0x2d,		//		1,    2,     3,
0x01,	0x02,	0x03,	0x04,	//		4,	5,	6,	 7,
0x05,	0x06,	0x07,	0x08,	//		8,	9,	10,	11,
0x09,	0x00,	0x0a,	0x1e,	//		12,	13,	14,	15,
0x0e,	0x29,	0x2c,	0x2b,	//		16,	17,	18,	19,
	0x10,	0x0f,	0x20,		//		20,   21,   22,
	0x11,	0x0d,	0x21,		//		23,   24,   25,
0x12,	0x14,	0x1c,	0x38,	//		26,	27,	28,	29,
0x13,	0x15,	0x1d,	0x3f	//		30,	31,	32,	33,
};
#else							//选择SAA3010T - JINXING - 32个按键
unsigned char code key_code[] = {
0x0c,					0x0d,	//		0,			1,
0x01,	0x02,	0x03,	0x04,	//		2,	3,	4,	5,
0x05,	0x06,	0x07,	0x08,	//		6,	7,	8,	9,
0x09,	0x00,	0x0a,	0x1e,	//		10,	11,	12,	13
0x20,	0x21,	0x2b,	0x2c,	//		14,	15,	16,	17
	0x10,	0x29,	0x0f,		//		18,   19,   20,
	0x11,	0x0e,	0x26,		//		21,   22,   23,
0x1c,	0x14,	0x12,	0x38,	//		24,	25,	26,	27,
0x1d,	0x15,	0x13,	0x3f	//		28,	29,	30,	31,
};
#endif
#define key_code_num	sizeof(key_code) //取得键码大小


/*--------------------------------------------------------------*/
//宏定义---便于理解和阅读
#if T3010_Int_Select				//外部中断1	
	#define Close_IRDA	EX1 = 0;	//关闭中断,等待任务执行完毕
	#define Open_IRDA	EX1 = 1;	//开启中断,接收遥控按键
#else								//外部中断0
	#define Close_IRDA	EX0 = 0;	//关闭中断,等待任务执行完毕
	#define Open_IRDA	EX0 = 1;	//开启中断,接收遥控按键
#endif


/*--------------------------------------------------------------*/
//函数声明
#if T3010_Timer_Select				//定时器1
	void T1_init(void);				//定时器1初始化
#else								//定时器0
	void T0_init(void);				//定时器0初始化
#endif

#if T3010_Int_Select				//外部中断1	
	void INT1_init(void);			//外部中断1初始化
#else								//外部中断0
	void INT0_init(void);			//外部中断0初始化
#endif

void IRDA_init(void);				//遥控接收初始化



#if T3010_Timer_Select				//定时器1
/*--------------------------------------------------------------*/
//定时器1初始化
void T1_init(void)
{
    TMOD |= 0x20;	//自动重新装载
    TH1  = 0xeb;	//20us
    TL1  = 0xeb;	//20us
    IE  |= 0x88;	//EA = 1;	ET1 = 1;
	PT1  = 1;		//优先级
	TR1  = 0;		//关闭
}
/*--------------------------------------------------------------*/
//定时器1计数
void T1_intservice(void) interrupt 3
{
	T20_C++;		//20us
}

#else								//定时器0						
/*--------------------------------------------------------------*/
//定时器0初始化
void T0_init(void)
{
    TMOD |= 0x02;	//自动重新装载
    TH0  = 0xeb;	//20us
    TL0  = 0xeb;	//20us
    IE  |= 0x82;	//EA = 1;	ET0 = 1;
	PT0  = 1;		//优先级
	TR0  = 0;		//关闭
}
/*--------------------------------------------------------------*/
//定时器0计数
void T0_intservice(void) interrupt 1
{
	T20_C++;		//20us
}
#endif



#if T3010_Int_Select	//外部中断1	
/*--------------------------------------------------------------*/
//外部中断1初始化
void INT1_init(void)
{
    EX1 = 1;		//外部中断INT1
    IT1 = 0;		//低电平触发
}

#else				//外部中断0
/*--------------------------------------------------------------*/
//外部中断0初始化
void INT0_init(void)
{
    EX0 = 1;		//外部中断INT0
    IT0 = 0;		//低电平触发
}
#endif


/*--------------------------------------------------------------*/
//外部中断1遥控接收
//起始位：	1.5位		1,1
//控制字：	 1 位		x
//用户码：	 5 位		0,0,0,0,0
//指令码：	 6 位		x,x,x,x,x,x
//数据长度： 14位		1,1,x,0,0,0,0,0,x,x,x,x,x,x
#if T3010_Int_Select				//外部中断1	
	void INT1_intservice(void) interrupt 2
#else								//外部中断0
	void INT1_intservice(void) interrupt 0
#endif
{
#if T3010_Int_Select				//外部中断1	
	EX1 = 0;						//关闭外部中断
#else								//外部中断0
	EX0 = 0;						//关闭外部中断
#endif

#if T3010_Timer_Select				//定时器1
	TR1 = 1;						//启动定时器
#else								//定时器0
	TR0 = 1;						//启动定时器
#endif

	T20_C = 0;						//清计数器
	IRDA_dat = 0;					//初始化数据
	while(T20_C < T_1_4 + D_ERROR)	//第一位起始位为1(IRDA = 0)
	{
		if(IRDA)					//防止自然误触发!!!
		{
			#if T3010_Timer_Select	//定时器1
				TR1 = 0;			//关闭定时器
			#else					//定时器0
				TR0 = 0;			//关闭定时器
			#endif

			#if T3010_Int_Select	//外部中断1	
				EX1 = 1;			//启动外部中断
			#else					//外部中断0
				EX0 = 1;			//启动外部中断
			#endif
			return;					//退出中断服务程序
		}
	}
									//第一位起始位丢弃
	T20_C = 0;						//清计数器
	IRDA_dat_bit = 0;				//接收数据位清0
	while(IRDA_dat_bit < 13)		//接收13位数据
	{
		if(T20_C > T_1 + D_ERROR)	//1.688ms
		{	
			IRDA_dat <<= 1;			//接收到的数据为高到低
			IRDA_dat |= IRDA;		//接收一位
			T20_C = 0;				//清计数器
			IRDA_dat_bit++;			//接收数据位++
		}
	}

#if T3010_Timer_Select				//定时器1
	TR1 = 0;						//关闭定时器
#else								//定时器0
	TR0 = 0;						//关闭定时器
#endif

	IRDA_dat = ~IRDA_dat;			//接收数据取反还原
	IRDA_sys = (unsigned char)((IRDA_dat & 0x07c0)>>6); //取得系统码
	if((IRDA_dat & 0x1000) && (IRDA_sys == 0x00))		//接收数据有效
	{	//第二个起始位1				//系统码00000
		IRDA_cmd = (unsigned char)(IRDA_dat & 0x3f);	//取得指令码
		IRDA_con = (bit)(IRDA_dat & 0x0800);			//取得控制字
		IRDA_ok  = 1;									//置位接收有效标志位								
	}
	else				//纠错处理
	{
		IRDA_cmd = 0;	//全部清0
		IRDA_con = 0;
		IRDA_ok  = 0;
	}
	while(!IRDA);		//等待IRDA变为1,否则又进入中断服务了!!!

#if T3010_Int_Select	//外部中断1	
	EX1 = 1;			//打开外部中断
#else					//外部中断0
	EX0 = 1;			//打开外部中断
#endif
}


/*--------------------------------------------------------------*/
//遥控接收初始化
void IRDA_init(void)
{
#if T3010_Timer_Select	//定时器1
	T1_init();			//定时器1初始化
#else					//定时器0
	T0_init();			//定时器0初始化
#endif

#if T3010_Int_Select	//外部中断1	
	INT1_init();		//外部中断1初始化
#else					//外部中断0
	INT0_init();		//外部中断0初始化
#endif	
}


/*--------------------------------------------------------------*/
//将指令码转化为按键键值(用户自定义)
unsigned char IRDA_get_key(void)
{
	unsigned char i;
	i = key_code_num;
	for(i = 0; i < key_code_num; i++)
	{						//查表-将指令码转化为按键键值(用户自定义)
		if(key_code[i] == IRDA_cmd) break;
	}
//	speaker();				//蜂鸣器指示
	return (i);
}
