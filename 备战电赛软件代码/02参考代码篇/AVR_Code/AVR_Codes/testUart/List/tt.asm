
;CodeVisionAVR C Compiler V2.03.4 Standard
;(C) Copyright 1998-2008 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega16
;Program type           : Application
;Clock frequency        : 16.000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 256 byte(s)
;Heap size              : 0 byte(s)
;Promote char to int    : Yes
;char is unsigned       : Yes
;global const stored in FLASH  : No
;8 bit enums            : Yes
;Enhanced core instructions    : On
;Smart register allocation : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega16
	#pragma AVRPART MEMORY PROG_FLASH 16384
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1024
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	.LISTMAC
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ANDI R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ORI  R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __CLRD1S
	LDI  R30,0
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+@1)
	LDI  R31,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	LDI  R22,BYTE3(2*@0+@1)
	LDI  R23,BYTE4(2*@0+@1)
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+@2)
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+@3)
	LDI  R@1,HIGH(@2+@3)
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+@3)
	LDI  R@1,HIGH(@2*2+@3)
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+@1
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+@1
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	LDS  R22,@0+@1+2
	LDS  R23,@0+@1+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+@2
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+@3
	LDS  R@1,@2+@3+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+@1
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	LDS  R24,@0+@1+2
	LDS  R25,@0+@1+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+@1,R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	STS  @0+@1+2,R22
	STS  @0+@1+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+@1,R0
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+@1,R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+@1,R@2
	STS  @0+@1+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __CLRD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R@1
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _rx_wr_index=R5
	.DEF _rx_rd_index=R4
	.DEF _rx_counter=R7
	.DEF _state=R6
	.DEF _timerCounter=R8
	.DEF _baCounter=R11

	.CSEG
	.ORG 0x00

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _timer2_ovf_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _usart_rx_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

_tbl10_G100:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G100:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

_0x3:
	.DB  0x68,0x65,0x6C,0x6C,0x6F,0xA

__GLOBAL_INI_TBL:
	.DW  0x06
	.DW  _str
	.DW  _0x3*2

_0xFFFFFFFF:
	.DW  0

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(0x400)
	LDI  R25,HIGH(0x400)
	LDI  R26,0x60
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;STACK POINTER INITIALIZATION
	LDI  R30,LOW(0x45F)
	OUT  SPL,R30
	LDI  R30,HIGH(0x45F)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(0x160)
	LDI  R29,HIGH(0x160)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x160

	.CSEG
;/*****************************************************
;This program was produced by the
;CodeWizardAVR V2.03.4 Standard
;Automatic Program Generator
;© Copyright 1998-2008 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com
;
;Project :
;Version :
;Date    : 2009-7-19
;Author  :
;Company :
;Comments:
;
;
;Chip type           : ATmega16L
;Program type        : Application
;Clock frequency     : 16.000000 MHz
;Memory model        : Small
;External RAM size   : 0
;Data Stack size     : 256
;*****************************************************/
;
;#include <mega16.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include <delay.h>
;#define RXB8 1
;#define TXB8 0
;#define UPE 2
;#define OVR 3
;#define FE 4
;#define UDRE 5
;#define RXC 7
;
;#define FRAMING_ERROR (1<<FE)
;#define PARITY_ERROR (1<<UPE)
;#define DATA_OVERRUN (1<<OVR)
;#define DATA_REGISTER_EMPTY (1<<UDRE)
;#define RX_COMPLETE (1<<RXC)
;
;// USART Receiver buffer
;#define RX_BUFFER_SIZE 64
;#define MyAddress 0x01      // ×Ô¼ºµÄµØÖ·
;/**************±äÁ¿ÉùÃ÷***************/
;char rx_buffer[RX_BUFFER_SIZE];
;
;#if RX_BUFFER_SIZE<256
;unsigned char rx_wr_index,rx_rd_index,rx_counter;
;#else
;unsigned int rx_wr_index,rx_rd_index,rx_counter;
;#endif
;// This flag is set on USART Receiver buffer overflow
;bit rx_buffer_overflow;
;// USART Receiver interrupt service routine
;char str[] = "hello\n";

	.DSEG
;char frameData[16];
;char state;
;bit statechanged = 0, serFlag = 0, timerFlag = 0;
;unsigned int timerCounter;
;char baCounter;
;/***************º¯ÊýÉùÃ÷****************/
;char doFrameOk();
;void doCommand();
;void uartSend(char sendCmd, char sendData);
;void balan();
;void move(char mode);
;void delay_1ms();
;void delayForSer(unsigned int ms);
;interrupt [USART_RXC] void usart_rx_isr(void)
; 0000 0045 {

	.CSEG
_usart_rx_isr:
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0046 char status,data;
; 0000 0047 serFlag = 1;
	ST   -Y,R17
	ST   -Y,R16
;	status -> R17
;	data -> R16
	SET
	BLD  R2,2
; 0000 0048 status=UCSRA;
	IN   R17,11
; 0000 0049 data=UDR;
	IN   R16,12
; 0000 004A if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R26,R17
	LDI  R27,0
	LDI  R30,LOW(28)
	LDI  R31,HIGH(28)
	AND  R30,R26
	AND  R31,R27
	SBIW R30,0
	BRNE _0x4
; 0000 004B    {
; 0000 004C    rx_buffer[rx_wr_index]=data;
	MOV  R30,R5
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer)
	SBCI R31,HIGH(-_rx_buffer)
	ST   Z,R16
; 0000 004D    if (++rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
	INC  R5
	LDI  R30,LOW(64)
	CP   R30,R5
	BRNE _0x5
	CLR  R5
; 0000 004E    if (++rx_counter == RX_BUFFER_SIZE)
_0x5:
	INC  R7
	LDI  R30,LOW(64)
	CP   R30,R7
	BRNE _0x6
; 0000 004F       {
; 0000 0050       rx_counter=0;
	CLR  R7
; 0000 0051       rx_buffer_overflow=1;
	SET
	BLD  R2,0
; 0000 0052       };
_0x6:
; 0000 0053    };
_0x4:
; 0000 0054 }
	LD   R16,Y+
	LD   R17,Y+
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	RETI
;
;// Timer 2 overflow interrupt service routine
;interrupt [TIM2_OVF] void timer2_ovf_isr(void)
; 0000 0058 {
_timer2_ovf_isr:
	ST   -Y,R26
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0059 // Reinitialize Timer 2 value
; 0000 005A TCNT2=0xff;
	LDI  R30,LOW(255)
	OUT  0x24,R30
; 0000 005B // Place your code here
; 0000 005C if(++timerCounter == 8000)
	MOVW R30,R8
	ADIW R30,1
	MOVW R8,R30
	CPI  R30,LOW(0x1F40)
	LDI  R26,HIGH(0x1F40)
	CPC  R31,R26
	BRNE _0x7
; 0000 005D {
; 0000 005E     timerCounter = 0;
	CLR  R8
	CLR  R9
; 0000 005F     timerFlag = 1;
	SET
	BLD  R2,3
; 0000 0060 }
; 0000 0061 }
_0x7:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R26,Y+
	RETI
