#include<c8051f020.h>
#define uchar unsigned char
#define uint  unsigned int
#define SYSCLK 22118400
sbit IN1=P3^0;
sbit IN2=P3^1;	     
sbit IN3=P3^2;
sbit IN4=P3^3;
sbit S0=P2^0;
sbit S1=P2^1;
sbit S2=P2^2;
sbit S3=P2^3;
sbit S4=P2^4;	 	 
	
uchar code table[]={0xff,0xd0,0xb0,0x80,0x00};	//PWM�����������
uchar code table1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};  //������������

uchar i,r,l,temp,lukou=1,num;

void SYSCLK_init()	//ʱ�ӳ�ʼ��
{
  	int i;                              
   	OSCXCN=0x67;                                                         
   	for(i=0;i<256;i++);           
   	while(!(OSCXCN&0x80)) ;         
   
		OSCICN=0x88;	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	 
}

void IO_init()		//IO�ڳ�ʼ��
{
	XBR0=0x50;		  //���濪�����üĴ���XBR0��XBR1��XBR2
	XBR1=0x00;
	XBR2=0x40;
	P0MDOUT=0xff;
	P3MDOUT=0xff;
	
}

void timer_init()
{
	CKCON=0x78;//ʱ�ӿ��ƼĴ���
	TMOD=0x11;//��������ʱ����ʽ�Ĵ���
	TH0=(65536-18432)/256;							//  tf0/tf1�ж������־λ  �����ڹ��ж�
	TL0=(65536-18432)%256;
	EA=1;
	ET0=1;
	ET1=1;
}

void PCA_int_init()		//PCA0��ʼ��
{
	IE=0x85;
	IP=0x00;
	EIP1=0x08;
	EIE1=0x08;
	PCA0MD=0x09;
	PCA0CPM0=0x42;
	PCA0CPM1=0x42;
	//PCA0CPL0=0x00;		   //����PCA0CP0������
	PCA0CPH0=table[4];
//	PCA0CPL1=0x00;		   //����PCA0CP1������
	PCA0CPH1=table[4];
	CR=0;
	IT0=1;
	IT1=1;
}

void delay(uint z)		//��ʱ
{
	uint x,y;
	for(y=z;y>0;y--)
		for(x=0;x<220;x++);
}  
void zhuanx(uchar zx)   //������ת��1����ת��2��������ת��3�� 
{                       //��ת��4������ǰ�У�5������ǰ�У�6����ͣ(7)����(8) 
      switch (zx)        //���ת��ƣ���תPB.0�������2���Ƶ�������תPD.0���ƵĻƵ��� 
        {                  //��ͣ�ƣ���ɫ����PD.1���� 
          case 1: l=4;
		  		  r=0;   //�������� 
         	 break; 
          case 2: r=4;
		  		  l=0;   //������ת 
   			 break; 
          case 3: l=2;
		          r=0;             //������ת 
             break; 
          case 4: r=2;
		          l=0;             //������ת 
             break; 
          case 5: l=r=2;              //����ǰ��
             break; 
          case 6: l=r=0;           //����ǰ�� 
             break; 
		  case 7: l=r=4;		//ɲ��
		     break;
			 }
	}

