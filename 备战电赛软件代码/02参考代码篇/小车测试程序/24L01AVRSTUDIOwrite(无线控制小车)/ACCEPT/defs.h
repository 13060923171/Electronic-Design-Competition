#undef EXTERN
#define EXTERN extern

#define 	NTSC 	0x55
#define 	PAL 	0xaa

//#define 	DefaultBright 	0x38
//#define 	DefaultContrast 0x81
//#define 	DefaultOLEDMode PAL

#define 	FlagAddr 		0x10	// 0x10~0x13
#define     Dataflag        0x20
#define     Dataflaga       0x40
//------------- define Const ---------------
#define 	ubyte 		unsigned char
#define 	uword 		unsigned int

#define 	t500msC  		100 
#define 	t20msC			1*20

//------------------------ define Flag -------------------
#define 	bit20ms 					BIT0
#define 	bitOledError 				BIT1
#define 	bitKeyContinue	BIT2
#define 	keybhits				BIT3
#define 	bitKeepKeyKeep	BIT4
#define 	bitKeepKeyL			BIT5
//#define 	bitLcmMode 		BIT6		// 1: NTSC Mode  0: PAL mode

//---------------- my macro --------------
#define 	set(i,j)   {i|=j;}
#define 	clr(i,j)   {i&=~j;}
#define 	chk(i,j) 	 (i&j) 
#define 	cpl(i,j) 	(i^=j)

/* enable global interrupts */
#define GIE      	        	   (SREG |= BIT(7))

/* disable global interrupts */
#define GID       	         	   (SREG &= ~BIT(7))

#define SLEEP()	asm("sleep")


/* enables an unsigned char to be used as a series of booleans */
#define BIT(x)	           (1 << (x))
#define SETBIT(x, y)   	   (x |= y)
#define CLEARBIT(x, y) 	   (x &= ~y)
#define CHECKBIT(x, y) 	   (x & y)



// ***** Define I/O pins *****


#define BIT7 0x80
#define BIT6 0x40
#define BIT5 0x20
#define BIT4 0x10
#define BIT3 0x08
#define BIT2 0x04
#define BIT1 0x02
#define BIT0 0x01


#define true 1
#define True 1

#define false 0
#define False 0

//#define LcmPAL 	0x80
//#define LcmNTSC 0xa0

//----------------------------------------
#define 	NULL_KEY 		255
#define 	Bright_Up 		1
#define 	Bright_Dw 		2
#define 	Contrast_Up 	3
#define 	Contrast_Dw 	4
#define 	ModeKey	 	8
//------------------------------------------
#define led6 	BIT1
#define led4 	BIT2
#define led3 	BIT3
#define led1 	BIT4
#define led5 	BIT5
#define led2 	BIT6