;
;#ifndef _DEBUG_TERMINAL_IO_
;// Get a character from the USART Receiver buffer
;#define _ALTERNATE_GETCHAR_
;#pragma used+
;char getchar(void)
; 0000 0068 {
_getchar:
; 0000 0069 char data;
; 0000 006A while (rx_counter==0);
	ST   -Y,R17
;	data -> R17
_0x8:
	TST  R7
	BREQ _0x8
; 0000 006B data=rx_buffer[rx_rd_index];
	MOV  R30,R4
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer)
	SBCI R31,HIGH(-_rx_buffer)
	LD   R17,Z
; 0000 006C if (++rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
	INC  R4
	LDI  R30,LOW(64)
	CP   R30,R4
	BRNE _0xB
	CLR  R4
; 0000 006D #asm("cli")
_0xB:
	cli
; 0000 006E --rx_counter;
	DEC  R7
; 0000 006F #asm("sei")
	sei
; 0000 0070 return data;
	MOV  R30,R17
	LD   R17,Y+
	RET
; 0000 0071 }
;#pragma used-
;#endif
;
;// Standard Input/Output functions
;#include <stdio.h>
;
;// Declare your global variables here
;
;void main(void)
; 0000 007B {
_main:
; 0000 007C // Declare your local variables here
; 0000 007D 
; 0000 007E // Input/Output Ports initialization
; 0000 007F // Port A initialization
; 0000 0080 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 0081 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 0082 PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
; 0000 0083 DDRA=0x00;
	OUT  0x1A,R30
; 0000 0084 
; 0000 0085 // Port B initialization
; 0000 0086 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 0087 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 0088 PORTB=0x00;
	OUT  0x18,R30
; 0000 0089 DDRB=0x00;
	OUT  0x17,R30
; 0000 008A 
; 0000 008B // Port C initialization
; 0000 008C // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 008D // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 008E PORTC=0x30;
	LDI  R30,LOW(48)
	OUT  0x15,R30
; 0000 008F DDRC=0x00;
	LDI  R30,LOW(0)
	OUT  0x14,R30
; 0000 0090 
; 0000 0091 // Port D initialization
; 0000 0092 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
; 0000 0093 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
; 0000 0094 PORTD=0x00;
	OUT  0x12,R30
; 0000 0095 DDRD=0x00;
	OUT  0x11,R30
; 0000 0096 
; 0000 0097 // Timer/Counter 0 initialization
; 0000 0098 // Clock source: System Clock
; 0000 0099 // Clock value: Timer 0 Stopped
; 0000 009A // Mode: Normal top=FFh
; 0000 009B // OC0 output: Disconnected
; 0000 009C TCCR0=0x00;
	OUT  0x33,R30
; 0000 009D TCNT0=0x00;
	OUT  0x32,R30
; 0000 009E OCR0=0x00;
	OUT  0x3C,R30
; 0000 009F 
; 0000 00A0 // Timer/Counter 1 initialization
; 0000 00A1 // Clock source: System Clock
; 0000 00A2 // Clock value: Timer 1 Stopped
; 0000 00A3 // Mode: Normal top=FFFFh
; 0000 00A4 // OC1A output: Discon.
; 0000 00A5 // OC1B output: Discon.
; 0000 00A6 // Noise Canceler: Off
; 0000 00A7 // Input Capture on Falling Edge
; 0000 00A8 // Timer 1 Overflow Interrupt: Off
; 0000 00A9 // Input Capture Interrupt: Off
; 0000 00AA // Compare A Match Interrupt: Off
; 0000 00AB // Compare B Match Interrupt: Off
; 0000 00AC TCCR1A=0x00;
	OUT  0x2F,R30
; 0000 00AD TCCR1B=0x00;
	CALL SUBOPT_0x0
; 0000 00AE TCNT1H=0x00;
; 0000 00AF TCNT1L=0x00;
; 0000 00B0 ICR1H=0x00;
; 0000 00B1 ICR1L=0x00;
; 0000 00B2 OCR1AH=0x00;
; 0000 00B3 OCR1AL=0x00;
; 0000 00B4 OCR1BH=0x00;
; 0000 00B5 OCR1BL=0x00;
; 0000 00B6 
; 0000 00B7 // Timer/Counter 2 initialization
; 0000 00B8 // Clock source: System Clock
; 0000 00B9 // Clock value: 15.625 kHz
; 0000 00BA // Mode: Normal top=FFh
; 0000 00BB // OC2 output: Disconnected
; 0000 00BC ASSR=0x00;
	LDI  R30,LOW(0)
	OUT  0x22,R30
; 0000 00BD TCCR2=0x07;
	LDI  R30,LOW(7)
	OUT  0x25,R30
; 0000 00BE TCNT2=0xff;
	LDI  R30,LOW(255)
	OUT  0x24,R30
; 0000 00BF OCR2=0x00;
	LDI  R30,LOW(0)
	OUT  0x23,R30
; 0000 00C0 
; 0000 00C1 // External Interrupt(s) initialization
; 0000 00C2 // INT0: Off
; 0000 00C3 // INT1: Off
; 0000 00C4 // INT2: Off
; 0000 00C5 MCUCR=0x00;
	OUT  0x35,R30
; 0000 00C6 MCUCSR=0x00;
	OUT  0x34,R30
; 0000 00C7 
; 0000 00C8 // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 00C9 TIMSK=0x00;
	OUT  0x39,R30
; 0000 00CA 
; 0000 00CB // USART initialization
; 0000 00CC // Communication Parameters: 8 Data, 1 Stop, No Parity
; 0000 00CD // USART Receiver: On
; 0000 00CE // USART Transmitter: On
; 0000 00CF // USART Mode: Asynchronous
; 0000 00D0 // USART Baud Rate: 19200
; 0000 00D1 UCSRA=0x00;
	OUT  0xB,R30
; 0000 00D2 UCSRB=0x98;
	LDI  R30,LOW(152)
	OUT  0xA,R30
; 0000 00D3 UCSRC=0x86;
	LDI  R30,LOW(134)
	OUT  0x20,R30
; 0000 00D4 UBRRH=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
; 0000 00D5 UBRRL=0x33;
	LDI  R30,LOW(51)
	OUT  0x9,R30
; 0000 00D6 PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
; 0000 00D7 DDRD=0x30;
	LDI  R30,LOW(48)
	OUT  0x11,R30
; 0000 00D8 DDRA=0xf0;
	LDI  R30,LOW(240)
	OUT  0x1A,R30
; 0000 00D9 PORTA = 0xff;
	LDI  R30,LOW(255)
	OUT  0x1B,R30
; 0000 00DA DDRA &= 0b11111100;
	IN   R30,0x1A
	LDI  R31,0
	ANDI R30,LOW(0xFC)
	ANDI R31,HIGH(0xFC)
	OUT  0x1A,R30
; 0000 00DB PORTA |= 0b00000011;
	IN   R30,0x1B
	LDI  R31,0
	ORI  R30,LOW(0x3)
	OUT  0x1B,R30
; 0000 00DC // Analog Comparator initialization
; 0000 00DD // Analog Comparator: Off
; 0000 00DE // Analog Comparator Input Capture by Timer/Counter 1: Off
; 0000 00DF ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
; 0000 00E0 SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x30,R30
; 0000 00E1 
; 0000 00E2 // Global enable interrupts
; 0000 00E3 
; 0000 00E4 
; 0000 00E5 // Timer/Counter 1 initialization
; 0000 00E6 // Clock source: System Clock
; 0000 00E7 // Clock value: 250.000 kHz
; 0000 00E8 // Mode: Ph. correct PWM top=01FFh
; 0000 00E9 // OC1A output: Non-Inv.
; 0000 00EA // OC1B output: Non-Inv.
; 0000 00EB // Noise Canceler: Off
; 0000 00EC // Input Capture on Falling Edge
; 0000 00ED // Timer 1 Overflow Interrupt: Off
; 0000 00EE // Input Capture Interrupt: Off
; 0000 00EF // Compare A Match Interrupt: Off
; 0000 00F0 // Compare B Match Interrupt: Off
; 0000 00F1 TCCR1A=0xa2;
	LDI  R30,LOW(162)
	OUT  0x2F,R30
; 0000 00F2 TCCR1B=0x12;
	LDI  R30,LOW(18)
	CALL SUBOPT_0x0
; 0000 00F3 TCNT1H=0x00;
; 0000 00F4 TCNT1L=0x00;
; 0000 00F5 ICR1H=0x00;
; 0000 00F6 ICR1L=0x00;
; 0000 00F7 OCR1AH=0x00;
; 0000 00F8 OCR1AL=0x00;
; 0000 00F9 OCR1BH=0x00;
; 0000 00FA OCR1BL=0x00;
; 0000 00FB ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
; 0000 00FC ICR1H = 0x01;
	LDI  R30,LOW(1)
	OUT  0x27,R30
; 0000 00FD ICR1L = 0xf0;
	LDI  R30,LOW(240)
	OUT  0x26,R30
; 0000 00FE #asm("sei")
	sei
; 0000 00FF puts(str);
	LDI  R30,LOW(_str)
	LDI  R31,HIGH(_str)
	ST   -Y,R31
	ST   -Y,R30
	CALL _puts
; 0000 0100 //*****µÚÒ»½×¶Î*******
; 0000 0101 while(getchar() != 0x0c);
_0xC:
	RCALL _getchar
	CPI  R30,LOW(0xC)
	BRNE _0xC
; 0000 0102 putchar(0x01);
	CALL SUBOPT_0x1
; 0000 0103 move(1);
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _move
; 0000 0104 delay_ms(2000);
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	CALL SUBOPT_0x2
; 0000 0105 move(0);
	CALL SUBOPT_0x3
; 0000 0106 putchar(0x02);
	CALL SUBOPT_0x4
; 0000 0107 delay_ms(1000);
	CALL SUBOPT_0x5
; 0000 0108                                         /*******±à´íµÄ*******/
; 0000 0109                                         //move(1);
; 0000 010A                                         //delay_ms(500);
; 0000 010B                                         //while(!PINA.0 || !PINA.1);
; 0000 010C                                         //move(0);
; 0000 010D                                         //putchar(0x02);
; 0000 010E                                         //delay_ms(1000);
; 0000 010F                                         //putchar(0x01);
; 0000 0110                                         //move(2);
; 0000 0111                                         //delay_ms(500);
; 0000 0112                                         //while(!PINA.0 || !PINA.1);
; 0000 0113                                         //move(0);
; 0000 0114                                         //putchar(0x02);
; 0000 0115                                         //delay_ms(1000);//ÔÝÍ£1sÐÝÏ¢Ò»ÏÂ
; 0000 0116 
; 0000 0117 
; 0000 0118 /********µÚ¶þ½×¶Î**********/
; 0000 0119 //while(getchar() != 0x0c);
; 0000 011A putchar(0x01);
	CALL SUBOPT_0x1
; 0000 011B TIMSK=0x40;
	LDI  R30,LOW(64)
	OUT  0x39,R30
; 0000 011C while (baCounter < 3)
_0xF:
	LDI  R30,LOW(3)
	CP   R11,R30
	BRSH _0x11
; 0000 011D {
; 0000 011E 
; 0000 011F //   Place your code here
; 0000 0120     if(rx_counter>=5)
	LDI  R30,LOW(5)
	CP   R7,R30
	BRLO _0x12
; 0000 0121     {
; 0000 0122         delay_ms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x2
; 0000 0123         if(getchar() == 0x55)
	RCALL _getchar
	CPI  R30,LOW(0x55)
	BRNE _0x13
; 0000 0124         {
; 0000 0125             if(doFrameOk())
	RCALL _doFrameOk
	CPI  R30,0
	BREQ _0x14
; 0000 0126             {
; 0000 0127                 doCommand();
	RCALL _doCommand
; 0000 0128             }
; 0000 0129         }
_0x14:
; 0000 012A     }
_0x13:
; 0000 012B     if(timerFlag && TIMSK)
_0x12:
	SBRS R2,3
	RJMP _0x16
	IN   R30,0x39
	CPI  R30,0
	BRNE _0x17
_0x16:
	RJMP _0x15
_0x17:
; 0000 012C     {
; 0000 012D         timerFlag = 0;
	CLT
	BLD  R2,3
; 0000 012E         balan();
	RCALL _balan
; 0000 012F     }
; 0000 0130 };
_0x15:
	RJMP _0xF
_0x11:
; 0000 0131 putchar(0x02);
	CALL SUBOPT_0x4
; 0000 0132 TIMSK=0x00;
	LDI  R30,LOW(0)
	OUT  0x39,R30
; 0000 0133 UCSRB.7 = 0;
	CBI  0xA,7
; 0000 0134 delay_ms(1000);
	CALL SUBOPT_0x5
; 0000 0135 /******µÚÈý½×¶Î******/
; 0000 0136 //while(getchar() != 0x0c);
; 0000 0137 putchar(0x01);
	CALL SUBOPT_0x1
; 0000 0138 move(1);
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _move
; 0000 0139 while(!PINA.0 || !PINA.1);
_0x1A:
	SBIS 0x19,0
	RJMP _0x1D
	SBIC 0x19,1
	RJMP _0x1C
_0x1D:
	RJMP _0x1A
_0x1C:
; 0000 013A move(0);
	CALL SUBOPT_0x3
; 0000 013B delay_ms(1000);
	CALL SUBOPT_0x5
; 0000 013C putchar(0x02);
	CALL SUBOPT_0x4
; 0000 013D delay_ms(1000);
	CALL SUBOPT_0x5
; 0000 013E /******µÚËÄ½×¶Î*****/
; 0000 013F putchar(0x01);
	CALL SUBOPT_0x1
; 0000 0140 move(2);
	LDI  R30,LOW(2)
	ST   -Y,R30
	RCALL _move
; 0000 0141 delay_ms(1000);
	CALL SUBOPT_0x5
; 0000 0142 while(!PINA.0 || !PINA.1);
_0x1F:
	SBIS 0x19,0
	RJMP _0x22
	SBIC 0x19,1
	RJMP _0x21
_0x22:
	RJMP _0x1F
_0x21:
; 0000 0143 move(0);
	CALL SUBOPT_0x3
; 0000 0144 putchar(0x02);
	CALL SUBOPT_0x4
; 0000 0145 while(1);
_0x24:
	RJMP _0x24
; 0000 0146 }
_0x27:
	RJMP _0x27