void panduan() //���õ���ADת��ֵ���� 
   {   

 
        //unsigned char lukou=0;  //lukou��������·���ж� 
       //������������⵽�źŵ���� 
        //2�Ŵ�������⵽�źţ�����û�м�⵽�ź� 
       if((S0==0)&&(S1==0)&&(S2==1)&&(S3==0)&&(S4==0)) 
       { 
             zhuanx(6); //���ø���ǰ�к��� 
       } 
       // 1�Ŵ�������⵽�źţ�����û�м�⵽�ź� 
       if((S0==0)&&(S1==1)&&(S2==0)&&(S3==0)&&(S4==0)) 
       { 
             zhuanx(3);//���õ�����ת���� 
       } 
       // 0�Ŵ�������⵽�źţ�����û�м�⵽ 
       if((S0==1)&&(S1==0)&&(S2==0)&&(S3==0)&&(S4==0)) 
       { 
             zhuanx(1); //���ø�����ת���� 
       } 
       //3�Ŵ�������⵽�źţ�����û�м�⵽�ź� 
       if((S0==0)&&(S1==0)&&(S2==0)&&(S3==1)&&(S4==0)) 
       { 
            zhuanx(4);  //���õ�����ת 
       } 
       //4 �Ŵ�������⵽�źţ�����û�м�⵽�ź� 
       if((S0==0)&&(S1==0)&&(S2==0)&&(S3==0)&&(S4==1)) 
       { 
           zhuanx(2);  //���ø�����ת 
       } 
       // 1,2�ż�⵽�ź� ����û��         �����������������м� 
       if((S0==0)&&(S1==1)&&(S2==1)&&(S3==0)&&(S4==0)) 
         { 
             zhuanx(3); // ���õ�����ת���� 
         } 
       //0,1�ż�⵽�ź� 
       if((S0==1)&&(S1==1)&&(S2==0)&&(S3==0)&&(S4==0)) 
         { 
             zhuanx(1);  //���ø�����ת���� 
         } 
         //2,3�ż�⵽�ź� 
       if((S0==0)&&(S1==0)&&(S2==1)&&(S3==1)&&(S4==0)) 
       { 
             zhuanx(4); //���õ�����ת���� 
       } 
       //3,4��⵽�ź� 
       if((S0==0)&&(S1==0)&&(S2==0)&&(S3==1)&&(S4==1)) 
         { 
             zhuanx(2);//���ø�����ת���� 
         } 
       //0,1,2���ź�              �� ���߼�� �� 
       if((S0==1)&&(S1==1)&&(S2==1)&&(S3==0)&&(S4==0)) 
         { 
            zhuanx(1);//��ת����  ���ø�����ת���� 
         } 
       //2,3,4���ź� 
       if((S0==0)&&(S1==0)&&(S2==1)&&(S3==1)&&(S4==1)) 
         { 
             zhuanx(2); //��ת����  ���ø�����ת���� 
         } 
       //1,2,3 ���ź� 
       if((S0==0)&&(S1==1)&&(S2==1)&&(S3==1)&&(S4==0)) 
        { 
             zhuanx(6); //  ���ø���ֱ�к��� 
        } 
	   //0,4 ���ź� 
	   if((S0==1)&&(S4==1)) 
        { 	   
             lukou++;
			 if(lukou==1)
			 {	
				 TR1=1;
				 
			  }	
			 if(lukou==2)
			 {	

				TR0=0;
				zhuanx(7);

				
				
			
								  //  ����ɲ������ 
			  }
		 if(lukou==3)
			    lukou=0;	

        } 	  
		

}

void display()
{	uchar qian,bai,shi,ge;
	qian=temp/1000;
	bai=temp%1000/100;
	shi=temp%1000%100/10;
	ge=temp%10;
	P4=0x7f;
	P7=table1[ge];
	delay(2);
	P4=0xbf;
	P7=table1[shi];
	delay(2);
	P4=0xdf;
	P7=table1[bai];
	delay(2);	
	P4=0xef;
	P7=table1[qian];
	delay(2);		
	
}

void run_init()
{
	IN1=1;		   //��ǰ��ʻ
	IN2=0;
	IN3=1;
	IN4=0;


}
void main()
{
             WDTCN=0xde;       //�رտ��Ź� 				 
	WDTCN=0xad;
	SYSCLK_init();
	IO_init();
	PCA_int_init();	
	run_init();
	timer_init();
	CR=1;
	
	while(1)
{
   
   panduan();
   display();
}    
}

void PCA_ISR (void) interrupt 9		//PCA�жϷ������
{
	CF=0;
//	PCA0CPL0=0x00;
	PCA0CPH0=table[r];
//	PCA0CPL1=0x00;
	PCA0CPH1=table[l]; 
	CCF0=1;
	CCF1=1;
}

void timer0() interrupt 1	   //��ʱ��0�ж�
{	uchar tt;
	TH0=(65536-18432)/256;
	TL0=(65536-18432)%256;
	tt++;
	if(tt==115)
	{
		temp++;
		tt=0;
	}
}

void timer1()  interrupt 3	 //��ʱ��1�ж�
{	
	
	TH1=(65536-18432)/256;
	TL1=(65536-18432)%256;
	TR0=1;
	
}  	

