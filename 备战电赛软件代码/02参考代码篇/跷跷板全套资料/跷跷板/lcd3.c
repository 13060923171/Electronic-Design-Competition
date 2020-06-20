#include<reg51.h> 
#define uchar unsigned char
sbit sclk=P1^6;//LCD��ʾ��ʱ������
sbit std=P1^7;//LCD��ʾ����������
/*-----------------------------------*/

uchar code tab1[]={
"%%%%��ӭʹ��%%%%"
"ѭ�������ϡ�����"
"                "
"��ƽ������С����"
};
uchar code tab32[]={
/*--     ������һ��ͼ��F:\��\��ͼ\HOCO12832.bmp     --*/
/*--     ���x�߶�=128x32     --*/
"С���Ա:       "
"          �ο���"
"          ̷�³�"
"          ����Զ"
};

uchar code tab5[]={
/*--     ������һ��ͼ��F:\��\��ͼ\COCK.bmp     --*/
/*--     ���x�߶�=128x64     --*/
"��ʼ����ɣ�    "
"����ʹ��!       "
"                "
"                "
};

//**************************************
/*---------------------------------------*/
//�ҷ����ڶ�ָ�����ʱ�򣬳����ȵ�����6λ�ĵ͵�ƽ��Ȼ����1������10λ��ָ���� ���ܹ�������16λ�����ݡ�
void wr_lcd (uchar dat_comm,uchar content)
{
     uchar a,i,j;
     a=content;
     sclk=0;
     std=1;
     for(i=0;i<5;i++)
     {
       sclk=1;
       sclk=0;
     }
     std=0;
     sclk=1;
     sclk=0;
     if(dat_comm)
       std=1;      //data
     else
       std=0;      //command
     sclk=1;
     sclk=0;
     std=0;
     sclk=1;
     sclk=0;
     for(j=0;j<2;j++)
     {
       for(i=0;i<4;i++)
       {
         a=a<<1;
         std=CY;
         sclk=1;
         sclk=0;
       }
       std=0;
       for(i=0;i<4;i++)
       {
         sclk=1;
         sclk=0;
       }
     }
}
//******************************************
/*------------------��ʼ��-----------------*/
void init_lcd (void)
{
     wr_lcd (0,0x30);     /*30---����ָ���*/   
     wr_lcd (0,0x01);     /*��������ַָ��ָ��00H*/
     delay1ms (100,0);
     wr_lcd (0,0x06);     /*�����ƶ�����*/
     wr_lcd (0,0x0c);     /*����ʾ�����α�*/
}
//*******************************************
/*--------------��ʾ����----------------*/
//�������Է�����ʾ�����Ƿǳ�����˼�ģ�����Ҫ������data1,data2�ϵ��������ݵ�ȡֵ��
//��ͬ��ȡֵ��Ӧ�Ų�ͬ�ĵ���Ч����
void lat_disp (uchar data1,uchar data2)
{
     uchar i,j,k,x;
     x=0x80;
     for(k=0;k<2;k++)
     {
       for(j=0;j<16;j++)
       {
         for(i=0;i<8;i++)
         {
           wr_lcd (0,0x34); //����ָ�������
           wr_lcd (0,0x80+j*2);
           wr_lcd (0,x+i);
           wr_lcd (0,0x30);
           wr_lcd (1,data1);
           wr_lcd (1,data1);
         }
         for(i=0;i<8;i++)
         {
           wr_lcd (0,0x34);
           wr_lcd (0,0x80+j*2+1);
           wr_lcd (0,x+i);
           wr_lcd (0,0x30);
           wr_lcd (1,data2);
           wr_lcd (1,data2);
         }
       }
       x=0x88;
     }
     wr_lcd (0,0x36);
}
/*-----------------------------------------------*/
/*---------------��ʾ���ֻ��ַ�----------------*/
void chn_disp (uchar code *chn)
{
     uchar i,j;
     wr_lcd (0,0x30);
     wr_lcd (0,0x80);
     for (j=0;j<4;j++)
     {
       for (i=0;i<16;i++)
       wr_lcd (1,chn[j*16+i]);
     }
}

//*****************************************************
/*--------------��DDRAM------------------*/
void clrram (void)
{
     wr_lcd (0,0x30);
     wr_lcd (0,0x01);     //�����ʾָ�
     delay1ms (180,0);
}


kaiji()
{	
     init_lcd ();
      lat_disp (0x00,0x00);/**/
delay1ms(50,0);
       chn_disp (tab1);
delay1ms(50,0);
       delay1ms (200,1000);
       clrram();
       lat_disp (0x00,0x00);
      chn_disp (tab32);
      delay1ms(150,1000);
      clrram();
	  lat_disp (0x00,0x00);
      chn_disp (tab5);
      delay1ms (50,1000); 
	  clrram();
	  }
