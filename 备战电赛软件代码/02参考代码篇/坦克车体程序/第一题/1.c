#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit P10=P1^0; //  控制左电机前进
sbit P11=P1^1;//控制左电机后退
sbit P12=P1^2;//控制右电机
sbit P13=P1^3;//控制右电机
sbit P14=P1^4;//寻迹左
sbit P15=P1^5;//寻迹中
sbit P16=P1^6;//寻迹右
fun1()
{ P10=0;P11=1;P12=0;P13=1;}
//fun2()
//{ P10=1;P11=0;P12=1;P13=0;}
fun3()
{ P10=0;P11=1;P12=1;P13=1;}
fun4()
{ P10=1;P11=1;P12=1;P13=0;}
fun5()
{ P10=1;P11=1;P12=1;P13=1;}
void main()
{ while(1)
  { if((P14==0&&P15==1&&P16==0)||
(P14==0&&P15==0&&P16==0))
	  fun1();
    if(P14==1&&P15==0&&P16==0)
	  fun4();
	 if(P14==0&&P15==0&&P16==1)
	 fun3();
	if(P10==4&&P15==1&&P16==1)
	 fun5();
  }
}