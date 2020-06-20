/*------------------------------------*/
//Note: Sounder_Driver
//File: Beep.H
//Date: 08-7-16
//Time: 21:46


/*------------------------------------*/
#ifndef __Beep_H__
#define __Beep_H__


/*------------------------------------*/
sbit Sounder=P3^6;		//Beep接口定义


/*------------------------------------*/
//函数声明
void delay_sound(void);
void speaker(void);


/*------------------------------------*/
void delay_sound(void)		//频率
{
	unsigned char j;
	for(j=55;j>0;j--);
}


/*------------------------------------*/
void speaker(void)	    //音长
{
	unsigned char i,j;
	for(i=2;i>0;i--)
		for(j=248;j>0;j--)
		{
			Sounder=~Sounder;
			delay_sound();			
		}
	Sounder = 1;	//关闭蜂鸣器
}


/*------------------------------------*/
#endif