#include "utili.h"


/*---------------��ʱ�ӳ���----------------*/
void delay (uint us) 
{
  	while(us--);
}
void delay1 (uint ms) 
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;
}
//�����ַ�������
uchar strlen(uchar *s)
{
	uchar len = 0;
	while(*s++) len ++;
	
	return len;
}