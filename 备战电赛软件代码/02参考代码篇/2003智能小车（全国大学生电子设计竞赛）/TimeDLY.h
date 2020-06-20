#ifndef  _TIMEDLY_H_
#define  _TIMEDLY_H_


//函数功能：短延时
void nNop(INT8U i);
	
//函数功能：长延时
void LDelay(INT16U i);

//函数功能：定时器T0溢出做延时
//说明：temp为延时，单位为：100μs(微秒)
void T0_Delay(INT16U temp);

#endif  //_TIMEDLY_H_