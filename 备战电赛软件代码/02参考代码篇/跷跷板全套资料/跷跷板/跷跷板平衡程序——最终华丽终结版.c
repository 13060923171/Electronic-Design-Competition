#include<reg51.h>
#define uchar unsigned char
sbit p2_0=P2^0;
sbit p2_1=P2^1;
sbit p2_2=P2^2;
sbit p2_3=P2^3;
sbit p2_4=P2^4;
sbit p2_5=P2^5;
sbit p2_6=P2^6;
sbit p1_0=P1^0;
sbit p1_1=P1^1;
sbit p1_2=P1^2;
sbit p1_3=P1^3;
sbit p1_4=P1^4;
sbit p1_5=P1^5;
sbit p1_6=P1^6;
sbit p2_7=P2^7;
sbit p3_2=P3^2;
sbit p3_6=P3^6;
sbit p3_4=P3^4;


int a,b,l,setnumber=5,n,r,t,kk,i,m=0,tk,vt,st,yx,z,TC=1;
int phzt=0/*平衡状态*/;dd=0/*等待时间*/;
char sec1,sec2,sec3,sec4,sd1=48,sd2=48,sd3=48,sd4=48,lc1,lc2,lc3,lc4;
float s,c=0.9742647;
double v;
sbit     E_CLK     =P1^6;//clock     input         同步时钟输入端
sbit     RW_SID=P1^7;//data     input/output                入、输出端
     //串行发送一字节数据
void SendByte(unsigned char dat)
    {	unsigned char i;
        for(i=0;i<8;i++)
           {	E_CLK=0;
                if(dat&0x80)RW_SID=1;
				else RW_SID=0;
                E_CLK=1;
                dat=dat<<1;
            }
    }
void SendCMD(unsigned char dat)
    {	SendByte(0xF8);//11111,00,0     RW=0,RS=0     同步标志
        SendByte(dat&0xF0);//高四位
        SendByte((dat&0x0F)<<4);//低四位
    }
//写显示数据或单字节字符
void SendDat(unsigned char dat)
    {	SendByte(0xFA);//11111,01,0     RW=0,RS=1
        SendByte(dat&0xF0);//高四位
        SendByte((dat&0x0F)<<4);//低四位
    }
    /*               写汉字到LCD     指定的位置
              x_add显示RAM的地址
              dat1/dat2显示汉字编码
    */


    //初始化     LCM
void initlcm(void)
    {	SendCMD(0x30);//功能设置，一次送8位数据，基本指令集
        SendCMD(0x0C);//0000,1100       整体显示，游标off，游标位置off
        SendCMD(0x01);//0000,0001     清DDRAM
        SendCMD(0x02);//0000,0010     DDRAM地址归位
        SendCMD(0x80);//1000,0000     设定DDRAM     7位地址000，0000到地址计数器AC
    }
void set_wenzi(void)
{    SendCMD(0x80);//1000,0001     设定DDRAM     7位地址000，0001到地址计数器AC.
     SendDat(0xd0);    //将ASCII码调出来,显示在液晶屏幕上.下同
     SendDat(0xa1);//小
     SendDat(0xb3);
     SendDat(0xb5);//车
     SendDat(0xd7);
     SendDat(0xb4);//状
     SendDat(0xcc);
     SendDat(0xac);//态
     SendDat(0xa3);
     SendDat(0xba);
     SendCMD(0x90); //设置液晶屏的显示地址,下同.
     SendDat(0xca);
     SendDat(0xb1);//时
     SendDat(0xbc);
     SendDat(0xe4); //间
     SendDat(0x3a);
     SendDat(sec4);
     SendDat(sec3);
     SendDat(sec2);
     SendDat(0x2e);
     SendDat(sec1);
     SendDat(0x73);
     SendCMD(0x88);
     SendDat(0xcb);
     SendDat(0xd9); //速
     SendDat(0xb6);
     SendDat(0xc8); //度
     SendDat(0x3a); //：
     SendDat(sd4);
     SendDat(sd3);
     SendDat(sd2);
     SendDat(0x2e);
     SendDat(sd1);
	 SendDat(0x63);
	 SendDat(0x6d);
     SendDat(0x2f);
     SendDat(0x73);
     SendCMD(0x98);
     SendDat(0xc2);
     SendDat(0xb7); //路
     SendDat(0xb3);
     SendDat(0xcc); // 程
     SendDat(0x3a);
     SendDat(lc4);
     SendDat(lc3);
     SendDat(0x2e);
     SendDat(lc2);
     SendDat(lc1);
     SendDat(0x6d);
}
xianshi()
    {   set_wenzi();    //将想要设置的文字显示在液晶屏幕上。
     	return;
	}
