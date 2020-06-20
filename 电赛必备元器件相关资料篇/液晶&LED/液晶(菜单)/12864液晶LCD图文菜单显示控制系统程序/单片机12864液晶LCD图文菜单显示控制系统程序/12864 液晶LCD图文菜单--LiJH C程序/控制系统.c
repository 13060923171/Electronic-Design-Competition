/**************************************************************************************
 �������ƣ�
           ʱ�� �¶ȿ���ϵͳ��128*64LCD+ds1302+ds18b20��
 ��Ҫ���ܣ�
           �¶ȼ� ʱ�� ���� ������ ������ ������ �¶ȿ��� ������ ϵͳ���õȡ�
 ��Ҫ˵����
           ����ȫ��������һ��C�ļ��С�
 ���ߣ�    
           LiJH
 ʱ�䣺   
           2008.6.20
 �汾��   
           LiJH-V1.0
**************************************************************************************/
#include    <AT89S53.h>
#include    <intrins.h>		
#define     uchar        unsigned char
#define     uint         unsigned int
#define     time_count   0x3cb0
#define     close        1
#define     open         0

/*============ ״̬���Ƽ��¸������߶��� ==============*/ 
 
#define     b20_io       P3_4		// DS18B20����	  
#define     pw_oc        P3_7		// ��������������(�Ƶ�)
#define     buzzer       P3_3 		// ������
#define     lcd_bl       P0_6		// ������ƿ�
#define     als_th       P0_4		// �����¶Ⱦ���(���)
#define     als_tl       P3_6		// �����¶Ⱦ���(�̵�)
#define	    keyboard     P1			// �������ݿ�


/*================= ST7920LCD ���� ===================*/

#define     comm         0			// lcdָ��
#define     dat          1			// lcd����
#define     lcd_rst      P0_7		// lcd��λ
#define     lcd_e        P3_0		// lcdʹ��
#define     lcd_rw       P3_1		// lcd��д
#define     lcd_rs       P3_2		// lcd��������
#define     busy         P2_7       // æµλ
#define     lcd_data     P2			// lcd(8bit)���ݴ��� 

  
/*=============== DS1302 �Ĵ�����ַ���� ===============*/

#define		d02_rst      P0_1		// DS1302ʹ��
#define		d02_data     P0_2		// DS1302������
#define		d02_sclk     P0_3		// DS1302����
#define     d02_wrc      0x8e		// 1302��дRAM����(1���� 0��ֹ)
#define     d02_year	 0x8c		// ��
#define     d02_weeks	 0x8a		// ����
#define     d02_month	 0x88		// ��
#define     d02_day		 0x86		// ��
#define     d02_hour	 0x84		// ʱ
#define     d02_minute	 0x82		// ��
#define     d02_second	 0x80		// ��
#define     d02_signs	 0xc0		// ϵͳ״̬(��¼���ñ��)
#define     d02_passw	 0xc2		// ����Ĵ�����λ����3λ��λ��ַ��2����ʼֵΪ��741456��
#define     d02_alm		 0xc8		// ����ʱ��Сʱλ����λΪ����λ��2��
#define     d02_temp	 0xcc		// �¶����޾���������λ��λλ���޼�2��
#define     d02_initsi   0xd4		// 1302��ʼ����ǣ�Ϊ0xb4ʱ��˵���Ѿ���ʼ����
#define     ret_ds1302()    d02_rst=0;d02_sclk=0;d02_rst=1	// 1302��д��λ
#define     end_ds1302()    d02_sclk=1;d02_rst=0			// 1302��д����



/*================================= ȫ�ֱ������� ==================================*/


typedef	struct			// ����ʱ������ 
{
  uchar	hour;
  uchar	minute;
  uchar	second;
}time;

typedef	struct			// ������������ 
{
  uchar	year;
  uchar	month;
  uchar	day;
  uchar	week;
}date;

typedef union 		    // �����¶�����
{
  int temp_data;
  uchar t_data[2];
}trdata;

trdata  bdata  temp;             // �¶Ⱦ���ֵ�ݴ�	
date    idata  today={'8',1,1,1};	 // �����ݴ�
time    idata  now={0x12,0,0} , save_time={0x12,0,0}; 
              // ʱ���ݴ�(now�Ƕ���ʱ���ݴ� save_time��һʱ���ݴ�)
uchar   idata  time_tick=20,time_tick1=20,alarm_time[2],als_temp[2], 
              // T0�ж���,     T1�ж���,     ����ʱ��,   �����¶�
               bl_delay=15,temp_delay=5,opera[2][12],resu[12], 
			  // ����ʱ��,  �¶ȸ���ʱ��, ���������ݼĴ�, ����Ĵ�
			   i,j,save1,temp_min=0; 
			   // ������i/j�±꣬�м��ݴ�,�¶ȸ��£��֣�



uchar  bdata   signs=0;		 // ϵͳ��ǼĴ���
  sbit bl1=signs^7; 		 // bl1,bl0����ѡ��λ(00 ��ʱ,01 ����,10 �ر�) 
  sbit bl0=signs^6;			
  sbit di1=signs^5;			 // di1,di0��ʾģʽ(00ģʽ1,01ģʽ2,10ģʽ3)
  sbit di0=signs^4;			
  sbit meu=signs^3;			 // �˵���ʾ��ʽ(0 ͼƬ,1 ����)
  sbit keb=signs^2;			 // ������(0 ����,1����)
  sbit alm=signs^1;			 // ��������λ(0 �ر�,1����)
  sbit als=signs^0;			 // ��������λ(0 ����,1�ر�)



uint   bdata   run_signs=0;     // ���б�ǼĴ���
  sbit ds18b20=run_signs^0;		// �¸������ڱ��(0�����ڣ�1����)
  sbit temp_pn=run_signs^1;		// �¶������ű��(0����1��)
  sbit temprun=run_signs^2;		// �¸������б��(0��1����)
  sbit nba_tim=run_signs^3;   	// �����ʱ�����(0�رգ�1����)
  sbit adju_si=run_signs^4;		// ���ָ����������(1����)
  sbit cal_del=run_signs^5;     // ��������ʾ���ǣ�0û����ʾ��
  sbit cal_run=run_signs^6;	    // ������ϱ�ǣ�1��Ч��
  sbit oth_run=run_signs^7;     // �������
  sbit alm_run=run_signs^8;     // ����ʱ�䵽���ǣ�1��Ч��
  sbit dis_sig=run_signs^9;     // ��ʾʱ���ǣ�1��Ч��
  sbit lock_op=run_signs^10;    // ����ʱ�䵽��ǣ�1��Ч��
  sbit b20time=run_signs^11;    // �¸������¶��¶ȱ��(1��Ч)
  sbit t0_isr =run_signs^12;    // TO�жϱ�ǣ�1��Ч(1s)
  sbit thl_als=run_signs^13;	// �¶Ⱦ������
  sbit init_d2=run_signs^14;	// ģʽ2��ʼ��ʾ
  sbit buzstop=run_signs^15;	// ������ֹͣ���




/*================================= ������ʾ���� ==================================*/ 

uchar code weeks[7][2]={"һ","��","��","��","��","��","��"}; // ����				//ͼƬ����
uchar code menu_cn[6][8]={"��    ��","��    ��","��    ��","��    ��","��    ��","��    ��"};
uchar code menu_cn_2[6][6]={"��  ��","��  ��","��  ��","��  ��","��  ��","��  ��"};// �˵�����
uchar code selected[2][16]={{"���롾      ��\x04 "},{"���á�    ��\x10 \x04 "}}; // ѡ��
uchar code secrets_menu[4][6]={"��  ��","��  ��","��ʼ��","��  ��"};           // ���ܲ˵�����
uchar code tool_menu[5][6]={"������","�¶ȼ�","��  ��","������","��  ��"};	   // ���߲˵�����
uchar code char_num[10]={"0123456789"};	 // ������ʾ����							 // ÿ������
uchar code dayofmonth[12]={0x31,0x28,0x31,0x30,0x31,0x30,0x31,0x31,0x30,0x31,0x30,0x31}; 
uchar code key_code[]={0xD7,0x7E,0x7D,0x7B,0xBE,0xBD,0xBB,0xDE,0xDD,0xDB,0xB7,0x77,0xEE,0xED,0xEB,0xE7};
uchar code ksp[4]={0x7F,0xBF,0xDF,0xEF};	 // ����ɨ�����
uchar code tool_cal[4][2]={"+ ","- ","* ","/ "}; // �������
uchar code alm_sign[2][2]={"��","��"};			 // ���忪��
uchar code set_bl_dm[2][3][4]={{"30��","����","�ر�"},{"ģ01","ģ02","ģ03"}};
uchar code set_mka[3][2][4]={{"ͼƬ","����"},{"����","����"},{"����","�ر�"}};
uchar code nba_oppo[8][2]={"��","��","��","��","��","��","��","��"};
uchar code set_menu[6][4]={"����","��ʾ","�˵�","����","����","�˳�"};


uchar code version[10][14]={   // �汾��Ϣ
                            {"��Զְҵ����ѧ"},
							{"  ��Ϣ�Ƽ�ѧԺ"},
					        {"08���ҵ���  "},
					        {"  ��05����02��"},
			                {"���ߣ������  "},
					        {"ָ����������  "},
					        {"�汾:LIJH-V1.0"},
					        {"    2008/06/19"},
					        {"лл���ʹ�ã�"},
					        {"         (ok)\x10"}};


uchar code timer_tips[12][16]={ // ������ʹ��˵��
                            {"ʹ�÷�����      "},
							{"  (+)(-)��(*)(/)"},
							{"�ֱ��������ķ���"},
							{"�����,(7)������"},
							{"һ��,(8)��24�븴"},
							{"λ,(9)����ͣ�뿪"},
							{"ʼ����ͣ״̬�£�"},
							{"������(OK)���ɷ�"},
							{"��ʱ��ģʽ-2����"},
							{"��(OK)�����س���"},
							{"����,(./S)������"},
							{"�˳�����.      \x1f"}};



/*================================= ������������ ==================================*/ 

 
typedef struct			// �������˵��������� 
{
  uchar shaft_num;
  uchar left_num;
  uchar right_num;
}menu_index;

typedef struct			// ���嵼���������� 
{
  uchar lnum;
  uchar rnum;
}index;

menu_index code index_m[6]={{3,5,1},{4,0,2},{5,1,3},{0,2,4},{1,3,5},{2,4,0}}; // �˵�����
index code index_t[6]={{4,1},{0,2},{1,3},{2,4},{3,0}};       // ���ߵ���
index code index_p[5]={{3,1},{0,2},{1,3},{2,0}};             // ���ܵ���
index code index_s[6]={{5,1},{0,2},{1,3},{2,4},{3,5},{4,0}}; // ���õ���
index code index_a[7]={{0x81,0x91},{0x83,0x91},{0x85,0x91},{0x89,0x81},
		                {0x8b,0x81},{0x8d,0x81},{0x8f,0x91}};       // �����µ�����
index code index_temp_set[3]={{0x8b,0x81},{0x8b,0x91},{0x8f,0x91}}; // �¶��趨�µ�����
index code index_alm[3]={{0x8c,0x81},{0x8e,0x81},{0x8f,0x91}};	    // �����������	
index code index_cal[3]={{0x86,0x90},{0x8e,0x80},{0x8e,0x90}};	    // ��������������
uchar code index_nba[4]={0x93,0x97,0x8b,0x8f};					    // ���������õ���





/*========================== �˵�ͼƬ�����x�߶�=32*24��=============================*/

uchar code menu_pic[6][96]={ 
	{// ���� 
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x00,0x00,0x02,
	 0x40,0x00,0x00,0x82,0x47,0xFF,0xE1,0xC2,0x47,0xFF,0xF3,0xE2,0x47,0x38,0x68,0x02,
	 0x46,0xDF,0x68,0x82,0x46,0xDF,0x69,0xC2,0x46,0xDE,0xE8,0x82,0x46,0xDD,0xE8,0x02,
	 0x46,0xDD,0xE8,0x02,0x47,0x3D,0xE8,0x02,0x47,0xFF,0xE8,0x02,0x47,0xFF,0xE9,0xC2,
	 0x42,0x00,0x18,0x02,0x41,0xFF,0xF3,0xE2,0x40,0x00,0x01,0xC2,0x40,0x00,0x00,0x82,
	 0x40,0x00,0x00,0x02,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// ����					
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x00,0x03,0x82,
	 0x41,0xF0,0x07,0xC2,0x43,0xFF,0xCF,0xE2,0x47,0x3F,0xFE,0x02,0x47,0x3F,0xFE,0x02,
	 0x43,0xFF,0xCF,0xE2,0x41,0xF0,0x0F,0xC2,0x40,0x00,0x03,0x82,0x40,0x00,0x00,0x02,
	 0x40,0x00,0x03,0xC2,0x40,0x00,0x03,0xC2,0x41,0x80,0x01,0x82,0x43,0xFF,0xF3,0xC2,
	 0x47,0xFF,0xFF,0xE2,0x47,0xFF,0xFF,0xE2,0x43,0xFF,0xF3,0xC2,0x41,0x80,0x01,0x82,
	 0x40,0x00,0x01,0x82,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// ����
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x43,0x18,0x00,0x02,
	 0x44,0xA4,0x0E,0x02,0x44,0xA4,0x1F,0x02,0x43,0x24,0x3F,0x02,0x40,0x00,0x7F,0x02,
	 0x40,0xF0,0xFC,0x02,0x43,0xF9,0xF0,0x02,0x43,0x03,0xE0,0x02,0x46,0x77,0x80,0x02,
	 0x46,0x7F,0x00,0x02,0x46,0x7E,0x00,0x02,0x46,0x7C,0x00,0x02,0x47,0x3D,0x00,0x02,
	 0x43,0x1D,0x81,0x22,0x43,0x81,0xB2,0x42,0x41,0xC3,0x4F,0xE2,0x40,0xFE,0x4A,0x42,
	 0x40,0x3C,0x32,0x42,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// ����
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0xFC,0x03,0x02,
	 0x41,0xFE,0x1F,0x02,0x43,0x87,0x03,0x82,0x43,0x03,0x03,0x02,0x43,0x03,0x03,0x02,
	 0x43,0x03,0x0F,0x02,0x43,0x03,0x03,0x02,0x43,0x03,0x03,0x02,0x47,0xFF,0x83,0x02,
	 0x47,0xFF,0x83,0x02,0x47,0xCF,0x83,0x02,0x47,0x87,0x83,0x02,0x47,0xCF,0x83,0x02,
	 0x47,0xCF,0x87,0x82,0x47,0xCF,0x8F,0xC2,0x47,0xCF,0x9C,0xE2,0x47,0xFF,0x8F,0xC2,
	 0x43,0xFF,0x07,0x82,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// �汾
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x00,0x00,0x02,
	 0x40,0x0F,0x00,0x62,0x40,0x1D,0x80,0xF2,0x40,0x38,0x41,0xF2,0x40,0x70,0x23,0xE2,
	 0x40,0xE2,0x17,0xC2,0x41,0xE4,0x4F,0x82,0x41,0xF0,0x9F,0x02,0x41,0x59,0x16,0x02,
	 0x42,0x4C,0x1D,0x02,0x42,0x46,0x20,0x82,0x42,0x47,0x00,0x42,0x44,0x43,0x80,0xC2,
	 0x44,0x44,0xC1,0xC2,0x44,0xC8,0x63,0x82,0x45,0x50,0x37,0x02,0x46,0x60,0x1E,0x02,
	 0x40,0x40,0x0C,0x02,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00},
	
	{// �˳�
	 0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x40,0x40,0xFF,0xE2,
	 0x40,0x60,0x80,0x62,0x43,0xF0,0x80,0xA2,0x43,0xF0,0x81,0x22,0x40,0x60,0x82,0x22,
	 0x40,0x40,0x84,0x22,0x40,0x00,0x88,0x22,0x40,0x00,0x88,0x22,0x47,0xFF,0xF8,0x22,
	 0x47,0xFF,0xF8,0x22,0x46,0x2A,0x2A,0x22,0x46,0xE3,0x78,0x22,0x46,0x77,0x78,0x22,
	 0x46,0xE3,0x78,0x62,0x46,0x2A,0x38,0x82,0x47,0xFF,0xF9,0x02,0x47,0xFF,0xFA,0x02,
	 0x40,0x00,0x0C,0x02,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,}};




/*================ ��ʾģʽ2���ּ��¶ȷ���ͼƬ�����x�߶�=16*32��=====================*/

