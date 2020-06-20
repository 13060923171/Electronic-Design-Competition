/************************/
/*						*/
/*	 前进循迹程序模块	*/
/*						*/
/************************/
#include<reg51.h>


void brightrun() //////////////
{	    b=1;
       
		p1_0=0;
		p1_1=1;
		p1_3=1;
		p1_4=1;
		a=6;
    	return;
}
 void bleftrun()//////////////
{	
	b=1;
	
	p1_0=1;
	p1_1=1;
	p1_3=0;
	p1_4=1;
	a=5;
	return;
}


void rightrun() 
{	b=1;
	p1_0=1;
	p1_1=0;
	p1_3=1;
	p1_4=1;
	a=2;
    	return;
}
 void leftrun()
{	
	b=1;
	p1_0=1;
	p1_1=1;
	p1_3=1;
	p1_4=0;
	a=1;
	return;
}


void forwordrun()
{	b=0;
	p1_0=1;
	p1_1=0;
	p1_3=1;/**/
	p1_4=0;/**/
	a=3;
	return;
}
void backrun()
{	b=0;
    
	p1_0=0;
	p1_1=1;
	p1_3=0;/**/
	p1_4=1;/**/
	a=4;
	return;
}
void xunji()
{	if(p2_0==1)								/*如果右边小灯不亮，则进行右转*/
	{	if(p2_0==1&&p2_1==1)				/*如果中间小灯和右边小灯都不亮，则小转*/
			rightrun();					
		else if(p2_0==1&&p2_1==0&&p2_2==0)	/*如果只有右边小灯不亮，则大转*/
		{	rightrun();	
			rightrun();
		}
	}
	if(p2_2==1)									/*如果左边小灯不亮，则进行左转*/
	{	if(p2_2==1&&p2_1==1)                    /*如果中间小灯和左边小灯都不亮，则小转*/ 
		   	leftrun();                      					
		else if(p2_0==0&&p2_1==0&&p2_2==1)      /*如果只有左边小灯不亮，则大转*/       
		{	leftrun();
			leftrun();
		}	
	}	
        if(p2_1==1&&p2_0==0&&p2_2==0)			/*如果只有中间灯亮，则前进*/
	{
		forwordrun();
	}		
	 	
	if((p2_0==0&&p2_1==0&&p2_2==0)||(p2_0==1&&p2_1==0&&p2_2==1)||(p2_0==1&&p2_1==1&&p2_2==1)) /*如果出现三个灯全亮或中间亮两边灭则执行上一次操作*/
		{
			switch(a)
			{case 1:leftrun();break;
		    	case 2:rightrun();break;
			default:{if(zt==3)	forwordrun();
					else backrun();}
			}
		}
}