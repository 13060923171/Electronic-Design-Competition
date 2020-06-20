/************************/
/*						*/
/*	  避障程序模块		*/
/*						*/
/************************/
#include <reg51.h>


bizhang()
{		unsigned int k;
		int zai ,p22;
 		for(zai=0;zai==0;)			/*判断前方是否有障碍物，有障碍物则左转直到无障碍物*/
		{	if(p3_2==0)			/*如果K1键按下则返回主函数*/
			{	delay1ms(1000,1000);
				return;
			}
			leftrun();			
 			zai=p2_7;
		}
		xianshi();				/*调用显示函数*/
		for (k=0;k<8000;k++)			/*再左转一定的余量，考虑车身的大小*/
		{	if(p3_2==0)			/*如果K1键按下则返回主函数*/
			{	delay1ms(1000,1000);
				return;
			}
			leftrun();
		}
		xianshi();				/*调用显示函数*/
		for(k=0;k<15000;k++)			/*前进一定的距离*/
		{	if(p3_2==0)
			{	delay1ms(1000,1000);
				return;
			}
			forwordrun();
	       		zai=p2_7;
			if(zai==0)		 	/*如果再次碰到障碍物，则返回主函数重新避障*/
	        	return;
		}
		xianshi();		 		/*调用显示函数*/
		for(k=0;k<10000;k++)			//右转直到碰到障碍物，但限定了最大的右转量//
                {	if(p3_2==0)			/*如果K1键按下则返回主函数*/
			{	delay1ms(1000,1000);
				return;
			}
			zai=p2_7;
			if (zai)
			rightrun();
			else
			break;
		}
		xianshi();				/*调用显示函数*/
		for(k=0;k<1000;k++)			//左转一定余量//
                {	if(p3_2==0)
			{	delay1ms(1000,1000);
				return;
			}
			leftrun();
		}		
		xianshi();
		for(p22=0;p22==0;)			//前进直到寻到迹//
        	{	if(p3_2==0)			/*如果K1键按下则返回主函数*/
			{	delay1ms(1000,1000);
				return;
			}
			zai=p2_7;
		   	p22=p2_2;
			if (!zai)			//如果碰到障碍物则左转一定量//
			{	for(k=0;k<20000;k++)
					{	if(p3_2==0)
						{	delay1ms(1000,1000);
				        		return;
						}
			        leftrun();
			}
           forwordrun();
	   xianshi();		//显示//
		}   		
	return;
}
}
