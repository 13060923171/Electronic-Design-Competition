#include<reg51.h>
#define uchar unsigned char
sbit p2_0=P2^0;
sbit p2_1=P2^1;
sbit p2_2=P2^2;
sbit p2_3=P2^3;
sbit p2_4=P2^4;
sbit p2_5=P2^5;
sbit p2_6=P2^6;
sbit p1_0=P1^0;
sbit p1_1=P1^1;
sbit p1_2=P1^2;
sbit p1_3=P1^3;
sbit p1_4=P1^4;
sbit p1_5=P1^5;
sbit p1_6=P1^6;
sbit p2_7=P2^7;
sbit p3_2=P3^2;
sbit p3_6=P3^6;
sbit p3_4=P3^4;


int a,b,l,setnumber=5,n,r,t,kk,i,m=0,tk,vt,st,yx,z,TC=1;
int phzt=0/*ƽ��״̬*/;dd=0/*�ȴ�ʱ��*/;
char sec1,sec2,sec3,sec4,sd1=48,sd2=48,sd3=48,sd4=48,lc1,lc2,lc3,lc4;
float s,c=0.9742647;
double v;
sbit     E_CLK     =P1^6;//clock     input         ͬ��ʱ�������
sbit     RW_SID=P1^7;//data     input/output                �롢�����
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
     SendDat(0xd0);    //��ASCII�������,��ʾ��Һ����Ļ��.��ͬ
     SendDat(0xa1);//С
     SendDat(0xb3);
     SendDat(0xb5);//��
     SendDat(0xd7);
     SendDat(0xb4);//״
     SendDat(0xcc);
     SendDat(0xac);//̬
     SendDat(0xa3);
     SendDat(0xba);
     SendCMD(0x90); //����Һ��������ʾ��ַ,��ͬ.
     SendDat(0xca);
     SendDat(0xb1);//ʱ
     SendDat(0xbc);
     SendDat(0xe4); //��
     SendDat(0x3a);
     SendDat(sec4);
     SendDat(sec3);
     SendDat(sec2);
     SendDat(0x2e);
     SendDat(sec1);
     SendDat(0x73);
     SendCMD(0x88);
     SendDat(0xcb);
     SendDat(0xd9); //��
     SendDat(0xb6);
     SendDat(0xc8); //��
     SendDat(0x3a); //��
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
     SendDat(0xb7); //·
     SendDat(0xb3);
     SendDat(0xcc); // ��
     SendDat(0x3a);
     SendDat(lc4);
     SendDat(lc3);
     SendDat(0x2e);
     SendDat(lc2);
     SendDat(lc1);
     SendDat(0x6d);
}
xianshi()
    {   set_wenzi();    //����Ҫ���õ�������ʾ��Һ����Ļ�ϡ�
     	return;
	}
