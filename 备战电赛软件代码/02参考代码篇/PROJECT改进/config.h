//未使用的函数建议注释掉,同时把头文件里的函数定义注释掉

#ifndef __config_H__
#define __config_H__

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef XTAL
#define XTAL 1         
#endif

#ifndef F_CPU
#define F_CPU 1000000
#endif
 
 
#define  TWPS1    1
#define  TWPS0    0

#define  TWINT    7
#define  TWEA     6
#define  TWSTA    5
#define  TWSTO    4
#define  TWWC     3
#define  TWEN     2
#define  TWIE     0

#define  TWGCE    0


/* ADC */

#define  ADEN     7
#define  ADSC     6
#define  ADATE    5
#define  ADFR     5    /*This definition was in old datasheet*/
#define  ADIF     4
#define  ADIE     3
#define  ADPS2    2
#define  ADPS1    1
#define  ADPS0    0
#define  REFS1    7
#define  REFS0    6
#define  ADLAR    5
#define  MUX4     4
#define  MUX3     3
#define  MUX2     2
#define  MUX1     1
#define  MUX0     0

/* Analog Comparator */

#define  ACD      7
#define  ACBG     6
#define  ACO      5
#define  ACI      4
#define  ACIE     3
#define  ACIC     2
#define  ACIS1    1
#define  ACIS0    0

/* USART */

#define  URSEL    7
#define  UMSEL    6
#define  UPM1     5
#define  UPM0     4
#define  USBS     3
#define  UCSZ1    2
#define  UCSZ0    1
#define  UCPOL    0
#define  RXCIE    7
#define  TXCIE    6
#define  UDRIE    5
#define  RXEN     4
#define  TXEN     3
#define  UCSZ2    2
#define  RXB8     1
#define  TXB8     0
#define  RXC      7
#define  TXC      6
#define  UDRE     5
#define  FE       4
#define  DOR      3
#define  OVR      3    /*This definition differs from the databook*/
#define  PE       2
#define  U2X      1
#define  MPCM     0

/* SPI */
#define  SPIE     7
#define  SPE      6
#define  DORD     5
#define  MSTR     4
#define  CPOL     3
#define  CPHA     2
#define  SPR1     1
#define  SPR0     0
#define  SPIF     7
#define  WCOL     6
#define  SPI2X    0



/* EEPROM */
#define  EERIE    3
#define  EEMWE    2
#define  EEWE     1
#define  EERE     0


/* Watchdog Timer Control Register */
#define  WDTOE    4
#define  WDE      3
#define  WDP2     2
#define  WDP1     1
#define  WDP0     0

/* Timer/Counter 2 */
#define  AS2      3
#define  TCN2UB   2
#define  OCR2UB   1
#define  TCR2UB   0
#define  FOC2     7
#define  WGM20    6
#define  COM21    5
#define  COM20    4
#define  WGM21    3
#define  CS22     2
#define  CS21     1
#define  CS20     0

/* Timer/Counter 1 */

#define  ICNC1    7
#define  ICES1    6
#define  WGM13    4
#define  WGM12    3
#define  CS12     2
#define  CS11     1
#define  CS10     0
#define  COM1A1   7
#define  COM1A0   6
#define  COM1B1   5
#define  COM1B0   4
#define  FOC1A    3
#define  FOC1B    2
#define  WGM11    1
#define  WGM10    0

/* Special Function I/O register */
#define  ADTS2    7
#define  ADTS1    6
#define  ADTS0    5
#define  ADHSM    4
#define  ACME     3
#define  PUD      2
#define  PSR2     1
#define  PSR10    0



/* Timer/Counter 0 */
#define  FOC0     7
#define  WGM00    6
#define  COM01    5
#define  COM00    4
#define  WGM01    3
#define  CS02     2
#define  CS01     1
#define  CS00     0

/* MCU general */
#define	 JTD      7
#define	 ISC2     6
#define	 JTRF     4
#define	 WDRF     3
#define  BORF     2
#define  EXTRF    1
#define  PORF     0
#define  SM2      7
#define  SE       6
#define  SM1      5
#define  SM0      4
#define  ISC11    3
#define  ISC10    2
#define  ISC01    1
#define  ISC00    0

/* SPM Conrol Register */
#define  SPMIE    7
#define  RWWSB    6
#define  RWWSRE   4
#define  BLBSET   3
#define  PGWRT    2
#define  PGERS    1
#define  SPMEN    0

/* Timer/Counter Interrupts */
#define  OCF2     7
#define  TOV2     6
#define  ICF1     5
#define  OCF1A    4
#define  OCF1B    3
#define  TOV1     2
#define  OCF0     1
#define  TOV0     0
#define  OCIE2    7
#define  TOIE2    6
#define  TICIE1   5
#define  OCIE1A   4
#define  OCIE1B   3
#define  TOIE1    2
#define  OCIE0    1
#define  TOIE0    0

/* General Interrupts */
#define  INTF1    7
#define  INTF0    6
#define  INTF2    5
#define  INT1     7
#define  INT0     6
#define  INT2     5
#define  IVSEL    1
#define  IVCE     0





