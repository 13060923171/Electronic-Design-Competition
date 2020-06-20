/************************/
/*						*/
/*	  显示程序模块		*/
/*						*/
/************************/
#include<reg51.h> 

     //串行发送一字节数据
void SendByte(unsigned char dat)
    {	unsigned char i;
        for(i=0;i<8;i++)
           {	E_CLK=0;
                if(dat&0x80)RW_SID=1;
				else RW_SID=0;
                E_CLK=1;
                dat=dat<<1;
            }
    }
void SendCMD(unsigned char dat)
    {	SendByte(0xF8);//11111,00,0     RW=0,RS=0     同步标志
        SendByte(dat&0xF0);//高四位
        SendByte((dat&0x0F)<<4);//低四位
    }
//写显示数据或单字节字符
void SendDat(unsigned char dat)
    {	SendByte(0xFA);//11111,01,0     RW=0,RS=1
        SendByte(dat&0xF0);//高四位
        SendByte((dat&0x0F)<<4);//低四位
    }
    /*               写汉字到LCD     指定的位置
              x_add显示RAM的地址
              dat1/dat2显示汉字编码
    */


    //初始化     LCM
void initlcm(void)
    {	SendCMD(0x30);//功能设置，一次送8位数据，基本指令集
        SendCMD(0x0C);//0000,1100       整体显示，游标off，游标位置off
        SendCMD(0x01);//0000,0001     清DDRAM
        SendCMD(0x02);//0000,0010     DDRAM地址归位
        SendCMD(0x80);//1000,0000     设定DDRAM     7位地址000，0000到地址计数器AC
    }
void set_wenzi(void)
{    SendCMD(0x80);//1000,0001     设定DDRAM     7位地址000，0001到地址计数器AC.
     SendDat(0xd0);//将ASCII码调出来,显示在液晶屏幕上.下同
     SendDat(0xa1);//小
     SendDat(0xb3);
     SendDat(0xb5);//车
     SendDat(0xd7);
     SendDat(0xb4);//状
     SendDat(0xcc);
     SendDat(0xac);//态
     SendDat(0xa3);
     SendDat(0xba);
	 SendDat(xszt1);
	 SendDat(xszt2);
	 SendDat(xszt3);
	 SendDat(xszt4);//当前状态（循迹、避障、平衡）
     SendCMD(0x90);//设置液晶屏的显示地址,下同.
     SendDat(0xca);
     SendDat(0xb1);//时
     SendDat(0xbc);
     SendDat(0xe4);//间
     SendDat(0x3a);
     SendDat(sec4);
     SendDat(sec3);
     SendDat(sec2);
     SendDat(0x2e);
     SendDat(sec1);
     SendDat(0x73);
     SendCMD(0x88);
     SendDat(0xcb);
     SendDat(0xd9);//速
     SendDat(0xb6);
     SendDat(0xc8);//度
     SendDat(0x3a);//：
     SendDat(sd4);
     SendDat(sd3);
     SendDat(sd2);
     SendDat(0x2e);
     SendDat(sd1);
	 SendDat(0x63);
	 SendDat(0x6d);
     SendDat(0x2f);
     SendDat(0x73);
     SendCMD(0x98);
     SendDat(0xc2);
     SendDat(0xb7);//路
     SendDat(0xb3);
     SendDat(0xcc);//程
     SendDat(0x3a);
     SendDat(lc4);
     SendDat(lc3);
     SendDat(0x2e);
     SendDat(lc2);
     SendDat(lc1);
     SendDat(0x6d);
}
xianshi()
    {  	set_wenzi();    //将想要设置的文字显示在液晶屏幕上。
     	return;
	}