void delay1ms(uchar t);	   //延时程序
void rightrun()
{	    b=1;

		p1_0=1;
		p1_1=0;
		p1_3=1;
		p1_4=1;
		a=2;
    	return;
}
 void leftrun()
{
	b=1;

	p1_0=0;
	p1_1=1;
	p1_3=1;
	p1_4=0;
	a=1;
	return;
}
void brightrun() //////////////
{	    b=1;
		p1_0=1;
		p1_1=0;
		p1_3=0;
		p1_4=1;
		a=2;
    	return;
}
 void bleftrun()//////////////
{	b=1;
	p1_0=0;
	p1_1=1;
	p1_3=1;
	p1_4=1;
	a=1;
	return;
}

void forwordrun()
{	b=0;

	p1_0=1;
	p1_1=0;
	p1_3=1;/**/
	p1_4=0;/**/
	a=3;
	return;
}
void backrun()
{	b=0;

	p1_0=0;
	p1_1=1;
	p1_3=0;/**/
	p1_4=1;/**/
	a=4;
	return;
}



timer0() interrupt 1 using 1
{	TH0=0xFc;
	TL0=0x18;

	r++;
	t++;
if(TC==1)
{	if(b==1)
	{
		if(m<setnumber)
		{	p1_2=0;
			p1_5=0;
		}
	else
		{	p1_2=1;
			p1_5=1;
		}
    	if (m>=10)
		m=0;
		m++;
	}
	else if(b==0)
	{		if(l<setnumber+1)
		{	p1_2=0;
			p1_5=0;
		}
		else
		{	p1_2=1;

		}
		if (l>setnumber+1)
		p1_5=1;

    	if (l>10)
	l=0;
    l++;
	}
	}
		if(t==100)
		{		if(phzt!=1&&phzt!=3)
					tk++;
				else 
				{	dd++;
					p3_4=!p3_4;
				}
				if(dd==50)
				{	if(phzt==1)
						phzt=2;
					if(phzt==3)
						phzt=4;	 //打破平衡,准备下跷跷板
					p3_4=1;
					tk=0;	//重新计时
					dd=0;
				}

                if (tk>=10000)
                	tk=0;
                t=tk;
                sec4=(t/1000)+0x30;
                t=t%1000;
                sec3=(t/100)+0x30;
                t=t%100;
                sec2=(t/10)+0x30;
                t=t%10;
                sec1=t+0x30;
                t=0;
		}
	if(r==1000)
		{		r=0;
				v=n*c;
				v=v*10;
                vt=v;
				if(vt>=10000)
				vt=9999;
                sd4=(vt/1000)+0x30;
                vt=vt%1000;
                sd3=(vt/100)+0x30;
                vt=vt%100;
                sd2=(vt/10)+0x30;
                vt=vt%10;
                sd1=vt+0x30;
 			    n=0;
		}

}


timer1() interrupt 3 using 2
 {                   n++;
				s=s+c;
				if(s>=10000)
				s=0;
                st=s;
                lc4=(st/1000)+0x30;
                st=st%1000;
                lc3=(st/100)+0x30;
                st=st%100;
                lc2=(st/10)+0x30;
                st=st%10;
                lc1=st+0x30;
}