;void balan()
; 0000 0148 {
_balan:
; 0000 0149     switch(state)
	MOV  R30,R6
	CALL SUBOPT_0x6
; 0000 014A     {
; 0000 014B         case 1:
	BRNE _0x2B
; 0000 014C                 move(4);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x7
; 0000 014D                 delayForSer(80);
; 0000 014E                 move(5);
	RJMP _0xF0
; 0000 014F             break;
; 0000 0150         case 2:
_0x2B:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x2C
; 0000 0151                 move(3);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x7
; 0000 0152                 delayForSer(80);
; 0000 0153                 move(5);
	RJMP _0xF0
; 0000 0154             break;
; 0000 0155         case 3:
_0x2C:
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0x2E
; 0000 0156             move(6);
	LDI  R30,LOW(6)
	RJMP _0xF1
; 0000 0157             break;
; 0000 0158         default:
_0x2E:
; 0000 0159             move(5);
_0xF0:
	LDI  R30,LOW(5)
_0xF1:
	ST   -Y,R30
	RCALL _move
; 0000 015A             break;
; 0000 015B     }
; 0000 015C }
	RET
;/*************ÒÆ¶¯º¯Êý(0:È«ËÙÍ£Ö¹, 1:È«ËÙÇ°ÐÐ,2:È«ËÙºóÍË, 3:ÂýËÙÇ°ÐÐ, 4:ÂýËÙºóÍË, 5:ÂýËÙÍ£Ö¹, 6:»ØÍËÉ²³µ(ÓÃÓÚÕÒµ½Æ½ºâ)) **********************/
;void move(char mode)
; 0000 015F {
_move:
; 0000 0160     switch(mode)
;	mode -> Y+0
	LD   R30,Y
	LDI  R31,0
; 0000 0161     {
; 0000 0162         case 0:
	SBIW R30,0
	BRNE _0x32
; 0000 0163             //É²³µÃüÁî
; 0000 0164             PORTA.4 = PORTA.6 = ~PORTA.4;
	CALL SUBOPT_0x8
	BRNE _0x33
	CBI  0x1B,6
	RJMP _0x34
_0x33:
	SBI  0x1B,6
_0x34:
	CPI  R30,0
	BRNE _0x35
	CBI  0x1B,4
	RJMP _0x36
_0x35:
	SBI  0x1B,4
_0x36:
; 0000 0165             PORTA.5 = PORTA.7 = ~PORTA.5;
	CALL SUBOPT_0x9
	BRNE _0x37
	CBI  0x1B,7
	RJMP _0x38
_0x37:
	SBI  0x1B,7
_0x38:
	CPI  R30,0
	BRNE _0x39
	CBI  0x1B,5
	RJMP _0x3A
_0x39:
	SBI  0x1B,5
_0x3A:
; 0000 0166             OCR1A = OCR1B = 0x01f0;
	CALL SUBOPT_0xA
; 0000 0167             delay_ms(250);
	LDI  R30,LOW(250)
	LDI  R31,HIGH(250)
	CALL SUBOPT_0x2
; 0000 0168             //ÓÐ±ØÒªÊ±¼ÓÉÏÇã½Ç¼ì²âÃüÁî
; 0000 0169 
; 0000 016A             PORTA.4 = PORTA.6 = 0;
	CALL SUBOPT_0xB
; 0000 016B             PORTA.5 = PORTA.7 = 0;
; 0000 016C             OCR1A = OCR1B = 0x01f0;
	RJMP _0xF2
; 0000 016D  //           putchar('4');
; 0000 016E             break;
; 0000 016F         case 1:
_0x32:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x43
; 0000 0170             PORTA.4 = PORTA.6 = 0;
	CALL SUBOPT_0xC
; 0000 0171             PORTA.5 = PORTA.7 = 1;
; 0000 0172             OCR1A = OCR1B = 0x0160;
	LDI  R30,LOW(352)
	LDI  R31,HIGH(352)
	RJMP _0xF2
; 0000 0173 //            putchar('2');
; 0000 0174             break;
; 0000 0175         case 2:
_0x43:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x4C
; 0000 0176             PORTA.4 = PORTA.6 = 1;
	CALL SUBOPT_0xD
; 0000 0177             PORTA.5 = PORTA.7 = 0;
; 0000 0178             OCR1A = OCR1B = 0x0160;
	LDI  R30,LOW(352)
	LDI  R31,HIGH(352)
	RJMP _0xF2
; 0000 0179 //            putchar('3');
; 0000 017A             break;
; 0000 017B         case 3:
_0x4C:
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0x55
; 0000 017C             PORTA.4 = PORTA.6 = 0;
	CALL SUBOPT_0xC
; 0000 017D             PORTA.5 = PORTA.7 = 1;
; 0000 017E             OCR1AH = OCR1BH = 0x01;
	CALL SUBOPT_0xE
; 0000 017F             OCR1AL = OCR1BL = 0xc0;
; 0000 0180 //            putchar('2');
; 0000 0181             break;
	RJMP _0x31
; 0000 0182         case 4:
_0x55:
	CPI  R30,LOW(0x4)
	LDI  R26,HIGH(0x4)
	CPC  R31,R26
	BRNE _0x5E
; 0000 0183             PORTA.4 = PORTA.6 = 1;
	CALL SUBOPT_0xD
; 0000 0184             PORTA.5 = PORTA.7 = 0;
; 0000 0185             OCR1AH = OCR1BH = 0x01;
	CALL SUBOPT_0xE
; 0000 0186             OCR1AL = OCR1BL = 0xc0;
; 0000 0187 //            putchar('3');
; 0000 0188             break;
	RJMP _0x31
; 0000 0189         case 5:
_0x5E:
	CPI  R30,LOW(0x5)
	LDI  R26,HIGH(0x5)
	CPC  R31,R26
	BRNE _0x67
; 0000 018A             //É²³µÃüÁî
; 0000 018B             PORTA.4 = PORTA.6 = ~PORTA.4;
	CALL SUBOPT_0x8
	BRNE _0x68
	CBI  0x1B,6
	RJMP _0x69
_0x68:
	SBI  0x1B,6
_0x69:
	CPI  R30,0
	BRNE _0x6A
	CBI  0x1B,4
	RJMP _0x6B
_0x6A:
	SBI  0x1B,4
_0x6B:
; 0000 018C             PORTA.5 = PORTA.7 = ~PORTA.5;
	CALL SUBOPT_0x9
	BRNE _0x6C
	CBI  0x1B,7
	RJMP _0x6D
_0x6C:
	SBI  0x1B,7
_0x6D:
	CPI  R30,0
	BRNE _0x6E
	CBI  0x1B,5
	RJMP _0x6F
_0x6E:
	SBI  0x1B,5
_0x6F:
; 0000 018D             OCR1A = OCR1B = 0x00ee;
	CALL SUBOPT_0xF
; 0000 018E             delay_ms(100);
; 0000 018F             //ÓÐ±ØÒªÊ±¼ÓÉÏÇã½Ç¼ì²âÃüÁî
; 0000 0190 
; 0000 0191             PORTA.4 = PORTA.6 = 0;
	CALL SUBOPT_0xB
; 0000 0192             PORTA.5 = PORTA.7 = 0;
; 0000 0193             OCR1A = OCR1B = 0x01f0;
	RJMP _0xF2
; 0000 0194 //            putchar('4');
; 0000 0195             break;
; 0000 0196         case 6:
_0x67:
	CPI  R30,LOW(0x6)
	LDI  R26,HIGH(0x6)
	CPC  R31,R26
	BREQ PC+3
	JMP _0x31
; 0000 0197             PORTA.4 = PORTA.6 = statechanged;
	LDI  R30,0
	SBRC R2,1
	LDI  R30,1
	CPI  R30,0
	BRNE _0x79
	CBI  0x1B,6
	RJMP _0x7A
_0x79:
	SBI  0x1B,6
_0x7A:
	CPI  R30,0
	BRNE _0x7B
	CBI  0x1B,4
	RJMP _0x7C
_0x7B:
	SBI  0x1B,4
_0x7C:
; 0000 0198             PORTA.5 = PORTA.7 = ~statechanged;
	LDI  R30,0
	SBRS R2,1
	LDI  R30,1
	CPI  R30,0
	BRNE _0x7D
	CBI  0x1B,7
	RJMP _0x7E
_0x7D:
	SBI  0x1B,7
_0x7E:
	CPI  R30,0
	BRNE _0x7F
	CBI  0x1B,5
	RJMP _0x80
_0x7F:
	SBI  0x1B,5
_0x80:
; 0000 0199             TIMSK = 0x00;
	LDI  R30,LOW(0)
	OUT  0x39,R30
; 0000 019A             OCR1A = OCR1B = 0x01f0;
	CALL SUBOPT_0xA
; 0000 019B             delay_ms(130);
	LDI  R30,LOW(130)
	LDI  R31,HIGH(130)
	CALL SUBOPT_0x2
; 0000 019C             //ÓÐ±ØÒªÊ±¼ÓÉÏÇã½Ç¼ì²âÃüÁî
; 0000 019D             PORTA.4 = PORTA.6 = ~PORTA.4;
	CALL SUBOPT_0x8
	BRNE _0x81
	CBI  0x1B,6
	RJMP _0x82
_0x81:
	SBI  0x1B,6
_0x82:
	CPI  R30,0
	BRNE _0x83
	CBI  0x1B,4
	RJMP _0x84
_0x83:
	SBI  0x1B,4
_0x84:
; 0000 019E             PORTA.5 = PORTA.7 = ~PORTA.5;
	CALL SUBOPT_0x9
	BRNE _0x85
	CBI  0x1B,7
	RJMP _0x86
_0x85:
	SBI  0x1B,7
_0x86:
	CPI  R30,0
	BRNE _0x87
	CBI  0x1B,5
	RJMP _0x88
_0x87:
	SBI  0x1B,5
_0x88:
; 0000 019F             OCR1A = OCR1B = 0x00ee;
	CALL SUBOPT_0xF
; 0000 01A0             delay_ms(100);
; 0000 01A1             PORTA.4 = PORTA.6 = 0;
	CBI  0x1B,6
	CBI  0x1B,4
; 0000 01A2             PORTA.5 = PORTA.7 = 0;
	CBI  0x1B,7
	CBI  0x1B,5
; 0000 01A3             OCR1A = OCR1B = 0x0000;
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
_0xF2:
	OUT  0x28+1,R31
	OUT  0x28,R30
	OUT  0x2A+1,R31
	OUT  0x2A,R30
; 0000 01A4 //            putchar('4');
; 0000 01A5     }
_0x31:
; 0000 01A6 }
	RJMP _0x2060001
