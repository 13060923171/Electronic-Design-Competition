/*-----------------------------------------------------------*/
#define LCD_OFF 0x3E
#define LCD_ON  0x3F

#define Add_X 0xB8     //the start address of the page 0 ;(0~7)
#define Add_Y 0x40     //the start address of the Y counter ; (0~64)
#define Add_Z 0xC0     //the start address of the DDRAM ; (0~64)
/*-----------------------------------------------------------*/
#define  LCD12864_DATA_PORT  P0

sbit  LCD12864_EN         =P1^4;
sbit  LCD12864_RW         =P1^3;       //0:write ; 1:read 
sbit  LCD12864_RS         =P1^2;	   //0:the command .1:the data
sbit  LCD12864_CS_L       =P1^1;       //select the left of the lcd when 1
sbit  LCD12864_CS_R       =P1^0;  
sbit  LCD12864_RST 	   	  =P1^5;
/*------------------------------------------------------------*/
void delayus(unsigned int us)
{
   while(us--);
}
void delayms(unsigned ms)
{
   unsigned int i,j;
   for (i=0;i<ms;i++)
   {
      for (j=0;j<1000;j++)
	  ;
   } 
}

/*--------------------------select the LCD--------------------*/
void LCDSel(unsigned char sel)
{
    switch(sel)
	{
	   case 0: LCD12864_CS_L=0;LCD12864_CS_R=0;break;	  
	   case 1: LCD12864_CS_L=1;LCD12864_CS_R=0;break;       //left 
	   case 2: LCD12864_CS_L=0;LCD12864_CS_R=1;break;		//right
	   default:;
	};
}
/*------------------------------------------------------------*/
void WaitLCD()
{
   unsigned char flag;

   LCD12864_DATA_PORT=0xFF;
   LCD12864_RW=1;
   LCD12864_RS=0;
   LCD12864_EN=1;
   LCD12864_EN=1;
   LCD12864_EN=0;

   LCD12864_DATA_PORT=0xFF;		//读有效数据
   LCD12864_RW=1;
   LCD12864_RS=0;
   ;
   LCD12864_EN=1;
   do
   {
      flag=LCD12864_DATA_PORT;
	  LCD12864_DATA_PORT=0xFF;
   }while(!((flag&0x80)==0x80));

   LCD12864_EN=0;
}				 //*/
/*-------------------------------------------------------------*/
void WriteDatToLCD12864(unsigned char dat)
{
   WaitLCD();
   LCD12864_RS=1;   //the data
   LCD12864_RW=0;   //write
   LCD12864_DATA_PORT=dat;
   LCD12864_EN=1;
   ;
   LCD12864_EN=0;
}
/*-------------------------------------------------------------*/
void WriteCmdToLCD12864(unsigned char cmd)
{
   WaitLCD();
   LCD12864_RS=0;   //the command
   LCD12864_RW=0;	//write
   LCD12864_DATA_PORT=cmd;
   LCD12864_EN=1;
   ;
   LCD12864_EN=0;
}
/*-------------------------------------------------------------*/
unsigned char ReadDatFromLCD12864(void)
{
   unsigned char dat;
   WaitLCD();
   LCD12864_DATA_PORT=0xFF;		      //读空操作
   LCD12864_RS=1;    //the data
   LCD12864_RW=1;     //read
   LCD12864_EN=1;
   LCD12864_EN=1
   ;
   LCD12864_EN=0;

   LCD12864_DATA_PORT=0xFF;			  //来读有效数据
   LCD12864_RS=1;    //the data
   LCD12864_RW=1;     //read
   LCD12864_EN=1;
   dat=LCD12864_DATA_PORT;
   LCD12864_EN=0;
   return dat;
}	
/*--------------------------------------------------------------*/
//from the chip manual
void LCD12864_init(void)
{
    LCD12864_RST=0;
	delayus(50);
	LCD12864_RST=1;
	LCDSel(0);       //left and right
	WriteCmdToLCD12864(LCD_OFF);
	WriteCmdToLCD12864(LCD_ON);
}
/*------------------------------------------------------------*/
//x:0~7
void SetX(unsigned char x)
{
    WriteCmdToLCD12864(Add_X+x);
}
/*------------------------------------------------------------*/
//y:0~127
void SetY(unsigned char y)
{
    WriteCmdToLCD12864(Add_Y+y);
}
/*------------------------------------------------------------*/
//z:0~63
void SetZ(unsigned char z)
{
    WriteCmdToLCD12864(Add_Z+z);
}
/*------------------------------------------------------------*/
void ClearLCD()
{
    int i,j;
    LCDSel(0);
	for(j=0;j<8;j++)
	{
	   WriteCmdToLCD12864(LCD_ON);
       SetX(j);  
       WriteCmdToLCD12864(Add_Y);  //diffrent from SetY(0),SetY(64);     
	   SetZ(0);
	   for (i=0;i<64;i++)
	   {
	      WriteDatToLCD12864(0x00);
	   }
	}	
}
/*------------------------------------------------------------*/
//左上角第一个点为原点，向下Y为轴，向右为X轴
//x:0~63~127    y:0~63
//flag :  0：擦除某个点
//        1：显示某个点 
unsigned char code Tab[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
void Dot(char x,char y,bit flag)
{
    unsigned char dat;
//	y=63-y;	                              //坐标轴的移动和反转
    if(x<64)
	{
	    LCDSel(1);
		SetX(y/8);   //set the page address
		SetY(x);         //set the Y adress
		dat=ReadDatFromLCD12864();

		if(flag)
		  dat=dat|(Tab[y%8]);
		else
		  dat=dat&(~(Tab[y%8]));

		SetY(x);         //set the Y adress
		WriteDatToLCD12864(dat);
	}
	else if(x<128)
	{
	    LCDSel(2);
		SetX(y/8);
		SetY(x-64);

		dat=ReadDatFromLCD12864();

		if(flag)
		  dat=dat|(Tab[y%8]);
		else
		  dat=dat&(~(Tab[y%8]));

		SetY(x-64);
		WriteDatToLCD12864(dat);
	}     
}	   
/*------------------------------------------------------------*/
//draw a line between point(x1,y1) and point(x2,y2)
//flag   0:erase the line    1:draw a line
void Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,bit flag)
{
    unsigned char i;
	unsigned char temp;
	float k;
	if(x1==x2)
	{
	   if(y1>y2)
	   {   temp=x1;x1=x2;x2=temp;
    	   temp=y1;y1=y2;y2=temp;
		}
	   for(i=y1;i<=y2;i++)
	   {
	       Dot(x1,i,flag);
	   }
	}else
	{
    	if(x1>x2)
    	{
    	   temp=x1;x1=x2;x2=temp;
    	   temp=y1;y1=y2;y2=temp;
     	 }

         k=(float)(y2-y1)/(float)(x2-x1);

         for(i=0;i<x2-x1;i++)
        {
           Dot(x1+i,(unsigned char)(y1+k*i),flag);
         }
	}
}	   
/*-------------------------------------------------------------*/
void Rect(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,bit flag)
{
    Line(x1,y1,x2,y1,flag);
	Line(x2,y1,x2,y2,flag);
	Line(x2,y2,x1,y2,flag);
	Line(x1,y2,x1,y1,flag);
} 
/*-------------------------------------------------------------*/
/*void RectArea(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,bit flag)
{
    unsigned char temp;
	unsigned char i;
    if(x1>x2)
	{
	    temp=x1;x1=x2;x2=temp; 
		temp=y1;y1=y2;y2=temp;
     }

	 for(i=0;i<=x2-x1;i++)
	 {
	    Line(x1+i,y1,x1+i,y2,flag);
	 }

}			  //*/
/*-------------------------------------------------------------*/
//16*16
//x行  0~7      y列 0~127
//flag 0:汉字反白显示
/*void hz_disp(unsigned char x,unsigned char y,unsigned char n,unsigned char code * hz,bit flag)
{
    unsigned char i,j;
	for (j=0;j<n;j++)
	{
	    //显示上半个汉字
	    for(i=0;i<16;i++)
		{
		    //点的位置是在左还是右
			if(y+16*j+i<64)
			{
			    LCDSel(1);
				WriteCmdToLCD12864(LCD_ON);
				SetX(x);
				SetZ(0);
				SetY(y+16*j+i);
				if(flag)
				    WriteDatToLCD12864(hz[32*j+i]);
			    else
				    WriteDatToLCD12864(~hz[32*j+i]);
			}
			else if(y+16*j+i<128)
			{
			    LCDSel(2);
				WriteCmdToLCD12864(LCD_ON);
				SetX(x);
				SetZ(0);
				SetY(y+16*j+i-64);
				if(flag)
				    WriteDatToLCD12864(hz[32*j+i]);
			    else
				    WriteDatToLCD12864(~hz[32*j+i]);
			}
		}
		//显示下半个汉字
		for(i=16;i<32;i++)
		{
		    //先判断点是在左还是在右
			if(y+16*j+i-16<64)
			{	if(x+1<8)	       //最后一行显示上半个字
			    {
			        LCDSel(1);
			    	WriteCmdToLCD12864(LCD_ON);
				    SetX(x+1);
				    SetZ(0);
				    SetY(y+16*j+i-16);
				    if(flag)
				        WriteDatToLCD12864(hz[32*j+i]);
			        else
				        WriteDatToLCD12864(~hz[32*j+i]);
				}
			}else if(y+16*j+i-16<127)
			{
			    if(x+1<8) 	      //最后一行
				{
			        LCDSel(2);
				    WriteCmdToLCD12864(LCD_ON);
				    SetX(x+1);
				    SetZ(0);
				    SetY(y+16*j+i-16-64);
				if(flag)
				    WriteDatToLCD12864(hz[32*j+i]);
			    else
				    WriteDatToLCD12864(~hz[32*j+i]);
				}
			}
		}
	}
}				  //*/
/*-------------------------------------------------------------*/
//x:行0~7
//y:列0~127
//asc: 指向标准交换码
//string: 指向要显示的字符串
//flag:  0 反白显示
void en_disp(unsigned char x,unsigned char y,unsigned char n,unsigned char code *asc,unsigned char *string,bit flag)
{
    unsigned char i,j,loc;
	for (j=0;j<n;j++)
	{
	    loc=string[j]-0x20;	    //确定要显示的字符在asc表中的位置（乘上16）
	    //显示上半个字母
	    for(i=0;i<8;i++)
		{
		    //点的位置是在左还是右
			if(y+8*j+i<64)
			{
			    LCDSel(1);
				WriteCmdToLCD12864(LCD_ON);
				SetX(x);
				SetZ(0);
				SetY(y+8*j+i);
				if(flag)
				    WriteDatToLCD12864(asc[16*loc+i]);
			    else
				    WriteDatToLCD12864(~asc[16*loc+i]);
			}
			else if(y+8*j+i<128)
			{
			    LCDSel(2);
				WriteCmdToLCD12864(LCD_ON);
				SetX(x);
				SetZ(0);
				SetY(y+8*j+i-64);
				if(flag)
				    WriteDatToLCD12864(asc[16*loc+i]);
			    else
				    WriteDatToLCD12864(~asc[16*loc+i]);
			}
		}
		//显示下半个字母
		for(i=8;i<16;i++)
		{
		    //先判断点是在左还是在右
			if(y+8*j+i-8<64)
			{  
			    if(x+1<8)	//最后一行
				{
			       LCDSel(1);
				   WriteCmdToLCD12864(LCD_ON);
				   SetX(x+1);
				   SetZ(0);
				   SetY(y+8*j+i-8);
				   if(flag)
				      WriteDatToLCD12864(asc[16*loc+i]);
			       else
				      WriteDatToLCD12864(~asc[16*loc+i]);
				}
			}else if(y+8*j+i-8<128)
			{
			    if(x+1<8) 	      //最后一行
				{
			        LCDSel(2);
				    WriteCmdToLCD12864(LCD_ON);
				    SetX(x+1);
				    SetZ(0);
				    SetY(y+8*j+i-8-64);
				if(flag)
				    WriteDatToLCD12864(asc[16*loc+i]);
			    else
				    WriteDatToLCD12864(~asc[16*loc+i]);
				}
			}
		}
	}  
}
/*-------------------------------------------------------------*/
//(x,y)要显示图片的左上角的坐标
//x  0~7   y  0~127
//l:要显示图片的长度
//w:要显示图片的宽度
//img:指向图片的数据
//执行有点慢
void img_disp(unsigned char x,unsigned char y,unsigned char l,unsigned char w,unsigned char code *img)
{
    unsigned char i,j;
	for(j=0;j<w/8;j++)
	{
	    if(j+x<8)       //保证不越下面的界限 
	    {
	   	    for(i=0;i<l;i++)
	    	{	
		       //先判断点的位置是在右半屏还是左半屏
		        if(y+i<64)
		    	{
			        LCDSel(1);
			        WriteCmdToLCD12864(LCD_ON);
			        SetX(x+j);
			        SetZ(0);
			        SetY(y+i);
		            WriteDatToLCD12864(img[j*l+i]);
		    	 }
			    else if(y+i<127)
		        {
			        LCDSel(2);
			        WriteCmdToLCD12864(LCD_ON);
			        SetX(x+j);
			        SetZ(0);
			        SetY(y+i-64);
			        WriteDatToLCD12864(img[j*l+i]);
			     }
			}
		}
	}
}
/*-------------------------------------------------------------*/
//显示128*64图片
//执行效率高
void img12864_disp(unsigned char code *img)
{
    unsigned char i,j;
	for(j=0;j<8;j++)
	{
	    LCDSel(1);
		WriteCmdToLCD12864(LCD_ON);
		SetX(j);
		SetZ(0);
		SetY(0);
		for(i=0;i<64;i++)
		   WriteDatToLCD12864(img[j*128+i]);
		LCDSel(2);
		WriteCmdToLCD12864(LCD_ON);
		SetX(j);
		SetZ(0);
		SetY(0);
		for(i=64;i<128;i++)
		   WriteDatToLCD12864(img[j*128+i]);
	}
}	//*/