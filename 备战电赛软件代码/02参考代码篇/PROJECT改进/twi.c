/*********************************************************************************************/ 
/********************以下是对ATmega16的自带I2C定义和使用**************************************/ 
/*********************************************************************************************/ 
#include "config.h"
#include "TWI.h"
               
/*
extern void Init_TWI(void) ;  															//TWI初始化
extern uchar TWI_Send(uchar Slave,uchar Address,uchar Data);                                 				//向IIC地址为Slave的从机的Address地址发送(写)一字节数据Data
extern uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT) ;                                	//向IIC地址为Slave的从机的Address地址开始发送N字节数据Data 
extern uchar TWI_Receive(uchar Slave,uchar Address) ;                                                                          //从IIC地址为Slave的从机的Address地址读取一字节数据Data，返回值为读取的数据
*/
//************TWI初始化*************/




/**************************************************** 
***************************************************** 
向IIC地址为Slave的从机的Address地址发送(写)一字节数据Data 
返回0:写成功 
返回非0:写失败 
****************************************************** 
******************************************************/ 
uchar TWI_Send(uchar Slave,uchar Address,uchar Data) 
{ 
	
	TWCR=0x80|0x20|0x04;                                             //或:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);对TWINT写1清除，使能TWI，发出Start信号 
	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Start信号已发出 
	if((TWSR&0xf8)!=0x08)                                            //或:if ((TWSR & 0xF8) != START),检测状态寄存器,屏蔽预分频位,如果状态字不是START转出错处理 
	return(1);                                                       //返回值1，表明从机没有对Start信号作应答 

                                                                 //Slave即SLA+W,即是从机地址 ,可以是0x18或0x20,此处只用0x18 
                                                                 //所谓的应答是“从器件”在收到地址和“写”后，将SDA电压拉低，由“主器件”读取 
                                                                 //ATmge16如果读到这个“低”电压则返回0x18,否则返回0x20。 
	TWDR=Slave;                                                      //更新地址寄存器,装Slave入到TWDR寄存器 
	TWCR=0x80|0x04;                                                  //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出从机地址信息 
	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位,表明Slave信号已发出 
	if((TWSR&0xf8)!=0x18)                                            //或:if ((TWSR & 0xF8) != MT_SLA_ACK),检测状态寄存器, 
	return(2);                                                       //返回值2，表明从机没有对Slave信号作应答 

                                                                 //Address是MT_SLA_ACK是否控发送器地址,可以是0x28或0x30, 此处只用0x28 
	TWDR=Address; 
	TWCR=0x80|0x04;                                                  //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出寄存器地址信息 
	while((TWCR&0x80)!=0x80) ;                                      //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Address信号已发出 
	if((TWSR&0xf8)!=0x28)                                           //检测状态寄存器(MT_DATA_ACK) 
	return(3);                                                      //返回值3，表明从机没有对Address信号作应答 

	TWDR=Data;                                                      //写数据到ZLG7290 
	TWCR=0x80|0x04;                                                 //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出数据信息 
	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Data信号已发出 
	if((TWSR&0xf8)!=0x28)                                           //if ((TWSR & 0xF8) != MT_DATA_ACK);检测状态寄存器 
	return(4);                                                      //返回值3，表明从机没有对Data信号作应答 

	TWCR=0x80|0x04|0x10;                                            //或:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);对TWINT写1清除，使能TWI，发出Stop信号 
	return(0); 

} 





/****************************************************************** 
********************************************************************* 
向IIC地址为Slave的从机的Address地址开始发送N字节数据Data 
****************************************************************** 
*******************************************************************/

/*uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT) 
 {
    uchar Count; 
    TWCR=0x80|0x20|0x04;                                                      //对TWINT写1清除，使能TWI，发出Start信号 
    while((TWCR&0x80)!=0x80) ;                                                //等待TWINT置位，表明Start信号已发出 
    if((TWSR&0xf8)!=0x08)                                                     //检测状态寄存器 
    return(1);                                                                //返回值1，表明从机没有对Start信号作应答 

    TWDR=Slave;                                                               //更新地址寄存器 
    TWCR=0x80|0x04;                                                           //对TWINT写1清除，使能TWI；发出从机地址信息 
    while((TWCR&0x80)!=0x80) ;                                                //等待TWINT置位，表明Slave信号已发出 
    if((TWSR&0xf8)!=0x18)                                                     //检测状态寄存器 
    return(2);                                                                //返回值2，表明从机没有对Slave信号作应答 

    TWDR=Address; 
    TWCR=0x80|0x04;                                                            //对TWINT写1清除，使能TWI；发出寄存器地址信息 
    while((TWCR&0x80)!=0x80) ;                                                 //等待TWINT置位，表明Address信号已发出 
    if((TWSR&0xf8)!=0x28)                                                      //检测状态寄存器 
    return(3);                                                                 //返回值3，表明从机没有对Address信号作应答 

    for(Count=0;Count<CNT;Count++)                                             //连续写N个字节 ,实现发送N字节数据Data 
       {
		 TWDR=Array[Count];                                                    //建立数组装载TWDR 
		 TWCR=0x80|0x04;                                                       //或(1<<TWINT)|(1<<TWEN);对TWINT写1清除，使能TWI；发出数据信息 
		 while((TWCR&0x80)!=0x80) ;                                            //等待TWINT置位，表明Data信号已发出 
		 if((TWSR&0xf8)!=0x28)                                                 //检测状态寄存器 
		 return(4);
	   }                                                                                 //返回值4，表明从机没有对Data信号作应答 

		TWCR=0x80|0x04|0x10;                                                  //对TWINT写1清除，使能TWI，发出Stop信号 
		return(0); 
 } 

 */
