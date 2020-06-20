#include<c8051f020.h>
#define uchar unsigned char
#define uint  unsigned int
#define SYSCLK 22118400
sfr16 ADC0=0xbe; 	//ADC0����
sfr16 ADC0GT =0xc4;
sfr16 ADCOLT =0xc6;
sfr PH0=0xFA;	    //������ߵ��������EN1
sfr PH1=0xFB;	    //�����ұߵ��������EN2

sbit IN1=P1^0;	    //Ϊ0ʱ�����ת��Ϊ1ʱ�����ת
sbit IN2=P1^1;	    //Ϊ0ʱ�����ת��Ϊ1ʱ�����ת
	 
sbit S1=P2^3;	    //�ұ���ഫ����
sbit S2=P2^4;		//�ұ��ڲഫ����
sbit S3=P2^5;		//�м䴫����
sbit S4=P2^6;		//����ڲഫ����
sbit S5=P2^7;		//�����ഫ����

uchar code table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	//����ܱ�
uint num,temp,t,r,a,b,ge,shi,bai,qian,v;

void ADC0_init(void)
{
	 AMX0CF=0x00;        //ȫ���������� 
	 AMX0SL=0x00;        //��ʼ��ͨ��0 
	 ADC0CF=0x60;        //ת��ʱ�ӷ�Ƶϵ��Ϊ12������Ϊ1 
	 ADC0CN=0xc0;        //11000000b�͹���ģʽ��3��SAR clk�ĸ���ʱ�䣩������ģʽ��д1��AD0BUSY�������Ҷ���    
     REF0CN=0x03;        //00000011bADC0��ѹ��׼ȡ��VREF0����,ADC1��ѹ��׼ȡ��VREF1���ţ��ڲ���ѹ��׼VREF���ʹ�ܣ��¶ȴ������ر�    
                         //�¶ȴ������ر�   
	 EIE2|=0x02;		 //ADC0ת�������ж�  
	 AD0EN=1;			 //����ADC0��ϵͳ����
}


void SYSCLK_init()	   //ʱ�ӳ�ʼ��
{
  	int i;                              
   	OSCXCN=0x67;       //ʹ���ⲿ����ʱ��Ƶ��                                                    
   	for(i=0;i<256;i++);           
   	while(!(OSCXCN&0x80));          
   	OSCICN=0x88; 	   //ʹ��ʱ�Ӷ�ʧ�������ѡ���ⲿ������Ϊϵͳʱ��
}

void IO_init()	   	   //IO�ڳ�ʼ��
{
	XBR0=0x10;	       //PCA0 �ⲿ���������������˿�����,CEX0��CEX1 ���� 2 ���˿�����
	XBR1=0x00;	  
	XBR2=0x40;	       //���濪��ʹ��
	P0MDOUT=0xff;      //����Ϊ���췽ʽ
	P1MDOUT=0xff;
	P74OUT=0xff;	
}



void PCA_int_init()		     //PCA0��ʼ��
{
	PCA0MD=0x08;             //ʹ��ϵͳʱ�� ��ֹ CF�ж�  	                     
	PCA0CPM0=0xC2;		 	 //ѡ�� 16 λ PWM,������Ʒ�ʽʹ�� 
	PCA0CPM1=0xC2;		     //ѡ�� 16 λ PWM,������Ʒ�ʽʹ�� 
	PCA0CN=0x40;			 //���� PCA0 ����������ʱ�� 
	PCA0L=0x00;
    PCA0H=0x00;
	PCA0CPL0=0x00;		     //����PCA0CP0������
	PCA0CPH0=240;
	PCA0CPL1=0x00;		     //����PCA0CP1������
	PCA0CPH1=240;

}
 

void timer0_init()		    // ��ʱ��0��ʼ��
{
    IE=0x82;
    TMOD=0x01;				//��ʱ��0������ʽ1
    TH0=(65536-18432)/256;	//��ʱ10ms
    TL0=(65536-18432)%256;
    TR0=0;
}

