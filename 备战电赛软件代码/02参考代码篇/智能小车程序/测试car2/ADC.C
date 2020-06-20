#include <msp430x16x.h>

#define uchar unsigned char
#define uint unsigned int
#define MAX_SAMPLE_TIMES 32    //采样次数

uchar num[]={'0','1','2','3','4','5','6','7','8','9','.'};  //用于输出AD转换值
uint Result[8]={0,0,0,0,0,0,0,0};  //输出
uint Temp[8]={0,0,0,0,0,0,0,0};   //临时存放数据
unsigned long Sum_results[8]={0,0,0,0,0,0,0,0};   //计算用
uint Count=0;
uint Adc_Flag=0;  //转换结束标志 1为结束

/*******************************************
函数名称：ADC12_Init()
功    能：ADC12初始化
参    数：无
返回值  ：无
********************************************/
void ADC12_Init()
{
  uint i;
  P6SEL|=0XFF;    //定义通道0~7
  ADC12CTL0=ADC12ON+MSC+SHT0_2+REFON+REF2_5V;
  for(i=0xffff;i>0;i--);//参考电压建立时间
  ADC12CTL1=CSTARTADD_0+ADC12SSEL_0 + SHP+CONSEQ_1;//序列通道，单多路次转换模式
  ADC12MCTL0 = SREF_1 + INCH_0 ;              //选择输入通道A0；参考电压:V+=Vref+,V-=AVss
  ADC12MCTL1 = SREF_1 + INCH_1+EOS ;              //通道A1
  /*ADC12MCTL2 = SREF_1 + INCH_2 ;              //通道A2
  ADC12MCTL3 = SREF_1 + INCH_3 ;              //通道A3
  ADC12MCTL4 = SREF_1 + INCH_4 ;              //通道A4
  ADC12MCTL5 = SREF_1 + INCH_5 ;              //通道A5
  ADC12MCTL6 = SREF_1 + INCH_6 ;              //通道A6
  ADC12MCTL7 = SREF_1 + EOS + INCH_7 ;        //通道A7，最后序列通道
  ADC12IE = 0x80 ;                       //使能中断ADC12IFG.7*/
  ADC12IE=0x02;                          //使能中断ADC12IFG.1*/
}


/*******************************************
函数名称：ADC12_Deal
功    能：开始ADC12转换
参    数：无
返回值  ：无
********************************************/
void ADC12_Deal(void)
{
  Adc_Flag=0;
  ADC12CTL0 |= ADC12SC + ENC;                                 //开始转换；转换器使能
  while(Adc_Flag==0)
    ADC12CTL0|=ADC12SC;
  if (Adc_Flag==1)
    ADC12CTL0&=~ENC;                                    //关闭转换
}



/*******************************************
函数名称：ADC_Init()
功    能：AD单通道单次转换设置
参    数：无
返回值  ：无
********************************************/
/*void ADC_Init(void)
{
    int i;
    //多路开关选择光敏元件
    
    //初始化AD转换
    P6SEL|=BIT7;
    ADC12CTL0 = 0x0070;             // 4周期采样（默认），2.5V基准，ADC允许，不启动？
    ADC12CTL1 = 0x0200;             // Use sampling timer
    ADC12MCTL0=0x10;               //选择输入通道A0；参考电压:V+=Vref+,V-=AVss
    for ( i=0; i<0x3600; i++);                // Delay for reference start-up
    ADC12CTL0 |= ENC;                         // Enable conversions
    ADC12CTL0 |= ADC12SC;           //启动第1次转换
}*/


/*******************************************
函数名称：Trans_val
功    能：将16进制ADC转换数据变换成三位10进制
          真实的模拟电压数据，并在液晶上显示
参    数：Hex_Val--16进制数据       
返回值  ：无
********************************************/
/*void Trans_val(uint Hex_Val)
{
    uchar a[3];
    long x;
    uint Volt;
    x=Hex_Val;
    Volt=x*250/4096;
    a[0]=Volt/100;
    a[1]=Volt/10%10;
    a[2]=Volt%10;               //顺序写入
    Write_Data(num[a[0]]);    //个位
    Write_Data(num[10]);      //小数点
    Write_Data(num[a[1]]);    
    Write_Data(num[a[2]]);
}
*/

/*******************************************
函数名称：__interrupt void ADC12_IRQ
功    能：ADC12中断服务子程序
参    数：无
返回值  ：无
********************************************/
#pragma vector = ADC_VECTOR                            
__interrupt void ADC12_IRQ(void)       
{   
    Count++ ;
    Temp[0] = ADC12MEM0 ;                                 //存结果,清除IFG
    Temp[1] = ADC12MEM1 ;                                         
    /*Temp[2] = ADC12MEM2 ;
    Temp[3] = ADC12MEM3 ;
    Temp[4] = ADC12MEM4 ;
    Temp[5] = ADC12MEM5 ; 
    Temp[6] = ADC12MEM6 ;
    Temp[7] = ADC12MEM7 ;*/
    if (Count <= MAX_SAMPLE_TIMES)         //没有达到最大采样值
    {

    Sum_results[0] += Temp[0];
    Sum_results[1] += Temp[1];
    /*Sum_results[2] += Temp[2];
    Sum_results[3] += Temp[3];
    Sum_results[4] += Temp[4];
    Sum_results[5] += Temp[5];
    Sum_results[6] += Temp[6];
    Sum_results[7] += Temp[7];*/
    }
    if (Count == MAX_SAMPLE_TIMES)           //达到最大采样值
   {
    Result[0] = Sum_results[0] / MAX_SAMPLE_TIMES;
    Result[1] = Sum_results[1] / MAX_SAMPLE_TIMES;
    /*Result[2] = Sum_results[2] / MAX_SAMPLE_TIMES;
    Result[3] = Sum_results[3] / MAX_SAMPLE_TIMES;
    Result[4] = Sum_results[4] / MAX_SAMPLE_TIMES;
    Result[5] = Sum_results[5] / MAX_SAMPLE_TIMES;
    Result[6] = Sum_results[6] / MAX_SAMPLE_TIMES;
    Result[7] = Sum_results[7] / MAX_SAMPLE_TIMES;*/
    Count=0;
    Adc_Flag=1;
    Sum_results[0] = 0;
    Sum_results[1] = 0;
    /*Sum_results[2] = 0;
    Sum_results[3] = 0;
    Sum_results[4] = 0;
    Sum_results[5] = 0;
    Sum_results[6] = 0;
    Sum_results[7] = 0;*/
    //ADC12CTL0 &= ~ENC ;                                        //关闭转换
   }
 }