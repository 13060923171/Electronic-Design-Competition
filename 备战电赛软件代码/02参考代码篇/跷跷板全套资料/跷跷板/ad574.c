#include<reg51.h>
sbit sts=P3^2;
sbit ale=P3^5;
sbit wr1=P3^6;
sbit rd1=P3^7;
sbit cs=P2^6;
sbit rc=P2^5;
sbit a0=P2^7;
int ad574()
{	int c;
	for(;;)
	if(sts==0)		//如果ad574没有启动则启动//
	{	wr1=0;		//选中AD674芯片，使能端有效//
        rd1=0;
        ale=1;		//ls373使能端有效//
		cs=0;		//ls373锁存cs，rc，a0信号，启动ad574//
		rc=0;		//启动信号//
		a0=0;		//8位转换//
		ale=0;		//关ls373，锁存信号//
	}
	else
	{	for(;sts==1;);	//等待ad574转换结束//
		wr1=1;		//关ad574片选//
		rd1=1;
		ale=1;		//先锁存ad574的读数据的相关信号//
		rc=1;		//读数据信号//
		a0=0;		//只读8位数据//
		cs=0;		//片选有效//
		ale=0;		//锁存//
		wr1=0;		//使ad574片选有效//
		rd1=0;
		c=P2;		//读取结果//
		break;
		
	}
return c;			//返回转换结果//

}