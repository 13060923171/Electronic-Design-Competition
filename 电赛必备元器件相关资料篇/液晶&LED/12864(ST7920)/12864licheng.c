
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int
/*TS12864A-3 端口定义*/
#define LCD_data  P0             //数据口
sbit LCD_RS  =  P2^4;            //寄存器选择输入 
sbit LCD_RW  =  P2^5;            //液晶读/写控制
sbit LCD_E   =  P2^6;            //液晶并行使能控制
sbit LCD_PSB =  P2^1;            //串/并方式控制

 

uchar code dis1[] = {
"你好色彩日格科技" //写英文字符的话一行写16个
"我们的未来不是梦"
"让世界更美好美丽"
"品质卓越至尊服务"
};

 

#define delayNOP(); {_nop_();_nop_();_nop_();};

void delay(int);
bit lcd_busy();
void lcd_init();
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar cmd);
void lcd_pos(uchar X,uchar Y);  

/* 主程序  */

void main()
 { 
    uchar i;
 unsigned int j=0,k;

   P0=0x00;    //初始化数据口
    lcd_init(); //初始化LCD             
 while(1)       
  {
    i = 0;
  
    delayNOP();
    delayNOP();
    delayNOP();
    delayNOP();
    delayNOP();
    lcd_pos(0,0); //定位到第一行第一个字符地址
        delay(15);//改变这个时间可以改变垂直方向的显示，
       //时间短就是动态显示，四行同时显示,时间不合适就会闪烁
    for(j=0;j<16;j++)      //显示8个汉字，每个汉字两个字符宽，
          //若是英文的话，数组中一行输入16个字符
    {                   
           lcd_wdat(dis1[i]);
           i++;
       for(k=25000;k>0;k--);//改变时间控制逐字输出
    }
   
    lcd_pos(1,0);
    delay(15);
    for(j=16;j<32;j++)
    {                   
           lcd_wdat(dis1[i]);
           i++;
    for(k=25000;k>0;k--);
    }
    
    lcd_pos(2,0); 
    delay(15);
    for(j=32;j<48;j++)
    {                   
           lcd_wdat(dis1[i]);
           i++;
    for(k=25000;k>0;k--);
    }
   
    lcd_pos(3,0); 
    delay(15);
    for(j=48;j<64;j++)
    {                   
           lcd_wdat(dis1[i]);
           i++;
    for(k=25000;k>0;k--);
    }

 }
}

void delay(int ms)
{
    while(ms--)
 {
      uchar i;
   for(i=0;i<250;i++)  //0.5ms
    {
     _nop_();      
  _nop_();
 // _nop_();
 // _nop_();
    }
 }
}  

 

/*写指令数据到LCD  RS=L，RW=L，E=脉冲，D0-D7=指令码。只有LCD_RS的值不同
故这个函数可以和下面的写数据的函数写到一起，用两个参数，读者自己改写*/

void lcd_wcmd(uchar cmd)
{                          

    LCD_RS = 0;//并行指令选择打开
    LCD_RW = 0;//写
    
 delayNOP();
    delayNOP();
 delayNOP();
   // P0 = cmd;
  
    LCD_E = 1;//打开并口
 delayNOP();
 _nop_(); 
  P0 = cmd;
    delayNOP();
 delayNOP();
    LCD_E = 0; //关闭并口 这个函数怎么写，完全参照时序图去写， 
   
}

/*写显示数据到LCD RS=H，RW=L，E=脉冲，D0-D7=数据。*/

void lcd_wdat(uchar dat)
{                          

    LCD_RS = 1;
    LCD_RW = 0;

 delayNOP();
    delayNOP();
 delayNOP();
    //P0 = dat;
 

 LCD_E = 1;
 delayNOP();
 _nop_(); 
 P0 = dat;
    delayNOP();
 delayNOP();
    LCD_E = 0; 
}

/*  LCD显示汉字初始化设定,只设必须的，若显示图片初始化设定得另外设定 */

void lcd_init()
{ 
 unsigned int i;
    LCD_PSB = 1;         //并口方式
    
    lcd_wcmd(0x34);      //扩充指令操作 0011xRGL  R=1扩充指令G=0关闭绘图显示L=0
 for(i=400;i>0;i--);
    lcd_wcmd(0x30);      //基本指令操作
   for(i=400;i>0;i--);
   lcd_wcmd(0x08);  //睡眠模式
   for(i=400;i>0;i--);
    lcd_wcmd(0x0C);      //显示开，关光标00001DCB  D=1整体显示,C=0关光标,B=0关游标位置
    for(i=80;i>0;i--);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(20);    //改变启动时间长短
/* lcd_wcmd(0x02);      
    delay(5);
 lcd_wcmd(0x80);      
    delay(5);*/
}

 

 

//设定显示位置 

void lcd_pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
    {X=0x80;}   //参照12864PDF文档设定
   else if (X==1)
    {X=0x90;}
   else if (X==2)
    {X=0x88;}
   else if (X==3)
   {X=0x98;}
   pos = X+Y ;  
    lcd_wcmd(pos); 
} 
