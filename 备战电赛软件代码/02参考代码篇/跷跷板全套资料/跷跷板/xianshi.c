/************************/
/*						*/
/*	  ��ʾ����ģ��		*/
/*						*/
/************************/
#include<reg51.h> 

     //���з���һ�ֽ�����
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
    {	SendByte(0xF8);//11111,00,0     RW=0,RS=0     ͬ����־
        SendByte(dat&0xF0);//����λ
        SendByte((dat&0x0F)<<4);//����λ
    }
//д��ʾ���ݻ��ֽ��ַ�
void SendDat(unsigned char dat)
    {	SendByte(0xFA);//11111,01,0     RW=0,RS=1
        SendByte(dat&0xF0);//����λ
        SendByte((dat&0x0F)<<4);//����λ
    }
    /*               д���ֵ�LCD     ָ����λ��
              x_add��ʾRAM�ĵ�ַ
              dat1/dat2��ʾ���ֱ���
    */


    //��ʼ��     LCM
void initlcm(void)
    {	SendCMD(0x30);//�������ã�һ����8λ���ݣ�����ָ�
        SendCMD(0x0C);//0000,1100       ������ʾ���α�off���α�λ��off
        SendCMD(0x01);//0000,0001     ��DDRAM
        SendCMD(0x02);//0000,0010     DDRAM��ַ��λ
        SendCMD(0x80);//1000,0000     �趨DDRAM     7λ��ַ000��0000����ַ������AC
    }
void set_wenzi(void)
{    SendCMD(0x80);//1000,0001     �趨DDRAM     7λ��ַ000��0001����ַ������AC.
     SendDat(0xd0);//��ASCII�������,��ʾ��Һ����Ļ��.��ͬ
     SendDat(0xa1);//С
     SendDat(0xb3);
     SendDat(0xb5);//��
     SendDat(0xd7);
     SendDat(0xb4);//״
     SendDat(0xcc);
     SendDat(0xac);//̬
     SendDat(0xa3);
     SendDat(0xba);
	 SendDat(xszt1);
	 SendDat(xszt2);
	 SendDat(xszt3);
	 SendDat(xszt4);//��ǰ״̬��ѭ�������ϡ�ƽ�⣩
     SendCMD(0x90);//����Һ��������ʾ��ַ,��ͬ.
     SendDat(0xca);
     SendDat(0xb1);//ʱ
     SendDat(0xbc);
     SendDat(0xe4);//��
     SendDat(0x3a);
     SendDat(sec4);
     SendDat(sec3);
     SendDat(sec2);
     SendDat(0x2e);
     SendDat(sec1);
     SendDat(0x73);
     SendCMD(0x88);
     SendDat(0xcb);
     SendDat(0xd9);//��
     SendDat(0xb6);
     SendDat(0xc8);//��
     SendDat(0x3a);//��
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
     SendDat(0xb7);//·
     SendDat(0xb3);
     SendDat(0xcc);//��
     SendDat(0x3a);
     SendDat(lc4);
     SendDat(lc3);
     SendDat(0x2e);
     SendDat(lc2);
     SendDat(lc1);
     SendDat(0x6d);
}
xianshi()
    {  	set_wenzi();    //����Ҫ���õ�������ʾ��Һ����Ļ�ϡ�
     	return;
	}