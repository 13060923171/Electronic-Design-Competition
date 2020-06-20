/************************/
/*						*/
/*	 ºóÍËÑ­¼£³ÌÐòÄ£¿é	*/
/*						*/
/************************/
#include<reg51.h>
htxunji()
{	if(p2_6==1)
		{	backrun();
		}
	if(p2_1==1&&p2_6==0&&p2_0==0&p2_2==0)
	   bleftrun();
		if(p2_0==1)	
		{switch(a)
		{	case 5:for(i=p2_6;i==0;i=p2_6)brightrun();break;
		    case 6:for(i=p2_6;i==0;i=p2_6)bleftrun();break;
			case 4: bleftrun();break;
			default:backrun();
		}
		}
     if(p2_1==1)
		switch(a)
		{	case 5:for(i=p2_6;i==0;i=p2_6)brightrun();break;
		    case 6:for(i=p2_6;i==0;i=p2_6)bleftrun();break;
			case 4: bleftrun();break;
			default:backrun();
		}
     if(p2_0==0&&p2_1==0&&p2_2==0)
	{ switch(a)
		{   case 5://for(hh=p2_6;hh==0;hh=p2_6)
					{	brightrun();
			//		    forwordrun();
					}break;
        	 case 6://for(hh=p2_6;hh==0;hh=p2_6)
			 		{	bleftrun();
			//			forwordrun();
					}
						break;
			default:backrun();
	
		}
	}
   }