/* Port A bits */
#define  PORTA7   7
#define  PORTA6   6
#define  PORTA5   5
#define  PORTA4   4
#define  PORTA3   3
#define  PORTA2   2
#define  PORTA1   1
#define  PORTA0   0
#define  PA7      7
#define  PA6      6
#define  PA5      5
#define  PA4      4
#define  PA3      3
#define  PA2      2
#define  PA1      1
#define  PA0      0

#define  DDA7     7
#define  DDA6     6
#define  DDA5     5
#define  DDA4     4
#define  DDA3     3
#define  DDA2     2
#define  DDA1     1
#define  DDA0     0

#define  PINA7    7
#define  PINA6    6
#define  PINA5    5
#define  PINA4    4
#define  PINA3    3
#define  PINA2    2
#define  PINA1    1
#define  PINA0    0

/* Port B bits */
#define  PORTB7   7
#define  PORTB6   6
#define  PORTB5   5
#define  PORTB4   4
#define  PORTB3   3
#define  PORTB2   2
#define  PORTB1   1
#define  PORTB0   0
#define  PB7      7
#define  PB6      6
#define  PB5      5
#define  PB4      4
#define  PB3      3
#define  PB2      2
#define  PB1      1
#define  PB0      0

#define  DDB7     7
#define  DDB6     6
#define  DDB5     5
#define  DDB4     4
#define  DDB3     3
#define  DDB2     2
#define  DDB1     1
#define  DDB0     0

#define  PINB7    7
#define  PINB6    6
#define  PINB5    5
#define  PINB4    4
#define  PINB3    3
#define  PINB2    2
#define  PINB1    1
#define  PINB0    0

/* Port C bits */
#define  PORTC7   7
#define  PORTC6   6
#define  PORTC5   5
#define  PORTC4   4
#define  PORTC3   3
#define  PORTC2   2
#define  PORTC1   1
#define  PORTC0   0
#define  PC7      7
#define  PC6      6
#define  PC5      5
#define  PC4      4
#define  PC3      3
#define  PC2      2
#define  PC1      1
#define  PC0      0

#define  DDC7     7
#define  DDC6     6
#define  DDC5     5
#define  DDC4     4
#define  DDC3     3
#define  DDC2     2
#define  DDC1     1
#define  DDC0     0

#define  PINC7    7
#define  PINC6    6
#define  PINC5    5
#define  PINC4    4
#define  PINC3    3
#define  PINC2    2
#define  PINC1    1
#define  PINC0    0

/* Port D bits */
#define  PORTD7   7
#define  PORTD6   6
#define  PORTD5   5
#define  PORTD4   4
#define  PORTD3   3
#define  PORTD2   2
#define  PORTD1   1
#define  PORTD0   0
#define  PD7      7
#define  PD6      6
#define  PD5      5
#define  PD4      4
#define  PD3      3
#define  PD2      2
#define  PD1      1
#define  PD0      0

#define  DDD7     7
#define  DDD6     6
#define  DDD5     5
#define  DDD4     4
#define  DDD3     3
#define  DDD2     2
#define  DDD1     1
#define  DDD0     0

#define  PIND7    7
#define  PIND6    6
#define  PIND5    5
#define  PIND4    4
#define  PIND3    3
#define  PIND2    2
#define  PIND1    1
#define  PIND0    0


/* Lock and Fuse Bits with LPM/SPM instructions */

/* lock bits */
#define  BLB12    5
#define  BLB11    4
#define  BLB02    3
#define  BLB01    2
#define  LB2      1
#define  LB1      0

/* fuses low bits */
#define  BODLEVEL 7
#define  BODEN    6
#define  SUT1     5
#define  SUT0     4
#define  CKSEL3   3
#define  CKSEL2   2
#define  CKSEL1   1
#define  CKSEL0   0

/* fuses high bits */
#define  OCDEN    7
#define  JTAGEN   6
#define  SPIEN    5
#define  CKOPT    4
#define  EESAVE   3
#define  BOOTSZ1  2
#define  BOOTSZ0  1
#define  BOOTRST  0

 
 
 
#define WDR() 	#asm("wdr")
#define SEI()	#asm("sei")
#define CLI()	#asm("cli")
#define NOP()	#asm("nop")
#define _WDR() 	#asm("wdr")
#define _SEI()	#asm("sei")
#define _CLI()	#asm("cli")
#define _NOP()	#asm("nop")

typedef unsigned char  uchar;			/* 定义可移植的无符号8位整数关键字    */
typedef signed   char  schar;			/* 定义可移植的有符号8位整数关键字    */
typedef unsigned int   uint;			/* 定义可移植的无符号16位整数关键字   */
typedef signed   int   sint;			/* 定义可移植的有符号16位整数关键字   */
typedef unsigned long  ulong;			/* 定义可移植的无符号32位整数关键字   */
typedef signed   long  slong;			/* 定义可移植的有符号32位整数关键字   */

#include <MEGA16.H>

#include  "TWI.h"
#include "delay.h"  
#include  "INIT.h"
#include "pcf8565.h" 
#include "lcd1602.h"


#endif