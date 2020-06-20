#include <reg52.h>
#include <asc.h>
#include <12864.h>
#include <imag.h>

#define uchar unsigned char
#define uint  unsigned int

//�˵���ʵ��
/*-------------------------------------------------------------*/
#define MENU_SIZE 18             //�˵�����

unsigned char KeyFuncIndex=0;    //��ŵ�ǰ�Ĳ˵�����

void (*KeyFuncPtr)();            //���尴������ָ��
//�������� 
typedef struct 
{
   unsigned char KeyStateIndex;   //��ǰ��״̬������
   unsigned char KeyDownState;    //�������¼�ʱ��״̬������
   unsigned char KeyUpState;      //�������ϼ�ʱ��״̬������
   unsigned char KeyEnterState;   //���»س���ʱ��״̬������
   void (*CurrentOperate)();      //��ǰ״̬Ӧ��ִ�еĹ��ܲ���
}  StateTab;
//
//
unsigned char code s0[]="MenuTest        ";
unsigned char code s1[]="Contact         ";
unsigned char code s2[]="LineTest        ";
unsigned char code s3[]="RectTest        ";
unsigned char code s4[]="ImagTest        ";
unsigned char code s5[]="Cancel          ";
unsigned char code s6[]="Univercity      ";
unsigned char code s7[]="E-Mail          ";
unsigned char code s8[]="QQ Number       ";
unsigned char code s9[]="Cancel          ";
unsigned char code s10[]="                ";
/*-------------------------------------------------------------*/
void Stat0(void)
{
    en_disp(0,0,16,Asc,s0,0);
	en_disp(2,0,16,Asc,s1,1); 
	en_disp(4,0,16,Asc,s10,1);
	en_disp(6,0,16,Asc,s10,1);
}
/*-------------------------------------------------------------*/
void Stat1(void)
{	
    en_disp(0,0,16,Asc,s0,1);
	en_disp(2,0,16,Asc,s1,0);
	en_disp(4,0,16,Asc,s10,1);
	en_disp(6,0,16,Asc,s10,1);
}
/*-------------------------------------------------------------*/
void Stat2(void)
{
    en_disp(0,0,16,Asc,s2,0);
	en_disp(2,0,16,Asc,s3,1);
	en_disp(4,0,16,Asc,s4,1);
	en_disp(6,0,16,Asc,s5,1);
}
/*-------------------------------------------------------------*/
void Stat3(void)
{
    en_disp(0,0,16,Asc,s2,1);
	en_disp(2,0,16,Asc,s3,0);
	en_disp(4,0,16,Asc,s4,1);
	en_disp(6,0,16,Asc,s5,1);
}
/*-------------------------------------------------------------*/
void Stat4(void)
{
    en_disp(0,0,16,Asc,s2,1);
	en_disp(2,0,16,Asc,s3,1);
	en_disp(4,0,16,Asc,s4,0);
	en_disp(6,0,16,Asc,s5,1);
}
/*-------------------------------------------------------------*/
void Stat5(void)
{
    en_disp(0,0,16,Asc,s2,1);
	en_disp(2,0,16,Asc,s3,1);
	en_disp(4,0,16,Asc,s4,1);
	en_disp(6,0,16,Asc,s5,0);
}
/*-------------------------------------------------------------*/
void Stat6(void)
{
	en_disp(0,0,16,Asc,s6,0);
	en_disp(2,0,16,Asc,s7,1);
	en_disp(4,0,16,Asc,s8,1);
	en_disp(6,0,16,Asc,s9,1);
}
/*-------------------------------------------------------------*/
void Stat7(void)
{
    en_disp(0,0,16,Asc,s6,1);
	en_disp(2,0,16,Asc,s7,0);
	en_disp(4,0,16,Asc,s8,1);
	en_disp(6,0,16,Asc,s9,1);
}
/*-------------------------------------------------------------*/
void Stat8(void)
{
    en_disp(0,0,16,Asc,s6,1);
	en_disp(2,0,16,Asc,s7,1);
	en_disp(4,0,16,Asc,s8,0);
	en_disp(6,0,10,Asc,s9,1);
}
/*-------------------------------------------------------------*/
void Stat9(void)
{
    en_disp(0,0,16,Asc,s6,1);
	en_disp(2,0,16,Asc,s7,1);
	en_disp(4,0,16,Asc,s8,1);
	en_disp(6,0,16,Asc,s9,0);
}	 
/*-------------------------------------------------------------*/
void Stat10(void)
{
    ClearLCD();
    Line(10,10,100,48,1);
}
/*-------------------------------------------------------------*/
void Stat11(void)
{
    ClearLCD();
    Rect(0,0,127,63,1);
	Rect(10,10,67,43,1);
}
/*-------------------------------------------------------------*/
void Stat12(void)
{
//    ClearLCD();
    img_disp(1,10,70,56,img2);
}
/*-------------------------------------------------------------*/
void Stat13(void)
{
    img12864_disp(img);
}
/*-------------------------------------------------------------*/
void Stat14(void)
{
	en_disp(0,0,16,Asc,s10,1);
    en_disp(2,0,16,Asc,"E-Mail:         ",1);
	en_disp(4,0,16,Asc," shehun12505669 ",1);
	en_disp(6,0,16,Asc,"     @126.com   ",1);
}
/*-------------------------------------------------------------*/
void Stat15(void)
{
    en_disp(0,0,16,Asc,s10,1);
    en_disp(2,0,16,Asc,"QQ Number:      ",1);
	en_disp(4,0,16,Asc,"   12505669     ",1);
	en_disp(6,0,16,Asc,s10,1);

}
/*-------------------------------------------------------------*/
 //���ݽṹ����
