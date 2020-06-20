#ifndef __lcd12864_h__
#define __lcd12864_h__

sbit psb_12864=P2^2;
sbit res_12864=P2^4;
sbit rs_12864=P2^5;
sbit rw_12864=P2^6;
sbit e_12864=P2^7;

void writecom_12864(uchar com)  //д����
{
	rw_12864=0;
	rs_12864=0;
	delay_us(1);
	P0=convert(com);
	e_12864=1;
	delay_us(1);
	e_12864=0;
}

void writebyte_12864(uchar dat)  //д����
{
	rw_12864=0;
	rs_12864=1;
	delay_us(1);
	P0=convert(dat);
	e_12864=1;
	delay_us(1);
	e_12864=0;
}

void initddrom_12864()    //LCD�ֿ��ʼ������
{
	delay_us(20);           
	psb_12864=1;              //����Ϊ8BIT���ڹ���ģʽ
	delay_us(1);          
	res_12864=0;              //��λ
	delay_us(1);           
	res_12864=1;              //��λ�ø�
	delay_us(5);	
	writecom_12864(0x30);   //����ָ�
	delay_ms(1);
	writecom_12864(0x02);   // ��ַ��λ
	delay_ms(1);
	writecom_12864(0x0c);   //������ʾ��,�α�ر�
	delay_ms(1);
	writecom_12864(0x01);   //�����ʾ
	delay_ms(1);
	writecom_12864(0x06);   //�α�����
	delay_ms(1);
}

void initgdrom_12864()   //LCD��ʾͼƬ(��չ)��ʼ������
{
	delay_us(20);            
	psb_12864=1;              //����Ϊ8BIT���ڹ���ģʽ
	delay_us(1);      
	res_12864=0;              //��λ
	delay_us(1);       
	res_12864=1;              //��λ�ø�
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

void disword_12864(uchar add,uchar *p) //ĳ��ַ��ʼ������ʾ�ַ�
{				 //�����ֺ��ַ����ʱӦע���������ַ���ӦΪż��
    writecom_12864(add);  
	while(1)
	{
		if(*p=='\0')return;
  		writebyte_12864(*p);
   		p++;
		//delay_ms(1);ʹ��ʾ������
	}
}

void disfullword_12864(uchar *add) //��ʾ�����ַ�
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

void disfullphoto_12864(uchar *add)//��ʾ����ͼ��
{
	uchar i,j;
	//��ʾ�ϰ�������
	for(i=0;i<32;i++)              
	{ 
		writecom_12864(0x80+i); 	//��ֱ��ַ 
		writecom_12864(0x80);       //ˮƽ��ַ
		for(j=0;j<16;j++)
		{
			writebyte_12864(*add);
			add++;
		}
	}	   
	//��ʾ�°�������
	for(i=0;i<32;i++)             
	{
		writecom_12864(0x80+i); 	//��ֱ��ַ
		writecom_12864(0x88);       //ˮƽ��ַ
		for(j=0;j<16;j++)
		{
			writebyte_12864(*add);
			add++;
		}
	}
}


/*  ָ��ο���ע�����ֻ���ָ�����չָ��

    writecom_12864(0x08);    //������ʾ�أ��α�أ��α�λ�ù�
    writecom_12864(0x0c);    //������ʾ�����α�أ��α�λ�ù�
    writecom_12864(0x0e);    //������ʾ�����α꿪���α�λ�ù�
    writecom_12864(0x0f);    //������ʾ�����α꿪���α�λ�ÿ�

    writecom_12864(0x02);    //λַ��λ���α�ص�ԭ��    
    writecom_12864(0x84);    //��DDRAM��ַ��Ϊ0x84���α��ڴ���˸

    //�����ĸ������ǿ���д���ַ��Ժ��꼰������ʾ���ƶ�  
    writecom_12864(0x07);    //�������������ʾ����   
    writecom_12864(0x05);    //�������������ʾ����   
    writecom_12864(0x06);    //�������������ʾ���ƶ�
    writecom_12864(0x04);    //�������������ʾ���ƶ�

    //�����ĸ���������д����ʾ���ݣ�ֱ�ӿ��ƹ���������ʾ���ƶ�  
    writecom_12864(0x10);    //�������
    writecom_12864(0x14);    //�������
    writecom_12864(0x18);    //������ʾ���ƣ�������  
    writecom_12864(0x1c);    //������ʾ���ƣ������� 
    
    //������չ����ģʽ����
    writecom_12864(0x34);    //����չ����ģʽ����ͼ��ʾ�ر�

    writecom_12864(0x04);    //ͬʱ����1��3��
    writecom_12864(0x04);    //�ٴη���1��3�У��൱�ڹر�1��3�з���
    writecom_12864(0x05);    //ͬʱ����2��4��
    writecom_12864(0x05);    //�ٴη���2��4�У��൱�ڹر�2��4�з���
 
    writecom_12864(0x08);    //����˯��ģʽ
    writecom_12864(0x0c);    //�˳�˯��ģʽ

    writecom_12864(0x01);    //�������ģʽ

    writecom_12864(0x36);    //����չ����ģʽ���򿪻�ͼ��ʾ
    writecom_12864(0x34);    //����չ����ģʽ���رջ�ͼ��ʾ

    writecom_12864(0x30);    //�ص�����ָ�

    writecom_12864(0x01);    //����   
}*/

#endif