;char doFrameOk()
; 0000 01A8 {
_doFrameOk:
; 0000 01A9     char flag=0, k=0,i,frameLen;
; 0000 01AA     if(getchar() == 0xaa)
	CALL __SAVELOCR4
;	flag -> R17
;	k -> R16
;	i -> R19
;	frameLen -> R18
	LDI  R17,0
	LDI  R16,0
	RCALL _getchar
	CPI  R30,LOW(0xAA)
	BRNE _0x91
; 0000 01AB         k++;
	SUBI R16,-1
; 0000 01AC     if(getchar() == MyAddress)
_0x91:
	RCALL _getchar
	CPI  R30,LOW(0x1)
	BRNE _0x92
; 0000 01AD         k++;
	SUBI R16,-1
; 0000 01AE     if(k == 2)
_0x92:
	CPI  R16,2
	BRNE _0x93
; 0000 01AF     {
; 0000 01B0         flag = 1;
	LDI  R17,LOW(1)
; 0000 01B1         frameLen = getchar();
	RCALL _getchar
	MOV  R18,R30
; 0000 01B2         for(i = 0;i < frameLen; i++)
	LDI  R19,LOW(0)
_0x95:
	CP   R19,R18
	BRSH _0x96
; 0000 01B3         {
; 0000 01B4             frameData[i] = getchar();
	MOV  R30,R19
	LDI  R31,0
	SUBI R30,LOW(-_frameData)
	SBCI R31,HIGH(-_frameData)
	PUSH R31
	PUSH R30
	RCALL _getchar
	POP  R26
	POP  R27
	ST   X,R30
; 0000 01B5         }
	SUBI R19,-1
	RJMP _0x95
_0x96:
; 0000 01B6 
; 0000 01B7     }
; 0000 01B8     return flag;
_0x93:
	MOV  R30,R17
	CALL __LOADLOCR4
	RJMP _0x2060002
; 0000 01B9 }
;void doCommand()
; 0000 01BB {
_doCommand:
; 0000 01BC //    int i = 1;
; 0000 01BD     switch(frameData[0])
	LDS  R30,_frameData
	CALL SUBOPT_0x6
; 0000 01BE     {
; 0000 01BF         case 0x01: uartSend(0xff, 0x00);
	BRNE _0x9A
	LDI  R30,LOW(255)
	RJMP _0xF3
; 0000 01C0         break;
; 0000 01C1         case 0x02:
_0x9A:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x9B
; 0000 01C2             PORTA.4 = PORTA.6 = 1;
	CALL SUBOPT_0xD
; 0000 01C3             PORTA.5 = PORTA.7 = 0;
; 0000 01C4             OCR1AH = OCR1BH = frameData[1];
	CALL SUBOPT_0x10
; 0000 01C5             OCR1AL = OCR1BL = frameData[2];
; 0000 01C6             putchar('2');
	LDI  R30,LOW(50)
	ST   -Y,R30
	RCALL _putchar
; 0000 01C7             break;
	RJMP _0x99
; 0000 01C8         case 0x03:
_0x9B:
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0xA4
; 0000 01C9             PORTA.4 = PORTA.6 = 0;
	CALL SUBOPT_0xC
; 0000 01CA             PORTA.5 = PORTA.7 = 1;
; 0000 01CB             OCR1AH = OCR1BH = frameData[1];
	CALL SUBOPT_0x10
; 0000 01CC             OCR1AL = OCR1BL = frameData[2];
; 0000 01CD             putchar('3');
	LDI  R30,LOW(51)
	ST   -Y,R30
	RCALL _putchar
; 0000 01CE             break;
	RJMP _0x99
; 0000 01CF         case 0x04:
_0xA4:
	CPI  R30,LOW(0x4)
	LDI  R26,HIGH(0x4)
	CPC  R31,R26
	BRNE _0xAD
; 0000 01D0 //É²³µÃüÁî
; 0000 01D1             PORTA.4 = PORTA.6 = ~PORTA.4;
	CALL SUBOPT_0x8
	BRNE _0xAE
	CBI  0x1B,6
	RJMP _0xAF
_0xAE:
	SBI  0x1B,6
_0xAF:
	CPI  R30,0
	BRNE _0xB0
	CBI  0x1B,4
	RJMP _0xB1
_0xB0:
	SBI  0x1B,4
_0xB1:
; 0000 01D2             PORTA.5 = PORTA.7 = ~PORTA.5;
	CALL SUBOPT_0x9
	BRNE _0xB2
	CBI  0x1B,7
	RJMP _0xB3
_0xB2:
	SBI  0x1B,7
_0xB3:
	CPI  R30,0
	BRNE _0xB4
	CBI  0x1B,5
	RJMP _0xB5
_0xB4:
	SBI  0x1B,5
_0xB5:
; 0000 01D3             OCR1A = OCR1B = 0x00ee;
	LDI  R30,LOW(238)
	LDI  R31,HIGH(238)
	OUT  0x28+1,R31
	OUT  0x28,R30
	OUT  0x2A+1,R31
	OUT  0x2A,R30
; 0000 01D4             delay_ms(250);
	LDI  R30,LOW(250)
	LDI  R31,HIGH(250)
	CALL SUBOPT_0x2
; 0000 01D5 //Çã½Ç¼ì²âÃüÁî
; 0000 01D6 //            if(state == 1)
; 0000 01D7 //            {
; 0000 01D8 //                PORTA.4 = PORTA.6 = 1;
; 0000 01D9 //                PORTA.5 = PORTA.7 =
; 0000 01DA //            }
; 0000 01DB //            if
; 0000 01DC             PORTA.4 = PORTA.6 = 0;
	CBI  0x1B,6
	CBI  0x1B,4
; 0000 01DD             PORTA.5 = PORTA.7 = 0;
	CBI  0x1B,7
	CBI  0x1B,5
; 0000 01DE             OCR1A = OCR1B = 0x01f0;
	CALL SUBOPT_0xA
; 0000 01DF             putchar('4');
	LDI  R30,LOW(52)
	ST   -Y,R30
	RCALL _putchar
; 0000 01E0             break;
	RJMP _0x99
; 0000 01E1         case 0x05:
_0xAD:
	CPI  R30,LOW(0x5)
	LDI  R26,HIGH(0x5)
	CPC  R31,R26
	BRNE _0xBE
; 0000 01E2             PORTA.4 = PORTA.7 = 1;
	SBI  0x1B,7
	SBI  0x1B,4
; 0000 01E3             PORTA.5 = PORTA.6 = 0;
	CBI  0x1B,6
	CBI  0x1B,5
; 0000 01E4             OCR1AH = OCR1BH = frameData[1];
	CALL SUBOPT_0x10
; 0000 01E5             OCR1AL = OCR1BL = frameData[2];
; 0000 01E6             putchar('5');
	LDI  R30,LOW(53)
	ST   -Y,R30
	RCALL _putchar
; 0000 01E7             break;
	RJMP _0x99
; 0000 01E8         case 0x06:
_0xBE:
	CPI  R30,LOW(0x6)
	LDI  R26,HIGH(0x6)
	CPC  R31,R26
	BRNE _0xC7
; 0000 01E9             PORTA.4 = PORTA.7 = 0;
	CBI  0x1B,7
	CBI  0x1B,4
; 0000 01EA             PORTA.5 = PORTA.6 = 1;
	SBI  0x1B,6
	SBI  0x1B,5
; 0000 01EB             OCR1AH = OCR1BH = frameData[1];
	CALL SUBOPT_0x10
; 0000 01EC             OCR1AL = OCR1BL = frameData[2];
; 0000 01ED             putchar('6');
	LDI  R30,LOW(54)
	ST   -Y,R30
	RCALL _putchar
; 0000 01EE             break;
	RJMP _0x99
; 0000 01EF         case 0x07:
_0xC7:
	CPI  R30,LOW(0x7)
	LDI  R26,HIGH(0x7)
	CPC  R31,R26
	BRNE _0xD0
; 0000 01F0             PORTA.4 = PORTA.6 = 1;
	CALL SUBOPT_0xD
; 0000 01F1             PORTA.5 = PORTA.7 = 0;
; 0000 01F2             OCR1AH = frameData[1];
	__GETB1MN _frameData,1
	OUT  0x2B,R30
; 0000 01F3             OCR1AL = frameData[2];
	CALL SUBOPT_0x11
; 0000 01F4             OCR1BH = frameData[3];
; 0000 01F5             OCR1BL = frameData[4];
; 0000 01F6             putchar('7');
	LDI  R30,LOW(55)
	ST   -Y,R30
	RCALL _putchar
; 0000 01F7             break;
	RJMP _0x99
; 0000 01F8         case 0x08:
_0xD0:
	CPI  R30,LOW(0x8)
	LDI  R26,HIGH(0x8)
	CPC  R31,R26
	BRNE _0xD9
; 0000 01F9             PORTA.4 = PORTA.6 = 0;
	CALL SUBOPT_0xC
; 0000 01FA             PORTA.5 = PORTA.7 = 1;
; 0000 01FB             OCR1AL = frameData[1];
	__GETB1MN _frameData,1
	OUT  0x2A,R30
; 0000 01FC             OCR1AL = frameData[2];
	CALL SUBOPT_0x11
; 0000 01FD             OCR1BH = frameData[3];
; 0000 01FE             OCR1BL = frameData[4];
; 0000 01FF             putchar('8');
	LDI  R30,LOW(56)
	ST   -Y,R30
	RCALL _putchar
; 0000 0200             break;
	RJMP _0x99
; 0000 0201     //½ÓÊÜÆ½ºâ×´Ì¬Ö¸Ê¾
; 0000 0202         case 0x09:
_0xD9:
	CPI  R30,LOW(0x9)
	LDI  R26,HIGH(0x9)
	CPC  R31,R26
	BRNE _0xE2
; 0000 0203             state = 1;
	LDI  R30,LOW(1)
	MOV  R6,R30
; 0000 0204             statechanged = 0;
	CLT
	BLD  R2,1
; 0000 0205             TIMSK = 0x40;
	LDI  R30,LOW(64)
	OUT  0x39,R30
; 0000 0206             putchar('9');
	LDI  R30,LOW(57)
	ST   -Y,R30
	RCALL _putchar
; 0000 0207             baCounter = 0;
	CLR  R11
; 0000 0208             break;
	RJMP _0x99
; 0000 0209         case 0x0a:
_0xE2:
	CPI  R30,LOW(0xA)
	LDI  R26,HIGH(0xA)
	CPC  R31,R26
	BRNE _0xE3
; 0000 020A             state = 2;
	LDI  R30,LOW(2)
	MOV  R6,R30
; 0000 020B             statechanged = 1;
	SET
	BLD  R2,1
; 0000 020C             TIMSK = 0x40;
	LDI  R30,LOW(64)
	OUT  0x39,R30
; 0000 020D             putchar('a');
	LDI  R30,LOW(97)
	ST   -Y,R30
	RCALL _putchar
; 0000 020E             baCounter = 0;
	CLR  R11
; 0000 020F             break;
	RJMP _0x99
; 0000 0210         case 0x0b:
_0xE3:
	CPI  R30,LOW(0xB)
	LDI  R26,HIGH(0xB)
	CPC  R31,R26
	BRNE _0xE5
; 0000 0211             state = 3;
	LDI  R30,LOW(3)
	MOV  R6,R30
; 0000 0212             timerFlag = 1;
	SET
	BLD  R2,3
; 0000 0213             putchar('b');
	LDI  R30,LOW(98)
	ST   -Y,R30
	RCALL _putchar
; 0000 0214             baCounter++;
	INC  R11
; 0000 0215             break;
	RJMP _0x99
; 0000 0216         default: uartSend(0xee, 0x00);
_0xE5:
	LDI  R30,LOW(238)
_0xF3:
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	RCALL _uartSend
; 0000 0217     }
_0x99:
; 0000 0218 }
	RET
