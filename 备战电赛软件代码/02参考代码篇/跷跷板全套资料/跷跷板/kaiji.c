#include <reg51.h>


uchar code tab1[]={
"    ��ӭʹ��    "
"                "
"ѭ�������ϡ�����"
"��ƽ�⣬����С��"
};
uchar code tab32[]={
/*--     ������һ��ͼ��F:\��\��ͼ\HOCO12832.bmp     --*/
/*--     ���x�߶�=128x32     --*/
"С���Ա��      "
"         ̷�³� "
"		  �ο��� "
"		  ����Զ "
};

uchar code tab5[]={
/*--     ������һ��ͼ��F:\��\��ͼ\COCK.bmp     --*/
/*--     ���x�߶�=128x64     --*/
"��ʼ����ɣ�    "
"����ʹ��!       "
"                "
"                "
};

/*-----------------------------------*/
void delay (uint us)      //delay time
{
     while(us--);
}
//**************************************

//**************************************
void delay1 (uint ms)
{
     uint i,j;
     for(i=0;i<ms;i++)
     for(j=0;j<15;j++)
     delay(1);
}
//**************************************
/*---------------------------------------*/
//�ҷ����ڶ�ָ�����ʱ�򣬳����ȵ�����6λ�ĵ͵�ƽ��Ȼ����1������10λ��ָ���� ���ܹ�������16λ�����ݡ�
void wr_lcd (uchar dat_comm,uchar content)
{
     uchar a,i,j;
     delay (50);
     a=content;
     E_CLK=0;
     RW_SID=1;
     for(i=0;i<5;i++)
     {
       E_CLK=1;
       E_CLK=0;
     }
     RW_SID=0;
     E_CLK=1;
     E_CLK=0;
     if(dat_comm)
       RW_SID=1;      //data
     else
       RW_SID=0;      //command
     E_CLK=1;
     E_CLK=0;
     RW_SID=0;
     E_CLK=1;
     E_CLK=0;
     for(j=0;j<2;j++)
     {
       for(i=0;i<4;i++)
       {
         a=a<<1;
         RW_SID=CY;
         E_CLK=1;
         E_CLK=0;
       }
       RW_SID=0;
       for(i=0;i<4;i++)
       {
         E_CLK=1;
         E_CLK=0;
       }
     }
}
//******************************************
/*------------------��ʼ��-----------------*/
void init_lcd (void)
{
     wr_lcd (comm,0x30);     /*30---����ָ���*/   
     wr_lcd (comm,0x01);     /*��������ַָ��ָ��00H*/
     delay (100);
     wr_lcd (comm,0x06);     /*�����ƶ�����*/
     wr_lcd (comm,0x0c);     /*����ʾ�����α�*/
}
//*******************************************
/*--------------��ʾ����----------------*/
//�������Է�����ʾ�����Ƿǳ�����˼�ģ�����Ҫ������data1,data2�ϵ��������ݵ�ȡֵ��
//��ͬ��ȡֵ��Ӧ�Ų�ͬ�ĵ���Ч����
void lat_disp (uchar data1,uchar data2)
{
     uchar i,j,k,x;
     x=x1;
     for(k=0;k<2;k++)
     {
       for(j=0;j<16;j++)
       {
         for(i=0;i<8;i++)
         {
           wr_lcd (comm,0x34); //����ָ�������
           wr_lcd (comm,y+j*2);
           wr_lcd (comm,x+i);
           wr_lcd (comm,0x30);
           wr_lcd (datk,data1);
           wr_lcd (datk,data1);
         }
         for(i=0;i<8;i++)
         {
           wr_lcd (comm,0x34);
           wr_lcd (comm,y+j*2+1);
           wr_lcd (comm,x+i);
           wr_lcd (comm,0x30);
           wr_lcd (datk,data2);
           wr_lcd (datk,data2);
         }
       }
       x=x2;
     }
     wr_lcd (comm,0x36);
}
/*-----------------------------------------------*/
/*---------------��ʾ���ֻ��ַ�----------------*/
void chn_disp (uchar code *chn)
{
     uchar i,j;
     wr_lcd (comm,0x30);
     wr_lcd (comm,0x80);
     for (j=0;j<4;j++)
     {
       for (i=0;i<16;i++)
       wr_lcd (datk,chn[j*16+i]);
     }
}
//**********************************************
/*-----------------------------------------------*/
//��data1=0xff,data2=0xffʱ,��x0,y0��������ʾ16xl*yl.�ҷ�������̶�yo��ֵΪ0x80��
//��ô��ȥ�ı�x0��ֵ�Ļ���x0��ֵ�Ƕ��پ�����һ�鷴�ף����׵ķ�ΧӦ����32*16����ǰ����
//x1=2,y1=16.
//���x1=1,y1=8. ��ô���׵ķ�ΧӦ����16*8��
void con_disp (uchar data1,uchar data2,uchar x0,uchar y0,uchar xl,uchar yl)
{
     uchar i,j;
     for(j=0;j<yl;j++)
     {
       for(i=0;i<xl;i++)
       {
         wr_lcd (comm,0x34);
         wr_lcd (comm,y0+j);
         wr_lcd (comm,x0+i);
         wr_lcd (comm,0x30);
         wr_lcd (datk,data1);     //д���ݵ�RAM.������䶼��һ������
         wr_lcd (datk,data2);
       }
     }
     wr_lcd (comm,0x36);
}
//*****************************************************
/*--------------��DDRAM------------------*/
void clrram (void)
{
     wr_lcd (comm,0x30);
     wr_lcd (comm,0x01);     //�����ʾָ�
     delay (180);
}

//**********************************************
/*------------------������--------------------*/
void kaiji ()
{
     SP=0x5f;
     init_lcd ();
     while (1)
     {
       lat_disp (0x00,0x00);/**/
delay1(1000);
       chn_disp (tab1);
delay1(1000);
      con_disp (0xff,0xff,0x80,0x82,1,16);
       delay1 (4000);
       clrram();
       lat_disp (0x00,0x00);
      chn_disp (tab32);
      delay1 (4000);
      clrram();
      chn_disp (tab5);
      delay1 (8000);
     }
}
