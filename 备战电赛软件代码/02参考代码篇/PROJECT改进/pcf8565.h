#ifndef _pcf8565_H__
#define _pcf8565_H__
//extern    unsigned char Read_PCF8563T(unsigned char Address);
extern    void Write_PCF8563T(unsigned char Address , char date);
extern   void start_PCF( void );
extern    char Read_sec( void);
extern   char Read_miu( void );

#endif
