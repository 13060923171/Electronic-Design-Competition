/*--------------------------------------------------------------*/
#include <AT89X52.H>
#include "Beep.H"
#include "IRDA.C"


/*--------------------------------------------------------------*/
//延时50000+0us 函数定义
void delay50ms(void)
{
    unsigned char i,j,k;
    for(i=165;i>0;i--)
    for(j=60;j>0;j--)
    for(k=1;k>0;k--);
}


/*--------------------------------------------------------------*/
void main(void)
{
	unsigned char temp_key;

	IRDA_init();					//遥控接收初始化

	while(1)
	{
		if(IRDA_ok) 					//有按键按下
		{
			Close_IRDA					//关闭红外,等待以下任务执行完毕	
			IRDA_ok = 0;				//清接收有效标志位
			temp_key = IRDA_get_key();	//取得键值
			speaker();
			if(temp_key == 4)		P1_0 = ~P1_0;	//任务0
			else if(temp_key == 5)	P1_1 = ~P1_1;
			else if(temp_key == 6)	P1_2 = ~P1_2;
			else if(temp_key == 7)	P1_3 = ~P1_3;
			else if(temp_key == 8)	P1_4 = ~P1_4;
			else if(temp_key == 9)	P1_5 = ~P1_5;
			else if(temp_key == 10)	P1_6 = ~P1_6;
			else if(temp_key == 11)	P1_7 = ~P1_7;	//任务7
		}
		delay50ms();
		Open_IRDA						//开启红外,接收遥控按键
	}
}

