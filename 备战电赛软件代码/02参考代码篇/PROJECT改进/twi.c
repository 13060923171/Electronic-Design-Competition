/*********************************************************************************************/ 
/********************�����Ƕ�ATmega16���Դ�I2C�����ʹ��**************************************/ 
/*********************************************************************************************/ 
#include "config.h"
#include "TWI.h"
               
/*
extern void Init_TWI(void) ;  															//TWI��ʼ��
extern uchar TWI_Send(uchar Slave,uchar Address,uchar Data);                                 				//��IIC��ַΪSlave�Ĵӻ���Address��ַ����(д)һ�ֽ�����Data
extern uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT) ;                                	//��IIC��ַΪSlave�Ĵӻ���Address��ַ��ʼ����N�ֽ�����Data 
extern uchar TWI_Receive(uchar Slave,uchar Address) ;                                                                          //��IIC��ַΪSlave�Ĵӻ���Address��ַ��ȡһ�ֽ�����Data������ֵΪ��ȡ������
*/
//************TWI��ʼ��*************/




/**************************************************** 
***************************************************** 
��IIC��ַΪSlave�Ĵӻ���Address��ַ����(д)һ�ֽ�����Data 
����0:д�ɹ� 
���ط�0:дʧ�� 
****************************************************** 
******************************************************/ 
uchar TWI_Send(uchar Slave,uchar Address,uchar Data) 
{ 
	
	TWCR=0x80|0x20|0x04;                                             //��:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);��TWINTд1�����ʹ��TWI������Start�ź� 
	while((TWCR&0x80)!=0x80) ;                                       //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ������Start�ź��ѷ��� 
	if((TWSR&0xf8)!=0x08)                                            //��:if ((TWSR & 0xF8) != START),���״̬�Ĵ���,����Ԥ��Ƶλ,���״̬�ֲ���STARTת������ 
	return(1);                                                       //����ֵ1�������ӻ�û�ж�Start�ź���Ӧ�� 

                                                                 //Slave��SLA+W,���Ǵӻ���ַ ,������0x18��0x20,�˴�ֻ��0x18 
                                                                 //��ν��Ӧ���ǡ������������յ���ַ�͡�д���󣬽�SDA��ѹ���ͣ��ɡ�����������ȡ 
                                                                 //�AATmge16�������������͡���ѹ�򷵻�0x18,���򷵻�0x20�� 
	TWDR=Slave;                                                      //���µ�ַ�Ĵ���,װSlave�뵽TWDR�Ĵ��� 
	TWCR=0x80|0x04;                                                  //��:TWCR = (1<<TWINT) | (1<<TWEN);��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ 
	while((TWCR&0x80)!=0x80) ;                                       //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ,����Slave�ź��ѷ��� 
	if((TWSR&0xf8)!=0x18)                                            //��:if ((TWSR & 0xF8) != MT_SLA_ACK),���״̬�Ĵ���, 
	return(2);                                                       //����ֵ2�������ӻ�û�ж�Slave�ź���Ӧ�� 

                                                                 //Address��MT_SLA_ACK�Ƿ�ط�������ַ,������0x28��0x30, �˴�ֻ��0x28 
	TWDR=Address; 
	TWCR=0x80|0x04;                                                  //��:TWCR = (1<<TWINT) | (1<<TWEN);��TWINTд1�����ʹ��TWI�������Ĵ�����ַ��Ϣ 
	while((TWCR&0x80)!=0x80) ;                                      //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ������Address�ź��ѷ��� 
	if((TWSR&0xf8)!=0x28)                                           //���״̬�Ĵ���(MT_DATA_ACK) 
	return(3);                                                      //����ֵ3�������ӻ�û�ж�Address�ź���Ӧ�� 

	TWDR=Data;                                                      //д���ݵ�ZLG7290 
	TWCR=0x80|0x04;                                                 //��:TWCR = (1<<TWINT) | (1<<TWEN);��TWINTд1�����ʹ��TWI������������Ϣ 
	while((TWCR&0x80)!=0x80) ;                                       //��:while (!(TWCR & (1<<TWINT)));�ȴ�TWINT��λ������Data�ź��ѷ��� 
	if((TWSR&0xf8)!=0x28)                                           //if ((TWSR & 0xF8) != MT_DATA_ACK);���״̬�Ĵ��� 
	return(4);                                                      //����ֵ3�������ӻ�û�ж�Data�ź���Ӧ�� 

	TWCR=0x80|0x04|0x10;                                            //��:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);��TWINTд1�����ʹ��TWI������Stop�ź� 
	return(0); 

} 





