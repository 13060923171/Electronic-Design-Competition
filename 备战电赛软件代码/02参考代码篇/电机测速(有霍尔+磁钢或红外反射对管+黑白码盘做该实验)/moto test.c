#include<reg52.h>
unsigned int i=0;
sbit P36=P3^6;
unsigned char j;
unsigned int kop;
unsigned char a[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char b[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
void delaym(unsigned int t)
{ unsigned char p;	      
while(t--)
{
for(p=0; p<125; p++);		
}
}

void main()
{
    EA = 1;
    TH0=0x00;
	TL0=0x00;
	ET0=1;
	ET1=1;		
  	TMOD = 0x15;// 打开定时器和计数器T0，因此矩形脉冲输入端接P3.4
	TH1=0x3C; //50ms
	TL1=0xB0;		
    TR0 = 1;
	TR1 = 1;
while(1)
{ 
  for(j=0;j<3;j++) //显示个数
  {
  P0=b[j];
  P2=j;
  delaym(2);
   }
  }
}
timer3() interrupt 3  //定时器中断时间到,就读取计数器值
{ TH1=0X3C;
 TL1=0XB0;
 i++;
if(i==20) //1s才进行技术脉冲值显示，即每秒更新一次速度
{
   kop=TH0;//计数器计入脉冲，每来一个矩形脉冲计数值加1
   kop=kop<<8;
   kop=kop+TL0;

b[2]=a[kop/100];       //显示计数器的值即实际测得速度
b[1]=a[kop%100/10];
b[0]=a[kop%100%10];
i=0;
TH0=0;//计数器速度获取后清零，进行下次获取
TL0=0;
}
}