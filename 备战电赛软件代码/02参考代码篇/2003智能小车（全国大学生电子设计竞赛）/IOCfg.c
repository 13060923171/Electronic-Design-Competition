#include<reg51.h>

/*���Ŷ���*/

//�����������
//��ߵ��
sbit  LMot_AP    = P0^0;
sbit  LMot_AN    = P0^1;
sbit  LMot_BP    = P0^2;
sbit  LMot_BN    = P0^3;
//�ұߵ��
sbit  RMot_AP    = P3^4;
sbit  RMot_AN    = P3^5;
sbit  RMot_BP    = P3^6;
sbit  RMot_BN    = P3^7;

//Ѱ�ߵ�·����
sbit  Follow_L 		= P0^5;
sbit  Follow_M   	= P0^6;
sbit  Follow_R  	= P0^7;

//�����������
sbit  Metal_DTC    = P3^2;

//�������ϰ���������
sbit  UltraDTC_TR   = P1^0;
sbit  UltraDTC_RE   = P1^1;

//��Դ�������
sbit  LightDTC_L   = P1^2;
sbit  LightDTC_M   = P1^3;
sbit  LightDTC_R   = P1^4;

//����ָʾ����
sbit  Guide   = P1^5;

//�������ʾ����
#define  DispData    P2
sbit  DS1CS     = P1^6;
sbit  DS0CS     = P1^7;