/********************************************************************** 
*********************************************************************** 
从IIC地址为Slave的从机的Address地址读取一字节数据Data，返回值为读取的数据 
//以下为主机接收模式 
/*在主机接收模式，主机可以从从机接收数据，为进入主机模式，必须发送START信号。 
紧接着的地址包格式决定进入MT或MR模式。 如果发送 SLA+W 进入MT模式；如果发送SLA+R则进入MR模式。 

************************************************************************ 
***********************************************************************/ 
uchar TWI_Receive(uchar Slave,uchar Address) 
{//发送Start 

	uchar Temp; 
	TWCR=0x80|0x20|0x04;                                                         //或:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);对TWINT写1清除；使能TWI；发出Start信号 
	while((TWCR&0x80)!=0x80) ;													 //等待TWINT置位，表明Start信号已发出 
	if((TWSR&0xf8)!=0x08) 														 //检测状态寄存器,{0x08表示:主机的TWSR状态码(在预分频位为"0"情况下)} 
	return(1); 																	 //返回值1，表明从机没有对Start信号作应答 
//发送Slave＋W 
	TWDR=Slave; 																//更新地址寄存器 
	TWCR=0x80|0x04;																 //对TWINT写1清除，使能TWI；发出从机地址信息 
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Slave信号已发出 
	if((TWSR&0xf8)!=0x18) 														//检测状态寄存器,(0x18表示:SLA+W从机已发送,接收到ACK) 
	return(2); 																	//返回值2，表明从机没有对Slave信号作应答 
//发送Address 
	TWDR=Address; 
	TWCR=0x80|0x04; 															//对TWINT写1清除，使能TWI；发出寄存器地址信息 
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Address信号已发出 
	if((TWSR&0xf8)!=0x28) 														//检测状态寄存器,(0x28表示:主控机地址已发送,接收到ACK) 
	return(3); 																	//返回值3，表明从机没有对Address信号作应答 

//上面三段程序定义是主机方式后,以下就是接收信号的定义 

//发送Start,重新启动TWI 
	TWCR=0x80|0x20|0x04; 														//对TWINT写1清除；使能TWI；发出Start信号 
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Start信号已发出 
	if((TWSR&0xf8)!=0x10) 									
	return(4);																			/*检测状态寄存器,在Repeat Start (状态0x10) 后，两线接口可以再次访问 
																				相同的从机，或不发送STOP信号来访问新的从机。REPEATED START使得主机 
																				可以在不丢失总线控制的条件下在从机、主机发送器及主机接收器模式间进行切换。*/ 
																				//返回值4，表明从机没有对Repeat Start信号作应答 
//发送Slave＋R 
	TWDR=Slave+1; 																//更新地址寄存器 
	TWCR=0x80|0x04; 															//对TWINT写1清除，使能TWI；发出从机地址信息 
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Slave信号已发出 
	if((TWSR&0xf8)!=0x40) 														//检测状态寄存器,(0x40表示:SLA+R(MR_SLA_ACK)已发送,接收到ACK) 
	return(5); 																	//返回值5，表明从机没有对Slave+R信号作应答 

	TWCR=0x80|0x04;																 //对TWINT写1清除，使能TWI；接受数据 
	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明数据已经接收接受 
	if((TWSR&0xf8)!=0x50) ; 													//检测状态寄存器,(0x50表示:接收到数据,ACK已返回) 
// return(6); 																		//返回值3，表明从机没有对Address信号作应答 

	Temp=TWDR; //读接收数据 

	TWCR=0x80|0x04; 															//发出NACK信号 

	TWCR=0x80|0x04|0x10; 														//发出Stop信号,或:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
	delay_ms(1); 
	TWCR=0x80;																	 //清除TWINT和禁止TWI(不加上这句程序只能读一次) 
	return(Temp); 
	
} 

