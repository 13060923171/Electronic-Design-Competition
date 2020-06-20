#ifndef __lcd12864_h__
#define __lcd12864_h__

sbit psb_12864=P2^2;
sbit res_12864=P2^4;
sbit rs_12864=P2^5;
sbit rw_12864=P2^6;
sbit e_12864=P2^7;

void writecom_12864(uchar com)  //写命令
{
	rw_12864=0;
	rs_12864=0;
	delay_us(1);
	P0=convert(com);
	e_12864=1;
	delay_us(1);
	e_12864=0;
}

void writebyte_12864(uchar dat)  //写数据
{
	rw_12864=0;
	rs_12864=1;
	delay_us(1);
	P0=convert(dat);
	e_12864=1;
	delay_us(1);
	e_12864=0;
}

void initddrom_12864()    //LCD字库初始化程序
{
	delay_us(20);           
	psb_12864=1;              //设置为8BIT并口工作模式
	delay_us(1);          
	res_12864=0;              //复位
	delay_us(1);           
	res_12864=1;              //复位置高
	delay_us(5);	
	writecom_12864(0x30);   //基本指令集
	delay_ms(1);
	writecom_12864(0x02);   // 地址归位
	delay_ms(1);
	writecom_12864(0x0c);   //整体显示打开,游标关闭
	delay_ms(1);
	writecom_12864(0x01);   //清除显示
	delay_ms(1);
	writecom_12864(0x06);   //游标右移
	delay_ms(1);
}

void initgdrom_12864()   //LCD显示图片(扩展)初始化程序
{
	delay_us(20);            
	psb_12864=1;              //设置为8BIT并口工作模式
	delay_us(1);      
	res_12864=0;              //复位
	delay_us(1);       
	res_12864=1;              //复位置高
	delay_us(5);
	
	writecom_12864(0x36); 
	delay_ms(1);            
	writecom_12864(0x36);  
	delay_ms(1);          
	writecom_12864(0x3E);  
	delay_ms(1);            
	writecom_12864(0x01);  
	delay_ms(1);         
}

void disword_12864(uchar add,uchar *p) //某地址开始连续显示字符
{				 //当汉字和字符混搭时应注意连续的字符数应为偶数
    writecom_12864(add);  
	while(1)
	{
		if(*p=='\0')return;
  		writebyte_12864(*p);
   		p++;
		//delay_ms(1);使显示更亮？
	}
}

void disfullword_12864(uchar *add) //显示整屏字符
{
	uchar i;
    writecom_12864(0x80);  
    delay_ms(1);
	for(i=0;i<32;i++)
	{
  		writebyte_12864(*add);
   		add++;
	}

    writecom_12864(0x90);  
    delay_ms(1);
	for(i=32;i<64;i++)
	{
  		writebyte_12864(*add);
   		add++;
	}
}

void disfullphoto_12864(uchar *add)//显示整屏图像
{
	uchar i,j;
	//显示上半屏内容
	for(i=0;i<32;i++)              
	{ 
		writecom_12864(0x80+i); 	//垂直地址 
		writecom_12864(0x80);       //水平地址
		for(j=0;j<16;j++)
		{
			writebyte_12864(*add);
			add++;
		}
	}	   
	//显示下半屏内容
	for(i=0;i<32;i++)             
	{
		writecom_12864(0x80+i); 	//垂直地址
		writecom_12864(0x88);       //水平地址
		for(j=0;j<16;j++)
		{
			writebyte_12864(*add);
			add++;
		}
	}
}


/*  指令参考，注意区分基本指令和扩展指令

    writecom_12864(0x08);    //整体显示关，游标关，游标位置关
    writecom_12864(0x0c);    //整体显示开，游标关，游标位置关
    writecom_12864(0x0e);    //整体显示开，游标开，游标位置关
    writecom_12864(0x0f);    //整体显示开，游标开，游标位置开

    writecom_12864(0x02);    //位址归位，游标回到原点    
    writecom_12864(0x84);    //将DDRAM地址设为0x84，游标在此闪烁

    //以下四个命令是控制写入字符以后光标及整屏显示的移动  
    writecom_12864(0x07);    //光标右移整体显示左移   
    writecom_12864(0x05);    //光标左移整体显示右移   
    writecom_12864(0x06);    //光标右移整体显示不移动
    writecom_12864(0x04);    //光标左移整体显示不移动

    //以下四个命令无需写入显示数据，直接控制光标和整屏显示的移动  
    writecom_12864(0x10);    //光标左移
    writecom_12864(0x14);    //光标右移
    writecom_12864(0x18);    //整体显示左移，光标跟随  
    writecom_12864(0x1c);    //整体显示右移，光标跟随 
    
    //进入扩展功能模式命令
    writecom_12864(0x34);    //打开扩展功能模式，绘图显示关闭

    writecom_12864(0x04);    //同时反白1、3行
    writecom_12864(0x04);    //再次反白1、3行，相当于关闭1、3行反白
    writecom_12864(0x05);    //同时反白2、4行
    writecom_12864(0x05);    //再次反白2、4行，相当于关闭2、4行反白
 
    writecom_12864(0x08);    //进入睡眠模式
    writecom_12864(0x0c);    //退出睡眠模式

    writecom_12864(0x01);    //进入待命模式

    writecom_12864(0x36);    //打开扩展功能模式，打开绘图显示
    writecom_12864(0x34);    //打开扩展功能模式，关闭绘图显示

    writecom_12864(0x30);    //回到基本指令集

    writecom_12864(0x01);    //清屏   
}*/

#endif