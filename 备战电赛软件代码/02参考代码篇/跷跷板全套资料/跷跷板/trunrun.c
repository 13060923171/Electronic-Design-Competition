#include<reg51.h>

void rightrun() 
{	    b=1;
        
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