main()
{	int cs=0/*跷跷板翻的次数*/,fdd=6/*防抖动系数*/;
    int fx=0/*前一次运动方向，标志*/,ph=0/*平衡的持久度*/;
    int fy=0/*反应时间*/,wt=0/*是否微调，标志*/,cf=1/*冲锋标志*/,cfsj=40,bc=0/*是否补偿,标志*/;
	long qj=10/*前进参数*/,ht=1/*后退参数*/,tz=0/*平衡状态参数*/,bf=400/*步进*/;
	yx=0;/*是否运行,标志位*/
	initlcm();
	sec1=48;
	sec2=48;
	sec3=48;
	sec4=48;
	lc1=48;
	lc2=48;
	lc3=48;
	lc4=48;
	TMOD=0x61;
	TH0=0xFC;
	TL0=0x18;
	TL1=-1;
	TH1=-1;
	IP=0x08;
	EA=0;ET0=1;ET1=1;
	TR0=1;TR1=1;
	yx=1;
	for(;;)
	{	xianshi();
		z=p3_2;
	    if(z==0)
		   {
		   		yx++;
				yx=(yx%2);
				EA=yx;
				TC=yx;
				p1_2=!yx;
				p1_5=!yx;
				delay1ms(1000);
		   }
		 if(p3_6==0)
		 {		delay1ms(1000);
		 			sec1=48;
					sec2=48;
					sec3=48;
					sec4=48;
					lc1=48;
					lc2=48;
					lc3=48;
					lc4=48;
					sd1=48;
					sd2=48;
					sd3=48;
					sd4=48;
					TH0=0xFC;
					TL0=0x18;
					TL1=-1;
					TH1=-1;
					yx=0;
					tk=0;
					s=0;
					EA=0;
					TC=1;
		 }
		   if(yx==1)
			{

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
 if((p2_5==0||phzt==2)&&phzt!=4)//后仰了
		{	fy++;//反应参数自加一
			qj++;//前进（后仰）参数；
			TC=1;//打开电机
			if(cf||phzt==2)
			{	//若在冲锋,反应参数到达20,自动满足前进条件
				qj=fdd*ht;
				bc=1;
               	if(cf&&tk>=35)
                   	setnumber=5;
				if(cf&&tk>=45)
					setnumber=4;
				if(phzt==2)
				if(p2_0&&p2_1&&p2_2)
					phzt=3;
                         }
			if(!cf&&fy>=200)//若没有冲锋,则反应参数到200后,自动对前进后退参数置一,并且打开微调
			{fy=0;
			qj=1;
			ht=1;
			wt=1;
			}

			if(qj>=fdd*ht)//防平衡器抖动；
			{	ph=0;//电机打开,平衡态系数置零
				fx=2;//方向为前


				for(i=0;i<=(bf)/*步进补偿*/;i++)
				{

//////////////////////////////////前进循迹程序段///////////////////////////
				if(p2_0==1)
					{	if(p2_0==1&&p2_1==1)
		 				{	rightrun();
						}
						else if(p2_0==1&&p2_1==0&&p2_2==0)
						{	rightrun();
						}
					}
					else if(p2_2==1)
					{	if(p2_2==1&&p2_1==1)
		 				{	leftrun();
						}
						else if(p2_0==0&&p2_1==0&&p2_2==1)
						{	leftrun();
						}
					}
    				else if(p2_1==1)
						forwordrun();
					else forwordrun();

/////////////////////////////////////////////////////////

					if((cs%2)&&(cs<=10))//跷跷板过平衡点次数
					{	cs++;
					}
					 qj=1;//前进参数置一
					if (cf||bc==2)
					{ 	ht=30;
						if(bc==2)
				 		bc=4;
					}//若在冲锋,后退参数置为30
					else if(tz<=100||(ht<2))//若没有冲锋,且没有进入平衡状态或后退参数<2时,就将其置一,否则除以2
						ht=1;
					else ht=6;

					}
			}
			TC=0;
	    	p1_2=1;
		    p1_5=1;//使电机停止
			}

/////////////////////////////////////////////////////////////////////////////
if(phzt==0)
	if(p2_4==0||(p2_3&&p2_5))//平衡了
		{   TC=1;	//准备电机运行
			if(cf)
				cfsj=cfsj+3;
			tz++;	//停止状态次数累加
			fy=0;	//可能进入平衡态,不需电机转动,反应参数清零

			if(cf==0&&tz>=100)//若无冲锋信号,平衡状态>100,跷跷板翻滚次数置到最大
			cs=8;             //直接进入微调
			if(cf==0&&tz>=5)//若无冲锋号,则可以进入平衡状态
			{/*	if(bc==1)
				if(fx==2)
				{	for(i=0;i<=((bf-100)/2);i++)
					forwordrun();
					for(i=0;i<=((bf-50)/2);i++)
					backrun();
					bc=0;
				}
				else if(fx==1)
				{	for(i=0;i<=((bf-50)/2);i++)
					backrun();
					for(i=0;i<=((bf-100)/2);i++)
					forwordrun();
					bc=0;
				}*/
				if(tz>=300)
				tz=300;  //停止态标志置到最大
				ph++;   //平衡态系数自加1
				if(ph>=15)
				{	
				qj=10;ht=10;
				}

				if(ph>=24&&setnumber>4)//若平衡态系数>=10,则前进、后退系数初值置为10,使其判断时间加长
				{tz=tz+10;
				ph=0;
				setnumber--;}
				if(ph>=500)//若平衡系数>=500,已经平衡.关闭中断,显示当前时间
				{phzt=1;
				p3_4=0;
				setnumber=4;
				}
				if(ph>=25&&fdd<=25)
			{	//防抖动参数,逐渐增加,加长判断时间
				fdd=fdd+1;
			}
			}//增大防抖动参数
			TC=0;//关闭电机
			fx=0;//方向为无
	    	p1_2=1;//关闭电机使能
		    p1_5=1;//关闭电机使能

		}




		 if((p2_3==0||phzt==4)&&phzt!=2)//前倾了

		{	if(cf)
				cfsj=cfsj+4;
				fy++;	//反应系数自加一
			if(phzt==4)
				ht=fdd*qj;	/****************还需要加程序********************/////////////////////////////////////////
			if(fy>=20&&cf)//若在冲锋,等待反应系数到100时,自动退出冲锋状态
			{	cf=0;
				fy=0;
				setnumber++;   //通过PWM调节前进的速度
			}
/*			else if(!cf&&bc==1)//若没有冲锋,当防抖动系数<=15时,直接使后退参数满足后退条件
			{	ht=fdd*qj;
				bc=2;
			}*/
			ht++;		//后退（前倾）参数自加一。
			TC=1;		//打开电电机
			if(fy>=200)//若反应参数到200则自动对前进后退参数,自动置一
			{fy=0;
			qj=1;
			ht=1;}
			if(fy>=333&&tz<=4)
			tz=0;
			if(ht>=fdd*qj&&(tz>=10)&&cf==0)//防平衡器抖动；
			{	ph=0;//电机启动,平衡态系数,自动归零
				fx=1;//方向为后
				for(i=0;i<=bf*(bc+1);i++)
				{
//////////////////////////////////////////////////////////////////////后退循迹程序段

/*						if(p2_0==1)
					{	if(p2_0==1&&p2_1==1)
		 				{	brightrun();
						}
						else if(p2_0==1&&p2_1==0&&p2_2==0)
						{	brightrun();
						}
					}
					else if(p2_2==1)
					{	if(p2_2==1&&p2_1==1)
		 				{	bleftrun();
						}
						else if(p2_0==0&&p2_1==0&&p2_2==1)
						{	bleftrun();
						}
					}
    				else if(p2_1==1)
			        	backrun();
					else */backrun();
/////////////////////////////////////////////////////////////////////////////

					if(wt==1||cs>=3)//若打开微调或当跷跷板翻滚次数达到3以上则调小步进
					{
						if(bf!=200)
 					bf=200;//再次调小步进

						if(!(cs%2)&&(cs<=10))//记录翻滚次数
						{	cs++;

						}
					}
				}
		 ht=1;//后退完毕,后退参数置一
			if(tz<=100||(qj<2))//没有进入过平衡态时或前进参数小于2时,前进参数置一,否则,除以2
			qj=1;
			else qj=6;
			if(bc>=1)
			{	bc=0;
				fdd=fdd+2;
				ht=100;
				qj=100;
				setnumber=6;
				bf=300;
			}
			}//重新设置前进后退参数
			TC=0;
	    	p1_2=1;
		    p1_5=1;//使电机停止
     	}


	}
	}
}
