#include<c8051f020.h>
#define uchar unsigned char
#define uint  unsigned int
#define SYSCLK 22118400
sbit IN1=P3^0;
sbit IN2=P3^1;	     
sbit IN3=P3^2;
sbit IN4=P3^3;
sbit S0=P2^0;
sbit S1=P2^1;
sbit S2=P2^2;
sbit S3=P2^3;
sbit S4=P2^4;	 	 
	
uchar code table[]={0xff,0xd0,0xb0,0x80,0x00};	//PWM输出控制数组
uchar code table1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};  //数码管输出数组

uchar i,r,l,temp,lukou=1,num;

void SYSCLK_init()	//时钟初始化
{
  	int i;                              
   	OSCXCN=0x67;                                                         
   	for(i=0;i<256;i++);           
   	while(!(OSCXCN&0x80)) ;         
   
		OSCICN=0x88;	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	 
}

void IO_init()		//IO口初始化
{
	XBR0=0x50;		  //交叉开关配置寄存器XBR0、XBR1、XBR2
	XBR1=0x00;
	XBR2=0x40;
	P0MDOUT=0xff;
	P3MDOUT=0xff;
	
}

void timer_init()
{
	CKCON=0x78;//时钟控制寄存器
	TMOD=0x11;//计数器定时器方式寄存器
	TH0=(65536-18432)/256;							//  tf0/tf1中断请求标志位  可用于关中断
	TL0=(65536-18432)%256;
	EA=1;
	ET0=1;
	ET1=1;
}

void PCA_int_init()		//PCA0初始化
{
	IE=0x85;
	IP=0x00;
	EIP1=0x08;
	EIE1=0x08;
	PCA0MD=0x09;
	PCA0CPM0=0x42;
	PCA0CPM1=0x42;
	//PCA0CPL0=0x00;		   //设置PCA0CP0脉冲宽度
	PCA0CPH0=table[4];
//	PCA0CPL1=0x00;		   //设置PCA0CP1脉冲宽度
	PCA0CPH1=table[4];
	CR=0;
	IT0=1;
	IT1=1;
}

void delay(uint z)		//延时
{
	uint x,y;
	for(y=z;y>0;y--)
		for(x=0;x<220;x++);
}  
void zhuanx(uchar zx)   //高速左转向（1）右转向（2）低速左转（3） 
{                       //右转（4）低速前行（5）高速前行（6）急停(7)倒退(8) 
      switch (zx)        //外加转弯灯，左转PB.0控制左边2个黄灯亮，右转PD.0控制的黄灯亮 
        {                  //急停灯（红色）有PD.1控制 
          case 1: l=4;
		  		  r=0;   //高速向左 
         	 break; 
          case 2: r=4;
		  		  l=0;   //高速右转 
   			 break; 
          case 3: l=2;
		          r=0;             //低速左转 
             break; 
          case 4: r=2;
		          l=0;             //低速右转 
             break; 
          case 5: l=r=2;              //低速前行
             break; 
          case 6: l=r=0;           //高速前行 
             break; 
		  case 7: l=r=4;		//刹车
		     break;
			 }
	}