uchar code mode_2num[11][64]={
	{// ͼƬ����0
	 0x00,0x00,0x3F,0xFC,0x5F,0xFA,0x6F,0xF6,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x40,0x02,0x00,0x00,
	 0x40,0x02,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x6F,0xF6,0x5F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
	
	{// ͼƬ����1
	 0x00,0x00,0x00,0x20,0x00,0x60,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,
     0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0x60,0x00,0x20,0x00,0x00,0x00,0x20,
	 0x00,0x60,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,
	 0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0x60,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00},
    
	{// ͼƬ����2
	 0x00,0x00,0x3F,0xFC,0x1F,0xFA,0x0F,0xF6,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x1F,0xFA,0x3F,0xFC,
	 0x5F,0xF8,0x60,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,
	 0x70,0x00,0x70,0x00,0x70,0x00,0x6F,0xF8,0x5F,0xFC,0x3F,0xFE,0x00,0x00,0x00,0x00},
	
	{// ͼƬ����3
	 0x00,0x00,0x7F,0xFC,0x3F,0xFA,0x1F,0xF6,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
     0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x1F,0xFA,0x3F,0xFC,
     0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
     0x00,0x0E,0x00,0x0E,0x00,0x0E,0x1F,0xF6,0x3F,0xFA,0x7F,0xFC,0x00,0x00,0x00,0x00},
	
	{// ͼƬ����4
	 0x00,0x00,0x40,0x02,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
 	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x5F,0xFA,0x3F,0xFC,
	 0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x02,0x00,0x00,0x00,0x00},
	
	{// ͼƬ����5
	 0x00,0x00,0x3F,0xFC,0x5F,0xF8,0x6F,0xF0,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,
	 0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x60,0x00,0x5F,0xF8,0x3F,0xFC,
	 0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x0F,0xF6,0x1F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
 	
	{// ͼƬ����6
	 0x00,0x00,0x3F,0xFC,0x5F,0xF8,0x6F,0xF0,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,
	 0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x60,0x00,0x5F,0xF8,0x3F,0xFC,
	 0x5F,0xFA,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x6F,0xF6,0x5F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
	
	{// ͼƬ����7
	 0x00,0x00,0x7F,0xFC,0x3F,0xFA,0x1F,0xF6,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x02,0x00,0x00,
	 0x00,0x02,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x02,0x00,0x00,0x00,0x00},
	
	{// ͼƬ����8
	 0x00,0x00,0x3F,0xFC,0x5F,0xFA,0x6F,0xF6,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x5F,0xFA,0x3F,0xFC,
	 0x5F,0xFA,0x60,0x06,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x6F,0xF6,0x5F,0xFA,0x3F,0xFC,0x00,0x00,0x00,0x00},
	
	{// ͼƬ����9
	 0x00,0x00,0x3F,0xFC,0x5F,0xFA,0x6F,0xF6,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,
	 0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x0E,0x60,0x06,0x5F,0xFA,0x3F,0xFC,
	 0x1F,0xFA,0x00,0x06,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,
	 0x00,0x0E,0x00,0x0E,0x00,0x0E,0x1F,0xF6,0x3F,0xFA,0x7F,0xFC,0x00,0x00,0x00,0x00},
	
	{// �¶ȷ���
     0x00,0x00,0x00,0x00,0x70,0xE0,0x53,0xF0,0x77,0xFA,0x0E,0x0E,0x0C,0x06,0x18,0x06,
     0x18,0x02,0x38,0x02,0x30,0x00,0x30,0x00,0x70,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
     0x60,0x00,0x60,0x00,0x70,0x00,0x30,0x00,0x30,0x00,0x38,0x00,0x18,0x02,0x18,0x02,
     0x0C,0x06,0x0E,0x06,0x07,0x0E,0x03,0xFA,0x00,0xF0,0x00,0x60,0x00,0x00,0x00,0x00}};





/*===================== ��ʾģʽ3����ͼƬ�����x�߶�=24*32��=========================*/

uchar code mode_3num[10][96]={
	{// ͼƬ����0
	 0x00,0x7E,0x00,0x01,0xE7,0x80,0x03,0xC3,0xC0,0x07,0x81,0xE0,0x0F,0x80,0xF0,0x0F,
	 0x00,0xF0,0x1F,0x00,0xF8,0x1E,0x00,0xF8,0x3E,0x00,0x78,0x3E,0x00,0x7C,0x3E,0x00,
	 0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,
	 0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,
	 0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x78,0x1E,0x00,0xF8,0x1F,0x00,0xF8,0x0F,0x00,
	 0xF0,0x0F,0x81,0xF0,0x07,0x81,0xE0,0x03,0xC3,0xC0,0x01,0xE7,0x80,0x00,0x7E,0x00},

	{// ͼƬ����1
	 0x00,0x0C,0x00,0x00,0x1C,0x00,0x00,0x7C,0x00,0x07,0xFC,0x00,0x00,0x3C,0x00,0x00,
	 0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,
	 0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,
	 0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,
	 0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,
	 0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3E,0x00,0x00,0x7F,0x00,0x07,0xFF,0xF0},
			   
	{// ͼƬ����2
	 0x00,0xFF,0x00,0x03,0xC7,0xC0,0x07,0x01,0xE0,0x0E,0x00,0xF0,0x1E,0x00,0xF8,0x1E,
	 0x00,0xF8,0x3E,0x00,0x78,0x3E,0x00,0x78,0x3F,0x00,0x78,0x3F,0x00,0x78,0x1F,0x00,
     0xF8,0x00,0x00,0xF8,0x00,0x00,0xF0,0x00,0x01,0xF0,0x00,0x03,0xE0,0x00,0x03,0xC0,
	 0x00,0x07,0x80,0x00,0x0F,0x00,0x00,0x1E,0x00,0x00,0x3C,0x00,0x00,0x78,0x00,0x00,
	 0xF0,0x00,0x01,0xE0,0x00,0x03,0xC0,0x00,0x07,0x80,0x1C,0x07,0x00,0x1C,0x0E,0x00,
	 0x38,0x1C,0x00,0x38,0x3C,0x00,0x78,0x3F,0xFF,0xF8,0x3F,0xFF,0xF8,0x3F,0xFF,0xF8},

	{// ͼƬ����3
	 0x00,0xFE,0x00,0x07,0x87,0x80,0x0F,0x03,0xC0,0x1E,0x01,0xE0,0x1E,0x01,0xF0,0x1E,
	 0x01,0xF0,0x1F,0x00,0xF0,0x1F,0x00,0xF0,0x1E,0x00,0xF0,0x00,0x00,0xF0,0x00,0x01,
	 0xF0,0x00,0x01,0xF0,0x00,0x03,0xE0,0x00,0x03,0xC0,0x00,0x0F,0x00,0x00,0xFE,0x00,
	 0x00,0x07,0x80,0x00,0x01,0xE0,0x00,0x00,0xF0,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,
	 0x00,0x78,0x00,0x00,0x7C,0x1E,0x00,0x7C,0x3F,0x00,0x7C,0x3F,0x00,0x7C,0x3F,0x00,
	 0x78,0x3E,0x00,0xF8,0x1E,0x00,0xF0,0x0F,0x01,0xE0,0x07,0x87,0xC0,0x01,0xFF,0x00},

	{// ͼƬ����4
	 0x00,0x03,0xC0,0x00,0x03,0xC0,0x00,0x07,0xC0,0x00,0x0F,0xC0,0x00,0x0F,0xC0,0x00,
	 0x1F,0xC0,0x00,0x3F,0xC0,0x00,0x3F,0xC0,0x00,0x77,0xC0,0x00,0x77,0xC0,0x00,0xE7,
	 0xC0,0x01,0xC7,0xC0,0x01,0xC7,0xC0,0x03,0x87,0xC0,0x07,0x07,0xC0,0x07,0x07,0xC0,
	 0x0E,0x07,0xC0,0x1E,0x07,0xC0,0x1C,0x07,0xC0,0x38,0x07,0xC0,0x38,0x07,0xC0,0x7F,
	 0xFF,0xFE,0x7F,0xFF,0xFE,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,
	 0xC0,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,0xC0,0x00,0x07,0xE0,0x00,0x7F,0xFE},

	{// ͼƬ����5
	 0x0F,0xFF,0xF8,0x0F,0xFF,0xF8,0x0F,0xFF,0xF8,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,
	 0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,0x00,0x0E,0x00,
	 0x00,0x0E,0x7F,0x00,0x0D,0xFF,0xC0,0x0F,0xC3,0xE0,0x1F,0x01,0xF0,0x1E,0x00,0xF8,
	 0x1E,0x00,0xF8,0x00,0x00,0x78,0x00,0x00,0x7C,0x00,0x00,0x7C,0x00,0x00,0x7C,0x00,
	 0x00,0x7C,0x1E,0x00,0x7C,0x3F,0x00,0x7C,0x3F,0x00,0x78,0x3F,0x00,0x78,0x3E,0x00,
	 0xF8,0x1E,0x00,0xF0,0x1E,0x01,0xF0,0x0E,0x01,0xE0,0x07,0x87,0xC0,0x00,0xFF,0x00},

	{// ͼƬ����6
	 0x00,0x3F,0xC0,0x00,0xF1,0xE0,0x03,0xC1,0xF0,0x07,0x81,0xF8,0x07,0x01,0xF8,0x0F,
	 0x00,0xF0,0x1F,0x00,0x00,0x1E,0x00,0x00,0x1E,0x00,0x00,0x3E,0x00,0x00,0x3E,0x00,
	 0x00,0x3E,0x00,0x00,0x3E,0x3F,0x80,0x3E,0xFF,0xE0,0x3F,0xE3,0xF0,0x3F,0x80,0xF8,
	 0x3F,0x00,0xF8,0x3F,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x7C,0x3E,0x00,0x3C,0x3E,
	 0x00,0x3C,0x3E,0x00,0x3C,0x3E,0x00,0x3C,0x3E,0x00,0x7C,0x1E,0x00,0x7C,0x1F,0x00,
	 0x78,0x0F,0x00,0x78,0x0F,0x80,0xF0,0x07,0xC0,0xE0,0x03,0xE3,0xC0,0x00,0xFF,0x00},

	{// ͼƬ����7
	 0x1F,0xFF,0xFC,0x1F,0xFF,0xFC,0x1F,0xFF,0xF8,0x1F,0x00,0x38,0x1C,0x00,0x70,0x1C,
	 0x00,0xE0,0x38,0x00,0xE0,0x38,0x01,0xC0,0x00,0x01,0xC0,0x00,0x03,0x80,0x00,0x03,
	 0x80,0x00,0x07,0x80,0x00,0x07,0x00,0x00,0x0F,0x00,0x00,0x0E,0x00,0x00,0x1E,0x00,
	 0x00,0x1E,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x7C,0x00,0x00,
	 0x78,0x00,0x00,0x78,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,
	 0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0x78,0x00},

	{// ͼƬ����8
	 0x00,0xFF,0x00,0x07,0xC3,0xC0,0x0F,0x00,0xE0,0x1E,0x00,0xF0,0x1E,0x00,0x78,0x3C,
	 0x00,0x78,0x3C,0x00,0x78,0x3C,0x00,0x7C,0x3E,0x00,0x78,0x3E,0x00,0x78,0x1F,0x00,
	 0x78,0x1F,0x80,0xF0,0x0F,0xE1,0xE0,0x07,0xFB,0xC0,0x01,0xFF,0x80,0x01,0xFF,0x80,
	 0x07,0xBF,0xC0,0x0F,0x0F,0xE0,0x1E,0x03,0xF0,0x3E,0x01,0xF8,0x3C,0x00,0xF8,0x3C,
	 0x00,0x7C,0x7C,0x00,0x7C,0x78,0x00,0x3C,0x78,0x00,0x3C,0x7C,0x00,0x3C,0x3C,0x00,
	 0x78,0x3C,0x00,0x78,0x1E,0x00,0x70,0x0F,0x00,0xE0,0x07,0xC3,0xC0,0x01,0xFF,0x00},

	{// ͼƬ����9
	 0x01,0xFE,0x00,0x07,0xC7,0x80,0x0F,0x01,0xE0,0x1E,0x01,0xE0,0x1E,0x00,0xF0,0x3E,
	 0x00,0xF8,0x3C,0x00,0x78,0x3C,0x00,0x78,0x7C,0x00,0x7C,0x7C,0x00,0x7C,0x7C,0x00,
	 0x7C,0x7C,0x00,0x7C,0x7C,0x00,0x7C,0x3C,0x00,0xFC,0x3E,0x00,0xFC,0x3E,0x01,0xFC,
     0x1F,0x03,0xFC,0x1F,0x8F,0xFC,0x0F,0xFF,0x7C,0x03,0xFC,0x7C,0x00,0x00,0x7C,0x00,
     0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0xF0,0x00,0x01,0xF0,0x0F,0x01,
	 0xE0,0x1F,0x01,0xE0,0x1F,0x03,0xC0,0x1F,0x07,0x80,0x0F,0x9F,0x00,0x03,0xFC,0x00}};




/*========================== ��Ǽ���ͼƬ�����x�߶�=16*16��===========================*/


uchar code running[7][32]={
     {// �������16*16
	  0x00,0x00,0x00,0x39,0x00,0x7B,0x00,0xFE,0x01,0xDC,0x0F,0x98,0x0F,0x18,0x0C,0x1F,
      0x0C,0x1F,0x0F,0x18,0x0F,0x98,0x01,0xDC,0x00,0xFE,0x00,0x7B,0x00,0x39,0x00,0x00,},
	 {// ���忪�����16*16
	  0x00,0x00,0x00,0x40,0x00,0xE0,0x01,0xB0,0x01,0xB0,0x03,0x18,0x03,0x18,0x06,0x0C,
      0x0C,0x06,0x0C,0xE6,0x1B,0x1B,0x0C,0x06,0x06,0x0C,0x03,0xF8,0x00,0xE0,0x00,0x00},
	 {// ���������16*16
      0x00,0x00,0x01,0xF8,0x03,0xFC,0x03,0x0C,0x03,0x0C,0x03,0x0C,0x03,0xFC,0x06,0x96,
      0x0E,0xA7,0x0E,0xC7,0x0E,0xA7,0x0E,0x97,0x07,0xFE,0x03,0xFC,0x01,0xF8,0x00,0x00,},
	 {// �¶ȼƱ��16*16
	  0x00,0x00,0x1F,0xF8,0x1F,0xF8,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
      0x01,0x90,0x01,0x98,0x01,0x9C,0x01,0x9E,0x01,0x9C,0x00,0x18,0x00,0x10,0x00,0x00},
	 {// ģʽ3��16*16
	  0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
      0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	 {// ģʽ2��16*16
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
      0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	  
	 {// ���������б��16*16
	  0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x1F,0xF8,0x01,0x80,0x01,0x80,0x01,0x90,
      0x01,0x98,0x01,0x9C,0x01,0x9E,0x01,0x9C,0x01,0x98,0x01,0x90,0x00,0x00,0x00,0x00}};


/*========================== �˵���ťͼƬ�����x�߶�=16*8��============================*/

uchar code menu_key[]=
	 {//�˵���ť
	  0x7F,0xFE,0x81,0x81,0x83,0xC1,0x87,0xE1,0x8F,0xF1,0x9F,0xF9,0x80,0x01,0x7F,0xFE};





/*=====================================================================================
 �������ƣ���ʱ���жϴ������ʼ������ʱ
 ����������ʱ�Ӹ��¡���������ʱ
 ȫ�ֱ�����time_tick��time_tick1��resu[2]��resu[3]��resu[4]��temp_delay��temp_min
 ����˵������
 ����˵����t0_isr(�жϱ��)��b20time(�¶ȸ��±��)
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


/*��ʱ��һ��10���������ڲ��ȣ� ������ʽ����ʱ��*/
void delay(unsigned int det)
{
  while(det--);
}



/*1s��ʱ*/
void delay_pw()
{
  delay(60000);
  delay(60000);
}


/*6������  6us */
void  delay_b20(uchar n)
{  
  while(n--);
}



/*��ʱ���жϴ���*/
void time0_isr(void) interrupt  TF0_VECTOR  using 1
{
  TR0=0;
  TL0=(time_count&0x00FF);
  TH0=(time_count>>8);
  TR0=1;
  if(--time_tick)  return;
  time_tick=20;
  t0_isr=1;
  if(nba_tim&!oth_run)	   // ��������������ʱ nba_tim==1��oth_run==0 
  {
    if(resu[3]==0)
	{
	  resu[3]=59;
	  if(resu[2]>0)	resu[2]--;
	}
	else resu[3]--;
	if(resu[2]==0&resu[3]==0)  // ����ʱ�䵽��ֹͣ
	{
	  TR0=0;TR1=0;EA=0;
	}
	return;
  }
  if((++temp_min)==60)	  // ʵ���¶ȵĸ���
  {
    temp_min=0;
    temp_delay--;
	if(temp_delay==0)
    {
	  b20time=1;temp_delay=5;
	}
  }	
}


/*24ʱ���жϴ���*/
void time1_isr(void) interrupt  TF1_VECTOR  using 2
{
  TR1=0;
  TL1=(time_count&0x00FF);
  TH1=(time_count>>8);
  TR1=1;
  if(--time_tick1) return;
  time_tick1=20;
  if(resu[4]>0)resu[4]--;	  //  ������24���ж�
  t0_isr=1;
  if(resu[4]==0)
  { TR0=0;TR1=0;EA=0;}
}


/*��ʱ����ʼ��*/
void time_init()			
{
  EA=0;
  TR0=0;
  TR1=0;
  TMOD=0x11;
  TL0=(time_count&0x00FF);
  TH0=(time_count>>8);
  TL1=(time_count&0x00FF);
  TH1=(time_count>>8);
  IE=0;
  ET0=1;
  ET1=1;
}



/*=====================================================================================
 �������ƣ�LCD����
 ��������������������д1���ֽڡ���������ʼ����
 ȫ�ֱ�������
 ����˵����dat_comm��0������ 1�����ݣ�,content����ֵ��
 ����˵�����޷���ֵ
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


/*���æµλ*/
void chk_busy (void)
{
  lcd_data = 0xff;
  lcd_rs = 0;    // ����
  lcd_rw = 1;	 // ��
  lcd_e = 1;
  while(busy==1);
  lcd_e = 0;
}


/*дLCD ������ʽ��0:����,1:��ֵ ������*/
void wr_lcd (uchar dat_comm,uchar content)
{
  chk_busy ();
  lcd_rs=dat_comm;
  lcd_rw = 0;         // д
  lcd_data=content;   // ������ݻ�����
  lcd_e=1;             
  lcd_e=0;
}


/*���ͼ*/
void clrgdram(void)
{
  uchar i,j;
  for(j=0;j<32;j++)
  {
    wr_lcd(comm,0x34);
    wr_lcd(comm,0x80+j);
    wr_lcd(comm,0x80);
    wr_lcd(comm,0x30);
    for(i=0;i<32;i++)
      wr_lcd(dat,0x00);  // д��0x00
   }
  wr_lcd(comm,0x36);
}


/*�����ʾ*/
void clear_dis(void)
{
  wr_lcd(comm,0x30);
  wr_lcd(comm,0x01);  // ��������ַָ��ָ��00H
  clrgdram();		  // ���ͼRAM
}



/*����LCD��ʼ��*/
void init_lcd (void)
{
  lcd_rst=1;
  wr_lcd(comm,0x30);  // 30---����ָ���  
  wr_lcd(comm,0x06);  // �����ƶ�����
  wr_lcd(comm,0x0c);  // ����ʾ�����α�
  clear_dis(); 
}





/*=====================================================================================
 �������ƣ�DS1302����
 ����������������������д���ݡ���д���ڣ�ʱ�䡢��ʼ����
 ȫ�ֱ�������
 ����˵����w_adder��д��ַ��,r_adder������ַ����w_data��д�����ݣ�
 ����˵�������ض�ȡ������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����������д����ֵ���½��ض�����ֵ��û�������󱳵�س��
 ======================================================================================*/


/*��1302д���� ������ʽ����ַ������*/
void w1302(uchar w_adder,uchar w_data)
{
  uchar d;
  ret_ds1302();
  for(d=0;d<16;d++)
  {
    if(d==8) w_adder=w_data;
	d02_data=(bit)(w_adder&0x01);
    d02_sclk=0;		 // ������
    w_adder>>=1;
    d02_sclk=1;
  }
  end_ds1302();    
}



/*����1302һ���ֽڵ����� ������ʽ������ַ*/
uchar r1302(uchar r_adder)
{
  uchar d=0;
  ret_ds1302();
  for(d=0;d<8;d++)	 // д��ַ
  {
    d02_data=(bit)(r_adder&0x01);
    d02_sclk=0;
    r_adder>>=1;
    d02_sclk=1;
  }
  r_adder=0;
  for(d=0;d<8;d++)	 // ������ 
  {
	d02_data=1;
	d02_sclk=1;		 // �½���
	r_adder>>=1;
	d02_sclk=0;
	if(d02_data) r_adder|=0x80;
  }
  end_ds1302();
  return(r_adder);
}



/*д��ʱ��������*/
void wds1302_time_date(void)
{
  w1302(d02_wrc,0x00);   			 // д����
  w1302(0x90,0xa5);                  // ��������2K������
  w1302(d02_year,today.year);   	 // ��
  w1302(d02_weeks,today.week);  	 // ����
  w1302(d02_month,today.month);   	 // ��
  w1302(d02_day,today.day);   		 // ��
  w1302(d02_hour,0x7f&now.hour);   	 // ʱ
  w1302(d02_minute,now.minute);   	 // ��
  w1302(d02_second,0x7f&now.second); // ��
  w1302(d02_wrc,0x80); 				 // д��ֹ
}




/*��ʼ��дDS1302 RAM*/
void init_wds1302_ram(void)
{
  w1302(d02_wrc,0x00); 
  w1302(0x90,0xa5);         // ��������2K������
  w1302(d02_signs,0x00);   	// ״̬�Ĵ�
  w1302(d02_passw,0x74);   	// ���봢����λ
  w1302(d02_passw+2,0x14);  // ��ʼֵΪ:741456
  w1302(d02_passw+4,0x56);  // �������һλ
  w1302(d02_alm,0x12);   	// ����ʱ(12Сʱ)
  w1302(d02_alm+2,0x00);   	// �����(0��)
  w1302(d02_temp,0xff);   	// �¶�����(255Ϊ�ر�) 	 
  w1302(d02_temp+2,0xff);   // �¶�����(�ر�)
  w1302(d02_initsi,0xb4);   // �Ƿ�Ҫ��ʼ�����(0xb4Ϊ�Ѿ���ʼ��)
  w1302(d02_wrc,0x80); 
}

/*дDS1302һ�ֽڵ�RAM*/
void wds1302_data(uchar adder,uchar _data)
{
  w1302(d02_wrc,0x00); 
  w1302(adder,_data);
  w1302(d02_wrc,0x80);
} 


/*��ȡ����*/
void rds1302_date(void)
{
  
  today.year=r1302(d02_year+1);	  
  today.week=r1302(d02_weeks+1);
  today.month=r1302(d02_month+1);
  today.day=r1302(d02_day+1);  
}


/*��ȡʱ��*/
void rds1302_time(void)
{
  now.hour=(r1302(d02_hour+1));
  now.minute=r1302(d02_minute+1);
  now.second=r1302(d02_second+1);
  if(now.hour==alarm_time[0]&&now.minute==alarm_time[1]) alm_run=1;
  else 										// ����ʱ��Ƚ�
  {
    alm_run=0;buzstop=0;buzzer=close;
  } 
}					

/*����������*/
void rds1302_data(void)
{
  signs=r1302(d02_signs+1);
  alarm_time[0]=r1302(d02_alm+1);
  alarm_time[1]=r1302(d02_alm+3);
  als_temp[0]=r1302(d02_temp+1);
  als_temp[1]=r1302(d02_temp+3);		   
}
 

/*DS1302��ʼ��*/
void init_ds1302(void)
{
  wds1302_time_date();
  init_wds1302_ram();
}


/*=====================================================================================
 �������ƣ�DS18b20����
 ����������������������д���ݡ�����ת������ʼ����
 ȫ�ֱ�������
 ����˵������
 ����˵�������ض�ȡ������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ���������С�����
 ======================================================================================*/



/*18B20��ʼ��*/
void init_ds18b20()
{
  b20_io=1;
  b20_io=0;
  delay_b20(120);		// ��ʱ��480us
  b20_io=1;
  delay_b20(12);	    // ����Ϊ�ߵ�ƽ����15��60us���ֵ͵�ƽ����ά��60��240us
  ds18b20=~b20_io;
  delay_b20(40);	
}


/*��18B20����һ���ֽ�*/
void send18b20_byte(uchar data_send)
{
  uchar d=0;
  for (d=0;d<8;d++)
  {
	b20_io=0;					// ���͵�ƽ��������д��һ��λ��15us��18B20����  
    b20_io=data_send&0x01;	    
    delay_b20(10);				// ��ʱ60us�����һ��
    b20_io=1;
    data_send>>=1;			    // ��λ��ʼ
  }
}


/*��18B20�����¶�ֵ 12λ�ֱ���*/  
void receive18b20(void)
{
  uchar n=1,d;
  for (d=0;d<16;d++)
  {
	if(d==8) n=0;
	b20_io=0;					  // ��������
	temp.t_data[n]>>=1;            
	b20_io=1;                     // ��������
	if(b20_io)					  // ����������ֵ
      temp.t_data[n]|=0x80;
    delay_b20(10);				  // �ȴ�60us
  }
}


/*����ת��*/
void conversion_t(void)
{
  uchar save=0,save1=0;
  if(temp.temp_data<0) 	         // �ж���ֵ����
    {temp_pn=1;temp.temp_data=~temp.temp_data+1;}
  else temp_pn=0;
  if(temp.t_data[1]&0x01==1) save+=6;	    // С������ֵת��
  if((temp.t_data[1]>>1)&0x01==1) save+=12;
  if((temp.t_data[1]>>2)&0x01==1) save+=25;
  if((temp.t_data[1]>>3)&0x01==1) save+=50;
  temp.temp_data>>=4;
  save1=temp.temp_data&0x00ff;				// ����������ֵ
  temp.t_data[0]=save1;
  temp.t_data[1]=save;
  thl_als=1;
  if(save1>=(als_temp[0]&0x7f))      // �¶ȱȽ�
  {
	if(!temp_pn||(temp_pn&(als_temp[0]&0x80)!=0)){ als_th=open;als_tl=close;}
  }
  else if(save1<(als_temp[1]&0x7f)&&als_temp[1]!=255)
  {
    if(!temp_pn||(temp_pn&(als_temp[1]&0x80)!=0)) {als_tl=open;als_th=close;}
  }
  else {als_th=als_tl=close;thl_als=0;buzstop=0;buzzer=close;} 
}



/*18b20����*/
void ds18b20_func(void)
{
  EA=0;
  TR0=0;
  temp.temp_data=0;
  init_ds18b20(); 
  send18b20_byte(0xcc); // ����ROM
  send18b20_byte(0x44); // �¶�ת��
  delay_pw();
  init_ds18b20();
  send18b20_byte(0xcc); 
  send18b20_byte(0xbe); // ��RAM
  receive18b20();		// ��ȡ�¶�ֵ
  conversion_t();		// ��ֵת��
  EA=1;
  TR0=1;
}



/*=====================================================================================
 �������ƣ����湹��
 ��������������1��2��3��ͼƬ�˵������ף���ͷ��16*16��32��ͼƬ
 ȫ�ֱ�������
 ����˵��������˵��
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����X����ÿһ��ÿ��16��
 ======================================================================================*/


/*���׻���ʾͼƬ ������ʽ��(H)8bit���ݣ�(L)8bit����,X0����(16bit)��Y0����(1bit),X����(16bit)��Y����(1bit)*/
void con_disp (uchar data1,uchar data2,uchar x0,uchar y0,uchar xl,uchar yl)
{
  uchar i,j;
  for(j=0;j<yl;j++)
  {
    for(i=0;i<xl;i++)
    {
      wr_lcd(comm,0x34);     // ��չ����
      wr_lcd(comm,y0+j);	 // дY����
      wr_lcd(comm,x0+i);	 // дX����
      wr_lcd(comm,0x30);	 // ��ͨ����
      wr_lcd(dat,data1);
      wr_lcd(dat,data2);
    }
  }
  wr_lcd(comm,0x36);		 // ����ͼ��ʾ
}



/*��ʾ���� ������ʽ: ��ʾλ��(0x80-0x9f),��ʾ��������ʾ����ָ��*/
void display_cnasc(uchar dis_bit,uchar num,uchar *cnasc)
{
  wr_lcd(comm,0x30);	     // ��ͨ����
  wr_lcd(comm,dis_bit);	     // д��ַ
  while(num>0)
  {
    wr_lcd(dat,*(cnasc++));
    wr_lcd(dat,*(cnasc++));
	num--; 
  }
}



/*������ͷ��ʾ*/
void dis_title(void)
{
  display_cnasc(0x80,1,"\x15 ");
  display_cnasc(0x90,1,"��");
  display_cnasc(0x97,1,"��");
  display_cnasc(0x98,1,"��");
  display_cnasc(0x9f,1,"��");
  display_cnasc(0x92,1,"\x1e ");
  display_cnasc(0x9a,1,"\x1f ");
  con_disp(0xff,0xff,0x80,0x80,3,16);
  con_disp(0xef,0xef,0x83,0x80,1,16);
  con_disp(0xde,0xf7,0x84,0x80,1,16);
  con_disp(0x76,0xdb,0x85,0x80,1,16);
  con_disp(0x55,0x55,0x86,0x80,1,16);
  con_disp(0x55,0x55,0x87,0x80,1,16);	
}


/*ͼƬ��ʾ��ͷ*/
void dis_title_pic(void)
{
  display_cnasc(0x80,2,"����");
  display_cnasc(0x86,2,"����");
}


/*����ʵ����ʾ��ͷ*/
void dis_title_e(void)
{
  display_cnasc(0x80,1," \x15");
  con_disp(0xff,0xff,0x80,0x80,1,16);
  con_disp(0xff,0xc7,0x81,0x80,1,16);
  con_disp(0xff,0xff,0x82,0x80,3,16);
  con_disp(0xe3,0xff,0x85,0x80,1,16);
  con_disp(0xff,0xff,0x86,0x80,2,16);
}



/*��ʾ�˵�ͼƬ  ��ʽ��ͼƬ����(1)��������ʾ(0)����ʾͼƬ�ĺ���*/
void anti_nor_pic (uchar cpl_sign,uchar menu_num)
{
  uchar i,j,x,y;
  x=menu_num*3;	   // ����ͼƬ��ʾ����
  if(x>6) x--;
  if(menu_num<3) y=16;	// ���°�����ʾ
  else y=8;
  if(cpl_sign==1) display_cnasc(0x82,4,menu_cn[menu_num]);
  for(j=0;j<24;j++)
  {
   for(i=0;i<2;i++)
   {
     wr_lcd(comm,0x34);
     wr_lcd(comm,0x80+y);
     wr_lcd(comm,0x80+x+i);  // ��������ʾ 16bit
     wr_lcd(comm,0x30);
     if(cpl_sign==1)	      // ͼƬ��ѡ
     {
	   wr_lcd(dat,~menu_pic[menu_num][j*4+i*2]);
       wr_lcd(dat,~menu_pic[menu_num][j*4+i*2+1]);
	 }
     else					  // ������ʾ
     {
	   wr_lcd(dat,menu_pic[menu_num][j*4+i*2]);
       wr_lcd(dat,menu_pic[menu_num][j*4+i*2+1]);
	 }
   }
   y++;
   if(y==32&&menu_num<3) {x+=8;y=0;}  // �������л�
  }
  wr_lcd(comm,0x36);
}




/*״̬16*16ͼƬ  ��ʽ����ɫ��1��������0��X���꣬Y���꣬״̬���루16*16��*/
void dis_run_state(uchar cpl,uchar x0,uchar y0,uchar state_num)
{
  uchar j;
  for(j=0;j<16;j++)
  {
    wr_lcd(comm,0x34);  // ��չ����
    wr_lcd(comm,y0++);	// дY��ַ
    wr_lcd(comm,x0);	// дX��ַ
    wr_lcd(comm,0x30);	// ��ͨ����
	if(cpl==1)			// д����
    {
	  wr_lcd(dat,~running[state_num][j*2]);
      wr_lcd(dat,~running[state_num][j*2+1]);
	}
	else
	{
	  wr_lcd(dat,running[state_num][j*2]);
      wr_lcd(dat,running[state_num][j*2+1]);
	}
  }
  wr_lcd(comm,0x36);
}

 

 /*��ʾ�˵���ť  ������ʽ����ɫ(1)������(0)*/
void dis_menu_key(uchar d)
{
  if(d==1)
  {
    for(d=0;d<8;d++)								  // ��ʾ�˵���ť
      con_disp(~menu_key[d*2],~menu_key[d*2+1],0x9f,0x97+d,1,1);
  }
  else
  {
    for(d=0;d<8;d++)								  // ��ʾ�˵���ť
      con_disp(menu_key[d*2],menu_key[d*2+1],0x9f,0x97+d,1,1);
  }
}




/*��ʾģʽ3�Ľ���*/
void dis_mode3(void)
{
  uchar i,j,h,l,x_0=0x80,y_0=0x90;
  h=now.hour>>4;       // ����ʮλ ��λ
  l=now.hour&0x0f;
  for(j=0;j<2;j++)
  {
    for(i=0;i<32;i++)  // ʮλ��λһ�����
    { 
      wr_lcd(comm,0x34);
      wr_lcd(comm,y_0++);
      wr_lcd(comm,x_0);
      wr_lcd(comm,0x30);
      wr_lcd(dat,mode_3num[h][i*3]);  // ����д���ݣ�X�����Զ���1
      wr_lcd(dat,mode_3num[h][i*3+1]);
      wr_lcd(dat,mode_3num[h][i*3+2]);
      wr_lcd(dat,mode_3num[l][i*3]);
      wr_lcd(dat,mode_3num[l][i*3+1]);
      wr_lcd(dat,mode_3num[l][i*3+2]); 
      if(y_0==0xa0){x_0=x_0+8;y_0=0x80;}
    }
    x_0=0x84,y_0=0x90;
    h=now.minute>>4;   // ����ʮλ ��λ
    l=now.minute&0x0f;
  }
  dis_run_state(0,0x83,0x90,4); // ��ʾ��
  dis_run_state(0,0x8b,0x80,4);					   
  y_0=0x80;			   // ��ʾ״̬���
  if(keb) {dis_run_state(0,0x87,y_0,2);y_0+=16;}
  if(alm) {dis_run_state(0,0x87,y_0,1);y_0+=16;}
  if(y_0<0xa0&&!als){dis_run_state(0,0x87,y_0,0);}
  if(y_0==0xa0&&!als){dis_run_state(0,0x8f,0x80,0);}
  dis_menu_key(0);
}




/*��ʾģʽ1��2�Ĺ��ò���*/
void mode1_2_union(void)
{
  uchar x=0x9b;
  display_cnasc(0x98,2,"����");
  if(nba_tim) dis_run_state(0,x,0x90,6);
  if(keb) {dis_run_state(0,x,0x90,2);x++;}    // ��ʾ���
  if(alm) {dis_run_state(0,x,0x90,1);x++;}
  if(!als){dis_run_state(0,x,0x90,0);}
  dis_menu_key(0);                           // ��ʾ�˵���ť
}


/*��ʾģʽ2������*/
void dis_date_mode2(uchar d)
{
  uchar save[2];
  display_cnasc(d+2,1,"��");
  display_cnasc(d+4,1,"��");
  display_cnasc(d+6,1,"��");
  display_cnasc(0x9a,1,weeks[today.week]);  // ��ʾ����
  save[0]=char_num[today.year>>4];	        // ����ʾ����ֵ�浽������
  save[1]=char_num[today.year&0x0f];
  display_cnasc(d+1,1,save);
  save[0]=char_num[today.month>>4];
  save[1]=char_num[today.month&0x0f];
  display_cnasc(d+3,1,save);
  save[0]=char_num[today.day>>4];
  save[1]=char_num[today.day&0x0f];
  display_cnasc(d+5,1,save);
}

/*��ʾģʽ2��ʱ��������ʾ  16*32*/
void num2_asc_dis(uchar num,uchar x_0)
{
  uchar i,y_0=0x90;
  for(i=0;i<32;i++)  // ʮλ��λһ�����
  { 
    wr_lcd(comm,0x36);
    wr_lcd(comm,y_0++);
    wr_lcd(comm,x_0);
    wr_lcd(comm,0x30);
    wr_lcd(dat,mode_2num[num][i*2]);
    wr_lcd(dat,mode_2num[num][i*2+1]);
	if(i==15){x_0+=8;y_0=0x80;}
  }
  wr_lcd(comm,0x36);
}

/*��ʾģʽ2��ʱ��*/
void dis_time_mode2(void)
{
  uchar h,l;
  if(now.hour!=save_time.hour||init_d2)
  {
    h=now.hour>>4;    // ����ʮλ ��λ
    l=now.hour&0x0f;
    num2_asc_dis(h,0x80);
	num2_asc_dis(l,0x80+1);
  }
  if(now.minute!=save_time.minute||init_d2)
  {
    h=now.minute>>4;    // ����ʮλ ��λ
    l=now.minute&0x0f;
    num2_asc_dis(h,0x83); 
	num2_asc_dis(l,0x83+1);
  }
  if(now.second!=save_time.second||init_d2)
  {
    h=now.second>>4;    // ����ʮλ ��λ
    l=now.second&0x0f;
    num2_asc_dis(h,0x86);
	num2_asc_dis(l,0x86+1);
  }
  if(init_d2)
  {
    dis_run_state(0,0x82,0x90,5); // ��ʾ��
    dis_run_state(0,0x8a,0x80,5);
    dis_run_state(0,0x85,0x90,5); // ��ʾ��
    dis_run_state(0,0x8d,0x80,5);
  }	
}


/*��ʾģʽ2�Ľ���*/
void dis_mode2(void)
{
  dis_date_mode2(0x81);	      // ��ʾ����
  display_cnasc(0x81,1,"20");
  dis_time_mode2();			  // ��ʾʱ��
  mode1_2_union();
}


/*��ʾģʽ1������*/
void dis_date_mode1(void)
{
  uchar save[2];
  display_cnasc(0x80,3,"���ڣ�"); 
  display_cnasc(0x90,3,"ʱ�䣺"); 
  display_cnasc(0x88,3,"�¶ȣ�");
  display_cnasc(0x9a,1,weeks[today.week]);  // ��ʾ����
  display_cnasc(0x83,1,"20");
  save[0]=char_num[today.year>>4];			  
  save[1]=char_num[today.year&0x0f];
  display_cnasc(0x84,1,save);
  save[0]='/';
  save[1]=char_num[today.month>>4];
  display_cnasc(0x85,1,save);
  save[0]=char_num[today.month&0x0f];
  save[1]='/';
  display_cnasc(0x86,1,save);
  save[0]=char_num[today.day>>4];
  save[1]=char_num[today.day&0x0f];
  display_cnasc(0x87,1,save);
  mode1_2_union();
}


/*��ʾģʽ1��ʱ��*/
void dis_time_mode1(void)
{
  uchar save[2];
  save[0]=char_num[now.hour>>4];
  save[1]=char_num[now.hour&0x0f];
  display_cnasc(0x93,1,save);
  save[0]=':';
  save[1]=char_num[now.minute>>4];
  display_cnasc(0x94,1,save);
  save[0]=char_num[now.minute&0x0f];
  save[1]=':';
  display_cnasc(0x95,1,save);
  save[0]=char_num[now.second>>4];
  save[1]=char_num[now.second&0x0f];
  display_cnasc(0x96,1,save);
}


/*��ʾģʽ1���¶�*/
void dis_temp_mode1(void)
{
  uchar save[2];
  if(ds18b20)
  {
	 if(temp.t_data[0]>99)
	 {
	    save[0]=char_num[temp.t_data[0]/100];
		save[1]=char_num[(temp.t_data[0]/10)%10];
		display_cnasc(0x8b,1,save);
		save[0]=char_num[temp.t_data[0]%10];
		save[1]='.';
		display_cnasc(0x8c,1,save);
		save[0]=char_num[temp.t_data[1]/10];
		save[1]=' ';
		display_cnasc(0x8d,1,save);
		display_cnasc(0x8e,1,"��");
	  }
	  else
	  {
        if(temp_pn) display_cnasc(0x8a,1,":-");
	    else  display_cnasc(0x8a,1,"��");
		save[0]=char_num[temp.t_data[0]/10];
		save[1]=char_num[temp.t_data[0]%10];
		display_cnasc(0x8b,1,save);
		save[0]='.';
		save[1]=char_num[temp.t_data[1]/10];
		display_cnasc(0x8c,1,save);
		display_cnasc(0x8d,1,"��");
	  }
	}
  else display_cnasc(0x8b,3,"FF.F��");
}



/*��ʾģʽ1�Ľ���*/
void dis_mode1(void)
{
  dis_date_mode1(); 
  dis_time_mode1();
  dis_temp_mode1();
}



/*������ʱ����ʾ*/
void nba_dis_time(void)
{
  uchar d,dis[2];
  for(d=2;d<4;d++)
  {
	dis[0]=char_num[resu[d]/10];
	dis[1]=char_num[resu[d]%10];
	display_cnasc(0x81+d*2,1,dis);
  }
  display_cnasc(0x86,1,": ");
  if(resu[0]==1)
  {
	dis[0]='\x07';
	dis[1]=char_num[resu[4]/10];
	display_cnasc(0x93,1,dis);
	dis[1]='\x07';
	dis[0]=char_num[resu[4]%10];
	display_cnasc(0x94,1,dis);
  }
}


/*=====================================================================================
 �������ƣ�����ɨ��
 ������������ȡ���̲������ݼ�����״̬��ʵ��
 ȫ�ֱ�����dis_sig  t0_isr  temprun  b20time  alm_run als_temp	 buzstop  als
           lock_op  nba_tim 
 ����˵������
 ����˵������������������ʱ�䵽��󷵻� 255
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/

/*ɨ�����*/
uchar  keypad_scan()
{
  uchar key,dis[2];
  keyboard=0xf0;
  while (keyboard==0xf0)     // �ȴ���������
  {	 
 
  /*--------------------- ����ƹر���ʱ -----------------------*/
	
	delay(1000);	 			     
	if((++dis[0])==0&&bl_delay!=0) bl_delay--;  // ���а�������bl_delay���¸�ֵ 	
	if(bl_delay==0&&!bl0) lcd_bl=close;         // ����ʱ�䵽�Ҳ��ǳ���ģʽʱ�رձ���

	  
  /*----------------------- ��������ʱ -------------------------*/	

	if(keb&&dis_sig&&bl_delay==0&&resu[11]<2) resu[11]++; // ���а�������j����
	if(bl_delay==0) bl_delay=15; 


  /*--------------------- ������30������ʱ ---------------------*/		

	if(lock_op)				         
	{
	  if((++i)==0) save1++;	  	
	  if(save1==15) return(255);
	}
	

  /*---------------------- ʱ�ӽ���ʱ����ʾ ---------------------*/
	
	if(dis_sig&&t0_isr||nba_tim&&oth_run)     // ����ʱ�ӽ���������������ʱ�ӽ�����ʾ
    {    
	  rds1302_date();	    // �������� 
      rds1302_time();		// ����ʱ��
	  if(di0) 	            // ģʽ2
	  {
	    dis_time_mode2(); 
		if(now.hour==0&&now.minute==0&&now.second==0)	dis_date_mode2(0x81); 
	  }
      else if(di1) 			// ģʽ3
	  { 
	    if(now.hour!=save_time.hour||now.minute!=save_time.minute)   // ʱ,�ֱ仯����ʾ
	      dis_mode3();	   
      }
      else 
	  {
	    dis_time_mode1();   // ģʽ1
	    if(now.hour==0&&now.minute==0&&now.second==0) dis_date_mode1();
	  }
	  save_time=now;        // ���¼�¼���´αȽ�
	  t0_isr=0;
	}
  
  
  /*-------------------- �¶ȼ���ʾ�������� ----------------------*/
  	
	if(temprun)
	{
	  if(!oth_run)
	  {
	    display_cnasc(0x96,2," key");    // �¶ȼ�ʱ��ʾ��������
		for(dis[1]=0;dis[1]<250;dis[1]++)
	    {
	      delay(1000);
		  if(keyboard!=0xf0)
		  {
		    keyboard=0x7f;
			if(keyboard==0x7d) return 0x7d;
			else  keyboard=0xf0;
		  }
	    }
		oth_run=1;
		display_cnasc(0x96,2,"    ");
	  }
	}
  
  
   /*------------------------ ���������� --------------------------*/

	
	if(((alm_run&&alm)||thl_als)&&!buzstop)	// �¶ȣ��������������
	{									    // �а�������ʱֹͣ
	  if(now.second==0)	lcd_bl=close;
	  delay(15000);							// �����¶�ʱ��Ƚ�
	  if(keyboard!=0xf0) break;
	  delay(15000);
	  if(keyboard!=0xf0) break;
	  delay(15000);
	  if(keyboard!=0xf0) break;
	  delay(15000);
	  if(alm_run)  lcd_bl=!lcd_bl;			// ֻ�����幦����
	  if(!als)buzzer=!buzzer;               // �������������������			    
	} 


  /*----------------------- �¶�ˢ����ʾ -------------------------*/

  	
	if((temprun&&oth_run)||(b20time&&!di0&&!di1&&dis_sig))    // �¶ȼƹ�����ģʽ1���� 
    {  
	 
	    ds18b20_func();					          // �¶ȼ�⹦��
		oth_run=0;                          	  // ����������
	    if(temprun) return 20;		              // �¶ȼƿ����󷵻�
	    if(b20time){ dis_temp_mode1();b20time=0;} // ʱ�ӽ���5���Ӹ���һ�Σ�T0��ʱ
    } 

  
   
   /*------------------------ ��������ʾʱ�� ------------------------*/
	
	if(nba_tim&!oth_run)	    // ��������ģʽ�� oth_run==0Ϊ��������ʱ״̬ 
	{
	  if(EA) display_cnasc(0x8b,2," R\x10 ");  // ��ʾ����״̬
	  else  display_cnasc(0x8b,2," S! ");
	  if(t0_isr)
	  {										   // ����ʱ��
	    nba_dis_time();
		if(resu[2]==0&&resu[3]==0)			   // �жϱ������ 
		{
		  resu[6]++;
		  if(resu[6]<resu[1])  display_cnasc(0x85,3,"��Ϣ  ");
		  else  display_cnasc(0x85,3,"���  ");
		}
		if(resu[4]==0||(resu[2]==0&&resu[3]==0))  // ʱ�䵽�ﾯ��
	    {
	       buzzer=open;
	       delay_pw();
		   if(resu[2]==0&&resu[3]==0)  // 24�� ��ʱ�侯��
		   {
		     delay_pw();
		   }
	       buzzer=close;
		   if(resu[4]==0) resu[4]=24;  // ���¸�ֵ
	    }
	    t0_isr=0;					   // ������´��жϲ���
	  }
	}
  }

			  
			        /*===== ���ϵȴ����� =====*/



  /*------------------------- �������º� --------------------------*/

  
  delay(8000); 	             // ������
  if(!lock_op)bl_delay=15;   // ����ʱ�丳ֵ 
  if(bl1) lcd_bl=close;	     // ���ڱ���ƹر�ʱ����
  else lcd_bl=open;
  if(alm_run|thl_als) {buzstop=1;buzzer=close;}
  if(keb&&dis_sig) 
  {
    if(resu[11]==2) return 255;  // Ҫ����������	
  	else resu[11]=0;			 // ʱ�䲻��ʱ���¼�ʱ
  }
  do
  {    	
	for(dis[0]=0;dis[0]<=3;dis[0]++)
    {
      keyboard=ksp[dis[0]];
	  if(keyboard!=ksp[dis[0]])
      {	
        key=keyboard;
        if(key!=ksp[dis[0]]) return(key);
      }
    }
  }while(1);
} 
 

/*����������*/
uchar  gotkey()
{
  uchar  save,n;
  keyboard=0xf0;
  while(keyboard!=0xf0);    // �ȴ������ɿ�
  delay(5000);	            // ������
  if(adju_si) 	            // ��д����
  {
	adju_si=0;
	for(n=0;n<60;n++)
  	{
	  delay(2000);
	  if(keyboard!=0xf0)
	  { 
	    adju_si=1;          // �ڸ���ʱ,�ڴ�ʱ���ڿ����ɶ�λ��
 	    break;
	  }
    }
  }			
  save=keypad_scan();	    // ɨ�����
  if(save==255)	return 255;	// Ҫ������������������ʱʱ�䵽��
  for (n=0;n<16;n++)
  {  
	if(save==key_code[n]) 
	   return(n);
  }
  return(20);
}



/*=====================================================================================
 �������ƣ�����
 �������������˵� ���ߵ���  ������ʾ
 ȫ�ֱ�������
 ����˵����
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


/*����������ʾ ������ʽ��k ��ť����ʾ��ַ*/
void key_dis(uchar k)
{
  display_cnasc(k,1,"\x04 ");
  delay(10000);
  if(k==0x92||k==0x96) display_cnasc(k,1,"\x1e ");
  else display_cnasc(k,1,"\x1f ");
}



/*ȷ������������ʾ*/
void enter_dis(void)
{
  display_cnasc(0x8f,1," \x09");
  delay(15000);
}



/*���˵�����*/
uchar dis_menu(void)
{
  uchar d,save,key;
  if(meu)
  {  
    display_cnasc(0x81,2,"�˵�");
	dis_title();
	display_cnasc(0x88,8,selected[0]);
	con_disp(0xff,0xff,0x8a,0x80,5,16);
  } 
  else	  // ��ʾͼƬ�˵�
  {
    dis_title_pic();
	for(d=0;d<6;d++)  
      anti_nor_pic(0,d);
	anti_nor_pic(1,0);
  }      
  save=d=0;
  do
  {
	if(meu)	  // ����ָʾ����
	{	  
	  display_cnasc(0x93,3,menu_cn_2[index_m[save].left_num]);
	  display_cnasc(0x8b,3,menu_cn_2[save]);
	  display_cnasc(0x9b,3,menu_cn_2[index_m[save].right_num]); 
    }
	else
	{
	  anti_nor_pic(0,d);     
	  anti_nor_pic(1,save);
	}
	d=save;			      // ������һ����
	while(1)
	{
	  key=gotkey();
	  if(key==11||(key==3&&meu))
	  { 
	    if(meu) enter_dis();
		break;
	  }
	  else if(meu)
	  { 
	    if(key==2)
		{
		  save=index_m[d].right_num;
		  key_dis(0x9a);
		  break;
		}
	    if(key==5)
		{
		  save=index_m[d].left_num;
		  key_dis(0x92);
		  break;
		}
		if((key==1&&meu))
	    {
	      d=5;
		  break;
	    } 
	  }
	  else 
	  {
	    if(key==3){save=index_m[d].right_num; break;} // ���ݵ�����Ѱ������
	    if(key==1){save=index_m[d].left_num; break;}
		if(key==2||key==5){save=index_m[d].shaft_num; break;}  
      }
	}
	if(key==11||((key==1|key==3)&&meu)) break;
  }while(1);
  return d;
}



/*���ߵ���*/
uchar tool_func(void)
{
  uchar save,d,key;
  display_cnasc(0x81,2,"����");
  dis_title();
  display_cnasc(0x88,8,selected[0]);
  con_disp(0xff,0xff,0x8a,0x80,5,16); 
  d=0;
  save=0;
  do
  {	  
	display_cnasc(0x93,3,tool_menu[index_t[save].lnum]);
	display_cnasc(0x8b,3,tool_menu[save]);
	display_cnasc(0x9b,3,tool_menu[index_t[save].rnum]); 
	d=save;
	while(1)
	{
	  key=gotkey();
	  if(key==1){ d=4;break;}				// ���ݰ���Ѱ������
	  if(key==11||key==3){enter_dis();break;}
	  if(key==2){save=index_t[d].rnum; key_dis(0x9a);break;}
	  if(key==5){save=index_t[d].lnum; key_dis(0x92);break;}
    }
	if(key==11||key==3||key==1) break;
  }while(1);
  return d;
}




/*���ĺ���ʾ����*/
void save_y_or_n(void)
{
  uchar key;
  clear_dis();
  oth_run=0;	  
  display_cnasc(0x92,5,"�Ƿ񱣴�? ");
  display_cnasc(0x89,6,"Y(OK)\\N(./S)");
  while(1)
  {
	key=gotkey();
	if(key==11) 
	{
	  display_cnasc(0x9a,5,"����ɹ��� ");
	  delay(50000);
	  oth_run=1;	 // Ҫ�󱣴�ʱoth_run���Ϊ1
	  return;
	}
	if(key==10) return;
  }
} 


/*=====================================================================================
 �������ƣ���������
 ����������ʱ�����ڵ���
 ȫ�ֱ�����today  now
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


/*����������ʾʱ��*/
void asjust_time()
{ 
  uchar save[2];
  display_cnasc(0x8a,1,"ʱ");
  display_cnasc(0x8c,1,"��");
  display_cnasc(0x8e,1,"��");
  save[0]=char_num[now.hour>>4];
  save[1]=char_num[now.hour&0x0f];
  display_cnasc(0x89,1,save);
  save[0]=char_num[now.minute>>4];
  save[1]=char_num[now.minute&0x0f];
  display_cnasc(0x8b,1,save);
  save[0]=char_num[now.second>>4];
  save[1]=char_num[now.second&0x0f];
  display_cnasc(0x8d,1,save);
}


/*���������������Ƚ�*/
uchar monthday(uchar year,uchar month)
{
  if(month==2&&year%4==0)
    return(29);
  else
  {	year=month&0x0f;
    month>>=4;
	month=month*10+year;	
    return(dayofmonth[month-1]);}
}



/*��������*/
void weeks_day()
{ 
  uchar d,buffer,day=4; 
  if(today.month>1)
  {
    buffer=(today.month>>4);   // ��BCD��תΪʮ����
	buffer=buffer*10;
	buffer+=(today.month&0x0f);
	for(d=0;d<buffer-1;d++)
	{
	  today.week=(dayofmonth[d]>>4);
	  today.week=today.week*10;
	  today.week+=(dayofmonth[d]&0x0f);
      day+=(today.week%7);
	}
  }
  buffer=(today.year>>4);
  buffer*=10;
  buffer+=(today.year&0x0f);
  if(today.year!=0)
  {
	day+=((buffer-1)/4)+buffer+1;
  }
  if(buffer%4==0&&today.month>2) day++;
  buffer=today.day>>4;
  buffer*=10;
  buffer+=(today.day&0x0f);
  today.week=(day+buffer)%7;
}



/*��������*/
void adjust_func(void)
{
  uchar d,save,b=0;
  dis_title_e();
  display_cnasc(0x82,3,menu_cn_2[0]);
  rds1302_date();
  rds1302_time();
  dis_date_mode2(0x90);
  display_cnasc(0x9a,1,"  "); 
  asjust_time();
  display_cnasc(0x9f,1,"��");
  display_cnasc(0x90,1,"��");
  display_cnasc(0x8f,1,"��");
  d=0;save=0;
  adju_si=0;
  do
  {
    con_disp(0x00,0x00,index_a[d].lnum,index_a[d].rnum,1,13);
    con_disp(0xff,0xff,index_a[save].lnum,index_a[save].rnum,1,13);
	d=save;
	while(1)
	{
	  save=gotkey();
	  if(save>=0&&save<10)
	  {
	    switch(d)
	    {
	      case 0: if(adju_si) // adju_si==1ʱ������������һ����
		          { 
				    today.year<<=4;
			        today.year+=save;adju_si=0;
				  } 
	              else 
			      {
			        today.year=save;adju_si=1;
			      }
		          break;
		  case 1: if(adju_si)
		          {
				    today.month<<=4;today.month+=save;
		            if(today.month>0x12) today.month=save;
                    else adju_si=0;
				  }
				  else  
				  {
				    today.month=save;adju_si=1;
				  }
				  break;
		  case 2: if(adju_si)
		          {
				    today.day<<=4;today.day+=save;  // �������Ƚ�
					if(today.year%4==0&&today.month==2&&today.day==29) ;
		            else if(today.day>monthday(today.year,today.month)) 
					  today.day=save;
					else adju_si=0;
				  }
				  else  {today.day=save;adju_si=1;}
				  break;
		  case 3: if(adju_si)
		          {
				    now.hour<<=4;now.hour+=save;
		  			if(now.hour>0x23) now.hour=save;
					else adju_si=0;
				  }
				  else  {now.hour=save;adju_si=1;}
				  break;
		  case 4: if(adju_si)
		          {
				    now.minute<<=4;now.minute+=save;
		  			if(now.minute>0x59) now.minute=save;
					else adju_si=0;
				  }
				  else  {now.minute=save;adju_si=1;}
				  break;
		  case 5: if(adju_si)
		          {
				    now.second<<=4;now.second+=save;
		  			if(now.second>0x59) now.second=save;
					else adju_si=0;
				  }
				  else  {now.second=save;adju_si=1;}
				  break;
		}
		if(today.day>monthday(today.year,today.month)) today.day=1;	// �����·�ʱ�ж�����
		if(today.month==0) today.month=1;
		if(today.day==0) today.day=1;
		if(d<3) dis_date_mode2(0x90); 	// ������ʾ
        else asjust_time();
		display_cnasc(0x9a,1,"  ");		// ��ȥ������ʾ
      }
	  if(save==11){save=d+1;adju_si=0;break;}
	  if(save==10&&d!=0){save=d-1;adju_si=0;break;}
	}
    if(save==7&d==6) break;
  }while(1);
  weeks_day();	  // ��������
  save_y_or_n();  // ��Ҫ����ʱoth_run��Ϊ1
  if(oth_run) wds1302_time_date();
}




/*=====================================================================================
 �������ƣ�����������
 ����������10λ���������ļӼ��˳�����
 ȫ�ֱ�����opera  resu  i  j
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/



/*��Ĵ���  ������ʽ��m=2ʱ����Ĵ�������m=1ʱ������1����m=0ʱ������0(1),
                        n=1ʱ,ֻ��Ĵ���������ʾ, 0����*/
void removal_data(uchar m,uchar n)
{
  uchar l;		// ��n=1ʱ,ֻ��Ĵ���������ʾ, 0����
  if(m==2)		// ��m=2ʱ����Ĵ���
  {			    // ��m=1ʱ������1
    if(n==0)	// ��m=0ʱ������0 ,1
	  for(l=0;l<6;l++)
	    display_cnasc(0x98+l,1,"  ");
    for(l=0;l<12;l++)
      resu[l]=0;
  }
  while(m<2)
  { 
    if(n==0)
	{
	  if(m==0) 
	  {  
	    for(l=0;l<6;l++)
	      display_cnasc(0x90+l,1,"  ");
		  display_cnasc(0x95,1," 0");
	  }	
	  else
	     for(l=0;l<6;l++)
	        display_cnasc(0x88+l,1,"  ");
	}
    for(l=0;l<12;l++)
      opera[m][l]=0;
	m++;
  }
}




/*��ʾ��ֵ  ������ʽ����ʾ�׵�ַ*/
void dis_cdata(uchar ass)
{
  uchar p,d,save[2];
  if(i==0&&opera[0][0]==1) 		// ��ʾ����
  {
    if(cal_run)  display_cnasc(0x98,1,"=-");
    else  display_cnasc(0x90,1," -");
  }
  d=opera[i][11];				// ����λ��
  if(opera[i][10]!=0&&opera[i][10]!=opera[i][11]+1) d++;
  save[0]=d/2;ass=ass-save[0]+5;	// ������ʾ�Ŀ�ʼλ��
  p=1;
  cal_del=0;					// ��ʾ���� ��0Ϊû����ʾ��
  if(d%2==1)					// ��λ��λ�����ȴ�����λ
  { 
    save[0]=' ';
    save[1]=char_num[opera[i][1]];
	display_cnasc(ass,1,save);
	p++;
  }
  ass++;						// ��ʾ��ַ��һ
  while(p<=opera[i][11])
  {									
    if(opera[i][10]==p&&(!cal_del)) {   // ��ʾ���������
	  save[0]='.';cal_del=1;}
    else { 
	  save[0]=char_num[opera[i][p]]; p++;}
    if(opera[i][10]==p&&(!cal_del)) { 
	  save[1]='.';cal_del=1;}
    else{ 
	  save[1]=char_num[opera[i][p]];p++;}
    display_cnasc(ass,1,save);
    ass++;
  }
}   


/*�Ӽ�����д������*/   
char input_data()
{
  uchar save=20,c;
  for(;i<2;i++)
  {
    while(1)
    {
pp:   save=gotkey();	// ������
	  if(save==20) continue;
	  if(save==11)		// ����Ϊȷ��ʱ,ѡ������ɾ��
	  {
	    c=save=0;
		while(1)		// ��ɫѡ��
	    { 
		  con_disp (0x00,0x00,index_cal[save].lnum,index_cal[save].rnum,2,16);
		  con_disp (0xff,0xff,index_cal[c].lnum,index_cal[c].rnum,2,16);	
		  save=gotkey();
		  if(save==10)
		  { 
		    save=c;
			if(++c==3) c=0;
		  }
		  else if(save==11) 
		  { 
            if(c==0) save=11;
		    else if(c==1) save=16;
			else  return 0;
		    break;
		  }
		  else break;
        }
		con_disp (0x00,0x00,index_cal[c].lnum,index_cal[c].rnum,2,16);	    
	  }
      if(save>=0&&save<=9&&j<10) 
	  {
	    if(i==1&&opera[1][0]==15&&j>8) goto pp;
        if(j==2&&opera[i][1]==0&&opera[i][10]==0)
          opera[i][1]=save;		// ��ֹС��λ����ʱ��λΪ��
	    else{
		  opera[i][j]=save;opera[i][11]=j;j++;}
		if(i==0)				// ��ʾ
		  dis_cdata(0x90);
		else
		  dis_cdata(0x88);
	  }
      else if(save==10&&j<10)	
      {
	    if(opera[i][10]==0)		// С�����ʱ����
	    {
		  if(opera[i][11]==0) {opera[i][10]=2;j++;}
	      else  opera[i][10]=j;
	    }
	  }
	  else if(save==16)	
	  {                           // ɾ������
        removal_data(i,0);j=1;
		if(i!=0){i--;j=10;}
      }
	  else if(i==1&&opera[1][11]!=0&&save>11&&save<16)
      { save1=save;break; }   		// ��������ʱ���������
      else if(save==11&&opera[1][11]==0) continue;// ������1Ϊ0ʱ�����
	  else if(save==11&&opera[1][11]!=0)  break; 
	  else if((i==0||i==1&&j==1)&&save>11&&save<16){i=0; break;}    
       // ��������0���������������,�ٰ����ż�����
	  else continue;
    }
    if(opera[i][10]>opera[i][11]||opera[i][10]==0)
      opera[i][10]=opera[i][11]+1;	// ����ʱС��λ���浽���
    while(opera[i][11]>=opera[i][10]&&opera[i][opera[i][11]]==0)
      opera[i][11]--;				// ȥ��С���������0
    if(i==0)
    {
      opera[1][0]=save;			// �������������ʾ
      display_cnasc(0x88,1,tool_cal[save-12]);
    }
    if(opera[0][11]==0) display_cnasc(0x95,1," 0");
    j=1;
    while(j<=opera[i][11]&&opera[i][j]==0) j++;//�ж��Ƿ�������Ƿ�ȫΪ0
    j--;
    if(j==opera[i][11]) opera[i][11]=0;
    j=1;
  }
  return 1;
}


/****************************************************************************************
����:  �˷�����
����:  ������ҵ��󱣴�
����:  i , j , opera[i][j],p,q,r,k,save1
����ֵ��1(�ɹ�),0(���λ0),-1(���)
/****************************************************************************************/

char multiplication()
{
  uchar p,q,r,k,save1=0; // pָ�������0 ,qָ�������1,rָ����
  if(opera[0][11]==0||opera[1][11]==0) return 0;
  resu[10]=opera[0][11]+opera[1][11]-opera[0][10]-opera[1][10]+2; // RESU��¼С������λ��
  q=opera[1][11];					// ��¼��1��λ��
  r=9;
  opera[1][11]=1;
  k=1;
  while(opera[1][k]==0) {opera[1][11]=opera[1][11]+1;k++;}	// ȥ��ǰ�˵�0
  k=1;
  while(opera[0][k]==0) k++;			// ȥ��ǰ�˵�0
  resu[11]=9;
  for(resu[0]=9;q>=opera[1][11];q--,resu[0]--)
  {
    if(opera[1][q]==0) continue;		// ��1��λ��Ϊ0ʱֱ�ӱȽ�
    for(p=opera[0][11],r=resu[0];p>=k;p--)
    {
      save1=opera[0][p]*opera[1][q];		// ����˺��������
      resu[r]+=(save1%10);//�������е���ȡ���ӵ������Rָ���λ
      save1/=10;
      save1=save1+(resu[r]/10);			// �����λ����
      resu[r]%=10;					// ����д��
	  if(r==1)	//��������ʱ��λ
	  {
	    if(save1==0&&p==k) break;	// ���㵽���λ��û��λ����
	    else if(resu[10]>0)		// Ҫ����С������
        { 
	      for(r=9;r>1;r--) resu[r]=resu[r-1]; 
	      r=1;
          resu[r]=save1;
	      resu[11]=1;
	      resu[10]--;
	      resu[0]++;
        }   
	    else   return -1;	 		// �������
	  }
	  if(r>1&&(r-1)<resu[11])			// ������λ�����λ
	  {
	    if(save1!=0) resu[11]=r-1;
	    else resu[11]=r;
	  }
	  if(r>1){ r--;resu[r]+=save1;}// ��RΪ1ʱֻ��Ҫ��λ��¼���
    }
  }
  while(resu[resu[11]]==0) resu[11]++; 	// ȥ��ǰ�˵�0
  resu[0]=opera[0][0];				// �������λ
  removal_data(0,1);					// �����ݲ�����ʾ
  if(resu[10]>8)   //���С����������9 λ������ʱ������Ƶ�ʹС����ŵ�2��λ��  
  {
    save1=resu[10]-10+resu[11]; 		// ��¼����λ��
    if(save1>7)  return 0; 	 //�����Ƶ���������Ч���ֵ�λ��ʱ���Ϊ0 
    else 	// ��������λ
    {
      for(p=save1+2,r=resu[11];p<10;p++,r++) opera[0][p]=resu[r];
	  opera[0][10]=2;opera[0][11]=9;opera[0][0]=resu[0];
    }		// ����Ч��С��С��������,������λΪ0  
  }	
  else  	//���С����������8λ����ʱ���������
  {	
    opera[0][0]=resu[0];  
    save1=10-resu[11]; 	// ��¼��Чλ��
    if(resu[10]>=save1) 	// С��������Ч���ֵ����
    {	
	  for(p=resu[10]+1,r=9;r>=resu[11];p--,r--)
        opera[0][p]=resu[r];	// ��С��������һλ��ʼ
      opera[0][10]=2;opera[0][11]=resu[10]+1;
    }
    else  				// С��������Ч������
    {  
	  for(p=1,r=resu[11];r<10;p++,r++) opera[0][p]=resu[r];
	    opera[0][11]=10-resu[11];
	  if(resu[10]==0) opera[0][10]=opera[0][11]+1;
	  else opera[0][10]=opera[0][11]-resu[10]+1;
    }
  }
  while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0) 
    opera[0][11]--;  	// ȥ��С���������0
  return 1;
}


/****************************************************************************************
����:  ��������
����:  ��������ұ���
����:  i , j , opera[i][j]
p,q,r, remainde, divisor, decimal
����ֵ��1(�ɹ�),0(���λ0),-1(���)
*****************************************************************************************/

char divider()
{
  unsigned long int divisor=0,remainder=0;
  uchar p,q,r;
  char decimal=0;
  if(opera[1][11]==0) return -1;
  if(opera[0][11]==0) return 0;
  for(r=1;r<12;r++)		// �ж������Ƿ����
  {
    if(opera[0][r]!=opera[1][r]) break;
  }
  if(r==12)
  { 
    p=opera[0][0];
    removal_data(0,1);
    opera[0][0]=r;
    opera[0][1]=1;
    opera[0][10]=2;
    opera[0][11]=1;
    return 1;
  }  
  decimal=(opera[0][11]-opera[0][10])-(opera[1][11]-opera[1][10]);
  // ��¼��������С�����λ���Ĳ� 
  if(decimal>0)		// ����0���ڲ�1ʱ���¼�¼С�����λ��
  {
    opera[0][10]=opera[0][11]-decimal+1;
    resu[10]=opera[0][10];
  }
  else  	// ����С�����û��������¼��Ч����λ��
  {	
    opera[0][11]-=decimal;
    opera[0][10]=opera[0][11]+1;
    resu[10]=opera[0][10];
  }
  q=1;
  divisor=0;
  while(q<=opera[1][11]) // ����1�������λ�
  {
    divisor=divisor*10+opera[1][q];q++;
  }
  if(divisor==1)   
  {
    if(resu[10]>10)	return -1;
	else return 1;
  }
  r=1;p=1;
  remainder=(unsigned long int)opera[0][1]; // �ȶ������Ĵ�������ֵ
  do
  {   
	if(r>9)  // ���λ�Ѿ��������1λ
	{  
  	  if(resu[1]==0&&resu[10]!=2) // �ж��Ƿ��е���λ
	  { 	
	    for(r=1;r<9;r++) resu[r]=resu[r+1];	
	    resu[9]=0;
	    if(resu[10]!=0) resu[10]--;
	    r=9;
	  }
      else if(p>=opera[0][11]||p>=opera[0][10])  break; // ������㵽���һλ��������С�������ʱ����
      else return -1; // �������
	}
	resu[r]=(uchar)(remainder/divisor);	// ������
    remainder=(remainder%divisor);		// ����д������
	p++;r++;	 
    if(p>opera[0][11]&&remainder==0) break; // ����������
    else if(p<=opera[0][11]&&p<10)		// ����*10��Ӻ�һλ
      {remainder=remainder*10+opera[0][p];}
    else   remainder=remainder*10;	 //����������*10
  }while(1);
  resu[11]=r-1;
  resu[0]=opera[0][0];
  removal_data(0,1);
  r=1;   
  while(resu[r]==0&&resu[10]!=(r+1)&&r<resu[11]) r++; 
//ȥ�����ǰ�˵�0
  if(r==resu[11]) return 0;
  resu[10]=resu[10]-r+1;
  p=1;
  while(r<=resu[11]){opera[0][p]=resu[r];r++;p++;}
  opera[0][0]=resu[0];opera[0][10]=resu[10];opera[0][11]=p-1;
  while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0) 
    opera[0][11]--;
  return 1;
}


/****************************************************************************************
����:  �ӷ�����
����:  ��С����λ�����������
����:  i , j , opera[i][j],p,q,r
����ֵ��1(�ɹ�),0(���λ0),-1(���)
/****************************************************************************************/

char adder()
{
  uchar p,q,r;
  if(opera[0][11]==0&&opera[1][11]==0)   return 0;
  else if(opera[0][11]==0)
    {for(q=0;q<12;q++) opera[0][q]=opera[1][q];return 1;}
  else if(opera[1][11]==0) return 1; 
  else
  {
    p=opera[0][10]-1;
    q=opera[1][10]-1;
    p>q?(r=p):(r=q);
    resu[10]=r+1;		// С����λ����
    resu[0]=opera[0][0];	// �������λ
    while(r>0)			// ��С����ǰ������Ӧ��Ӻ�ŵ������
    {
      if(p>0&&q>0){resu[r]=opera[0][p]+opera[1][q];p--;q--;}
      else if(p>0&&q==0){resu[r]=opera[0][p];p--;}
      else	{resu[r]=opera[1][q];q--;}
      r--;
    }
    r=resu[10];
    p=opera[0][10];
    q=opera[1][10];
    while(r<10)			// ��С���������ŵ������
    {
      if(p>opera[0][11]&&q>opera[1][11]) break;
      else if(p<=opera[0][11]&&q<=opera[1][11])
        {resu[r]=opera[0][p]+opera[1][q];p++;q++;}
      else if(p<=opera[0][11]&&q>opera[1][11])
        {resu[r]=opera[0][p];p++;}
      else	{resu[r]=opera[1][q];q++;}
      r++;
    }
    r--;
    resu[11]=r;
    p=0;
    while(r>0)			// ���ҵ��󽫴���9������λ
    {
      resu[r]+=p;			// pΪ��λ���
      p=0;
      if(resu[r]>9) {resu[r]-=10;p=1;}
      r--;
    }
    if(p==1)				// �����λ
    {
      if(resu[10]==10) return -1;
      else {
        for(r=9;r>1;r--) resu[r]=resu[r-1];
        resu[1]=1;resu[10]++;}
    }
   removal_data(0,1);
   r=0; 
   while(r<12){opera[0][r]=resu[r];r++;}
   while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0)
     opera[0][11]--;
   return 1;
  }
}

/****************************************************************************************
 ����:  ��������
 ����:  �ȱȽ����������Ĵ�С��one ,two����
����:  i , j , opera[i][j],p,q,r��one��two
����ֵ��1(�ɹ�),0(���λ0),-1(���)
/****************************************************************************************/

char subtraction()
{
  uchar p,q,r,one,two;
  if(opera[0][11]==0&&opera[1][11]==0)   return 0;
  else if(opera[0][11]==0)
  {
    for(q=1;q<12;q++) 
    opera[0][q]=opera[1][q];
	opera[0][0]=!opera[0][0];return 1;
  }
  else if(opera[1][11]==0) return 1; 
  else
  { // �Ƚϴ�С(��ŵ�one)
    if(opera[0][10]>opera[1][10]){one=0;two=1;}
    else if(opera[1][10]>opera[0][10]) {one=1;two=0;}
    else
    {
      r=1;
	  while(r<10&&opera[0][r]==opera[1][r]) r++;
	  if(r==10) {removal_data(0,1);return 0;}
      opera[0][r]>opera[1][r]?(one=0,two=1):(one=1,two=0);
    }
    p=opera[one][10]-1;
    q=opera[two][10]-1;
    r=p;			// ����С����ǰ����
    resu[10]=r+1;	// ����С����λ
    resu[0]=0;		// ������λ���
    while(r>0)		// С����ǰ���
    {
      if(p>0&&q>0)
	  {
	    opera[two][q]+=resu[0];
        resu[0]=0;
        if(opera[one][p]>=opera[two][q])
          resu[r]=opera[one][p]-opera[two][q];
	    else
	    {
		  resu[0]=1;
		  resu[r]=10+opera[one][p]-opera[two][q];
	    }
	    p--;q--;
	  }
	  else 
	  {	    
        if(opera[one][p]<resu[0])
          {resu[r]=10+opera[one][p]-resu[0];}
	    else
		  {resu[r]=opera[one][p]-resu[0];resu[0]=0;p--;}
	  }
      r--;
    }
    r=resu[10];
    p=opera[one][10];
    q=opera[two][10];
    while(r<10)		// ��С���������ŵ������
    {
      if(p>opera[one][11]&&q>opera[two][11]) break;
      else if(p<=opera[one][11]&&q<=opera[two][11])
	  {
        if(opera[one][p]>=opera[two][q])  resu[r]=opera[one][p]-opera[two][q];
	    else
	    {
	      resu[r]=10+opera[one][p]-opera[two][q];
	      resu[0]=r;
	      for(r-=1;r>0;r--) // ��ǰ��λ
		  {
	        if(resu[r]==0) resu[r]=9;
		    else{resu[r]-=1;break;}
		  }
		  r=resu[0];	
	    }
	    p++;q++;
	  }				            
      else if(p<=opera[one][11]&&q>opera[two][11])
        {resu[r]=opera[one][p];p++;}
      else	
	  {
	    resu[r]=10-opera[two][q];  
	    resu[0]=r;
	    for(r-=1;r>0;r--)
        {
		  if(resu[r]==0) resu[r]=9;
	      else{ resu[r]-=1;break;}
		}
	    r=resu[0];
	    q++;	
	  }
      if(r==9) 		// ��������λ
	  {
	    if(resu[1]==0&&resu[10]!=2) 
        {
          for(r=1;r<9;r++) resu[r]=resu[r+1];
		  resu[10]--;resu[9]=0;r=8;
        }
	  }
      r++;
    }
    r--;
    resu[11]=r;
    if(one==0)resu[0]=opera[0][0];	// ����λд��
    else resu[0]=!opera[0][0];
    removal_data(0,1);
    r=1;
    while(resu[r]==0&&resu[10]!=(r+1)) r++; // ȥ��ǰ��0
    resu[10]=resu[10]-r+1;			// ����д��С����
    p=1;
    while(r<=resu[11]){opera[0][p]=resu[r];r++;p++;}
    opera[0][0]=resu[0];opera[0][10]=resu[10];opera[0][11]=p-1;
    while(opera[0][11]>=opera[0][10]&&opera[0][opera[0][11]]==0) opera[0][11]--;
    return 1;
  }
}


/****************************************************************************************
����:  ������������
����:  ���ݷ���λ,��������ú���
����:  state,save
����ֵ����
****************************************************************************************/


void calculator(void)
{
  char data state;
  uchar data save;
  save1=0;
  dis_title_e();
  display_cnasc(0x82,3,tool_menu[0]); 
  display_cnasc(0x96,2,"����");
  display_cnasc(0x8e,2,"ɾ��");
  display_cnasc(0x9e,2,"�˳�");
  removal_data(2,0); 	// 0Ϊ�����ʾ
  removal_data(0,0);
  i=0;j=1;
  while(1)
  {	
    cal_run=0;
    state=input_data();
    if(state==1)  // ����opera[1][0]����ֵ���ó���
    {
      if(opera[1][0]==12&&opera[0][0]==0||opera[1][0]==13&&opera[0][0]==1) 
	    state=adder();
      else if(opera[1][0]==12&&opera[0][0]==1||opera[1][0]==13&&opera[0][0]==0)
	    state=subtraction();
      else if(opera[1][0]==14) state=multiplication();
      else state=divider();
    }
	if(state==0)  return;
    i=0;j=1;
    cal_run=1;
    switch(state)
    {
      case -1: 						// ������
	          removal_data(2,0);
              removal_data(0,0);  
		      display_cnasc(0x8a,3,"���!!");
		      save1=0;
			  break;
      case  0: 	       				// ���Ϊ��
			  if(save1==0)
			  { 
			    removal_data(2,1);
				removal_data(0,1);
			    display_cnasc(0x98,1,"= ");
			    display_cnasc(0x9d,1," 0");
			  }
			  break;				// �����ʾ
      case  1: 
			  if(save1==0) 
			  { 
			    cal_run=1;
			    display_cnasc(0x98,1,"= ");
                dis_cdata(0x98);
			  }
			  break;
    }
	if(save1==0) save=gotkey();	 // save1��¼���������
	cal_run=0;	
	if((save>11&&save<16&&state==1)||save1!=0) 
	{
	  removal_data(2,0);
	  removal_data(1,0);
	  display_cnasc(0x90,6,"               ");
	  if(opera[0][11]==0) display_cnasc(0x95,1," 0");
	  else dis_cdata(0x90);
	  if(save1!=0)save=save1;
	  save1=0;
	  display_cnasc(0x88,1,tool_cal[save-12]);
	  opera[1][0]=save;
	  i=1;j=1;
	}
	else if(save>=0&&save<10&&state==1)
	{
	  removal_data(2,0);
	  removal_data(0,0);
	  opera[0][1]=save;
	  opera[0][11]++;
	  dis_cdata(0x90);
	  i=0;j=2;
	}
	else if(save==10)
	{  
	  removal_data(2,0);
	  removal_data(0,0);
      opera[i][10]=2;
	  opera[i][11]=1;
      j=2;
	}
	else 
	{ 
	  removal_data(2,0);
	  removal_data(0,0);
	}
  }											
}
  

/*=====================================================================================
 �������ƣ��¶ȹ���
 ������������������¶�
 ȫ�ֱ�����temprun  oth_run  als_temp  adju_si 
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


/*�¶���ʾ*/
uchar temp_func_dis(void)
{
  uchar save[2],k;
  clear_dis();
  dis_title_e();
  display_cnasc(0x82,3,"�¶ȼ�");  
  display_cnasc(0x98,3,"�趨");
  display_cnasc(0x9e,3,"�˳�");
  display_cnasc(0x8e,1," \x1f");
  num2_asc_dis(10,0x85);
  temprun=1;
  while(1)
  {
	if(ds18b20)
    {
      if(temp.t_data[0]>99)	  // �¶�ֵ����99ʱ
	  {
	    save[0]=temp.t_data[0]/100;
        save[1]=(temp.t_data[0]/10)%10;
        num2_asc_dis(save[0],0x80);
	    num2_asc_dis(save[1],0x81);
	    save[0]=char_num[temp.t_data[0]%10];
	    num2_asc_dis(save[0],0x82);
      }
	  else					 // �¶�С��100 
	  {
        con_disp(0x00,0x00,0x80,0x90,1,16);
		con_disp(0x00,0x00,0x88,0x90,1,16);
		if(temp_pn) con_disp(0x3f,0xfc,0x80,0x9d,1,3);
	    else  con_disp(0x00,0x00,0x80,0x9d,1,3);
	    save[0]=temp.t_data[0]/10;
	    save[1]=temp.t_data[0]%10;
	    num2_asc_dis(save[0],0x81);
	    num2_asc_dis(save[1],0x82);
	  }
	  save[0]='.';
	  save[1]=char_num[temp.t_data[1]/10];
	  display_cnasc(0x8b,1,save);
	  save[0]=char_num[temp.t_data[1]%10];
	  if(save[0]=='1') save[1]='2';		   // ��������
	  else if(save[0]=='7') save[1]='5';
	  else if(save[0]=='8'||save[0]=='1') save[1]='7';
	  else save[1]='0';
	  display_cnasc(0x8c,1,save);
    }
	else 
	{
      clear_dis();
      display_cnasc(0x92,4,"û�з���");
      display_cnasc(0x8c,4,"�¸�����");
	  delay_pw();
	  temprun=0;
	  return 0;
	}
	k=gotkey();
	if(k==2)	 // �������ò˵�
	{
	  display_cnasc(0x8e,1," \x04");
	  delay(35000);
	  display_cnasc(0x8e,1," \x1e");
	  display_cnasc(0x9a,4,"\x11 (ok) \x10");
	  return 1;
	}
  }
}

/*�����¶�����*/
uchar temp_func_set(void)
{
  uchar save[2];
  temprun=0;
  oth_run=0;
  rds1302_data();
  while(1)
  {  
	if(oth_run)
	{
	  con_disp (0x00,0x00,0x88,0x90,2,16);
	  con_disp (0xff,0xff,0x8e,0x90,2,16);
	}
	else
	{
	  con_disp (0xff,0xff,0x88,0x90,2,16);
	  con_disp (0x00,0x00,0x8e,0x90,2,16);
	}	
	save[0]=gotkey();
	if(save[0]==1||save[0]==3) oth_run=!oth_run; 
	if(save[0]==5) return 1;
	if(save[0]==11) 
	{  
	  if(oth_run) return 0;
	  else  
	  { 
		clear_dis();
		dis_title_e();
		save[1]=save[0]=0;
		oth_run=0;
	    dis_title_e();
        display_cnasc(0x82,3,"��  ��"); 
	    display_cnasc(0x90,5,"�����¶ȣ�");
		display_cnasc(0x88,7,"���ڡ�    ����"); 
		display_cnasc(0x98,8,"���ڡ�    ���� \x10"); 
		while(1)
		{
		  if(save[1]==2) con_disp(0x00,0x00,index_temp_set[save[1]].lnum,index_temp_set[save[1]].rnum,2,13);
		  else con_disp(0x00,0x00,index_temp_set[save[1]].lnum,index_temp_set[save[1]].rnum,2,13);
		  if(save[0]==2) con_disp(0x0f,0xff,index_temp_set[save[0]].lnum,index_temp_set[save[0]].rnum,1,13);
          else con_disp(0xff,0xff,index_temp_set[save[0]].lnum,index_temp_set[save[0]].rnum,2,13);
	      save[1]=save[0];
          adju_si=0;
		  oth_run=0;
		  while(1)
		  {
		    for(i=0;i<2;i++)
		    {
		      j=0x8b+i*16;
			  if(als_temp[i]==255) display_cnasc(j,2," off"); // 255ֵΪ�ر�ֵ
		      else 
			  {
			    save1=save[1];
			    if(als_temp[i]>127)	 // ����ֵ����127ʱ�¶�ֵΪ����
			    { 
				  display_cnasc(j,1,"- ");
				  save[0]=char_num[(als_temp[i]-128)/10];
				  save[1]=char_num[(als_temp[i]-128)%10];
				  display_cnasc(j+1,1,save);
			    }
			    else
			    {
				  save[0]=' ';
				  save[1]=char_num[als_temp[i]/100];
				  if(save[1]=='0') save[1]=' ';
				  display_cnasc(j,1,save);
				  save[0]=char_num[als_temp[i]/10%10];
				  save[1]=char_num[als_temp[i]%10];
				  display_cnasc(j+1,1,save);
			    }
			    save[1]=save1;
			  }
		    }		
			save[0]=gotkey();
			do		   // ������ŵȴ���ֵ�������һλ
			{
			  if(save[0]==13){ oth_run=1;display_cnasc(0x8b+save[1]*16,2,"-   ");}
			  else if(save[0]==12){ oth_run=0;display_cnasc(0x8b+save[1]*16,2,"    ");}
			  else break;
			  save[0]=gotkey();
			  if(save[0]>=0&&save[0]<10||save[0]==11) break;
			}while(1);
			if(save[0]==14){ als_temp[save[1]]=255;}  // �˺�ֱ�ӽ������¶ȹر�
			if(save[0]>=0&&save[0]<10&&save[1]<2)
		    {
			  if(adju_si)
		      { 
			    adju_si=0;
			    if(oth_run) als_temp[save[1]]&=0x7f;  // ��ȥ����λ
				if(als_temp[save[1]]>24&&!oth_run) als_temp[save[1]]=240;
        		else als_temp[save[1]]=als_temp[save[1]]*10+save[0];
				if(oth_run&&als_temp[save[1]]>50) als_temp[save[1]]=50; 		
				if(!oth_run&&als_temp[save[1]]>120) als_temp[save[1]]=120; 
				if(!oth_run&&als_temp[save[1]]<100) adju_si=1;	// ������ֵ����д
				if(oth_run&&als_temp[save[1]]<10)  adju_si=1;
			  }
			  else
			  { 
			    als_temp[save[1]]=save[0];
				adju_si=1;
			  }
			  if(oth_run) als_temp[save[1]]=als_temp[save[1]]|0x80;	 // д�����
			  else als_temp[save[1]]=als_temp[save[1]]&0x7f;
			}			
			if(save[0]==11)
			{
			  if(als_temp[1]!=255&&als_temp[0]!=255)	   // �������¶ȵ���
			  {
     			if(save[1]==0&&als_temp[0]<als_temp[1]){als_temp[0]=als_temp[1];}
			    if(save[1]==1&&als_temp[1]>als_temp[0]){als_temp[1]=als_temp[0];}
			  }
			  save[0]=save[1]+1;adju_si=0;break;}
	        if(save[0]==10&&save[1]!=0) {save[0]=save[1]-1;adju_si=0;break;}
		  }
		  oth_run=0;
		  if(save[0]==3&&save[1]==2) break;
		}
		save_y_or_n();
		if(oth_run)
		{ 
		  wds1302_data(d02_temp,als_temp[0]);
		  wds1302_data(d02_temp+2,als_temp[1]);
		}
		return 1;
	  }
	}
  }
}		   
			          
		 
/*�¶ȼ�������*/		 
void temp_func(void)
{		 
  uchar t;	
  while(1)
  {
    oth_run=1;
	t=temp_func_dis();
    if(t==1)
	{
	  t=temp_func_set();
	  if(t==0) return;
	}
    else return;
  }
} 
/*=====================================================================================
 �������ƣ����幦��
 �����������趨����
 ȫ�ֱ�����alarm_time adju_si
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


void alarm_func(void)
{
  uchar save[2],a,d;
  clear_dis();
  dis_title_e();
  display_cnasc(0x82,3,"��  ��");  
  display_cnasc(0x91,6,"״̬��\x11    \x10");
  display_cnasc(0x89,3,"���壺"); 
  display_cnasc(0x9f,1,"��");
  display_cnasc(0x90,1,"��");
  display_cnasc(0x8f,1,"��"); 
  adju_si=0; 
  a=0;d=0;
  save[0]=char_num[alarm_time[0]>>4];
  save[1]=char_num[alarm_time[0]&0x0f];
  display_cnasc(0x8c,1,save);
  display_cnasc(0x8d,1,": ");
  save[0]=char_num[alarm_time[1]>>4];
  save[1]=char_num[alarm_time[1]&0x0f];	
  display_cnasc(0x8e,1,save);
  do 
  {
    if(a==0)
	{    
	  display_cnasc(0x95,1,alm_sign[alm]);
      display_cnasc(0x94,1,"\x11 ");
      display_cnasc(0x96,1," \x10");
	  con_disp(0x00,0x00,index_alm[d-1].lnum,index_alm[d-1].rnum,1,14);
	}
    if(a>0&&a<4)
	{
	  if(d==0)
	  {
		display_cnasc(0x94,1,"  ");
        display_cnasc(0x96,1,"  ");
	  }
	  else 
	   con_disp(0x00,0x00,index_alm[d-1].lnum,index_alm[d-1].rnum,1,14);
	 con_disp(0xff,0xff,index_alm[a-1].lnum,index_alm[a-1].rnum,1,14);	
    }	 	  
	d=a;
	while(1)
	{
	  a=gotkey();
	  if((a==1||a==3)&&d==0)
	  { 
	    alm=!alm;
		if(a==1) display_cnasc(0x94,1,"\x09 ");
		else display_cnasc(0x96,1," \x09");
		delay(35000);
		if(a==1) display_cnasc(0x94,1,"\x11 ");
        else display_cnasc(0x96,1," \x10");
		display_cnasc(0x95,1,alm_sign[alm]);
	  }
	  if(d>0&&d<3&&a>=0&&a<10)
	  {
	    if(adju_si)
		{
		  alarm_time[d-1]<<=4;alarm_time[d-1]+=a;
		  if(d==1&&alarm_time[0]>0x23) alarm_time[0]=a;
		  else if(d==2&&alarm_time[1]>0x59) alarm_time[1]=a;
		  else adju_si=0;
		}
	    else  
		{
		  alarm_time[d-1]=a;adju_si=1;
		}
		save[0]=char_num[alarm_time[d-1]>>4];
	    save[1]=char_num[alarm_time[d-1]&0x0f];
	    display_cnasc(0x8c+(d-1)*2,1,save);
	  }
	  if(a==11){a=d+1;break;}
	  if(a==10&&d!=0){a=d-1;break;}
	}
	adju_si=0;
	if(a==4) 
	{ 
	  save_y_or_n();
	  wds1302_data(d02_signs,signs);
	  wds1302_data(d02_alm,alarm_time[0]);
	  wds1302_data(d02_alm+2,alarm_time[1]);
	  return;
	}
  }while(1);
}



/*=====================================================================================
 �������ƣ�����������
 ��������������  ʱ��  24��  
 ȫ�ֱ�����resu  opera
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/

/*��������*/
void nba_set(void)
{
  uchar nba,save1,key,save[2];
  clear_dis();
  dis_title_e(); 
  if(resu[7]==1) display_cnasc(0x82,3,"��  ʱ");
  else  
  {
    display_cnasc(0x82,3,"��  ��");
    opera[0][0]=0;
    opera[1][0]=1;
  }
  resu[6]=0;	// ����ֵ
  resu[0]=1;
  resu[1]=4;
  resu[2]=12;
  resu[3]=0;
  resu[4]=24;
  display_cnasc(0x90,7,"��1:��  ��2:��");
  display_cnasc(0x88,7,"24��:Y  ����:4");
  display_cnasc(0x98,8,"ÿ��ʱ��: 12����");
  save1=0;nba=0;
  while(1)
  {
    if(save1==4) display_cnasc(0x9c,1,":");
	else display_cnasc(index_nba[save1],1,"  ");
	if(nba==4)  display_cnasc(0x9c,1,":\x10");
	else display_cnasc(index_nba[nba],1,"\x11 "); 
	save1=nba;
	key=gotkey();	
	if(key==1||key==3)
	{	  
	  switch(nba)     // ������������
	  {
	    case 0:			    
		case 1:
		        if(key==1){if(opera[nba][0]>0) opera[nba][0]--;
				           else opera[nba][0]=7;}
				if(key==3){if(opera[nba][0]<7) opera[nba][0]++;
				           else opera[nba][0]=0;}
				display_cnasc(index_nba[nba]-1,1,nba_oppo[opera[nba][0]]);
				break;
		case 2:
				if(resu[0]>0) resu[0]=0;
				else resu[0]=1;
				if(resu[0]>0) 	display_cnasc(0x8a,1,":Y");
				else display_cnasc(0x8a,1,":N");
				break;
		case 3:
				if(key==1){if(resu[1]>1) resu[1]--;
				            else  resu[1]=4;}
			    if(key==3){if(resu[1]<4) resu[1]++;
			               else resu[1]=1;}
				save[0]=':';
				save[1]=char_num[resu[1]];
		       	display_cnasc(0x8e,1,save);
				break;
		case 4:
				if(key==1){if(resu[2]>0) resu[2]--;
				            else  resu[2]=60;}
			    if(key==3){if(resu[2]<60) resu[2]++;
			               else resu[2]=0;}
				save[0]=char_num[resu[2]/10];
				save[1]=char_num[resu[2]%10];
		       	display_cnasc(0x9d,1,save);
				resu[5]=resu[2];
				break;
      }
	}
	if(key==11) 
	{
	  nba++; if(nba==5) return;			   
	}
	if(key==10&&nba!=0)  nba--;
  }
}



/*��������Ԫ��*/
void nba_oth_dis(void)
{
  uchar save[2];
  if(resu[7]==1) display_cnasc(0x80,5,"��ʱʱ�䣺");
  else display_cnasc(0x80,5,"����ʱ�䣺");
  display_cnasc(0x98,1,nba_oppo[opera[0][0]]);
  display_cnasc(0x9e,1,nba_oppo[opera[1][0]]);
  display_cnasc(0x99,1,"��");
  display_cnasc(0x9f,1,"��");
  save[0]='0';
  save[1]=char_num[resu[6]+1];
  display_cnasc(0x9b,1,save);
  display_cnasc(0x9c,1,"st");
  con_disp(0xff,0xff,0x83,0x90,2,1); // ��������
  con_disp(0xff,0xff,0x8b,0x8f,2,1);
  con_disp(0x80,0x00,0x83,0x91,1,15);
  con_disp(0x00,0x01,0x84,0x91,1,15);
  con_disp(0x80,0x00,0x8b,0x80,1,15);
  con_disp(0x00,0x01,0x8c,0x80,1,15);
  nba_dis_time();
  if(resu[0]==0) display_cnasc(0x93,2,"\x07--\x07");
  display_cnasc(0x8b,2," S! ");
}



/*������������*/
void nba_timer(void)
{
  uchar save,key,nba=0;
  als=di0=1;alm=keb=di1=oth_run=0;
  time_init();  
  display_cnasc(0x82,3,"������");
  dis_title_e(); 
  removal_data(0,1); 
  while(1)	  // ʹ��˵��
  {
	display_cnasc(0x90,8,timer_tips[nba]);
	display_cnasc(0x88,8,timer_tips[nba+1]);
	display_cnasc(0x98,8,timer_tips[nba+2]);
	key=gotkey();
	if(key==11)
	{
	  nba+=3;
	  if(nba>9) break;
	}
	if(key==10)
	{
	  if(nba>0) nba-=3 ;
	}
  }
nn:
  nba_set();
  clear_dis();
  nba_oth_dis();
  nba_tim=1;
  while(1)
  {	
	for(save=0x80,nba=0;nba<2;nba++)
	{
	  if(opera[nba][1]>99)	// ������ʾ
	  {
	    num2_asc_dis(opera[nba][1]/100,save);
		num2_asc_dis(opera[nba][1]/10%10,save+1);
		num2_asc_dis(opera[nba][1]%10,save+2);
	  }
	  else
	  {
		num2_asc_dis(opera[nba][1]/10,save);
		num2_asc_dis(opera[nba][1]%10,save+1);
      }
	  if(opera[1][1]>99) save=0x85;
	  else save=0x86;
	}
	
	do
	{
	key=gotkey();
  ss:
	switch(key)
	{
	  case 12:
	  case 13:
	           if(key==12)
			     {if(opera[0][1]<250) opera[0][1]++;}
	           else 
			     {if(opera[0][1]>0) opera[0][1]--;}
			   save=110;
	 		   break;
	  case 14:
	  case 15:
	           if(key==14)
			     {if(opera[1][1]<250) opera[1][1]++;}
	           else 
			     {if(opera[1][1]>0) opera[1][1]--;}
			   save=110;
			   break;
	  case  7:
	           TR1=TR0=EA=0;
	           if((++resu[3])>59)
	           {
	             resu[3]=0;
		         if((++resu[2])>59)
				 { 
				   resu[3]=0;
				   if(++resu[2]>60) resu[2]=0;
				 }
			   }
		       if(resu[4]<24) resu[4]++;
			   nba_dis_time();
			   break;
	  case  8:
	           if(resu[0]==1)
			   { 
			     TL1=(time_count&0x00FF);
                 TH1=(time_count>>8);
			     resu[4]=24; nba_dis_time();
				 TR0=TR1=EA=1;
			   }
			   break;
	  case  9: 
	           if(resu[2]==0&&resu[3]==0)
	           {
	             resu[2]=resu[5];resu[4]=24;
				 if(resu[6]==resu[1])
				 {							  // �жϱȷֽ��м�ʱ��
				   if(opera[0][1]==opera[1][1]){nba_tim=0;resu[7]=1; goto nn;}
				   while(gotkey()!=10);		  // ����ֻ���˳�
				   signs=r1302(d02_signs+1); 
				   nba_tim=0;
				   return;
				 }
				 else nba_oth_dis();
				 break;
	           } 
			   if(EA) {TR0=TR1=EA=0;break;}
	           if(!EA&resu[4]!=0){ EA=TR0=1;if(resu[0]==1) TR1=1;}
			   break;														 
	  case 10:
	  case 11:
	           if(!EA)			// ��ͣ״̬�� ok�����η���ʱ�ӽ���
			   {                //  ./s �������˳�
			     delay(6000);
				 save=gotkey();
				 if(save==key)
				 {
				   if(key==11)
				   {
				     clear_dis();oth_run=1;
				     rds1302_time();
					 init_d2=1;
				     dis_mode2();
					 init_d2=0;
				     while(gotkey()!=11);
				     oth_run=0;
					 save=110;
				     clear_dis();
				     nba_oth_dis();
				   }
				   else{ signs=r1302(d02_signs+1); nba_tim=0;return;}
			     }
				 else{ key=save;goto ss;}
			   }
			   break;	  
	}
	if(save==110)  break;
    }while(1);	
  }  
}

	 
/*=====================================================================================
 �������ƣ����ù���
 ����������ϵͳ״̬����  
 ȫ�ֱ�����signs
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


/*���õ������� ������ʽ��ѡ���������������ֵ*/
uchar set_dis(uchar menu,uchar sub)
{
  uchar save;
  con_disp(0xff,0xff,0x8e,0x80,2,16);
  con_disp(0x00,0x00,0x8a,0x80,4,16);
  display_cnasc(0x96,1,"\x1e ");
  display_cnasc(0x9e,1,"\x1f ");
  display_cnasc(0x92,1,"  ");
  display_cnasc(0x9a,1,"  ");
  display_cnasc(0x8d,1,"\x11 ");
  display_cnasc(0x8a,1,"  ");
  while(1)
  {
    
    if(menu<2)
      display_cnasc(0x8e,2,set_bl_dm[menu][sub]);
    else
	  display_cnasc(0x8e,2,set_mka[menu-2][sub]);
	save=gotkey();
	if(save==5)
	{
	  key_dis(0x96);
	  if(sub==0&&menu<2) sub=2;
	  else if(sub==0&&menu>1) sub=1;
	  else sub--;
    }
	if(save==2)
	{
	  key_dis(0x9e);
	  if(sub==2&&menu<2) sub=0;
	  else if(sub==1&&menu>1) sub=0;
	  else sub++;
	}
	if(save==1||save==11)
	{
	  display_cnasc(0x8d,1,"\x04 ");
        delay(30000);
	  con_disp(0x00,0x00,0x8e,0x80,2,16);
      con_disp(0xff,0xff,0x8a,0x80,4,16);
      display_cnasc(0x92,1,"\x1e ");
      display_cnasc(0x9a,1,"\x1f ");
      display_cnasc(0x96,1,"  ");
      display_cnasc(0x9e,1,"  ");
	  display_cnasc(0x8a,1,"��");
	  display_cnasc(0x8d,3,"��\x10 \x04 ");
	  return(sub);	  // ��������ֵ
	}
  }
}


/*���õ���*/
void set_func(void)
{
  uchar d,key,save;
  display_cnasc(0x81,2,"����");
  dis_title();
  display_cnasc(0x88,8,selected[1]);
  con_disp(0xff,0xff,0x8a,0x80,4,16);
  save=d=0;
  do
  {	  
	display_cnasc(0x93,2,set_menu[index_s[save].lnum]);
	display_cnasc(0x8b,2,set_menu[save]);
	display_cnasc(0x9b,2,set_menu[index_s[save].rnum]); 
	d=save;
	while(1)
	{
	  key=gotkey();
	  if(key==11||key==3){ enter_dis();break;}
	  if(key==5){save=index_s[d].lnum;key_dis(0x92); break;}
	  if(key==2){save=index_s[d].rnum;key_dis(0x9a); break;}
	  if(key==1){ wds1302_data(d02_signs,signs); return;}
    }
	if(key==11||key==3)
	{
      save=0;
      for(key=0;key<2;key++)  //  �������αȽ� �״�Ϊ��ʱֵ ���Ϊ�趨�ø��� 
      {
        switch(d)
        {
          case 0: 
		          if(key==1)
		          {
				    bl0=bl1=0;
					if(save==1) bl0=1;
					if(save==2)	bl1=1;
		          }
	              else 
				  {if(bl0) save=1; if(bl1) save=2;}
				  if(bl1) lcd_bl=close;
				  else lcd_bl=open;
			      break;
	      case 1: 
		          if(key==1)
		          {
				    di0=di1=0;
					if(save==1) di0=1;
					if(save==2)	di1=1;
		          }
			      else {if(di0) save=1; if(di1) save=2;}
			      break;
	      case 2: 
		          if(key==1) meu=(bit)save;
	              else {if(meu) save++;}	      
			      break;
          case 3: 
		          if(key==1) keb=(bit)save;
	              else{if(keb) save++;}	      
			      break;
          case 4: 
		          if(key==1) als=(bit)save;
	              else{if(als) save++;}	      
			      break;
          case 5: wds1302_data(d02_signs,signs);
	              return;
        }
        if(key==0) save=set_dis(d,save);
      }
	  save=d;
	}
  }while(1);
}


/*=====================================================================================
 �������ƣ����ܹ���
 ����������ϵͳ����  ������  �������  
 ȫ�ֱ�����resu  
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/


/*��������*/
void passwork_finish()
{
  uchar i;
  for(i=0;i<6;i+=2)
  {
    resu[i]<<=4;
	resu[i]+=resu[i+1];
  }
}



/*��ȡ����*/
uchar input_passwork()
{
  uchar i,save[2];
  save[0]=' ';
  display_cnasc(0x9d,3,"(./S)\x10");
  for(i=0;i<6;i++)
  {
    while(1)
	{
	  resu[i]=gotkey();
	  if(resu[i]>=0&&resu[i]<10) break;
	  if(resu[i]==10) return 10;
	}
    save[1]=char_num[resu[i]];
	display_cnasc(0x89+i,1,save); 
	delay(20000);
	display_cnasc(0x89+i,1," *");
  }
  return 11;
}



/*����ȷ��*/
void input_second_passwork()
{
  uchar i,save[2];
  save[0]=' ';
  display_cnasc(0x89,6,"            ");
  for(i=0;i<6;i++)
  {    
	if(resu[i]!=gotkey())	break;
	save[1]=char_num[resu[i]];
	display_cnasc(0x89+i,1,save); 
	delay(20000);
	display_cnasc(0x89+i,1," *");
  }
  if(i!=6)
  {
    display_cnasc(0x89,6,"���벻��ͬ��");
	delay(50000);
	display_cnasc(0x9a,6,"����� \x10    ");
	while(gotkey()==0xf0);
	return;
  }
  else
  {
	passwork_finish();
	for(i=0;i<6;i+=2)
	  wds1302_data(d02_passw+i,resu[i]);
	for(i=0;i<6;i+=2)
	  if(resu[i]!=r1302(d02_passw+i+1)) break;
	if(i==6)
	 {display_cnasc(0x98,8,"\x10 ���ĳɹ�!(OK) ");
	  while(gotkey()!=11);return;}
	else
	  display_cnasc(0x9a,6,"����ʧ�ܣ�  ");
  }
  delay_pw();
}				



/*����Ƚ�*/
uchar passwork_comp()
{
  uchar i;
  passwork_finish();
  for(i=0;i<6;i+=2)
	if(resu[i]!=r1302(d02_passw+i+1)) break;
  if(i==6)	 return 1;
  else
  { 
    display_cnasc(0x89,6,"  �������");
	delay_pw();
	display_cnasc(0x89,6,"            ");
	return 110;
  }
}



/*���ܵ���*/
void passw_func(void)
{
  uchar save,d,key;
  d=0;save=0;
  do
  {
    clear_dis();
	display_cnasc(0x81,2,"����");
    dis_title();
    display_cnasc(0x88,8,selected[0]);
    con_disp(0xff,0xff,0x8a,0x80,5,16); 
    do
    {	  
	  display_cnasc(0x93,3,secrets_menu[index_p[save].lnum]);
	  display_cnasc(0x8b,3,secrets_menu[save]);
	  display_cnasc(0x9b,3,secrets_menu[index_p[save].rnum]);  
	  d=save;
	  while(1)
	  {
	    key=gotkey();
	    if(key==11||key==3){enter_dis();break;}
	    if(key==2){save=index_p[d].rnum; key_dis(0x9a);break;}
	    if(key==5){save=index_p[d].lnum; key_dis(0x92);break;}
		if(key==1){return;}
      }
      if(key==11||key==3)
	  {	
		if(d==3) return;		
		clear_dis();
		dis_title_e();
        switch(d)
        {
          case 0: 
		          display_cnasc(0x82,3,"������");
				  display_cnasc(0x90,5,"�������룺");
				  while(1)
				  {
				    key=input_passwork();
				    if(key==10) break;
				    key=passwork_comp();
				    if(key==1) 
		            {
				      display_cnasc(0x90,8,"������״̬������");
		              display_cnasc(0x88,8,"��1-������2-�ر�");
				      display_cnasc(0x98,8,"���Զ���ʱ30��  ");
					  pw_oc=open;
				      save1=i=0;   
				      lock_op=1;   // ������ǵȴ���ʱ
				      while(1)
				      {
					    key=gotkey(); // ����255Ϊ��ʱʱ�䵽��
					    if(key==255){ lock_op=0;pw_oc=close;break;}
					    if(key==1) 
					    {
					      display_cnasc(0x88,1,"��");
					      display_cnasc(0x98,1,"��");
					      lock_op=0;
					    }
					    if(key==2)
					    {
					      display_cnasc(0x88,1,"��");
						  display_cnasc(0x98,1,"��");
						  display_cnasc(0x8c,1,"��");
						  delay(60000);
						  clear_dis();
					      dis_title_e();
		                  display_cnasc(0x82,3,"������");
					      display_cnasc(0x90,5,"����������");
						  display_cnasc(0x8a,6," 5���رգ�");
					      for(save=0;save<5;save++)
					      { 
					        display_cnasc(0x99+save,1,"��"); 
						    delay_pw();
					      }
						  display_cnasc(0x9e,1,"��");
						  pw_oc=open;
						  delay(30000);
						  pw_oc=close;
					      lock_op=0;
					      break;
					    }
				      }
					  break;
				    }
				  } 
		 	      break;
	      case 1: 
		          display_cnasc(0x82,3,"��  ��");
				  display_cnasc(0x90,6,"��������룺");
				  while(1)
				  {
				    key=input_passwork();
				    if(key==10) break;
				    key=passwork_comp();
				    if(key==1)
				    {
				      display_cnasc(0x92,1,"��");
					  display_cnasc(0x89,6,"            ");
				      key=input_passwork();
					  if(key==10) break;
					  display_cnasc(0x90,6,"ȷ�����룺  ");
					  input_second_passwork();
					  break;		
				    }
				  }
				  break;
	      case 2: 
		          display_cnasc(0x82,3,"��ʼ��");
          		  display_cnasc(0x91,6,"�Ƿ��ʼ��? ");
                  display_cnasc(0x89,6,"Y(OK)\\N(./S)");
				  while(1)
				  {
				    key=gotkey();
					if(key==11)
					{
					  clear_dis();
					  dis_title_e();
					  display_cnasc(0x82,3,"��ʼ��");
					  display_cnasc(0x90,5,"�������룺");
					  do
					  {
					    key=input_passwork();
					    if(key==10) break;
					    key=passwork_comp();
					    if(key==1)
					    {
						  display_cnasc(0x90,5,"������ȷ��");
						  display_cnasc(0x89,7,"\x10 ��ʼ����... ");
						  init_ds1302();
						  rds1302_data();
						  delay_pw();
						  display_cnasc(0x9a,6,"\x10 ��ɣ�(OK)");
						  while(gotkey()!=11);
						  key=10;
						  break;
					    }
					  }while(1);
					}
					if(key==10) break;
				  }
				  break;
		}
		key=99;save=d;
	  }
	  if(key==99) break;
	}while(1);
  }while(1);
}


/*=====================================================================================
 �������ƣ��汾��Ϣ
 ����������  
 ȫ�ֱ�����
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/

void version_func(void)
{
  uchar save,v=0;
  display_cnasc(0x82,3,"��  ��");
  dis_title_e();
  do
  {
	display_cnasc(0x90,7,version[v]);
	display_cnasc(0x88,7,version[v+1]);
	display_cnasc(0x98,7,version[v+2]);
	if(v!=0) display_cnasc(0x97,1," \x1e");
	else if(v==0)
	  display_cnasc(0x97,1,"Ժ"); 
	else display_cnasc(0x97,1,"  ");
	if(v!=7) display_cnasc(0x9f,1," \x1f");
	else display_cnasc(0x9f,1,"  ");
	save=gotkey();
	if(save==2&&v<7)
	{
	  display_cnasc(0x9f,1," \x04");
      delay(30000);
	  v++;
	}
	if(save==5&&v>0)
	{
	  display_cnasc(0x97,1," \x04");
      delay(30000);
	  v--;
	}
	if(save==1) return;
	if((save==11||save==3)&&v==7) 
	{
	  display_cnasc(0x9e,2,") \x09 ");
      delay(30000);
	  return;	  
	}
  }while(1);
}



/*=====================================================================================
 �������ƣ�ϵͳ������
 ����������  
 ȫ�ֱ�����
 ����˵����������
 ����˵������
 �� �� �ˣ�LIJH
 ��    ����1.0
 ˵    ����
 ======================================================================================*/
main()
{
  uchar key; 							 
  init_lcd();						  // ��ʼ��lcd
  lcd_bl=als_tl=als_th=buzzer=pw_oc=close;
  key=r1302(d02_initsi+1);            // ����1302��ʼ�����λ
  if(bl1) lcd_bl=close;
  else lcd_bl=open;
  display_cnasc(0x92,4,"��ӭʹ��");   // ��ʾ�������桰��ӭʹ�á�
  display_cnasc(0x9d,3,"��LiJH");
  if(key!=0xb4)  init_ds1302();       // DS1302��ʼ��
  rds1302_data();					  // ����������������
  rds1302_date();
  rds1302_time();                     // ������ʱ��   
  if(!ds18b20) init_ds18b20();		  // ��ʼ��18b20
  if(ds18b20) 
  {  			                     
	ds18b20_func();					   // �����¶�
  } 
  time_init();                         // ��ʱ����ʼ��
  delay_pw();resu[11]=0; 
  while(1)
  {  
	EA=1;
	TR0=1;
	dis_sig=1;	
	clear_dis();
	if(di0){init_d2=1;dis_mode2();init_d2=0;}
    else if(di1) dis_mode3();
    else dis_mode1();	
	do
	{
	  key=gotkey();
	  if(key==255)			// ������������
	  {
		TR0=0;
		dis_sig=0;
		clear_dis();
		dis_title_e();
		display_cnasc(0x82,3,"������");
		display_cnasc(0x90,5,"�������룺");
		while(1)
		{
		  keb=0;
		  key=input_passwork();
		  if(key==10) {keb=1;resu[11]=2;break;}
		  key=passwork_comp();
		  if(key==1) {keb=1;resu[11]=0;break;} 	  
		}
		rds1302_date();
        rds1302_time();
	    save_time=now;
		break;
	  }
	  if(key==11)
	  {
	    dis_menu_key(1);
		delay(35000);
		do
		{
		  dis_sig=0;
		  TR0=0;
		  clear_dis();
		  key=dis_menu();
		  clear_dis();
		  if(key==0) adjust_func();
		  else if(key==1) // ������Ӧ������S
		  {
		    while(1)
			{
			  clear_dis();
		      key=tool_func();
			  if(key==4) break;
			  clear_dis(); 
			  switch(key)
			  {
			    case 0:	calculator();
				        break;
				case 1: temp_func();
				    	break;
				case 2: alarm_func();
				        break;
				case 3: nba_timer();
				        break;
			  }
			}
		  }
		  else if(key==2)	set_func();
		  else if(key==3)	passw_func();
		  else if(key==4) version_func();
		  else
		  {
		    rds1302_date();
            rds1302_time();
			save_time=now;
			resu[11]=0;
			dis_sig=1;
			break;
		  }
		}while(1);
	  }
	  if(dis_sig) break;
	}while(1);
  }
}