void delay1ms(uint z)		//��ʱ1ms
{
	uint x,y,i;
	for(y=z;y>0;y--)
		{  
        for(x=0;x<10;x++)
            for(i=0;i<221;i++);
    }
	
}
/*********************************************************/
void waycontrol()          //�������
{    
    num=P2;
    num=num&0xf8;
    switch(num)
    {
        case 0x20:        //��������ֵ00100  01110   
        case 0x70:        
        {   IN1=0;
            IN2=0; 
            PH0=0xff;    //��·���
            PH1=0xff;    //��·���
            AD0INT=0;
            AD0BUSY=1;
	        a=ADC0H;
	        b=ADC0L;
            v=a*256+b;
			if(1.61<v<1.69)
			  {
			   if(1.65<v<1.67)
			   {														   
			     PH0=PH1=0x00;
		         P6=0xf7;          //��������  ����������� 
			     delay1ms(5000);
				 PH0=PH1=0x60;
			   }
			   else
			   {
			    PH0=0x60;
			    PH1=0x60;
			   }
			  }
			
			   else if(v>1.70)
				{
				 delay1ms(2000);
				 PH0=0X40;
				 PH1=0X40;
				}
			
				else
				{
				 PH0=0X60;
				 PH1=0X60;	  
				}
	    } break;

        case 0x30:        //00110 00111 01111
        case 0x38:
        case 0x78:

        {	IN1=0;
            IN2=0; 
            PH0=0xff;
            PH1=0x80;
        } break;

        case 0x18:        //00011 00001		   ��ת
        case 0x08:        
        {
		  PH0=0x00;
		  PH1=0x00;	
		  delay1ms(2);	  
		  IN1=0;
		  IN2=1;
          PH0=0xff;
           PH1=0x20;            
        } break;

        case 0x60:        //01100 11100 11110
        case 0xe0:
        case 0xf0:
        {   IN1=0;
            IN2=0; 
            PH0=0x80;
            PH1=0xff;
        } break; 

        case 0xc0:        //11000 10000
        case 0x80:        
        {
			PH0=0x00;
		    PH1=0x00;	
		    delay1ms(2);	  
		    IN1=1;
		    IN2=0;
            PH0=0x20;
            PH1=0xff;
        } break;

        case 0x00:        //00000
        {	IN1=0;
            IN2=0; 
            if(PH0<PH1)
            {
                PH0=0x40;
                PH1=0xff;
            }
            if(PH0>PH1)
            {
                PH0=0xff;
                PH1=0x40;
            }
        } break; 

	 case 0xf8:
	 {
                PH0=0x00;
                PH1=0x00;	  
	 }
	 break;
    }

}
/***************************************************************/
void display(uchar qian,uchar bai,uchar shi,uchar ge)    //�������ʾ����
{
    if(qian!=0)
    {
        P4=0xef;
        P7=table[qian];
        delay1ms(2);
    }
    if(qian!=0||bai!=0)
    {
        P4=0xdf;
        P7=table[bai];
        delay1ms(2);
    }
    if(qian!=0||bai!=0||shi!=0)
    {
        P4=0xbf;
        P7=table[shi];
        delay1ms(2);
    }
    P4=0x7f;
    P7=table[ge];
    delay1ms(2);    
}
/******************************************/
void timer0() interrupt 1
{
    TH0=(65536-18432)/256;
    TL0=(65536-18432)%256;
    t++;
    if(t==100)
    {
        t=0;
        ge++;
        if(ge==10)        //�����
        {
            ge=0;
            shi++;
        }
        if(shi==10)
        {
            shi=0;
            bai++;
        }
        if(bai==10)
        {
            bai=0;
            qian++;
        }
        if(qian==10)
            ge=shi=bai=qian=0;
    }
 	   
}  

void main()        //������
{
    WDTCN=0xde;      //�رտ��Ź� 
    WDTCN=0xad;
    SYSCLK_init();
    IO_init();
    ADC0_init();
    PCA_int_init();
    timer0_init();
    IN1=0;
    IN2=0;    
    TR0=1;
    while(1)
       {
	    display(qian,bai,shi,ge);
        waycontrol();
	   }
     
}	   
/*****************************************************************/