void panduan() //将得到的AD转换值导入 
   {   

 
        //unsigned char lukou=0;  //lukou变量用于路口判断 
       //单个传感器检测到信号的情况 
        //2号传感器检测到信号，其余没有检测到信号 
       if((S0==0)&&(S1==0)&&(S2==1)&&(S3==0)&&(S4==0)) 
       { 
             zhuanx(6); //调用高速前行函数 
       } 
       // 1号传感器检测到信号，其余没有检测到信号 
       if((S0==0)&&(S1==1)&&(S2==0)&&(S3==0)&&(S4==0)) 
       { 
             zhuanx(3);//调用低速左转函数 
       } 
       // 0号传感器检测到信号，其余没有检测到 
       if((S0==1)&&(S1==0)&&(S2==0)&&(S3==0)&&(S4==0)) 
       { 
             zhuanx(1); //调用高速左转函数 
       } 
       //3号传感器检测到信号，其余没有检测到信号 
       if((S0==0)&&(S1==0)&&(S2==0)&&(S3==1)&&(S4==0)) 
       { 
            zhuanx(4);  //调用低速右转 
       } 
       //4 号传感器检测到信号，其余没有检测到信号 
       if((S0==0)&&(S1==0)&&(S2==0)&&(S3==0)&&(S4==1)) 
       { 
           zhuanx(2);  //调用高速右转 
       } 
       // 1,2号检测到信号 其余没有         白线在两个传感器中间 
       if((S0==0)&&(S1==1)&&(S2==1)&&(S3==0)&&(S4==0)) 
         { 
             zhuanx(3); // 调用低速左转函数 
         } 
       //0,1号检测到信号 
       if((S0==1)&&(S1==1)&&(S2==0)&&(S3==0)&&(S4==0)) 
         { 
             zhuanx(1);  //调用高速左转函数 
         } 
         //2,3号检测到信号 
       if((S0==0)&&(S1==0)&&(S2==1)&&(S3==1)&&(S4==0)) 
       { 
             zhuanx(4); //调用低速右转函数 
       } 
       //3,4检测到信号 
       if((S0==0)&&(S1==0)&&(S2==0)&&(S3==1)&&(S4==1)) 
         { 
             zhuanx(2);//调用高速右转函数 
         } 
       //0,1,2有信号              （ 折线检测 ） 
       if((S0==1)&&(S1==1)&&(S2==1)&&(S3==0)&&(S4==0)) 
         { 
            zhuanx(1);//左转折线  调用高速左转函数 
         } 
       //2,3,4有信号 
       if((S0==0)&&(S1==0)&&(S2==1)&&(S3==1)&&(S4==1)) 
         { 
             zhuanx(2); //右转折线  调用高速右转函数 
         } 
       //1,2,3 有信号 
       if((S0==0)&&(S1==1)&&(S2==1)&&(S3==1)&&(S4==0)) 
        { 
             zhuanx(6); //  调用高速直行函数 
        } 
	   //0,4 有信号 
	   if((S0==1)&&(S4==1)) 
        { 	   
             lukou++;
			 if(lukou==1)
			 {	
				 TR1=1;
				 
			  }	
			 if(lukou==2)
			 {	

				TR0=0;
				zhuanx(7);

				
				
			
								  //  调用刹车函数 
			  }
		 if(lukou==3)
			    lukou=0;	

        } 	  
		

}

void display()
{	uchar qian,bai,shi,ge;
	qian=temp/1000;
	bai=temp%1000/100;
	shi=temp%1000%100/10;
	ge=temp%10;
	P4=0x7f;
	P7=table1[ge];
	delay(2);
	P4=0xbf;
	P7=table1[shi];
	delay(2);
	P4=0xdf;
	P7=table1[bai];
	delay(2);	
	P4=0xef;
	P7=table1[qian];
	delay(2);		
	
}

void run_init()
{
	IN1=1;		   //向前行驶
	IN2=0;
	IN3=1;
	IN4=0;


}
void main()
{
             WDTCN=0xde;       //关闭看门狗 				 
	WDTCN=0xad;
	SYSCLK_init();
	IO_init();
	PCA_int_init();	
	run_init();
	timer_init();
	CR=1;
	
	while(1)
{
   
   panduan();
   display();
}    
}

void PCA_ISR (void) interrupt 9		//PCA中断服务程序
{
	CF=0;
//	PCA0CPL0=0x00;
	PCA0CPH0=table[r];
//	PCA0CPL1=0x00;
	PCA0CPH1=table[l]; 
	CCF0=1;
	CCF1=1;
}

void timer0() interrupt 1	   //定时器0中断
{	uchar tt;
	TH0=(65536-18432)/256;
	TL0=(65536-18432)%256;
	tt++;
	if(tt==115)
	{
		temp++;
		tt=0;
	}
}

void timer1()  interrupt 3	 //定时器1中断
{	
	
	TH1=(65536-18432)/256;
	TL1=(65536-18432)%256;
	TR0=1;
	
}  	

