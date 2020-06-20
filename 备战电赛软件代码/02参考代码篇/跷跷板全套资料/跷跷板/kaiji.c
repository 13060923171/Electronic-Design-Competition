#include <reg51.h>


uchar code tab1[]={
"    欢迎使用    "
"                "
"循迹、避障、跷跷"
"板平衡，智能小车"
};
uchar code tab32[]={
/*--     调入了一幅图像：F:\梁\画图\HOCO12832.bmp     --*/
/*--     宽度x高度=128x32     --*/
"小组成员：      "
"         谭德超 "
"		  廖俊博 "
"		  朱沛远 "
};

uchar code tab5[]={
/*--     调入了一幅图像：F:\梁\画图\COCK.bmp     --*/
/*--     宽度x高度=128x64     --*/
"初始化完成，    "
"可以使用!       "
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
//我发现在读指令码的时候，程序先调用了6位的低电平，然再在1调用了10位的指令码 。总共调用了16位的数据。
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
/*------------------初始化-----------------*/
void init_lcd (void)
{
     wr_lcd (comm,0x30);     /*30---基本指令动作*/   
     wr_lcd (comm,0x01);     /*清屏，地址指针指向00H*/
     delay (100);
     wr_lcd (comm,0x06);     /*光标的移动方向*/
     wr_lcd (comm,0x0c);     /*开显示，关游标*/
}
//*******************************************
/*--------------显示点阵----------------*/
//经过调试发现显示点阵是非常有意思的，最主要的在于data1,data2上的两组数据的取值，
//不同的取值对应着不同的点阵效果。
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
           wr_lcd (comm,0x34); //扩充指令操作。
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
/*---------------显示汉字或字符----------------*/
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
//当data1=0xff,data2=0xff时,在x0,y0处反白显示16xl*yl.我发现如果固定yo的值为0x80，
//那么再去改变x0的值的话，x0的值是多少就在那一块反白，反白的范围应该是32*16。但前提是
//x1=2,y1=16.
//如果x1=1,y1=8. 那么反白的范围应该是16*8。
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
         wr_lcd (datk,data1);     //写数据到RAM.这类语句都是一个道理。
         wr_lcd (datk,data2);
       }
     }
     wr_lcd (comm,0x36);
}
//*****************************************************
/*--------------清DDRAM------------------*/
void clrram (void)
{
     wr_lcd (comm,0x30);
     wr_lcd (comm,0x01);     //清除显示指令。
     delay (180);
}

//**********************************************
/*------------------主程序--------------------*/
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