/****************************************************************** 
********************************************************************* 
��IIC��ַΪSlave�Ĵӻ���Address��ַ��ʼ����N�ֽ�����Data 
****************************************************************** 
*******************************************************************/

/*uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT) 
 {
    uchar Count; 
    TWCR=0x80|0x20|0x04;                                                      //��TWINTд1�����ʹ��TWI������Start�ź� 
    while((TWCR&0x80)!=0x80) ;                                                //�ȴ�TWINT��λ������Start�ź��ѷ��� 
    if((TWSR&0xf8)!=0x08)                                                     //���״̬�Ĵ��� 
    return(1);                                                                //����ֵ1�������ӻ�û�ж�Start�ź���Ӧ�� 

    TWDR=Slave;                                                               //���µ�ַ�Ĵ��� 
    TWCR=0x80|0x04;                                                           //��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ 
    while((TWCR&0x80)!=0x80) ;                                                //�ȴ�TWINT��λ������Slave�ź��ѷ��� 
    if((TWSR&0xf8)!=0x18)                                                     //���״̬�Ĵ��� 
    return(2);                                                                //����ֵ2�������ӻ�û�ж�Slave�ź���Ӧ�� 

    TWDR=Address; 
    TWCR=0x80|0x04;                                                            //��TWINTд1�����ʹ��TWI�������Ĵ�����ַ��Ϣ 
    while((TWCR&0x80)!=0x80) ;                                                 //�ȴ�TWINT��λ������Address�ź��ѷ��� 
    if((TWSR&0xf8)!=0x28)                                                      //���״̬�Ĵ��� 
    return(3);                                                                 //����ֵ3�������ӻ�û�ж�Address�ź���Ӧ�� 

    for(Count=0;Count<CNT;Count++)                                             //����дN���ֽ� ,ʵ�ַ���N�ֽ�����Data 
       {
		 TWDR=Array[Count];                                                    //��������װ��TWDR 
		 TWCR=0x80|0x04;                                                       //��(1<<TWINT)|(1<<TWEN);��TWINTд1�����ʹ��TWI������������Ϣ 
		 while((TWCR&0x80)!=0x80) ;                                            //�ȴ�TWINT��λ������Data�ź��ѷ��� 
		 if((TWSR&0xf8)!=0x28)                                                 //���״̬�Ĵ��� 
		 return(4);
	   }                                                                                 //����ֵ4�������ӻ�û�ж�Data�ź���Ӧ�� 

		TWCR=0x80|0x04|0x10;                                                  //��TWINTд1�����ʹ��TWI������Stop�ź� 
		return(0); 
 } 

 */
