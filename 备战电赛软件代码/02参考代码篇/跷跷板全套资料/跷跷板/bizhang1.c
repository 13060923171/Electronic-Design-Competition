/************************/
/*						*/
/*	  ���ϳ���ģ��		*/
/*						*/
/************************/
#include <reg51.h>


bizhang()
{		unsigned int k;
		int zai ,p22;
 		for(zai=0;zai==0;)			/*�ж�ǰ���Ƿ����ϰ�����ϰ�������תֱ�����ϰ���*/
		{	if(p3_2==0)			/*���K1�������򷵻�������*/
			{	delay1ms(1000,1000);
				return;
			}
			leftrun();			
 			zai=p2_7;
		}
		xianshi();				/*������ʾ����*/
		for (k=0;k<8000;k++)			/*����תһ�������������ǳ���Ĵ�С*/
		{	if(p3_2==0)			/*���K1�������򷵻�������*/
			{	delay1ms(1000,1000);
				return;
			}
			leftrun();
		}
		xianshi();				/*������ʾ����*/
		for(k=0;k<15000;k++)			/*ǰ��һ���ľ���*/
		{	if(p3_2==0)
			{	delay1ms(1000,1000);
				return;
			}
			forwordrun();
	       		zai=p2_7;
			if(zai==0)		 	/*����ٴ������ϰ���򷵻����������±���*/
	        	return;
		}
		xianshi();		 		/*������ʾ����*/
		for(k=0;k<10000;k++)			//��תֱ�������ϰ�����޶���������ת��//
                {	if(p3_2==0)			/*���K1�������򷵻�������*/
			{	delay1ms(1000,1000);
				return;
			}
			zai=p2_7;
			if (zai)
			rightrun();
			else
			break;
		}
		xianshi();				/*������ʾ����*/
		for(k=0;k<1000;k++)			//��תһ������//
                {	if(p3_2==0)
			{	delay1ms(1000,1000);
				return;
			}
			leftrun();
		}		
		xianshi();
		for(p22=0;p22==0;)			//ǰ��ֱ��Ѱ����//
        	{	if(p3_2==0)			/*���K1�������򷵻�������*/
			{	delay1ms(1000,1000);
				return;
			}
			zai=p2_7;
		   	p22=p2_2;
			if (!zai)			//��������ϰ�������תһ����//
			{	for(k=0;k<20000;k++)
					{	if(p3_2==0)
						{	delay1ms(1000,1000);
				        		return;
						}
			        leftrun();
			}
           forwordrun();
	   xianshi();		//��ʾ//
		}   		
	return;
}
}
