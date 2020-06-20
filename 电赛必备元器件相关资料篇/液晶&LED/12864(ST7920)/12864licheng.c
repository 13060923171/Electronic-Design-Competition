
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int
/*TS12864A-3 �˿ڶ���*/
#define LCD_data  P0             //���ݿ�
sbit LCD_RS  =  P2^4;            //�Ĵ���ѡ������ 
sbit LCD_RW  =  P2^5;            //Һ����/д����
sbit LCD_E   =  P2^6;            //Һ������ʹ�ܿ���
sbit LCD_PSB =  P2^1;            //��/����ʽ����

 

uchar code dis1[] = {
"���ɫ���ո�Ƽ�" //дӢ���ַ��Ļ�һ��д16��
"���ǵ�δ��������"
"���������������"
"Ʒ��׿Խ�������"
};

 

#define delayNOP(); {_nop_();_nop_();_nop_();};

void delay(int);
bit lcd_busy();
void lcd_init();
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar cmd);
void lcd_pos(uchar X,uchar Y);  

/* ������  */

void main()
 { 
    uchar i;
 unsigned int j=0,k;

   P0=0x00;    //��ʼ�����ݿ�
    lcd_init(); //��ʼ��LCD             
 while(1)       
  {
    i = 0;
  
    delayNOP();
    delayNOP();
    delayNOP();
    delayNOP();
    delayNOP();
    lcd_pos(0,0); //��λ����һ�е�һ���ַ���ַ
        delay(15);//�ı����ʱ����Ըı䴹ֱ�������ʾ��
       //ʱ��̾��Ƕ�̬��ʾ������ͬʱ��ʾ,ʱ�䲻���ʾͻ���˸
    for(j=0;j<16;j++)      //��ʾ8�����֣�ÿ�����������ַ���
          //����Ӣ�ĵĻ���������һ������16���ַ�
    {                   
           lcd_wdat(dis1[i]);
           i++;
       for(k=25000;k>0;k--);//�ı�ʱ������������
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

 

/*дָ�����ݵ�LCD  RS=L��RW=L��E=���壬D0-D7=ָ���롣ֻ��LCD_RS��ֵ��ͬ
������������Ժ������д���ݵĺ���д��һ�������������������Լ���д*/

void lcd_wcmd(uchar cmd)
{                          

    LCD_RS = 0;//����ָ��ѡ���
    LCD_RW = 0;//д
    
 delayNOP();
    delayNOP();
 delayNOP();
   // P0 = cmd;
  
    LCD_E = 1;//�򿪲���
 delayNOP();
 _nop_(); 
  P0 = cmd;
    delayNOP();
 delayNOP();
    LCD_E = 0; //�رղ��� ���������ôд����ȫ����ʱ��ͼȥд�� 
   
}

/*д��ʾ���ݵ�LCD RS=H��RW=L��E=���壬D0-D7=���ݡ�*/

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

/*  LCD��ʾ���ֳ�ʼ���趨,ֻ�����ģ�����ʾͼƬ��ʼ���趨�������趨 */

void lcd_init()
{ 
 unsigned int i;
    LCD_PSB = 1;         //���ڷ�ʽ
    
    lcd_wcmd(0x34);      //����ָ����� 0011xRGL  R=1����ָ��G=0�رջ�ͼ��ʾL=0
 for(i=400;i>0;i--);
    lcd_wcmd(0x30);      //����ָ�����
   for(i=400;i>0;i--);
   lcd_wcmd(0x08);  //˯��ģʽ
   for(i=400;i>0;i--);
    lcd_wcmd(0x0C);      //��ʾ�����ع��00001DCB  D=1������ʾ,C=0�ع��,B=0���α�λ��
    for(i=80;i>0;i--);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(20);    //�ı�����ʱ�䳤��
/* lcd_wcmd(0x02);      
    delay(5);
 lcd_wcmd(0x80);      
    delay(5);*/
}

 

 

//�趨��ʾλ�� 

void lcd_pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
    {X=0x80;}   //����12864PDF�ĵ��趨
   else if (X==1)
    {X=0x90;}
   else if (X==2)
    {X=0x88;}
   else if (X==3)
   {X=0x98;}
   pos = X+Y ;  
    lcd_wcmd(pos); 
} 