StateTab code KeyTab[MENU_SIZE]=
{
   	{0,1,1,2,   (*Stat0)},    //����
	{1,0,0,6,   (*Stat1)},	   //����

	{2,3,5,10,  (*Stat2)},     //����
	{3,4,2,11,  (*Stat3)},	   //����
	{4,5,3,12,  (*Stat4)},	   //����
	{5,2,4,0,   (*Stat5)},	   //����

	{6,7,9,13,  (*Stat6)},     //����
	{7,8,6,14,  (*Stat7)},     //
	{8,9,7,15,  (*Stat8)},   
	{9,6,8,1 ,  (*Stat9)},     //����

	{10,2,2,2,(*Stat10)},    //��		   LineTest
	{11,3,3,3,(*Stat11)},	   //		   RectTest
	{12,4,4,4,(*Stat12)},	   //          ImgTest

	{13,6,6,6,(*Stat13)},	    //��		Univercity
	{14,7,7,7,(*Stat14)},		//          E-Mail
 	{15,8,8,8,(*Stat15)}		//          QQ Numeber 
};
/*-------------------------------------------------------------*/
void MenuOperate(unsigned char key)
{
    switch(key)
	{
	    case  0x30:		       //���ϵļ�
		{
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpState;
			break; 
		}
		case  0x28:			  //�س���
		{
			KeyFuncIndex=KeyTab[KeyFuncIndex].KeyEnterState;
			break; 
		}
		case  0x18:			  //���µļ�
		{
			KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDownState;
			break; 
		}
		//�˴���Ӱ����������
	}
	//������ִ�а����Ĳ���
	KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;

	(*KeyFuncPtr)();     //ִ�е�ǰ�İ�������
}	 //*/
/*-------------------------------------------------------------*/
unsigned char keyscan()
{
    unsigned char key;
	P2=0xFF;
	key=P2&0x38;
	delayms(8);
	//�������ȥ����
	return key;
}		 
/*--------------------------------------------------------------*/
void int_0(void) interrupt 0   using 0
{
   	 unsigned char key;
	 key=keyscan();
	 MenuOperate(key);
}
/*-------------------------------------------------------------*/
void main (void)
{
    LCD12864_init();
	ClearLCD();
	//���ж�
	EX0=1;
	//���õ�ƽ������ʽ
	IT0=1;
	//�������ж�
	EA=1;
	//
	MenuOperate(0);
    while(1)
	{
	}
}