;void uartSend(char sendCmd, char sendData)
; 0000 021A {
_uartSend:
; 0000 021B         putchar(0x55);
;	sendCmd -> Y+1
;	sendData -> Y+0
	LDI  R30,LOW(85)
	ST   -Y,R30
	RCALL _putchar
; 0000 021C         putchar(0xaa);
	LDI  R30,LOW(170)
	ST   -Y,R30
	RCALL _putchar
; 0000 021D         putchar(0x02);
	CALL SUBOPT_0x4
; 0000 021E         putchar(sendCmd);
	LDD  R30,Y+1
	ST   -Y,R30
	RCALL _putchar
; 0000 021F         putchar(sendData);
	LD   R30,Y
	ST   -Y,R30
	RCALL _putchar
; 0000 0220         putchar('\n');
	LDI  R30,LOW(10)
	ST   -Y,R30
	RCALL _putchar
; 0000 0221 }
	ADIW R28,2
	RET
;void delay_1ms(void)
; 0000 0223 
; 0000 0224 {
_delay_1ms:
; 0000 0225 
; 0000 0226   unsigned int i;
; 0000 0227 
; 0000 0228   for(i=1;i<2000 && rx_counter<5;i++)
	ST   -Y,R17
	ST   -Y,R16
;	i -> R16,R17
	__GETWRN 16,17,1
_0xE7:
	__CPWRN 16,17,2000
	BRSH _0xE9
	LDI  R30,LOW(5)
	CP   R7,R30
	BRLO _0xEA
_0xE9:
	RJMP _0xE8
_0xEA:
; 0000 0229  ;
	__ADDWRN 16,17,1
	RJMP _0xE7
_0xE8:
; 0000 022A 
; 0000 022B }
	LD   R16,Y+
	LD   R17,Y+
	RET