void delay1ms(uchar t);	   //��ʱ����
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

	p1_0=0;
	p1_1=1;
	p1_3=1;
	p1_4=0;
	a=1;
	return;
}
void brightrun() //////////////
{	    b=1;
		p1_0=1;
		p1_1=0;
		p1_3=0;
		p1_4=1;
		a=2;
    	return;
}
 void bleftrun()//////////////
{	b=1;
	p1_0=0;
	p1_1=1;
	p1_3=1;
	p1_4=1;
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



timer0() interrupt 1 using 1
{	TH0=0xFc;
	TL0=0x18;

	r++;
	t++;
if(TC==1)
{	if(b==1)
	{
		if(m<setnumber)
		{	p1_2=0;
			p1_5=0;
		}
	else
		{	p1_2=1;
			p1_5=1;
		}
    	if (m>=10)
		m=0;
		m++;
	}
	else if(b==0)
	{		if(l<setnumber+1)
		{	p1_2=0;
			p1_5=0;
		}
		else
		{	p1_2=1;

		}
		if (l>setnumber+1)
		p1_5=1;

    	if (l>10)
	l=0;
    l++;
	}
	}
		if(t==100)
		{		if(phzt!=1&&phzt!=3)
					tk++;
				else 
				{	dd++;
					p3_4=!p3_4;
				}
				if(dd==50)
				{	if(phzt==1)
						phzt=2;
					if(phzt==3)
						phzt=4;	 //����ƽ��,׼�������ΰ�
					p3_4=1;
					tk=0;	//���¼�ʱ
					dd=0;
				}

                if (tk>=10000)
                	tk=0;
                t=tk;
                sec4=(t/1000)+0x30;
                t=t%1000;
                sec3=(t/100)+0x30;
                t=t%100;
                sec2=(t/10)+0x30;
                t=t%10;
                sec1=t+0x30;
                t=0;
		}
	if(r==1000)
		{		r=0;
				v=n*c;
				v=v*10;
                vt=v;
				if(vt>=10000)
				vt=9999;
                sd4=(vt/1000)+0x30;
                vt=vt%1000;
                sd3=(vt/100)+0x30;
                vt=vt%100;
                sd2=(vt/10)+0x30;
                vt=vt%10;
                sd1=vt+0x30;
 			    n=0;
		}

}


timer1() interrupt 3 using 2
 {                   n++;
				s=s+c;
				if(s>=10000)
				s=0;
                st=s;
                lc4=(st/1000)+0x30;
                st=st%1000;
                lc3=(st/100)+0x30;
                st=st%100;
                lc2=(st/10)+0x30;
                st=st%10;
                lc1=st+0x30;
}

main()
{	int cs=0/*���ΰ巭�Ĵ���*/,fdd=6/*������ϵ��*/;
    int fx=0/*ǰһ���˶����򣬱�־*/,ph=0/*ƽ��ĳ־ö�*/;
    int fy=0/*��Ӧʱ��*/,wt=0/*�Ƿ�΢������־*/,cf=1/*����־*/,cfsj=40,bc=0/*�Ƿ񲹳�,��־*/;
	long qj=10/*ǰ������*/,ht=1/*���˲���*/,tz=0/*ƽ��״̬����*/,bf=400/*����*/;
	yx=0;/*�Ƿ�����,��־λ*/
	initlcm();
	sec1=48;
	sec2=48;
	sec3=48;
	sec4=48;
	lc1=48;
	lc2=48;
	lc3=48;
	lc4=48;
	TMOD=0x61;
	TH0=0xFC;
	TL0=0x18;
	TL1=-1;
	TH1=-1;
	IP=0x08;
	EA=0;ET0=1;ET1=1;
	TR0=1;TR1=1;
	yx=1;
	for(;;)
	{	xianshi();
		z=p3_2;
	    if(z==0)
		   {
		   		yx++;
				yx=(yx%2);
				EA=yx;
				TC=yx;
				p1_2=!yx;
				p1_5=!yx;
				delay1ms(1000);
		   }
		 if(p3_6==0)
		 {		delay1ms(1000);
		 			sec1=48;
					sec2=48;
					sec3=48;
					sec4=48;
					lc1=48;
					lc2=48;
					lc3=48;
					lc4=48;
					sd1=48;
					sd2=48;
					sd3=48;
					sd4=48;
					TH0=0xFC;
					TL0=0x18;
					TL1=-1;
					TH1=-1;
					yx=0;
					tk=0;
					s=0;
					EA=0;
					TC=1;
		 }
		   if(yx==1)
			{

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
 if((p2_5==0||phzt==2)&&phzt!=4)//������
		{	fy++;//��Ӧ�����Լ�һ
			qj++;//ǰ����������������
			TC=1;//�򿪵��
			if(cf||phzt==2)
			{	//���ڳ��,��Ӧ��������20,�Զ�����ǰ������
				qj=fdd*ht;
				bc=1;
               	if(cf&&tk>=35)
                   	setnumber=5;
				if(cf&&tk>=45)
					setnumber=4;
				if(phzt==2)
				if(p2_0&&p2_1&&p2_2)
					phzt=3;
                         }
			if(!cf&&fy>=200)//��û�г��,��Ӧ������200��,�Զ���ǰ�����˲�����һ,���Ҵ�΢��
			{fy=0;
			qj=1;
			ht=1;
			wt=1;
			}

			if(qj>=fdd*ht)//��ƽ����������
			{	ph=0;//�����,ƽ��̬ϵ������
				fx=2;//����Ϊǰ


				for(i=0;i<=(bf)/*��������*/;i++)
				{

//////////////////////////////////ǰ��ѭ�������///////////////////////////
				if(p2_0==1)
					{	if(p2_0==1&&p2_1==1)
		 				{	rightrun();
						}
						else if(p2_0==1&&p2_1==0&&p2_2==0)
						{	rightrun();
						}
					}
					else if(p2_2==1)
					{	if(p2_2==1&&p2_1==1)
		 				{	leftrun();
						}
						else if(p2_0==0&&p2_1==0&&p2_2==1)
						{	leftrun();
						}
					}
    				else if(p2_1==1)
						forwordrun();
					else forwordrun();

/////////////////////////////////////////////////////////

					if((cs%2)&&(cs<=10))//���ΰ��ƽ������
					{	cs++;
					}
					 qj=1;//ǰ��������һ
					if (cf||bc==2)
					{ 	ht=30;
						if(bc==2)
				 		bc=4;
					}//���ڳ��,���˲�����Ϊ30
					else if(tz<=100||(ht<2))//��û�г��,��û�н���ƽ��״̬����˲���<2ʱ,�ͽ�����һ,�������2
						ht=1;
					else ht=6;

					}
			}
			TC=0;
	    	p1_2=1;
		    p1_5=1;//ʹ���ֹͣ
			}

/////////////////////////////////////////////////////////////////////////////
if(phzt==0)
	if(p2_4==0||(p2_3&&p2_5))//ƽ����
		{   TC=1;	//׼���������
			if(cf)
				cfsj=cfsj+3;
			tz++;	//ֹͣ״̬�����ۼ�
			fy=0;	//���ܽ���ƽ��̬,������ת��,��Ӧ��������

			if(cf==0&&tz>=100)//���޳���ź�,ƽ��״̬>100,���ΰ巭�������õ����
			cs=8;             //ֱ�ӽ���΢��
			if(cf==0&&tz>=5)//���޳���,����Խ���ƽ��״̬
			{/*	if(bc==1)
				if(fx==2)
				{	for(i=0;i<=((bf-100)/2);i++)
					forwordrun();
					for(i=0;i<=((bf-50)/2);i++)
					backrun();
					bc=0;
				}
				else if(fx==1)
				{	for(i=0;i<=((bf-50)/2);i++)
					backrun();
					for(i=0;i<=((bf-100)/2);i++)
					forwordrun();
					bc=0;
				}*/
				if(tz>=300)
				tz=300;  //ֹ̬ͣ��־�õ����
				ph++;   //ƽ��̬ϵ���Լ�1
				if(ph>=15)
				{	
				qj=10;ht=10;
				}

				if(ph>=24&&setnumber>4)//��ƽ��̬ϵ��>=10,��ǰ��������ϵ����ֵ��Ϊ10,ʹ���ж�ʱ��ӳ�
				{tz=tz+10;
				ph=0;
				setnumber--;}
				if(ph>=500)//��ƽ��ϵ��>=500,�Ѿ�ƽ��.�ر��ж�,��ʾ��ǰʱ��
				{phzt=1;
				p3_4=0;
				setnumber=4;
				}
				if(ph>=25&&fdd<=25)
			{	//����������,������,�ӳ��ж�ʱ��
				fdd=fdd+1;
			}
			}//�������������
			TC=0;//�رյ��
			fx=0;//����Ϊ��
	    	p1_2=1;//�رյ��ʹ��
		    p1_5=1;//�رյ��ʹ��

		}




		 if((p2_3==0||phzt==4)&&phzt!=2)//ǰ����

		{	if(cf)
				cfsj=cfsj+4;
				fy++;	//��Ӧϵ���Լ�һ
			if(phzt==4)
				ht=fdd*qj;	/****************����Ҫ�ӳ���********************/////////////////////////////////////////
			if(fy>=20&&cf)//���ڳ��,�ȴ���Ӧϵ����100ʱ,�Զ��˳����״̬
			{	cf=0;
				fy=0;
				setnumber++;   //ͨ��PWM����ǰ�����ٶ�
			}
/*			else if(!cf&&bc==1)//��û�г��,��������ϵ��<=15ʱ,ֱ��ʹ���˲��������������
			{	ht=fdd*qj;
				bc=2;
			}*/
			ht++;		//���ˣ�ǰ�㣩�����Լ�һ��
			TC=1;		//�򿪵���
			if(fy>=200)//����Ӧ������200���Զ���ǰ�����˲���,�Զ���һ
			{fy=0;
			qj=1;
			ht=1;}
			if(fy>=333&&tz<=4)
			tz=0;
			if(ht>=fdd*qj&&(tz>=10)&&cf==0)//��ƽ����������
			{	ph=0;//�������,ƽ��̬ϵ��,�Զ�����
				fx=1;//����Ϊ��
				for(i=0;i<=bf*(bc+1);i++)
				{
//////////////////////////////////////////////////////////////////////����ѭ�������

/*						if(p2_0==1)
					{	if(p2_0==1&&p2_1==1)
		 				{	brightrun();
						}
						else if(p2_0==1&&p2_1==0&&p2_2==0)
						{	brightrun();
						}
					}
					else if(p2_2==1)
					{	if(p2_2==1&&p2_1==1)
		 				{	bleftrun();
						}
						else if(p2_0==0&&p2_1==0&&p2_2==1)
						{	bleftrun();
						}
					}
    				else if(p2_1==1)
			        	backrun();
					else */backrun();
/////////////////////////////////////////////////////////////////////////////

					if(wt==1||cs>=3)//����΢�������ΰ巭�������ﵽ3�������С����
					{
						if(bf!=200)
 					bf=200;//�ٴε�С����

						if(!(cs%2)&&(cs<=10))//��¼��������
						{	cs++;

						}
					}
				}
		 ht=1;//�������,���˲�����һ
			if(tz<=100||(qj<2))//û�н����ƽ��̬ʱ��ǰ������С��2ʱ,ǰ��������һ,����,����2
			qj=1;
			else qj=6;
			if(bc>=1)
			{	bc=0;
				fdd=fdd+2;
				ht=100;
				qj=100;
				setnumber=6;
				bf=300;
			}
			}//��������ǰ�����˲���
			TC=0;
	    	p1_2=1;
		    p1_5=1;//ʹ���ֹͣ
     	}


	}
	}
}
