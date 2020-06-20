
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



int a,b,l,setnumber,count,count1,hh,n,r,t,kk,i,m=0,tk,vt,st,yx,z;
char sec1,sec2,sec3,sec4,sd1=48,sd2=48,sd3=48,sd4=48,lc1,lc2,lc3,lc4;
void pwm(int number)
{       setnumber=number;		/*有效脉冲的个数等于设定的值*/
		TMOD=0x01;
        TH0=0xfc;
        TL0=0x18;
        EA=1;			
        ET0=1;
        TR0=1;					/*启动定时器0*/
		}
time0() interrupt 1 using 1
{   TR0=0;						/*关定时器0*/
    if(b==0)
	{if (count < setnumber+1)		/*如果计数变量小于给定值则给电机送有效电平*/
  	{	p1_2=0;
  	 	p1_5=0;
     }
    else						/*否则关电机驱动*/
	{   p1_2=1;
  	 
    }
	if(count>setnumber+1)
	p1_5=1;
	
		
   count++;
   if (count>=10)				/*设定一个周期的脉冲个数为10*/
   count=0;
   }
   
   if(b==1)
   {		{if (count1 < setnumber)		/*如果计数变量小于给定值则给电机送有效电平*/
  	{	p1_2=0;
  	 	p1_5=0;
     }
    else						/*否则关电机驱动*/
	{   p1_2=1;
  	 
    }
	if(count1>setnumber)
	p1_5=1;
	
		
   count1++;
   if (count1>=10)				/*设定一个周期的脉冲个数为10*/
   count1=0;
   }
   }
   TH0=0xfc;					/*重新设定计数初值，开定时器*/
   TL0=0x18;
   TR0=1;
}

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
	
	p1_0=1;
	p1_1=1;
	p1_3=1;
	p1_4=0;
	a=1;
	return;
}




void brightrun() //////////////
{	    b=1;
       
		p1_0=0;
		p1_1=1;
		p1_3=1;
		p1_4=1;
		a=6;
    	return;
}
 void bleftrun()//////////////
{	
	b=1;
	
	p1_0=1;
	p1_1=1;
	p1_3=0;
	p1_4=1;
	a=5;
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
void xunji()
{	if(p2_0==1)					/*如果右边小灯不亮，则进行右转*/
	{	if(p2_0==1&&p2_1==1)			/*如果中间小灯和右边小灯都不亮，则小转*/
			rightrun();					
		else if(p2_0==1&&p2_1==0&&p2_2==0)	/*如果只有右边小灯不亮，则大转*/
		{	rightrun();	
			rightrun();
		}
	}
	if(p2_2==1)					/*如果左边小灯不亮，则进行左转*/
	{	if(p2_2==1&&p2_1==1)                    /*如果中间小灯和左边小灯都不亮，则小转*/ 
		   	leftrun();                      					
		else if(p2_0==0&&p2_1==0&&p2_2==1)      /*如果只有左边小灯不亮，则大转*/       
		{	leftrun();
			leftrun();
		}	
	}	
        if(p2_1==1&&p2_0==0&&p2_2==0)			/*如果只有中间灯亮，则前进*/
	{//	cesu()  ;
		forwordrun();
	}		
	 	
	if((p2_0==0&&p2_1==0&&p2_2==0)||(p2_0==1&&p2_1==0&&p2_2==1)||(p2_0==1&&p2_1==1&&p2_2==1)) /*如果出现三个灯全亮或中间亮两边灭则执行上一次操作*/
		{
			switch(a)
			{case 1:leftrun();break;
		    	case 2:rightrun();break;
			default:backrun();
			}
		}
}


main()
{	pwm(3);
for(;;)
{
	xunji();
	if(p2_0==1&&p2_1==1&&p2_2==1)
	break;
	}
for(;;)
{	if(p2_6==1)
		{	backrun();
		}
	if(p2_1==1&&p2_6==0&&p2_0==0&p2_2==0)
	   bleftrun();
		if(p2_0==1)	
		{switch(a)
		{	case 5:for(hh=p2_6;hh==0;hh=p2_6)brightrun();break;
		    case 6:for(hh=p2_6;hh==0;hh=p2_6)bleftrun();break;
			case 4: bleftrun();break;
			default:backrun();
		}
		}
     if(p2_1==1)
		switch(a)
		{	case 5:for(hh=p2_6;hh==0;hh=p2_6)brightrun();break;
		    case 6:for(hh=p2_6;hh==0;hh=p2_6)bleftrun();break;
			case 4: bleftrun();break;
			default:backrun();
		}
     if(p2_0==0&&p2_1==0&&p2_2==0)
	{ switch(a)
		{   case 5://for(hh=p2_6;hh==0;hh=p2_6)
					{	bleftrun();
			//		    forwordrun();
					}break;
        	 case 6://for(hh=p2_6;hh==0;hh=p2_6)
			 		{	brightrun();
			//			forwordrun();
					}
						break;
			default:backrun();
	
		}
	}
   }
}
