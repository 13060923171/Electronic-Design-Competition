#include  "config.h"  

#define    sec    0X02
#define   miu    0X03
#define   com   0X00  

 /* uchar Read_PCF8563T(uchar Address)
 {
   uchar temp;
   temp=TWI_Receive(0xA2, Address);
   return temp;
 }  */
 
  void Write_PCF8563T(uchar Address , uchar date)
 {
 	 
  TWI_Send(0xA2, Address, date); 
	 
 } 
void start_PCF( void )
{
 Write_PCF8563T( com , 0x00 );
 delay_ms(2);
 Write_PCF8563T( sec , 0x00 ); 
 delay_ms(2);
 Write_PCF8563T( miu , 0x00 );
}

uchar Read_sec( void )
{
   uchar temp;
   temp=TWI_Receive(0xA2, sec);
   return temp;         
}                                               
uchar Read_miu( void )
{
   uchar temp;
   temp=TWI_Receive(0xA2, miu);
   return temp;
}
