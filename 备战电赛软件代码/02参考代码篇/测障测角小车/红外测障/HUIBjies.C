#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int 

//uchar code LED[4]={0xdc,0xe3,0x9c,0xce};
                       
uchar jianhao _at_ 0x40;
uchar code LCD[]={0x20,0x5e,0x76,0x54,0x42};
sbit  SH_CP=P2^2;	//595
sbit  ST_CP=P2^3;
sbit  DS=P2^4;
sbit  ACC7=ACC^7;

sbit  RS=P2^5;		//Lcd
sbit  RW=P2^6;
sbit  E =P2^7;
sbit p31=P3^1;
sbit p32=P3^2;                       
//中断接收
void inter0(void) interrupt 0
{ 
#pragma  asm
	     PUSH ACC
		 PUSH PSW
		 CLR   EX0
         JNB   P3.1, READ1
READOUT0:SETB  EX0
		 SETB  EA
		 POP PSW
		 POP ACC
         RETI
READ1:   CLR    A
         MOV    DPH, A
	     MOV    DPL, A
HARD1:  JB     P3.1,HARD11
        INC    DPTR
	    NOP
	    NOP
	    AJMP   HARD1
HARD11:  MOV    A,DPH       
	 	 JZ     READOUT0
		 CLR    A
READ11:  INC   A
READ12:  JNB   P3.1,READ12
         MOV   R1,#06H
READ13:  JNB   P3.1,READ11
         MOV   R5,#255
         DJNZ  R5,$
	     DJNZ   R1,READ13
	//	 DEC   A
		 MOV   40H,A
		LJMP READOUT0
#pragma  endasm
}
void Delay_1ms(uint j)
{ uchar i=0;
    for(;j>0;j--)
      for(i=0;i<125;i++);
}
void delay(uchar t, uchar i)
{	for(;t>0;t--)
		{ for(;i>0;i--); }				 	
}

void sent(unsigned char dat)
{	uchar i;
   	ACC=dat;
   	for(i=8;i>0;i--)
   		{ 	ST_CP=0;	
			DS=ACC7;
			SH_CP=0;
		    ACC=ACC<<1;
			SH_CP=1;
   		}  
   	delay(1,3);
    ST_CP=1;
}

void lcdwd(uchar da)
{	RS=1;
	RW=0;
	E=1;
	sent(da);
	delay(1,100);
	E=0;
}

void lcdwc(uchar c)
{	RS=0;
	RW=0;
	E=1;
	sent(c);
	delay(1,100);
	E=0;
}

void lcdinit()
{	delay(15,125);
	lcdwc(0x38);
	delay(5,125);
	lcdwc(0x38);
	delay(5,125);
	lcdwc(0x38);
	delay(5,125);
	lcdwc(0x08);
	lcdwc(0x01);
	lcdwc(0x06);
	lcdwc(0x0c);
}

void WriteString(uchar *s,uchar a,uchar b)//字符串由N个单字字符组成
{	for(;*s!=0;s++)
		{	lcdwc(0x80+(a++)+0x40*b);//地址
			lcdwd(*s);				 //数据
	 	}			
}

void display()
{uchar s1[4],*ps;
s1[0]='y';
s1[1]='k';
s1[2]=' ';
s1[3]=LCD[jianhao];
ps=&s1;
WriteString(ps,0,0);}
//主程序 
void main()
{ 
  IE=0;EX0=1;EA=1; lcdinit();//jianhao=0;
   for(;;)
   {
    display();
	}
}

