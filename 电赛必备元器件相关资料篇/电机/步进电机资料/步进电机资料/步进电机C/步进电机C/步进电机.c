/*
 1�����������ڲ���4�ಽ������������� 
 2����Ҫ����ñ���߶Ű��߰��ź�����˺Ͷ�Ӧ�Ĳ�������ź��������������
 3���ٶȲ����Ե��ڵĹ��죬��Ȼ��û������ת����
 ���԰��ո���ԭ��ͼ����
  ��P1.1-P1.4
*/

#include <reg52.h>


unsigned char code F_Rotation[4]={0x02,0x04,0x08,0x10};//��ת���
unsigned char code B_Rotation[4]={0x10,0x08,0x040,0x02};//��ת���

void Delay(unsigned int i)//��ʱ
{
 while(--i);
}

main()
{
 
 unsigned char i;

 while(1)
 {
  for(i=0;i<4;i++)      //4��
     {
     P1=F_Rotation[i];  //�����Ӧ���� �������л��ɷ�ת���
     Delay(500);        //�ı�����������Ե������ת��
	 }
  }
}