;void delayForSer(unsigned int ms)
; 0000 022D {
_delayForSer:
; 0000 022E     int i;
; 0000 022F     for(i = 0; i < ms && rx_counter<5; i++)
	ST   -Y,R17
	ST   -Y,R16
;	ms -> Y+2
;	i -> R16,R17
	__GETWRN 16,17,0
_0xEC:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	CP   R16,R30
	CPC  R17,R31
	BRSH _0xEE
	LDI  R30,LOW(5)
	CP   R7,R30
	BRLO _0xEF
_0xEE:
	RJMP _0xED
_0xEF:
; 0000 0230         delay_1ms();
	RCALL _delay_1ms
	__ADDWRN 16,17,1
	RJMP _0xEC
_0xED:
; 0000 0231 }
	LDD  R17,Y+1
	LDD  R16,Y+0
_0x2060002:
	ADIW R28,4
	RET
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

	.CSEG
_putchar:
     sbis usr,udre
     rjmp _putchar
     ld   r30,y
     out  udr,r30
_0x2060001:
	ADIW R28,1
	RET
_puts:
	ST   -Y,R17
_0x2000003:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	MOV  R17,R30
	CPI  R30,0
	BREQ _0x2000005
	ST   -Y,R17
	RCALL _putchar
	RJMP _0x2000003
