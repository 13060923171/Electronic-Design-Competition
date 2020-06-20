unsigned char serRcved;
bit serFlag = 0;
/*********************************
函数名：tran_init()(tranmit initialize)
功能:串口初始化
     通讯有关参数初始化
作者：于振南
日期：06.02.25
包含子函数:sel_bps
*********************************/
void ser_init()        /* 通讯有关参数初始化 */
{
//    sel_bps(3); /* 选择通讯速率：0=1200,1=2400,2=4800,3=9600,4=19.2k */
    /* T1 use sio */
    TMOD=0x21;  /* T1=MODE2,sio; T0=MODE1,16bit,use time */
    PS=1;       /* SIO int high  优先级 */
    EA=1;
    ET1=0;

   SM0=0;
   SM1=1;      /* SM0=0 SM1=1,mode1,10bit          */
   SM2=0;      /* data int,无校验(TB8=bit_duble偶) */
	PCON = 0x00;
	TH1 = 0xfd;
	TL1 = 0xfd;
    TR1=1;
    REN=1;
    RI=0;
    TI=0;
    ES=1;

}

void putchar(unsigned char dat)
{
 SBUF=dat;
 while(!TI);
 TI=0;
}

//-----------------------------------------------------------
//串口发送字符串子程序
//-----------------------------------------------------------
void putstr(unsigned char *s)
{
   while (*s)
   {
      putchar(*s);
      s++;
   }
}

/*
//-----------------------------------------------------------
//串口发送字符串子程序(带有换行符)
//-----------------------------------------------------------
void puts(unsigned char *s)
{
   while (*s)
   {
      putchar(*s);
      s++;
   }
   putchar(0x0d);   //回车换行
   putchar(0x0a);
}
*/

/*
//-----------------------------------------------------------
//数组转换成可发送的字符串
//-----------------------------------------------------------
char *buffer2str(unsigned char *hhh)
{
   unsigned char i=0;

   while (i<40)
   {
      hhh[i]=hhh[i]+48;
      i++;
   }	
   return hhh;
}

//-----------------------------------------------------------
//数字转换成可发送的字符串
//-----------------------------------------------------------
char *long2str(unsigned long hh)
{
   char *ss="    ";

   ss[0]=48+hh/1000;
   ss[1]=48+(hh%1000)/100;
   ss[2]=48+(hh%100)/10;
   ss[3]=48+(hh%10);

   return ss;	
}


*/

void  sio_int() interrupt 4 using 2
{
      ES=0;
      if(RI)        /* RI==1 */
      {
	  	serFlag = 1;  
		serRcved=SBUF;
		RI=0;
      }
      ES=1;
}

//void main()
//{
//	tran_init();
//	putchar('a');
//	while(1);
//}