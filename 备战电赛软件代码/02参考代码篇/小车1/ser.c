unsigned char serRcved;
bit serFlag = 0;
/*********************************
��������tran_init()(tranmit initialize)
����:���ڳ�ʼ��
     ͨѶ�йز�����ʼ��
���ߣ�������
���ڣ�06.02.25
�����Ӻ���:sel_bps
*********************************/
void ser_init()        /* ͨѶ�йز�����ʼ�� */
{
//    sel_bps(3); /* ѡ��ͨѶ���ʣ�0=1200,1=2400,2=4800,3=9600,4=19.2k */
    /* T1 use sio */
    TMOD=0x21;  /* T1=MODE2,sio; T0=MODE1,16bit,use time */
    PS=1;       /* SIO int high  ���ȼ� */
    EA=1;
    ET1=0;

   SM0=0;
   SM1=1;      /* SM0=0 SM1=1,mode1,10bit          */
   SM2=0;      /* data int,��У��(TB8=bit_dubleż) */
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
//���ڷ����ַ����ӳ���
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
//���ڷ����ַ����ӳ���(���л��з�)
//-----------------------------------------------------------
void puts(unsigned char *s)
{
   while (*s)
   {
      putchar(*s);
      s++;
   }
   putchar(0x0d);   //�س�����
   putchar(0x0a);
}
*/

/*
//-----------------------------------------------------------
//����ת���ɿɷ��͵��ַ���
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
//����ת���ɿɷ��͵��ַ���
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