_0x2000005:
	LDI  R30,LOW(10)
	ST   -Y,R30
	RCALL _putchar
	LDD  R17,Y+0
	ADIW R28,3
	RET

	.CSEG

	.CSEG

	.DSEG
_rx_buffer:
	.BYTE 0x40
_str:
	.BYTE 0x7
_frameData:
	.BYTE 0x10
_p_S1020024:
	.BYTE 0x2

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x0:
	OUT  0x2E,R30
	LDI  R30,LOW(0)
	OUT  0x2D,R30
	OUT  0x2C,R30
	OUT  0x27,R30
	OUT  0x26,R30
	OUT  0x2B,R30
	OUT  0x2A,R30
	OUT  0x29,R30
	OUT  0x28,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1:
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _putchar

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x2:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3:
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _move

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x4:
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _putchar

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x5:
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	RJMP SUBOPT_0x2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6:
	LDI  R31,0
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x7:
	ST   -Y,R30
	CALL _move
	LDI  R30,LOW(80)
	LDI  R31,HIGH(80)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _delayForSer

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x8:
	LDI  R30,0
	SBIS 0x1B,4
	LDI  R30,1
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x9:
	LDI  R30,0
	SBIS 0x1B,5
	LDI  R30,1
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xA:
	LDI  R30,LOW(496)
	LDI  R31,HIGH(496)
	OUT  0x28+1,R31
	OUT  0x28,R30
	OUT  0x2A+1,R31
	OUT  0x2A,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB:
	CBI  0x1B,6
	CBI  0x1B,4
	CBI  0x1B,7
	CBI  0x1B,5
	LDI  R30,LOW(496)
	LDI  R31,HIGH(496)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC:
	CBI  0x1B,6
	CBI  0x1B,4
	SBI  0x1B,7
	SBI  0x1B,5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD:
	SBI  0x1B,6
	SBI  0x1B,4
	CBI  0x1B,7
	CBI  0x1B,5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE:
	LDI  R30,LOW(1)
	OUT  0x29,R30
	OUT  0x2B,R30
	LDI  R30,LOW(192)
	OUT  0x28,R30
	OUT  0x2A,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xF:
	LDI  R30,LOW(238)
	LDI  R31,HIGH(238)
	OUT  0x28+1,R31
	OUT  0x28,R30
	OUT  0x2A+1,R31
	OUT  0x2A,R30
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	RJMP SUBOPT_0x2

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x10:
	__GETB1MN _frameData,1
	OUT  0x29,R30
	OUT  0x2B,R30
	__GETB1MN _frameData,2
	OUT  0x28,R30
	OUT  0x2A,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x11:
	__GETB1MN _frameData,2
	OUT  0x2A,R30
	__GETB1MN _frameData,3
	OUT  0x29,R30
	__GETB1MN _frameData,4
	OUT  0x28,R30
	RET


	.CSEG
_delay_ms:
	ld   r30,y+
	ld   r31,y+
	adiw r30,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA0
	wdr
	sbiw r30,1
	brne __delay_ms0
__delay_ms1:
	ret

__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
