#include<c8051f020.h>
#define uchar unsigned char
#define uint  unsigned int
#define SYSCLK 22118400
sfr16 ADC0=0xbe; 	//ADC0数据
sfr16 ADC0GT =0xc4;
sfr16 ADCOLT =0xc6;
sfr PH0=0xFA;	    //控制左边电机，即接EN1
sfr PH1=0xFB;	    //控制右边电机，即接EN2

sbit IN1=P1^0;	    //为0时电机正转、为1时电机反转
sbit IN2=P1^1;	    //为0时电机正转、为1时电机反转
	 
sbit S1=P2^3;	    //右边外侧传感器
sbit S2=P2^4;		//右边内侧传感器
sbit S3=P2^5;		//中间传感器
sbit S4=P2^6;		//左边内侧传感器
sbit S5=P2^7;		//左边外侧传感器

uchar code table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	//数码管表
uint num,temp,t,r,a,b,ge,shi,bai,qian,v;

void ADC0_init(void)
{
	 AMX0CF=0x00;        //全部单端输入 
	 AMX0SL=0x00;        //开始在通道0 
	 ADC0CF=0x60;        //转换时钟分频系数为12，增益为1 
	 ADC0CN=0xc0;        //11000000b低功耗模式（3个SAR clk的跟踪时间）；启动模式：写1到AD0BUSY；数据右对齐    
     REF0CN=0x03;        //00000011bADC0电压基准取自VREF0引脚,ADC1电压基准取自VREF1引脚，内部电压基准VREF输出使能，温度传感器关闭    
                         //温度传感器关闭   
	 EIE2|=0x02;		 //ADC0转换结束中断  
	 AD0EN=1;			 //允许ADC0子系统工作
}


void SYSCLK_init()	   //时钟初始化
{
  	int i;                              
   	OSCXCN=0x67;       //使用外部振荡器时钟频率                                                    
   	for(i=0;i<256;i++);           
   	while(!(OSCXCN&0x80));          
   	OSCICN=0x88; 	   //使能时钟丢失检测器，选择外部振荡器作为系统时钟
}

void IO_init()	   	   //IO口初始化
{
	XBR0=0x10;	       //PCA0 外部计数器输入连到端口引脚,CEX0、CEX1 连到 2 个端口引脚
	XBR1=0x00;	  
	XBR2=0x40;	       //交叉开关使能
	P0MDOUT=0xff;      //设置为推挽方式
	P1MDOUT=0xff;
	P74OUT=0xff;	
}



void PCA_int_init()		     //PCA0初始化
{
	PCA0MD=0x08;             //使用系统时钟 禁止 CF中断  	                     
	PCA0CPM0=0xC2;		 	 //选择 16 位 PWM,脉宽调制方式使能 
	PCA0CPM1=0xC2;		     //选择 16 位 PWM,脉宽调制方式使能 
	PCA0CN=0x40;			 //允许 PCA0 计数器／定时器 
	PCA0L=0x00;
    PCA0H=0x00;
	PCA0CPL0=0x00;		     //设置PCA0CP0脉冲宽度
	PCA0CPH0=240;
	PCA0CPL1=0x00;		     //设置PCA0CP1脉冲宽度
	PCA0CPH1=240;

}
 

void timer0_init()		    // 定时器0初始化
{
    IE=0x82;
    TMOD=0x01;				//定时器0工作方式1
    TH0=(65536-18432)/256;	//定时10ms
    TL0=(65536-18432)%256;
    TR0=0;
}

void delay1ms(uint z)		//延时1ms
{
	uint x,y,i;
	for(y=z;y>0;y--)
		{  
        for(x=0;x<10;x++)
            for(i=0;i<221;i++);
    }
	
}
/*********************************************************/
void waycontrol()          //方向控制
{    
    num=P2;
    num=num&0xf8;
    switch(num)
    {
        case 0x20:        //传感器的值00100  01110   
        case 0x70:        
        {   IN1=0;
            IN2=0; 
            PH0=0xff;    //左路电机
            PH1=0xff;    //右路电机
            AD0INT=0;
            AD0BUSY=1;
	        a=ADC0H;
	        b=ADC0L;
            v=a*256+b;
			if(1.61<v<1.69)
			  {
			   if(1.65<v<1.67)
			   {														   
			     PH0=PH1=0x00;
		         P6=0xf7;          //蜂鸣器叫  发光二极管亮 
			     delay1ms(5000);
				 PH0=PH1=0x60;
			   }
			   else
			   {
			    PH0=0x60;
			    PH1=0x60;
			   }
			  }
			
			   else if(v>1.70)
				{
				 delay1ms(2000);
				 PH0=0X40;
				 PH1=0X40;
				}
			
				else
				{
				 PH0=0X60;
				 PH1=0X60;	  
				}
	    } break;

        case 0x30:        //00110 00111 01111
        case 0x38:
        case 0x78:

        {	IN1=0;
            IN2=0; 
            PH0=0xff;
            PH1=0x80;
        } break;

        case 0x18:        //00011 00001		   右转
        case 0x08:        
        {
		  PH0=0x00;
		  PH1=0x00;	
		  delay1ms(2);	  
		  IN1=0;
		  IN2=1;
          PH0=0xff;
           PH1=0x20;            
        } break;

        case 0x60:        //01100 11100 11110
        case 0xe0:
        case 0xf0:
        {   IN1=0;
            IN2=0; 
            PH0=0x80;
            PH1=0xff;
        } break; 

        case 0xc0:        //11000 10000
        case 0x80:        
        {
			PH0=0x00;
		    PH1=0x00;	
		    delay1ms(2);	  
		    IN1=1;
		    IN2=0;
            PH0=0x20;
            PH1=0xff;
        } break;

        case 0x00:        //00000
        {	IN1=0;
            IN2=0; 
            if(PH0<PH1)
            {
                PH0=0x40;
                PH1=0xff;
            }
            if(PH0>PH1)
            {
                PH0=0xff;
                PH1=0x40;
            }
        } break; 

	 case 0xf8:
	 {
                PH0=0x00;
                PH1=0x00;	  
	 }
	 break;
    }

}
/***************************************************************/
void display(uchar qian,uchar bai,uchar shi,uchar ge)    //数码管显示函数
{
    if(qian!=0)
    {
        P4=0xef;
        P7=table[qian];
        delay1ms(2);
    }
    if(qian!=0||bai!=0)
    {
        P4=0xdf;
        P7=table[bai];
        delay1ms(2);
    }
    if(qian!=0||bai!=0||shi!=0)
    {
        P4=0xbf;
        P7=table[shi];
        delay1ms(2);
    }
    P4=0x7f;
    P7=table[ge];
    delay1ms(2);    
}
/******************************************/
void timer0() interrupt 1
{
    TH0=(65536-18432)/256;
    TL0=(65536-18432)%256;
    t++;
    if(t==100)
    {
        t=0;
        ge++;
        if(ge==10)        //秒计数
        {
            ge=0;
            shi++;
        }
        if(shi==10)
        {
            shi=0;
            bai++;
        }
        if(bai==10)
        {
            bai=0;
            qian++;
        }
        if(qian==10)
            ge=shi=bai=qian=0;
    }
 	   
}  

void main()        //主函数
{
    WDTCN=0xde;      //关闭看门狗 
    WDTCN=0xad;
    SYSCLK_init();
    IO_init();
    ADC0_init();
    PCA_int_init();
    timer0_init();
    IN1=0;
    IN2=0;    
    TR0=1;
    while(1)
       {
	    display(qian,bai,shi,ge);
        waycontrol();
	   }
     
}	   
/*****************************************************************/