/********************************************************************** 
*********************************************************************** 
��IIC��ַΪSlave�Ĵӻ���Address��ַ��ȡһ�ֽ�����Data������ֵΪ��ȡ������ 
//����Ϊ��������ģʽ 
/*����������ģʽ���������ԴӴӻ��������ݣ�Ϊ��������ģʽ�����뷢��START�źš� 
�����ŵĵ�ַ����ʽ��������MT��MRģʽ�� ������� SLA+W ����MTģʽ���������SLA+R�����MRģʽ�� 

************************************************************************ 
***********************************************************************/ 
uchar TWI_Receive(uchar Slave,uchar Address) 
{//����Start 

	uchar Temp; 
	TWCR=0x80|0x20|0x04;                                                         //��:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);��TWINTд1�����ʹ��TWI������Start�ź� 
	while((TWCR&0x80)!=0x80) ;													 //�ȴ�TWINT��λ������Start�ź��ѷ��� 
	if((TWSR&0xf8)!=0x08) 														 //���״̬�Ĵ���,{0x08��ʾ:������TWSR״̬��(��Ԥ��ƵλΪ"0"�����)} 
	return(1); 																	 //����ֵ1�������ӻ�û�ж�Start�ź���Ӧ�� 
//����Slave��W 
	TWDR=Slave; 																//���µ�ַ�Ĵ��� 
	TWCR=0x80|0x04;																 //��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ 
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Slave�ź��ѷ��� 
	if((TWSR&0xf8)!=0x18) 														//���״̬�Ĵ���,(0x18��ʾ:SLA+W�ӻ��ѷ���,���յ�ACK) 
	return(2); 																	//����ֵ2�������ӻ�û�ж�Slave�ź���Ӧ�� 
//����Address 
	TWDR=Address; 
	TWCR=0x80|0x04; 															//��TWINTд1�����ʹ��TWI�������Ĵ�����ַ��Ϣ 
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Address�ź��ѷ��� 
	if((TWSR&0xf8)!=0x28) 														//���״̬�Ĵ���,(0x28��ʾ:���ػ���ַ�ѷ���,���յ�ACK) 
	return(3); 																	//����ֵ3�������ӻ�û�ж�Address�ź���Ӧ�� 

//�������γ�������������ʽ��,���¾��ǽ����źŵĶ��� 

//����Start,��������TWI 
	TWCR=0x80|0x20|0x04; 														//��TWINTд1�����ʹ��TWI������Start�ź� 
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Start�ź��ѷ��� 
	if((TWSR&0xf8)!=0x10) 									
	return(4);																			/*���״̬�Ĵ���,��Repeat Start (״̬0x10) �����߽ӿڿ����ٴη��� 
																				��ͬ�Ĵӻ����򲻷���STOP�ź��������µĴӻ���REPEATED STARTʹ������ 
																				�����ڲ���ʧ���߿��Ƶ��������ڴӻ�������������������������ģʽ������л���*/ 
																				//����ֵ4�������ӻ�û�ж�Repeat Start�ź���Ӧ�� 
//����Slave��R 
	TWDR=Slave+1; 																//���µ�ַ�Ĵ��� 
	TWCR=0x80|0x04; 															//��TWINTд1�����ʹ��TWI�������ӻ���ַ��Ϣ 
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ������Slave�ź��ѷ��� 
	if((TWSR&0xf8)!=0x40) 														//���״̬�Ĵ���,(0x40��ʾ:SLA+R(MR_SLA_ACK)�ѷ���,���յ�ACK) 
	return(5); 																	//����ֵ5�������ӻ�û�ж�Slave+R�ź���Ӧ�� 

	TWCR=0x80|0x04;																 //��TWINTд1�����ʹ��TWI���������� 
	while((TWCR&0x80)!=0x80) ; 													//�ȴ�TWINT��λ�����������Ѿ����ս��� 
	if((TWSR&0xf8)!=0x50) ; 													//���״̬�Ĵ���,(0x50��ʾ:���յ�����,ACK�ѷ���) 
// return(6); 																		//����ֵ3�������ӻ�û�ж�Address�ź���Ӧ�� 

	Temp=TWDR; //���������� 

	TWCR=0x80|0x04; 															//����NACK�ź� 

	TWCR=0x80|0x04|0x10; 														//����Stop�ź�,��:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
	delay_ms(1); 
	TWCR=0x80;																	 //���TWINT�ͽ�ֹTWI(������������ֻ�ܶ�һ��) 
	return(Temp); 
	
} 

