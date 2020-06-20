#include<reg51.h> 
#define uchar unsigned char
sbit sclk=P1^6;//LCD显示。时钟输入
sbit std=P1^7;//LCD显示。数据输入
/*-----------------------------------*/

uchar code tab1[]={
"%%%%欢迎使用%%%%"
"循迹、避障、跷跷"
"                "
"板平衡智能小车。"
};
uchar code tab32[]={
/*--     调入了一幅图像：F:\梁\画图\HOCO12832.bmp     --*/
/*--     宽度x高度=128x32     --*/
"小组成员:       "
"          廖俊博"
"          谭德超"
"          朱沛远"
};

uchar code tab5[]={
/*--     调入了一幅图像：F:\梁\画图\COCK.bmp     --*/
/*--     宽度x高度=128x64     --*/
"初始化完成，    "
"可以使用!       "
"                "
"                "
};

//**************************************
/*---------------------------------------*/
//我发现在读指令码的时候，程序先调用了6位的低电平，然再在1调用了10位的指令码 。总共调用了16位的数据。
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
/*------------------初始化-----------------*/
void init_lcd (void)
{
     wr_lcd (0,0x30);     /*30---基本指令动作*/   
     wr_lcd (0,0x01);     /*清屏，地址指针指向00H*/
     delay1ms (100,0);
     wr_lcd (0,0x06);     /*光标的移动方向*/
     wr_lcd (0,0x0c);     /*开显示，关游标*/
}
//*******************************************
/*--------------显示点阵----------------*/
//经过调试发现显示点阵是非常有意思的，最主要的在于data1,data2上的两组数据的取值，
//不同的取值对应着不同的点阵效果。
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
           wr_lcd (0,0x34); //扩充指令操作。
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
/*---------------显示汉字或字符----------------*/
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
/*--------------清DDRAM------------------*/
void clrram (void)
{
     wr_lcd (0,0x30);
     wr_lcd (0,0x01);     //清除显示指令。
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
