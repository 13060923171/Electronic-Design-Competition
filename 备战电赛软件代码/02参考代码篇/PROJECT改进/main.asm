
;CodeVisionAVR C Compiler V1.25.6 Standard
;(C) Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega16L
;Program type           : Application
;Clock frequency        : 1.000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External SRAM size     : 0
;Data Stack size        : 256 byte(s)
;Heap size              : 0 byte(s)
;Promote char to int    : No
;char is unsigned       : Yes
;8 bit enums            : Yes
;Word align FLASH struct: No
;Enhanced core instructions    : On
;Smart register allocation : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega16L
	#pragma AVRPART MEMORY PROG_FLASH 16384
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1024
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

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

	.CSEG
	.ORG 0

	.INCLUDE "main.vec"
	.INCLUDE "main.inc"

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
	LDI  R24,13
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
;       1 /*****************************************************
;       2 This program was produced by the
;       3 CodeWizardAVR V1.25.6c Professional
;       4 Automatic Program Generator
;       5 ?Copyright 1998-2006 Pavel Haiduc, HP  vula_adcfoTech s.r.l.
;       6 http://www.hp vula_adcfotech.com
;       7 
;       8 Project : 小车
;       9 Version :  0.00
;      10 Date    : 2007-8-30
;      11 Author  : momo
;      12 Company : zhong bei da xue
;      13 Comments:
;      14 
;      15 
;      16 Chip type           : ATmega16L
;      17 Program type        : Application
;      18 Clock frequency     : 1.000000 MHz
;      19 Memory model        : Small
;      20 External SRAM size  : 0
;      21 Data Stack size     : 256
;      22 *****************************************************/
;      23 #include "config.h"
;      24 	#ifndef __SLEEP_DEFINED__
	#ifndef __SLEEP_DEFINED__
;      25 	#define __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
;      26 	.EQU __se_bit=0x40
	.EQU __se_bit=0x40
;      27 	.EQU __sm_mask=0xB0
	.EQU __sm_mask=0xB0
;      28 	.EQU __sm_powerdown=0x20
	.EQU __sm_powerdown=0x20
;      29 	.EQU __sm_powersave=0x30
	.EQU __sm_powersave=0x30
;      30 	.EQU __sm_standby=0xA0
	.EQU __sm_standby=0xA0
;      31 	.EQU __sm_ext_standby=0xB0
	.EQU __sm_ext_standby=0xB0
;      32 	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_adc_noise_red=0x10
;      33 	.SET power_ctrl_reg=mcucr
	.SET power_ctrl_reg=mcucr
;      34 	#endif
	#endif
;      35 
;      36 
;      37 #define   startCPA    TCCR1A|=0X40
;      38 #define   startCPB    TCCR1A|=0X10
;      39 
;      40 #define   stopCPA     TCCR1A&=0XBF
;      41 #define   stopCPB     TCCR1A&=0XEF
;      42 
;      43 #define   MBgo          PORTD.0=0
;      44 #define   MBback       PORTD.0=1
;      45 
;      46 #define   MAgo          PORTD.1=0
;      47 #define   MAback       PORTD.1=1
;      48 /****************************************************
;      49 全局变量声明
;      50 *****************************************************/
;      51 volatile unsigned char  flage=0X00;
_flage:
	.BYTE 0x1
;      52 volatile unsigned char Timer[3]={0,0,0};
_Timer:
	.BYTE 0x3
;      53 volatile unsigned char  write;
_write:
	.BYTE 0x1
;      54 volatile unsigned char BalanceTime=0;
_BalanceTime:
	.BYTE 0x1
;      55 unsigned char  vula_adc;
;      56  flash char *step[5]={ "stepA","stepB","stepC","back ","over "} ;
_step:
	.BYTE 0xA
;      57  flash char *mode[3]={"normal","advanc","demo  "};
_mode:
	.BYTE 0x6
;      58  flash char *display[5]={"frist","sec   ","tree  ","four  ","five  "};
_display:
	.BYTE 0xA
;      59 /******************************************************
;      60 系统中断    （显示，蜂鸣）
;      61 *******************************************************/
;      62  interrupt [TIM0_COMP ] void tim0_comp_isr(void)
;      63  {

	.CSEG
_tim0_comp_isr:
	ST   -Y,R0
	ST   -Y,R1
	ST   -Y,R15
	ST   -Y,R22
	ST   -Y,R23
	ST   -Y,R24
	ST   -Y,R25
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;      64     static unsigned char buzz;

	.DSEG

	.CSEG
;      65     switch ( flage )
	LDS  R30,_flage
;      66      {
;      67     case 0X04:
	CPI  R30,LOW(0x4)
	BRNE _0x9
;      68                     Timer[0]=Read_sec( );
	CALL SUBOPT_0x0
;      69                    if( Timer[0]==Timer[2] )
	BREQ _0xB
;      70                    ;
;      71                    else
;      72                    {
;      73                        BalanceTime++;
	CALL SUBOPT_0x1
;      74                        LCD_write_char( 13 , 1 ,  ( BalanceTime%10)+'0' );
;      75                        LCD_write_char( 12 , 1 ,  ( BalanceTime/10)+'0' );
;      76                        Timer[2]=Timer[0]   ;
;      77                     }
_0xB:
;      78                     goto j1;
	RJMP _0xC
;      79                     break;
;      80     case 0X84:
_0x9:
	CPI  R30,LOW(0x84)
	BRNE _0xD
;      81                    Timer[0]=Read_sec( );
	CALL SUBOPT_0x0
;      82                    if( Timer[0]==Timer[2] )
	BREQ _0xF
;      83                    ;
;      84                    else
;      85                    {
;      86                        BalanceTime++;
	CALL SUBOPT_0x1
;      87                        LCD_write_char( 13 , 1 ,  ( BalanceTime%10)+'0' );
;      88                        LCD_write_char( 12 , 1 ,  ( BalanceTime/10)+'0' );
;      89                        Timer[2]=Timer[0]   ;
;      90                     }
_0xF:
;      91    case 0X80:
	RJMP _0x10
_0xD:
	CPI  R30,LOW(0x80)
	BRNE _0x11
_0x10:
;      92                     if( (++buzz) == 10 )
	INC  R4
	LDI  R30,LOW(10)
	CP   R30,R4
	BRNE _0x12
;      93                       {
;      94                             buzz=0;
	CLR  R4
;      95                              PORTC.2=1;
	SBI  0x15,2
;      96                              flage -=0x80;
	LDS  R30,_flage
	SUBI R30,LOW(128)
	STS  _flage,R30
;      97                             }
;      98      j1:
_0x12:
_0xC:
;      99     case 0X00:
	RJMP _0x13
_0x11:
	CPI  R30,0
	BRNE _0x15
_0x13:
;     100                    Timer[0]=Read_sec( );
	CALL _Read_sec
	STS  _Timer,R30
;     101                    write=Timer[0];
	STS  _write,R30
;     102                    LCD_write_char( 11 , 0 ,   (write& 0x0f)+ '0' );
	LDI  R30,LOW(11)
	CALL SUBOPT_0x2
	ANDI R30,LOW(0xF)
	CALL SUBOPT_0x3
;     103                    LCD_write_char( 10 , 0 ,   ( (write>>4)&0x07) + '0' );
	LDI  R30,LOW(10)
	CALL SUBOPT_0x2
	SWAP R30
	ANDI R30,LOW(0x7)
	CALL SUBOPT_0x3
;     104 
;     105                    Timer[1]=Read_miu( );
	CALL _Read_miu
	__PUTB1MN _Timer,1
;     106                    write=Timer[1];
	__GETB1MN _Timer,1
	STS  _write,R30
;     107                    LCD_write_char(  4 , 0  ,  (write& 0x0f)+'0');
	LDI  R30,LOW(4)
	CALL SUBOPT_0x2
	ANDI R30,LOW(0xF)
	CALL SUBOPT_0x3
;     108                    break;
;     109 
;     110     default:   break;
_0x15:
;     111     }
;     112   }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R25,Y+
	LD   R24,Y+
	LD   R23,Y+
	LD   R22,Y+
	LD   R15,Y+
	LD   R1,Y+
	LD   R0,Y+
	RETI
;     113  /*****************************************************************************
;     114  系统中断T2     (寻线校正)
;     115  *****************************************************************************/
;     116   interrupt [TIM2_COMP ] void tim2_comp_isr(void)
;     117  {
_tim2_comp_isr:
	ST   -Y,R30
	IN   R30,SREG
	ST   -Y,R30
;     118      if(PORTD.1==1)
	SBIS 0x12,1
	RJMP _0x16
;     119        {
;     120 
;     121           if(PIND.6==1)
	SBIS 0x10,6
	RJMP _0x17
;     122 
;     123               startCPA;
	IN   R30,0x2F
	ORI  R30,0x40
	RJMP _0xDF
;     124 
;     125                else
_0x17:
;     126 
;     127                      stopCPA;
	IN   R30,0x2F
	ANDI R30,0xBF
_0xDF:
	OUT  0x2F,R30
;     128 
;     129 
;     130           if(PIND.7==1)
	SBIS 0x10,7
	RJMP _0x19
;     131 
;     132               startCPB;
	IN   R30,0x2F
	ORI  R30,0x10
	RJMP _0xE0
;     133 
;     134                 else
_0x19:
;     135 
;     136                      stopCPB;
	IN   R30,0x2F
	ANDI R30,0xEF
_0xE0:
	OUT  0x2F,R30
;     137 
;     138        }
;     139       else
	RJMP _0x1B
_0x16:
;     140         {
;     141            if(PIND.2==1)
	SBIS 0x10,2
	RJMP _0x1C
;     142 
;     143               startCPA;
	IN   R30,0x2F
	ORI  R30,0x40
	RJMP _0xE1
;     144 
;     145                 else
_0x1C:
;     146 
;     147                       stopCPA;
	IN   R30,0x2F
	ANDI R30,0xBF
_0xE1:
	OUT  0x2F,R30
;     148 
;     149 
;     150           if(PIND.3==1)
	SBIS 0x10,3
	RJMP _0x1E
;     151 
;     152               startCPB;
	IN   R30,0x2F
	ORI  R30,0x10
	RJMP _0xE2
;     153 
;     154                 else
_0x1E:
;     155 
;     156                       stopCPB;
	IN   R30,0x2F
	ANDI R30,0xEF
_0xE2:
	OUT  0x2F,R30
;     157          }
_0x1B:
;     158  }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R30,Y+
	RETI
;     159  /**********************************************************
;     160  读取ad值
;     161  ***********************************************************/
;     162  unsigned char get_ad(void) {
_get_ad:
;     163 
;     164 	unsigned char i;
;     165 	 ADMUX = 0x60;			              /*基准AVCC、左对齐、通道0*/
	ST   -Y,R17
;	i -> R17
	LDI  R30,LOW(96)
	OUT  0x7,R30
;     166                   ADCSRA = 0xC2;
	LDI  R30,LOW(194)
	OUT  0x6,R30
;     167                   			                               /*使能、开启、4分频*/
;     168 	while(!(ADCSRA & (1 << ADIF)));	                     /*等待*/
_0x20:
	SBIS 0x6,4
	RJMP _0x20
;     169 	i = ADCH;
	IN   R17,5
;     170 	ADCSRA &= ~(1 << ADIF);		              /*清标志*/
	CBI  0x6,4
;     171 	ADCSRA &= ~(1 << ADEN);		              /*关闭转换*/
	CBI  0x6,7
;     172 
;     173 	return i;
	MOV  R30,R17
	RJMP _0xDE
;     174 }
;     175 /*********************************************************************
;     176 蜂鸣器函数
;     177 *********************************************************************/
;     178 void beep( void )
;     179 {
_beep:
;     180    PORTC.2=0;
	CBI  0x15,2
;     181    delay_ms(500);
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	CALL SUBOPT_0x4
;     182    PORTC.2=1;
	SBI  0x15,2
;     183 }
	RET
;     184  /**********************************************************************
;     185  根据ad值选择平衡调整模式
;     186  **********************************************************************/
;     187 unsigned char  output( void )
;     188 {
_output:
;     189   if( vula_adc>=0X7D &&  vula_adc<=0X80 )
	LDI  R30,LOW(125)
	CP   R5,R30
	BRLO _0x24
	LDI  R30,LOW(128)
	CP   R30,R5
	BRSH _0x25
_0x24:
	RJMP _0x23
_0x25:
;     190                 return 0;
	LDI  R30,LOW(0)
	RET
;     191    if(  vula_adc>=0X7C && vula_adc<=0X81 )
_0x23:
	LDI  R30,LOW(124)
	CP   R5,R30
	BRLO _0x27
	LDI  R30,LOW(129)
	CP   R30,R5
	BRSH _0x28
_0x27:
	RJMP _0x26
_0x28:
;     192                   return 1 ;
	LDI  R30,LOW(1)
	RET
;     193     if(  vula_adc>=0X7B &&  vula_adc<=0X82 )
_0x26:
	LDI  R30,LOW(123)
	CP   R5,R30
	BRLO _0x2A
	LDI  R30,LOW(130)
	CP   R30,R5
	BRSH _0x2B
_0x2A:
	RJMP _0x29
_0x2B:
;     194                    return  2 ;
	LDI  R30,LOW(2)
	RET
;     195       if(  vula_adc>=0X7A &&  vula_adc<=0X83)
_0x29:
	LDI  R30,LOW(122)
	CP   R5,R30
	BRLO _0x2D
	LDI  R30,LOW(131)
	CP   R30,R5
	BRSH _0x2E
_0x2D:
	RJMP _0x2C
_0x2E:
;     196                     return 3 ;
	LDI  R30,LOW(3)
	RET
;     197        if(  vula_adc>=0X79 &&  vula_adc<=0X84)
_0x2C:
	LDI  R30,LOW(121)
	CP   R5,R30
	BRLO _0x30
	LDI  R30,LOW(132)
	CP   R30,R5
	BRSH _0x31
_0x30:
	RJMP _0x2F
_0x31:
;     198                      return  4 ;
	LDI  R30,LOW(4)
	RET
;     199           if(  vula_adc>=0X78 &&  vula_adc<=0X85)
_0x2F:
	LDI  R30,LOW(120)
	CP   R5,R30
	BRLO _0x33
	LDI  R30,LOW(133)
	CP   R30,R5
	BRSH _0x34
_0x33:
	RJMP _0x32
_0x34:
;     200                       return  5 ;
	LDI  R30,LOW(5)
	RET
;     201             if(  vula_adc>=0X77 &&  vula_adc<=0X86)
_0x32:
	LDI  R30,LOW(119)
	CP   R5,R30
	BRLO _0x36
	LDI  R30,LOW(134)
	CP   R30,R5
	BRSH _0x37
_0x36:
	RJMP _0x35
_0x37:
;     202                         return  6 ;
	LDI  R30,LOW(6)
	RET
;     203           if(  vula_adc>=0X76 &&  vula_adc<=0X87)
_0x35:
	LDI  R30,LOW(118)
	CP   R5,R30
	BRLO _0x39
	LDI  R30,LOW(135)
	CP   R30,R5
	BRSH _0x3A
_0x39:
	RJMP _0x38
_0x3A:
;     204                           return  7  ;
	LDI  R30,LOW(7)
	RET
;     205        if(  vula_adc>=0X75 && vula_adc<=0X88)
_0x38:
	LDI  R30,LOW(117)
	CP   R5,R30
	BRLO _0x3C
	LDI  R30,LOW(136)
	CP   R30,R5
	BRSH _0x3D
_0x3C:
	RJMP _0x3B
_0x3D:
;     206                             return  8 ;
	LDI  R30,LOW(8)
	RET
;     207       if(vula_adc >=0X74 && vula_adc<= 0X89)
_0x3B:
	LDI  R30,LOW(116)
	CP   R5,R30
	BRLO _0x3F
	LDI  R30,LOW(137)
	CP   R30,R5
	BRSH _0x40
_0x3F:
	RJMP _0x3E
_0x40:
;     208                            return 9;
	LDI  R30,LOW(9)
	RET
;     209 
;     210 }
_0x3E:
	RET
;     211 
;     212 /**************************************************************************************************
;     213 平衡驱动控制函数
;     214 **************************************************************************************************/
;     215 void  mortorgo( unsigned char high , unsigned char low, unsigned int  go_time, unsigned int  back_time )
;     216  {
_mortorgo:
;     217       OCR1AH=high;
;	high -> Y+5
;	low -> Y+4
;	go_time -> Y+2
;	back_time -> Y+0
	LDD  R30,Y+5
	OUT  0x2B,R30
;     218       OCR1AL = low;                                                      //new
	LDD  R30,Y+4
	OUT  0x2A,R30
;     219       OCR1BH=high;
	LDD  R30,Y+5
	OUT  0x29,R30
;     220       OCR1BL= low;
	LDD  R30,Y+4
	OUT  0x28,R30
;     221 
;     222        if( vula_adc<0x7F)
	LDI  R30,LOW(127)
	CP   R5,R30
	BRSH _0x41
;     223       {
;     224         MAgo;
	CBI  0x12,1
;     225         MBgo;
	CBI  0x12,0
;     226       }
;     227       else
	RJMP _0x42
_0x41:
;     228       {
;     229        MAback;
	SBI  0x12,1
;     230        MBback;
	SBI  0x12,0
;     231       }
_0x42:
;     232 
;     233       startCPA ;
	CALL SUBOPT_0x5
;     234       startCPB ;
;     235 
;     236       delay_ms( go_time ) ;
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	CALL SUBOPT_0x4
;     237 
;     238       stopCPA ;
	CALL SUBOPT_0x6
;     239       stopCPB;
;     240 
;     241        if( vula_adc<0x7F)
	LDI  R30,LOW(127)
	CP   R5,R30
	BRSH _0x43
;     242       {
;     243        MAback;
	SBI  0x12,1
;     244        MBback;
	SBI  0x12,0
;     245       }
;     246       else
	RJMP _0x44
_0x43:
;     247       {
;     248         MAgo;
	CBI  0x12,1
;     249         MBgo;
	CBI  0x12,0
;     250       }
_0x44:
;     251 
;     252       startCPA ;
	CALL SUBOPT_0x5
;     253       startCPB ;
;     254 
;     255       delay_ms( back_time ) ;
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x4
;     256 
;     257       stopCPA ;
	CALL SUBOPT_0x6
;     258       stopCPB;
;     259  }
	ADIW R28,6
	RET
;     260 void Findbenlen( void )
;     261 {
_Findbenlen:
;     262        unsigned char Flage_balan=1;
;     263        unsigned char Flage_select;
;     264        unsigned char count=0;
;     265    do{
	CALL __SAVELOCR4
;	Flage_balan -> R17
;	Flage_select -> R16
;	count -> R19
	LDI  R17,1
	LDI  R19,0
_0x46:
;     266 
;     267        vula_adc = get_ad ( );
	CALL _get_ad
	MOV  R5,R30
;     268 
;     269        Flage_select = output(  ) ;
	CALL _output
	MOV  R16,R30
;     270 
;     271       switch ( output(  ) ) {
	CALL _output
;     272                       case 0 :
	CPI  R30,0
	BRNE _0x4B
;     273 
;     274                                   stopCPA ;
	CALL SUBOPT_0x6
;     275                                   stopCPB;
;     276                                   if(count<=9)
	CPI  R19,10
	BRSH _0x4C
;     277                                        count++;
	SUBI R19,-1
;     278                                      else
	RJMP _0x4D
_0x4C:
;     279                                        Flage_balan=0;
	LDI  R17,LOW(0)
;     280                                  break;
_0x4D:
	RJMP _0x4A
;     281                       case 1 :
_0x4B:
	CPI  R30,LOW(0x1)
	BRNE _0x4E
;     282                                   count=0;
	LDI  R19,LOW(0)
;     283                                   mortorgo( 0x03,0xFF , 400, 350 ) ;
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(255)
	ST   -Y,R30
	LDI  R30,LOW(400)
	LDI  R31,HIGH(400)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(350)
	LDI  R31,HIGH(350)
	RJMP _0xE3
;     284                                  break;
;     285                       case 2 :
_0x4E:
	CPI  R30,LOW(0x2)
	BRNE _0x4F
;     286                                   count=0;
	CALL SUBOPT_0x7
;     287                                   mortorgo( 0x02,0xFF , 800, 700 ) ;
	LDI  R30,LOW(800)
	LDI  R31,HIGH(800)
	CALL SUBOPT_0x8
	RJMP _0xE3
;     288                                  break;
;     289                       case 3 :
_0x4F:
	CPI  R30,LOW(0x3)
	BRNE _0x50
;     290                                  count=0;
	CALL SUBOPT_0x7
;     291                                  mortorgo( 0x02,0xFF , 900, 800 ) ;
	CALL SUBOPT_0x9
	LDI  R30,LOW(800)
	LDI  R31,HIGH(800)
	RJMP _0xE3
;     292                                  break;
;     293                       case 4 :
_0x50:
	CPI  R30,LOW(0x4)
	BRNE _0x51
;     294                                  count=0;
	CALL SUBOPT_0x7
;     295                                  mortorgo( 0x02,0xFF , 900, 700 ) ;
	LDI  R30,LOW(900)
	LDI  R31,HIGH(900)
	CALL SUBOPT_0x8
	RJMP _0xE3
;     296                                  break;
;     297                       case 5 :
_0x51:
	CPI  R30,LOW(0x5)
	BRNE _0x52
;     298                                   count=0;
	CALL SUBOPT_0x7
;     299                                 mortorgo( 0x02,0xFF , 900, 650 ) ;
	CALL SUBOPT_0x9
	LDI  R30,LOW(650)
	LDI  R31,HIGH(650)
	RJMP _0xE3
;     300                                  break;
;     301                       case 6 :
_0x52:
	CPI  R30,LOW(0x6)
	BRNE _0x53
;     302                                   count=0;
	CALL SUBOPT_0x7
;     303                                  mortorgo( 0x02,0xFF , 900,600 ) ;
	CALL SUBOPT_0x9
	LDI  R30,LOW(600)
	LDI  R31,HIGH(600)
	RJMP _0xE3
;     304                                  break;
;     305                        case 7 :
_0x53:
	CPI  R30,LOW(0x7)
	BRNE _0x54
;     306                                   count=0;
	CALL SUBOPT_0x7
;     307                                   mortorgo( 0x02,0xFF , 1000, 700 ) ;
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	CALL SUBOPT_0x8
	RJMP _0xE3
;     308                                  break;
;     309                        case 8 :
_0x54:
	CPI  R30,LOW(0x8)
	BRNE _0x55
;     310                                  count=0;
	CALL SUBOPT_0x7
;     311                                  mortorgo( 0x02,0xFF , 1000,600 ) ;
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(600)
	LDI  R31,HIGH(600)
	RJMP _0xE3
;     312                                 break;
;     313                        case 9 :
_0x55:
	CPI  R30,LOW(0x9)
	BRNE _0x57
;     314                                   count=0;
;     315                                   mortorgo( 0x02,0xFF , 1000,500) ;
;     316                                   break;
;     317 
;     318                     default:
_0x57:
;     319                                   count=0;
_0xE4:
	LDI  R19,LOW(0)
;     320                                   mortorgo( 0x02,0xFF , 1000,500) ;
	LDI  R30,LOW(2)
	ST   -Y,R30
	LDI  R30,LOW(255)
	ST   -Y,R30
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
_0xE3:
	ST   -Y,R31
	ST   -Y,R30
	CALL _mortorgo
;     321                                  break;
;     322     };
_0x4A:
;     323        delay_ms(200);
	LDI  R30,LOW(200)
	LDI  R31,HIGH(200)
	CALL SUBOPT_0x4
;     324   }
;     325 
;     326  while( Flage_balan );
	CPI  R17,0
	BREQ _0x47
	RJMP _0x46
_0x47:
;     327 }
	CALL __LOADLOCR4
	ADIW R28,4
	RET
;     328 
;     329 unsigned char SelectMode ( void )
;     330 {
_SelectMode:
;     331   unsigned char temp=0;
;     332   LCD_write_str( 0 , 0 ,"press START key");
	ST   -Y,R17
;	temp -> R17
	LDI  R17,0
	CALL SUBOPT_0xA
	__POINTW1FN _0,85
	CALL SUBOPT_0xB
;     333   LCD_write_str( 0 , 1 ,"zhong bei da xue ");
	CALL SUBOPT_0xC
	__POINTW1FN _0,101
	CALL SUBOPT_0xB
;     334 
;     335   while( PINC.6 ==1 );
_0x58:
	SBIC 0x13,6
	RJMP _0x58
;     336   while(PINC.6 ==0);
_0x5B:
	SBIS 0x13,6
	RJMP _0x5B
;     337 
;     338   LCD_clear( );
	CALL _LCD_clear
;     339   LCD_write_str( 0 , 0 ,"mode:");
	CALL SUBOPT_0xA
	__POINTW1FN _0,119
	CALL SUBOPT_0xB
;     340   LCD_write_str( 0 , 1 ,"select mode");
	CALL SUBOPT_0xC
	__POINTW1FN _0,125
	CALL SUBOPT_0xB
;     341   LCD_write_str( 6 , 0 ,mode[0]);
	LDI  R30,LOW(6)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	LDS  R30,_mode
	LDS  R31,_mode+1
	CALL SUBOPT_0xB
;     342     delay_ms( 1000 );
	CALL SUBOPT_0xD
;     343   do
_0x5F:
;     344     {
;     345       if( PINC.7==0 )
	SBIC 0x13,7
	RJMP _0x61
;     346           {
;     347              while(PINC.7==0);
_0x62:
	SBIS 0x13,7
	RJMP _0x62
;     348 				++temp;
	SUBI R17,-LOW(1)
;     349 
;     350              if(temp==3)
	CPI  R17,3
	BRNE _0x65
;     351                 temp=0;
	LDI  R17,LOW(0)
;     352 
;     353                LCD_write_str( 6 , 0 ,mode[temp]);
_0x65:
	LDI  R30,LOW(6)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	MOV  R30,R17
	LDI  R26,LOW(_mode)
	LDI  R27,HIGH(_mode)
	CALL SUBOPT_0xE
;     354           }
;     355     }
_0x61:
;     356      while(PINC.6==1);
	SBIC 0x13,6
	RJMP _0x5F
;     357      while(PINC.6==0);
_0x66:
	SBIS 0x13,6
	RJMP _0x66
;     358       LCD_clear( );
	CALL _LCD_clear
;     359    return(temp);
	MOV  R30,R17
_0xDE:
	LD   R17,Y+
	RET
;     360 }
;     361 
;     362 /*******************************************************
;     363 基本要求部分
;     364 *******************************************************/
;     365 void nomal ( void )
;     366 {
_nomal:
;     367  /*****************************************************
;     368  显示时钟，初始设置
;     369  *****************************************************/
;     370     LCD_write_str( 0 , 0 ,"minc:  sec: ");
	CALL SUBOPT_0xA
	__POINTW1FN _0,137
	CALL SUBOPT_0xB
;     371     LCD_write_str( 0 ,  1 , step[0] );
	CALL SUBOPT_0xC
	LDS  R30,_step
	LDS  R31,_step+1
	CALL SUBOPT_0xB
;     372     delay_ms(1000);
	CALL SUBOPT_0xD
;     373     beep(  );
	CALL SUBOPT_0xF
;     374     start_PCF( );
;     375 /********************************************************
;     376   小车动作初始化
;     377 ********************************************************/
;     378     MBgo ;
	CBI  0x12,0
;     379     MAgo;
	CBI  0x12,1
;     380     OCR1AH=0x02;
	CALL SUBOPT_0x10
;     381     OCR1AL =0xFF;                                                       //new
;     382     OCR1BH=0x02;
;     383     OCR1BL=0xFF;
;     384     startCPA ;
;     385      startCPB ;
;     386     delay_ms(2000);
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	CALL SUBOPT_0x4
;     387     OCR1AH=0x01;
	CALL SUBOPT_0x11
;     388     OCR1AL =0xFF;                                                       //new
;     389     OCR1BH=0x01;
;     390     OCR1BL=0xFF;
;     391     TCCR2  = 0x0A;                                   //自动寻线开
	LDI  R30,LOW(10)
	OUT  0x25,R30
;     392 
;     393     SEI( );                                                  //全局中断开
	sei
;     394     /**********************************************
;     395     第一阶段
;     396     **********************************************/
;     397    while ( Timer[0] != 0X14 ) ;
_0x69:
	LDS  R26,_Timer
	CPI  R26,LOW(0x14)
	BRNE _0x69
;     398    LCD_write_str( 0 ,  1 , step[1] );
	CALL SUBOPT_0xC
	__GETW1MN _step,2
	CALL SUBOPT_0xB
;     399 
;     400    CLI( );
	cli
;     401    beep(  );
	CALL SUBOPT_0xF
;     402    start_PCF( );
;     403    SEI( );
	sei
;     404    /************************************************
;     405    寻找平衡
;     406    ************************************************/
;     407    TCCR2  = 0x00;                                      //寻线关
	LDI  R30,LOW(0)
	OUT  0x25,R30
;     408     Findbenlen(  );
	CALL SUBOPT_0x12
;     409     PORTC.2=0;
;     410     flage+=0x80;                                        //开启蜂鸣，中断关闭
;     411 
;     412    /************************************************
;     413    找到平衡
;     414    ************************************************/
;     415    CLI( );
	cli
;     416    start_PCF( );                                                     //需要改动
	CALL _start_PCF
;     417    LCD_write_str( 0 ,  1 , step[2] );
	CALL SUBOPT_0xC
	__GETW1MN _step,4
	CALL SUBOPT_0xB
;     418    LCD_write_str( 6 ,  1 , "Balan:"  );
	CALL SUBOPT_0x13
;     419    flage+=0x04;                                                      //显示平衡
	LDS  R30,_flage
	SUBI R30,-LOW(4)
	STS  _flage,R30
;     420     SEI( );
	sei
;     421 
;     422     do
_0x6D:
;     423        {
;     424         if( Timer[0]==0X05 )
	LDS  R26,_Timer
	CPI  R26,LOW(0x5)
	BRNE _0x6F
;     425             {
;     426                          flage -=0x04;
	LDS  R30,_flage
	SUBI R30,LOW(4)
	STS  _flage,R30
;     427                          CLI( );
	cli
;     428                          start_PCF( );
	CALL _start_PCF
;     429                          SEI( );
	sei
;     430             }
;     431        }
_0x6F:
;     432    while ( (flage==0X04 )|| ( flage==0X84  ) );
	LDS  R26,_flage
	CPI  R26,LOW(0x4)
	BREQ _0x70
	CPI  R26,LOW(0x84)
	BRNE _0x6E
_0x70:
	JMP  _0x6D
_0x6E:
;     433 
;     434 
;     435    /***************************************************
;     436    改变速度
;     437    ***************************************************/
;     438    CLI( );
	cli
;     439    OCR1AH=0x01;
	CALL SUBOPT_0x11
;     440    OCR1AL =0xFF;                                                       //new
;     441    OCR1BH=0x01;
;     442    OCR1BL=0xFF;
;     443    SEI( );
	sei
;     444 
;     445     MBgo;
	CBI  0x12,0
;     446     MAgo;
	CBI  0x12,1
;     447     startCPA ;
	CALL SUBOPT_0x5
;     448     startCPB ;
;     449   /************************************************
;     450    慢走，直到传感器感知木板落下
;     451   ************************************************/
;     452   TCCR2  = 0x0A;
	LDI  R30,LOW(10)
	OUT  0x25,R30
;     453    /**********************************************
;     454    检测传感器状态，没黑线时停下
;     455    ***********************************************/
;     456   while(PIND.2||PIND.3);                                           //关寻线
_0x72:
	SBIC 0x10,2
	RJMP _0x75
	SBIS 0x10,3
	RJMP _0x74
_0x75:
	RJMP _0x72
_0x74:
;     457 
;     458 
;     459   CLI( );
	cli
;     460   stopCPA ;
	CALL SUBOPT_0x6
;     461   stopCPB;
;     462   TCCR2  = 0x00;
	LDI  R30,LOW(0)
	OUT  0x25,R30
;     463   beep(  );
	CALL SUBOPT_0xF
;     464   start_PCF( );
;     465   delay_us(10);
	__DELAY_USB 3
;     466   SEI( );
	sei
;     467   /***********************************************
;     468    等待五秒，倒车返回
;     469   ***********************************************/
;     470   MBback;
	SBI  0x12,0
;     471   MAback;
	SBI  0x12,1
;     472   while( Timer[0]!=0X05 )  ;
_0x77:
	LDS  R26,_Timer
	CPI  R26,LOW(0x5)
	BRNE _0x77
;     473   LCD_write_str( 0 ,  1 , step[3] );
	CALL SUBOPT_0xC
	__GETW1MN _step,6
	CALL SUBOPT_0xB
;     474   beep(  );
	CALL _beep
;     475 
;     476   OCR1AH=0x02;
	CALL SUBOPT_0x10
;     477   OCR1AL =0xFF;                                                       //new
;     478   OCR1BH=0x02;
;     479   OCR1BL=0xFF;
;     480 
;     481   startCPA ;
;     482   startCPB ;
;     483 
;     484    TCCR2  = 0x0A;
	LDI  R30,LOW(10)
	OUT  0x25,R30
;     485 
;     486   delay_ms(2000);
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	CALL SUBOPT_0x4
;     487 
;     488    OCR1AH=0x01;
	CALL SUBOPT_0x11
;     489   OCR1AL =0xFF;                                                       //new
;     490   OCR1BH=0x01;
;     491   OCR1BL=0xFF;
;     492 
;     493 
;     494    while(PIND.6||PIND.7);
_0x7A:
	SBIC 0x10,6
	RJMP _0x7D
	SBIS 0x10,7
	RJMP _0x7C
_0x7D:
	RJMP _0x7A
_0x7C:
;     495    beep( );
	CALL _beep
;     496    LCD_write_str( 0 ,  1 , step[4] );
	CALL SUBOPT_0xC
	__GETW1MN _step,8
	CALL SUBOPT_0xB
;     497 
;     498    CLI( );
	cli
;     499 
;     500    stopCPA ;
	CALL SUBOPT_0x6
;     501   stopCPB;
;     502  }
	RET
;     503 
;     504   /******************************************************
;     505   发挥部分
;     506   ******************************************************/
;     507 void advance ( void )
;     508 {
_advance:
;     509    unsigned char find=1;
;     510    unsigned char j=0;
;     511 
;     512    LCD_write_str( 0 , 0 ,"min:  sec: ");
	ST   -Y,R17
	ST   -Y,R16
;	find -> R17
;	j -> R16
	LDI  R16,0
	LDI  R17,1
	CALL SUBOPT_0xA
	__POINTW1FN _0,157
	CALL SUBOPT_0xB
;     513 
;     514    start_PCF( );
	CALL _start_PCF
;     515 
;     516    MBgo ;
	CBI  0x12,0
;     517    MAgo;
	CBI  0x12,1
;     518    startCPA  ;
	CALL SUBOPT_0x5
;     519    startCPB  ;
;     520 
;     521     SEI( );
	sei
;     522  /*****************************************************
;     523 　当两个传感器都在线上时开启寻线功能
;     524  *****************************************************/
;     525    while(find)
_0x7F:
	CPI  R17,0
	BREQ _0x81
;     526    {
;     527      if( PIND.2==1 )
	SBIC 0x10,2
;     528         find=0;
	LDI  R17,LOW(0)
;     529 
;     530       if(PIND.3==1)
	SBIC 0x10,3
;     531          find=0;
	LDI  R17,LOW(0)
;     532    } ;
	RJMP _0x7F
_0x81:
;     533 
;     534  TCCR2  = 0x0A;
	LDI  R30,LOW(10)
	OUT  0x25,R30
;     535 
;     536  /*****************************************************
;     537  一定时间后，减速找平衡
;     538  *****************************************************/
;     539     while( Timer[0]<=0X30  );
_0x84:
	LDS  R26,_Timer
	CPI  R26,LOW(0x31)
	BRLO _0x84
;     540     while ( get_ad( ) < 0X7F ) ;
_0x87:
	CALL _get_ad
	CPI  R30,LOW(0x7F)
	BRLO _0x87
;     541        TCCR2  = 0x00;
	LDI  R30,LOW(0)
	OUT  0x25,R30
;     542  /****************************************************
;     543  找到后，给出平衡指示，
;     544  *****************************************************/
;     545   do{
_0x8B:
;     546         Findbenlen(  );
	CALL SUBOPT_0x12
;     547         PORTC.2=0;
;     548         flage+=0x80;
;     549 
;     550          CLI( );
	cli
;     551          LCD_write_str( 0 ,  1 ,display[j] );
	CALL SUBOPT_0xC
	MOV  R30,R16
	LDI  R26,LOW(_display)
	LDI  R27,HIGH(_display)
	CALL SUBOPT_0xE
;     552          LCD_write_str( 6 ,  1 , "Balan:"  );
	CALL SUBOPT_0x13
;     553          flage+=0x04;
	LDS  R30,_flage
	SUBI R30,-LOW(4)
	STS  _flage,R30
;     554          SEI( );
	sei
;     555 /******************************************************
;     556 检测平衡状态，不平衡时继续寻找
;     557 *******************************************************/
;     558           while( get_ad( ) < 0X81 &&  get_ad( )>  0X7B );
_0x8D:
	CALL _get_ad
	CPI  R30,LOW(0x81)
	BRSH _0x90
	CALL _get_ad
	CPI  R30,LOW(0x7C)
	BRSH _0x91
_0x90:
	RJMP _0x8F
_0x91:
	RJMP _0x8D
_0x8F:
;     559           flage -=0x04;
	LDS  R30,_flage
	SUBI R30,LOW(4)
	STS  _flage,R30
;     560           BalanceTime=0;                                                  //平衡显示清零
	LDI  R30,LOW(0)
	STS  _BalanceTime,R30
;     561           LCD_write_str( 0,  1 , "                                   "  );
	CALL SUBOPT_0xC
	__POINTW1FN _0,169
	CALL SUBOPT_0xB
;     562          if(j<5)
	CPI  R16,5
	BRSH _0x92
;     563           j++;
	SUBI R16,-1
;     564           else
	RJMP _0x93
_0x92:
;     565           j=0;
	LDI  R16,LOW(0)
;     566      }
_0x93:
;     567     while(1);
	JMP  _0x8B
;     568 
;     569 /*******************************************************
;     570 找到后，给出平衡指示
;     571 *******************************************************/
;     572 /*******************************************************
;     573    Findbenlen(  );
;     574    PORTC.2=0;
;     575    flage+=0x80;
;     576 
;     577    CLI( );
;     578    LCD_write_str( 0 ,  1 , display[1] );
;     579    LCD_write_str( 6 ,  1 , "Balan:"  );
;     580    flage+=0x04;
;     581    SEI( );
;     582 
;     583     delay_ms( 5000 );
;     584     flage -=0x04;                                                         //关闭平衡显示
;     585     TCCR0  = 0x00;                                                     //关闭显示
;     586 
;     587     MBgo ;
;     588     MAback ;
;     589     OCR1AH=0x02;
;     590     OCR1AL =0xFF;                                                       //new
;     591     OCR1BH=0x02;
;     592     OCR1BL=0xFF;
;     593     startCPA  ;
;     594     startCPB  ;
;     595     delay_ms( 4000 );                                                  //速度减慢一倍，延时增加一倍
;     596      find=1;
;     597       LCD_write_str( 0 ,  1 ,display[2] );
;     598 
;     599      while(find)                                                //当两个传感器有一个在线上时
;     600    {
;     601      if( PIND.2==1 )
;     602         find=0;
;     603 
;     604       if(PIND.3==1)
;     605          find=0;
;     606    } ;
;     607 
;     608     MBgo ;
;     609     MAgo;
;     610 
;     611     TCCR2  = 0x0A;                                               //开寻线
;     612 
;     613     delay_ms(5000);
;     614                                                                           //5秒后加速
;     615     OCR1AH=0x01;
;     616     OCR1AL =0xFF;                                                       //new
;     617     OCR1BH=0x01;
;     618     OCR1BL=0xFF;
;     619 
;     620     delay_ms(5000);
;     621 
;     622                                                                            //关寻线
;     623     TCCR2  = 0x00;
;     624 
;     625     stopCPA ;
;     626      stopCPB;
;     627     MBgo ;                                                              //梦幻舞步
;     628     MAback;
;     629     startCPA  ;
;     630     startCPB  ;
;     631 
;     632     find=1;
;     633     delay_ms(6000);
;     634 
;     635        while(find)                                                //当两个传感器有一个在线上时
;     636    {
;     637      if( PIND.2==1 )
;     638         find=0;
;     639 
;     640       if(PIND.3==1)
;     641          find=0;
;     642   } ;
;     643 
;     644     MBgo ;                                                              //梦幻舞步
;     645     MAgo;
;     646      TCCR2  = 0x0A;
;     647                                                     //开寻线
;     648      delay_ms(5000);
;     649 
;     650 //***********************************************************************
;     651 反向转弯
;     652 //***********************************************************************
;     653       TCCR2  = 0x00;
;     654 
;     655      stopCPA ;
;     656      stopCPB;
;     657     MBback ;                                                              //梦幻舞步
;     658     MAgo;
;     659     delay_ms(1000);
;     660     startCPA  ;
;     661     startCPB  ;
;     662 
;     663     find=1;
;     664     delay_ms(6000);
;     665 
;     666        while(find)                                                //当两个传感器有一个在线上时
;     667    {
;     668      if( PIND.2==1 )
;     669         find=0;
;     670 
;     671       if(PIND.3==1)
;     672          find=0;
;     673   } ;
;     674 
;     675     MBgo ;                                                              //梦幻舞步
;     676     MAgo;
;     677      TCCR2  = 0x0A;
;     678 
;     679   **************************************************************************/
;     680 }
;     681 
;     682 void demo( void )
;     683 {
_demo:
;     684 
;     685     unsigned char find=1;
;     686 
;     687     LCD_write_str( 0 , 0 ,"min:  sec: ");
	ST   -Y,R17
;	find -> R17
	LDI  R17,1
	CALL SUBOPT_0xA
	__POINTW1FN _0,157
	CALL SUBOPT_0xB
;     688 
;     689     start_PCF( );
	CALL _start_PCF
;     690 
;     691     MAgo ;
	CBI  0x12,1
;     692     MBback;
	SBI  0x12,0
;     693     startCPA  ;
	CALL SUBOPT_0x5
;     694     startCPB  ;
;     695 
;     696     SEI( );
	sei
;     697 
;     698      find=1;
	LDI  R17,LOW(1)
;     699 
;     700      while(find)                                                //当两个传感器有一个在线上时
_0x94:
	CPI  R17,0
	BREQ _0x96
;     701    {
;     702      if( PIND.2==1 )
	SBIC 0x10,2
;     703         find=0;
	LDI  R17,LOW(0)
;     704 
;     705       if(PIND.3==1)
	SBIC 0x10,3
;     706          find=0;
	LDI  R17,LOW(0)
;     707    } ;
	RJMP _0x94
_0x96:
;     708 
;     709     MBgo ;
	CALL SUBOPT_0x14
;     710     MAgo;
;     711 
;     712     TCCR2  = 0x0A;                                               //开寻线
;     713 
;     714     delay_ms(5000);
	LDI  R30,LOW(5000)
	LDI  R31,HIGH(5000)
	CALL SUBOPT_0x4
;     715                                                                           //5秒后加速
;     716 
;     717 
;     718     delay_ms(5000);
	LDI  R30,LOW(5000)
	LDI  R31,HIGH(5000)
	CALL SUBOPT_0x4
;     719 
;     720                                                                            //关寻线
;     721     TCCR2  = 0x00;
	LDI  R30,LOW(0)
	OUT  0x25,R30
;     722 
;     723     stopCPA ;
	CALL SUBOPT_0x6
;     724      stopCPB;
;     725     MBgo ;                                                              //梦幻舞步
	CBI  0x12,0
;     726     MAback;
	SBI  0x12,1
;     727     startCPA  ;
	CALL SUBOPT_0x5
;     728     startCPB  ;
;     729 
;     730     find=1;
	LDI  R17,LOW(1)
;     731     delay_ms(10000);
	LDI  R30,LOW(10000)
	LDI  R31,HIGH(10000)
	CALL SUBOPT_0x4
;     732 
;     733        while(find)                                                //当两个传感器有一个在线上时
_0x99:
	CPI  R17,0
	BREQ _0x9B
;     734    {
;     735      if( PIND.2==1 )
	SBIC 0x10,2
;     736         find=0;
	LDI  R17,LOW(0)
;     737 
;     738       if(PIND.3==1)
	SBIC 0x10,3
;     739          find=0;
	LDI  R17,LOW(0)
;     740   } ;
	RJMP _0x99
_0x9B:
;     741 
;     742     MBgo ;                                                              //梦幻舞步
	CALL SUBOPT_0x14
;     743     MAgo;
;     744      TCCR2  = 0x0A;
;     745                                                     //开寻线
;     746      delay_ms(8000);
	LDI  R30,LOW(8000)
	LDI  R31,HIGH(8000)
	CALL SUBOPT_0x4
;     747 
;     748 //***********************************************************************
;     749 //反向转弯
;     750 //***********************************************************************
;     751       TCCR2  = 0x00;
	LDI  R30,LOW(0)
	OUT  0x25,R30
;     752 
;     753      stopCPA ;
	CALL SUBOPT_0x6
;     754      stopCPB;
;     755     MBback ;                                                              //梦幻舞步
	SBI  0x12,0
;     756     MAgo;
	CBI  0x12,1
;     757     delay_ms(1000);
	CALL SUBOPT_0xD
;     758     startCPA  ;
	CALL SUBOPT_0x5
;     759     startCPB  ;
;     760 
;     761     find=1;
	LDI  R17,LOW(1)
;     762     delay_ms(9000);
	LDI  R30,LOW(9000)
	LDI  R31,HIGH(9000)
	CALL SUBOPT_0x4
;     763 
;     764        while(find)                                                //当两个传感器有一个在线上时
_0x9E:
	CPI  R17,0
	BREQ _0xA0
;     765    {
;     766      if( PIND.2==1 )
	SBIC 0x10,2
;     767         find=0;
	LDI  R17,LOW(0)
;     768 
;     769       if(PIND.3==1)
	SBIC 0x10,3
;     770          find=0;
	LDI  R17,LOW(0)
;     771   } ;
	RJMP _0x9E
_0xA0:
;     772 
;     773     MBgo ;                                                              //梦幻舞步
	CALL SUBOPT_0x14
;     774     MAgo;
;     775      TCCR2  = 0x0A;
;     776 
;     777 
;     778 }
	LD   R17,Y+
	RET
;     779 
;     780 void main ( void )
;     781 {
_main:
;     782  /*******************************************************
;     783 变量定义
;     784  *******************************************************/
;     785   unsigned char  key;
;     786 
;     787 
;     788 /********************************************************
;     789 初始化设备
;     790 *********************************************************/
;     791   Init_IO( );
;	key -> R17
	CALL _Init_IO
;     792   Init_T0(  );
	CALL _Init_T0
;     793   Init_T1(  );
	CALL _Init_T1
;     794   Init_T2(  );
	CALL _Init_T2
;     795   Init_TWI( );
	CALL _Init_TWI
;     796   Init_device( ) ;
	CALL _Init_device
;     797   LCD_init( );
	CALL _LCD_init
;     798 
;     799   delay_ms( 10 );
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL SUBOPT_0x4
;     800   LCD_clear( );
	CALL _LCD_clear
;     801   delay_ms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x4
;     802 
;     803   /********************************************************
;     804   模式选择
;     805   ********************************************************/
;     806   key=SelectMode( );
	CALL _SelectMode
	MOV  R17,R30
;     807 
;     808 switch (key)
	MOV  R30,R17
;     809     {
;     810     case 0:
	CPI  R30,0
	BRNE _0xA6
;     811                  nomal (  );
	CALL _nomal
;     812                  break;
	RJMP _0xA5
;     813      case 1:
_0xA6:
	CPI  R30,LOW(0x1)
	BRNE _0xA7
;     814                  advance ( );
	CALL _advance
;     815                    break;
	RJMP _0xA5
;     816       case 2:
_0xA7:
	CPI  R30,LOW(0x2)
	BRNE _0xA9
;     817                   demo( );
	CALL _demo
;     818                   break;
;     819     default:
_0xA9:
;     820                    break;
;     821     };
_0xA5:
;     822 
;     823 
;     824   while(1);                                             //结束
_0xAA:
	RJMP _0xAA
;     825 }
_0xAD:
	RJMP _0xAD
;     826 /*********************************************************************************************/
;     827 /********************以下是对ATmega16的自带I2C定义和使用**************************************/
;     828 /*********************************************************************************************/
;     829 #include "config.h"
;     830 	#ifndef __SLEEP_DEFINED__
	#ifndef __SLEEP_DEFINED__
;     831 	#define __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
;     832 	.EQU __se_bit=0x40
	.EQU __se_bit=0x40
;     833 	.EQU __sm_mask=0xB0
	.EQU __sm_mask=0xB0
;     834 	.EQU __sm_powerdown=0x20
	.EQU __sm_powerdown=0x20
;     835 	.EQU __sm_powersave=0x30
	.EQU __sm_powersave=0x30
;     836 	.EQU __sm_standby=0xA0
	.EQU __sm_standby=0xA0
;     837 	.EQU __sm_ext_standby=0xB0
	.EQU __sm_ext_standby=0xB0
;     838 	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_adc_noise_red=0x10
;     839 	.SET power_ctrl_reg=mcucr
	.SET power_ctrl_reg=mcucr
;     840 	#endif
	#endif
;     841 #include "TWI.h"
;     842 
;     843 /*
;     844 extern void Init_TWI(void) ;  															//TWI初始化
;     845 extern uchar TWI_Send(uchar Slave,uchar Address,uchar Data);                                 				//向IIC地址为Slave的从机的Address地址发送(写)一字节数据Data
;     846 extern uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT) ;                                	//向IIC地址为Slave的从机的Address地址开始发送N字节数据Data
;     847 extern uchar TWI_Receive(uchar Slave,uchar Address) ;                                                                          //从IIC地址为Slave的从机的Address地址读取一字节数据Data，返回值为读取的数据
;     848 */
;     849 //************TWI初始化*************/
;     850 
;     851 
;     852 
;     853 
;     854 /****************************************************
;     855 *****************************************************
;     856 向IIC地址为Slave的从机的Address地址发送(写)一字节数据Data
;     857 返回0:写成功
;     858 返回非0:写失败
;     859 ******************************************************
;     860 ******************************************************/
;     861 uchar TWI_Send(uchar Slave,uchar Address,uchar Data)
;     862 {
_TWI_Send:
;     863 
;     864 	TWCR=0x80|0x20|0x04;                                             //或:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);对TWINT写1清除，使能TWI，发出Start信号
;	Slave -> Y+2
;	Address -> Y+1
;	Data -> Y+0
	LDI  R30,LOW(164)
	OUT  0x36,R30
;     865 	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Start信号已发出
_0xAE:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xAE
;     866 	if((TWSR&0xf8)!=0x08)                                            //或:if ((TWSR & 0xF8) != START),检测状态寄存器,屏蔽预分频位,如果状态字不是START转出错处理
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x8)
	BREQ _0xB1
;     867 	return(1);                                                       //返回值1，表明从机没有对Start信号作应答
	LDI  R30,LOW(1)
	RJMP _0xDD
;     868 
;     869                                                                  //Slave即SLA+W,即是从机地址 ,可以是0x18或0x20,此处只用0x18
;     870                                                                  //所谓的应答是“从器件”在收到地址和“写”后，将SDA电压拉低，由“主器件”读取
;     871                                                                  //ATmge16如果读到这个“低”电压则返回0x18,否则返回0x20。
;     872 	TWDR=Slave;                                                      //更新地址寄存器,装Slave入到TWDR寄存器
_0xB1:
	LDD  R30,Y+2
	OUT  0x3,R30
;     873 	TWCR=0x80|0x04;                                                  //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出从机地址信息
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     874 	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位,表明Slave信号已发出
_0xB2:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xB2
;     875 	if((TWSR&0xf8)!=0x18)                                            //或:if ((TWSR & 0xF8) != MT_SLA_ACK),检测状态寄存器,
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x18)
	BREQ _0xB5
;     876 	return(2);                                                       //返回值2，表明从机没有对Slave信号作应答
	LDI  R30,LOW(2)
	RJMP _0xDD
;     877 
;     878                                                                  //Address是MT_SLA_ACK是否控发送器地址,可以是0x28或0x30, 此处只用0x28
;     879 	TWDR=Address;
_0xB5:
	LDD  R30,Y+1
	OUT  0x3,R30
;     880 	TWCR=0x80|0x04;                                                  //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出寄存器地址信息
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     881 	while((TWCR&0x80)!=0x80) ;                                      //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Address信号已发出
_0xB6:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xB6
;     882 	if((TWSR&0xf8)!=0x28)                                           //检测状态寄存器(MT_DATA_ACK)
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x28)
	BREQ _0xB9
;     883 	return(3);                                                      //返回值3，表明从机没有对Address信号作应答
	LDI  R30,LOW(3)
	RJMP _0xDD
;     884 
;     885 	TWDR=Data;                                                      //写数据到ZLG7290
_0xB9:
	LD   R30,Y
	OUT  0x3,R30
;     886 	TWCR=0x80|0x04;                                                 //或:TWCR = (1<<TWINT) | (1<<TWEN);对TWINT写1清除，使能TWI；发出数据信息
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     887 	while((TWCR&0x80)!=0x80) ;                                       //或:while (!(TWCR & (1<<TWINT)));等待TWINT置位，表明Data信号已发出
_0xBA:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xBA
;     888 	if((TWSR&0xf8)!=0x28)                                           //if ((TWSR & 0xF8) != MT_DATA_ACK);检测状态寄存器
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x28)
	BREQ _0xBD
;     889 	return(4);                                                      //返回值3，表明从机没有对Data信号作应答
	LDI  R30,LOW(4)
	RJMP _0xDD
;     890 
;     891 	TWCR=0x80|0x04|0x10;                                            //或:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);对TWINT写1清除，使能TWI，发出Stop信号
_0xBD:
	LDI  R30,LOW(148)
	OUT  0x36,R30
;     892 	return(0);
	LDI  R30,LOW(0)
	RJMP _0xDD
;     893 
;     894 }
;     895 
;     896 
;     897 
;     898 
;     899 
;     900 /******************************************************************
;     901 *********************************************************************
;     902 向IIC地址为Slave的从机的Address地址开始发送N字节数据Data
;     903 ******************************************************************
;     904 *******************************************************************/
;     905 
;     906 /*uchar TWI_SendStr(uchar Slave,uchar Address,uchar *Array,uchar CNT)
;     907  {
;     908     uchar Count;
;     909     TWCR=0x80|0x20|0x04;                                                      //对TWINT写1清除，使能TWI，发出Start信号
;     910     while((TWCR&0x80)!=0x80) ;                                                //等待TWINT置位，表明Start信号已发出
;     911     if((TWSR&0xf8)!=0x08)                                                     //检测状态寄存器
;     912     return(1);                                                                //返回值1，表明从机没有对Start信号作应答
;     913 
;     914     TWDR=Slave;                                                               //更新地址寄存器
;     915     TWCR=0x80|0x04;                                                           //对TWINT写1清除，使能TWI；发出从机地址信息
;     916     while((TWCR&0x80)!=0x80) ;                                                //等待TWINT置位，表明Slave信号已发出
;     917     if((TWSR&0xf8)!=0x18)                                                     //检测状态寄存器
;     918     return(2);                                                                //返回值2，表明从机没有对Slave信号作应答
;     919 
;     920     TWDR=Address;
;     921     TWCR=0x80|0x04;                                                            //对TWINT写1清除，使能TWI；发出寄存器地址信息
;     922     while((TWCR&0x80)!=0x80) ;                                                 //等待TWINT置位，表明Address信号已发出
;     923     if((TWSR&0xf8)!=0x28)                                                      //检测状态寄存器
;     924     return(3);                                                                 //返回值3，表明从机没有对Address信号作应答
;     925 
;     926     for(Count=0;Count<CNT;Count++)                                             //连续写N个字节 ,实现发送N字节数据Data
;     927        {
;     928 		 TWDR=Array[Count];                                                    //建立数组装载TWDR
;     929 		 TWCR=0x80|0x04;                                                       //或(1<<TWINT)|(1<<TWEN);对TWINT写1清除，使能TWI；发出数据信息
;     930 		 while((TWCR&0x80)!=0x80) ;                                            //等待TWINT置位，表明Data信号已发出
;     931 		 if((TWSR&0xf8)!=0x28)                                                 //检测状态寄存器
;     932 		 return(4);
;     933 	   }                                                                                 //返回值4，表明从机没有对Data信号作应答
;     934 
;     935 		TWCR=0x80|0x04|0x10;                                                  //对TWINT写1清除，使能TWI，发出Stop信号
;     936 		return(0);
;     937  }
;     938 
;     939  */
;     940 /**********************************************************************
;     941 ***********************************************************************
;     942 从IIC地址为Slave的从机的Address地址读取一字节数据Data，返回值为读取的数据
;     943 //以下为主机接收模式
;     944 /*在主机接收模式，主机可以从从机接收数据，为进入主机模式，必须发送START信号。
;     945 紧接着的地址包格式决定进入MT或MR模式。 如果发送 SLA+W 进入MT模式；如果发送SLA+R则进入MR模式。
;     946 
;     947 ************************************************************************
;     948 ***********************************************************************/
;     949 uchar TWI_Receive(uchar Slave,uchar Address)
;     950 {//发送Start
_TWI_Receive:
;     951 
;     952 	uchar Temp;
;     953 	TWCR=0x80|0x20|0x04;                                                         //或:TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);对TWINT写1清除；使能TWI；发出Start信号
	ST   -Y,R17
;	Slave -> Y+2
;	Address -> Y+1
;	Temp -> R17
	LDI  R30,LOW(164)
	OUT  0x36,R30
;     954 	while((TWCR&0x80)!=0x80) ;													 //等待TWINT置位，表明Start信号已发出
_0xBE:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xBE
;     955 	if((TWSR&0xf8)!=0x08) 														 //检测状态寄存器,{0x08表示:主机的TWSR状态码(在预分频位为"0"情况下)}
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x8)
	BREQ _0xC1
;     956 	return(1); 																	 //返回值1，表明从机没有对Start信号作应答
	LDI  R30,LOW(1)
	LDD  R17,Y+0
	RJMP _0xDD
;     957 //发送Slave＋W
;     958 	TWDR=Slave; 																//更新地址寄存器
_0xC1:
	LDD  R30,Y+2
	OUT  0x3,R30
;     959 	TWCR=0x80|0x04;																 //对TWINT写1清除，使能TWI；发出从机地址信息
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     960 	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Slave信号已发出
_0xC2:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xC2
;     961 	if((TWSR&0xf8)!=0x18) 														//检测状态寄存器,(0x18表示:SLA+W从机已发送,接收到ACK)
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x18)
	BREQ _0xC5
;     962 	return(2); 																	//返回值2，表明从机没有对Slave信号作应答
	LDI  R30,LOW(2)
	LDD  R17,Y+0
	RJMP _0xDD
;     963 //发送Address
;     964 	TWDR=Address;
_0xC5:
	LDD  R30,Y+1
	OUT  0x3,R30
;     965 	TWCR=0x80|0x04; 															//对TWINT写1清除，使能TWI；发出寄存器地址信息
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     966 	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Address信号已发出
_0xC6:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xC6
;     967 	if((TWSR&0xf8)!=0x28) 														//检测状态寄存器,(0x28表示:主控机地址已发送,接收到ACK)
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x28)
	BREQ _0xC9
;     968 	return(3); 																	//返回值3，表明从机没有对Address信号作应答
	LDI  R30,LOW(3)
	LDD  R17,Y+0
	RJMP _0xDD
;     969 
;     970 //上面三段程序定义是主机方式后,以下就是接收信号的定义
;     971 
;     972 //发送Start,重新启动TWI
;     973 	TWCR=0x80|0x20|0x04; 														//对TWINT写1清除；使能TWI；发出Start信号
_0xC9:
	LDI  R30,LOW(164)
	OUT  0x36,R30
;     974 	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Start信号已发出
_0xCA:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xCA
;     975 	if((TWSR&0xf8)!=0x10)
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x10)
	BREQ _0xCD
;     976 	return(4);																			/*检测状态寄存器,在Repeat Start (状态0x10) 后，两线接口可以再次访问
;     977 																				相同的从机，或不发送STOP信号来访问新的从机。REPEATED START使得主机
;     978 																				可以在不丢失总线控制的条件下在从机、主机发送器及主机接收器模式间进行切换。*/
	LDI  R30,LOW(4)
	LDD  R17,Y+0
	RJMP _0xDD
;     979 																				//返回值4，表明从机没有对Repeat Start信号作应答
;     980 //发送Slave＋R
;     981 	TWDR=Slave+1; 																//更新地址寄存器
_0xCD:
	LDD  R30,Y+2
	SUBI R30,-LOW(1)
	OUT  0x3,R30
;     982 	TWCR=0x80|0x04; 															//对TWINT写1清除，使能TWI；发出从机地址信息
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     983 	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明Slave信号已发出
_0xCE:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xCE
;     984 	if((TWSR&0xf8)!=0x40) 														//检测状态寄存器,(0x40表示:SLA+R(MR_SLA_ACK)已发送,接收到ACK)
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x40)
	BREQ _0xD1
;     985 	return(5); 																	//返回值5，表明从机没有对Slave+R信号作应答
	LDI  R30,LOW(5)
	LDD  R17,Y+0
	RJMP _0xDD
;     986 
;     987 	TWCR=0x80|0x04;																 //对TWINT写1清除，使能TWI；接受数据
_0xD1:
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     988 	while((TWCR&0x80)!=0x80) ; 													//等待TWINT置位，表明数据已经接收接受
_0xD2:
	IN   R30,0x36
	ANDI R30,LOW(0x80)
	CPI  R30,LOW(0x80)
	BRNE _0xD2
;     989 	if((TWSR&0xf8)!=0x50) ; 													//检测状态寄存器,(0x50表示:接收到数据,ACK已返回)
	IN   R30,0x1
	ANDI R30,LOW(0xF8)
	CPI  R30,LOW(0x50)
	BREQ _0xD5
;     990 // return(6); 																		//返回值3，表明从机没有对Address信号作应答
;     991 
;     992 	Temp=TWDR; //读接收数据
_0xD5:
	IN   R17,3
;     993 
;     994 	TWCR=0x80|0x04; 															//发出NACK信号
	LDI  R30,LOW(132)
	OUT  0x36,R30
;     995 
;     996 	TWCR=0x80|0x04|0x10; 														//发出Stop信号,或:TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	LDI  R30,LOW(148)
	OUT  0x36,R30
;     997 	delay_ms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x4
;     998 	TWCR=0x80;																	 //清除TWINT和禁止TWI(不加上这句程序只能读一次)
	LDI  R30,LOW(128)
	OUT  0x36,R30
;     999 	return(Temp);
	MOV  R30,R17
	LDD  R17,Y+0
_0xDD:
	ADIW R28,3
	RET
;    1000 
;    1001 }
;    1002 
;    1003 #include  "config.h"
;    1004 	#ifndef __SLEEP_DEFINED__
	#ifndef __SLEEP_DEFINED__
;    1005 	#define __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
;    1006 	.EQU __se_bit=0x40
	.EQU __se_bit=0x40
;    1007 	.EQU __sm_mask=0xB0
	.EQU __sm_mask=0xB0
;    1008 	.EQU __sm_powerdown=0x20
	.EQU __sm_powerdown=0x20
;    1009 	.EQU __sm_powersave=0x30
	.EQU __sm_powersave=0x30
;    1010 	.EQU __sm_standby=0xA0
	.EQU __sm_standby=0xA0
;    1011 	.EQU __sm_ext_standby=0xB0
	.EQU __sm_ext_standby=0xB0
;    1012 	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_adc_noise_red=0x10
;    1013 	.SET power_ctrl_reg=mcucr
	.SET power_ctrl_reg=mcucr
;    1014 	#endif
	#endif
;    1015 
;    1016 /*****************************************************
;    1017 IO初始化
;    1018 *****************************************************/
;    1019 void Init_IO ( void )
;    1020 {
_Init_IO:
;    1021 
;    1022 // PA接液晶用，状态输入，开启上拉，用7个口,剩PA0 接确认键。
;    1023 
;    1024 PORTA=0xFE;
	LDI  R30,LOW(254)
	OUT  0x1B,R30
;    1025 DDRA=0xFE;
	OUT  0x1A,R30
;    1026 
;    1027 PORTB=0x00;
	LDI  R30,LOW(0)
	OUT  0x18,R30
;    1028 DDRB=0x00;
	OUT  0x17,R30
;    1029 
;    1030 /******************************************************************************************************
;    1031  PC0(SCL)  PC1(SDA) PC7( )
;    1032 ******************************************************************************************************/
;    1033 PORTC=0xF7;
	LDI  R30,LOW(247)
	OUT  0x15,R30
;    1034 DDRC=0x07;
	LDI  R30,LOW(7)
	OUT  0x14,R30
;    1035 /*******************************************************************************************************
;    1036 PD0 (B电机正反转)   PD1(A电机正反转 )      PD2(传感器D)  PD3(传感器C)
;    1037 PD4(CPB)       PD5(CPA)     PD6(传感器B)       PD7(传感器A)
;    1038 ********************************************************************************************************/
;    1039 //PORTD=0XCC;
;    1040  PORTD=0X00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
;    1041 DDRD=0X33;
	LDI  R30,LOW(51)
	OUT  0x11,R30
;    1042 
;    1043 }
	RET
;    1044 /*******************************************************************
;    1045 T1初始化，输出两路脉冲。未开中断，CTC模式
;    1046 OCR1A=0x00FF;OCR1B=0x00FF;
;    1047 *******************************************************************/
;    1048 void  Init_T1(  void )
;    1049 {
_Init_T1:
;    1050  TCCR1A=0x00;
	LDI  R30,LOW(0)
	OUT  0x2F,R30
;    1051  TCCR1B=0x0A;
	LDI  R30,LOW(10)
	OUT  0x2E,R30
;    1052 
;    1053  OCR1AH=0x01;
	CALL SUBOPT_0x11
;    1054  OCR1AL=0xFF;
;    1055 
;    1056  OCR1BH=0x01;
;    1057  OCR1BL=0xFF;
;    1058 }
	RET
;    1059 /*********************************************************************
;    1060 T0初始化
;    1061 *********************************************************************/
;    1062 void Init_T0( void )
;    1063 {
_Init_T0:
;    1064   TCCR0  = 0x00;//停止定时器
	LDI  R30,LOW(0)
	OUT  0x33,R30
;    1065   TCNT0  = 0x00;//初始值
	OUT  0x32,R30
;    1066   OCR0   = 0x79;//匹配值
	LDI  R30,LOW(121)
	OUT  0x3C,R30
;    1067   TIMSK |= 0x02;//中断允许
	IN   R30,0x39
	ORI  R30,2
	OUT  0x39,R30
;    1068   TCCR0  = 0x0D;//启动定时器
	LDI  R30,LOW(13)
	OUT  0x33,R30
;    1069 }
	RET
;    1070 /***********************************************************************
;    1071 T2初始化
;    1072 ***********************************************************************/
;    1073  void Init_T2( void )
;    1074  {
_Init_T2:
;    1075    TCCR2  = 0x00;//停止定时器
	LDI  R30,LOW(0)
	OUT  0x25,R30
;    1076    ASSR   = 0x00;//异步时钟模式
	OUT  0x22,R30
;    1077    TCNT2  = 0x00;//初始值
	OUT  0x24,R30
;    1078    OCR2   = 0x3E;//匹配值
	LDI  R30,LOW(62)
	OUT  0x23,R30
;    1079    TIMSK |= 0x80;//中断允许
	IN   R30,0x39
	ORI  R30,0x80
	OUT  0x39,R30
;    1080    TCCR2  = 0x00;//启动定时器
	LDI  R30,LOW(0)
	OUT  0x25,R30
;    1081  }
	RET
;    1082 
;    1083 void Init_device( void )
;    1084 {
_Init_device:
;    1085 
;    1086 MCUCR=0x00;
	LDI  R30,LOW(0)
	OUT  0x35,R30
;    1087 MCUCSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x34,R30
;    1088 
;    1089 // Timer(s)/Counter(s) Interrupt(s) initialization
;    1090 
;    1091 // Analog Comparator initialization
;    1092 // Analog Comparator: Off
;    1093 // Analog Comparator Input Capture by Timer/Counter 1: Off
;    1094 ACSR=0x80;
	OUT  0x8,R30
;    1095 SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x30,R30
;    1096 
;    1097 }
	RET
;    1098 
;    1099 void Init_TWI(void)
;    1100  {
_Init_TWI:
;    1101 	TWCR= 0x00;                                   //禁止TWI
	LDI  R30,LOW(0)
	OUT  0x36,R30
;    1102 	TWBR= 0x15;                                      //波特率15
	LDI  R30,LOW(21)
	OUT  0x0,R30
;    1103                                                //当电压为5V时, 上拉电阻为10K时, SCL和SDA的一个时钟周期为10us; 上拉电阻为1K时, SCL和SDA的一个时钟周期为2.5us;
;    1104                                                //TWBR值需要仔细调节以配合ZLG7290的IIC频率要求
;    1105 	TWSR= 0x01;                                    //预分频系数1,或(0<<TWPS1)|(0<<TWSP0);
	LDI  R30,LOW(1)
	OUT  0x1,R30
;    1106 	                                                                   //IIC从机地址SLAVE=0xa0
;    1107 	TWCR= 0x04;                                    //使能TWI，禁止中断,即:TWCR =(1<<TWEN),
	LDI  R30,LOW(4)
	OUT  0x36,R30
;    1108                                                //禁止中断即TWINT被清零，TWI立即开始工作,因此，在
;    1109                                                //清零TWINT之前一定要首先完成对地址寄存器TWAR，
;    1110                                                //状态寄存器TWSR，以及数据寄存器TWDR的访问。
;    1111  }
	RET
;    1112 /*
;    1113 	LCD1602液晶屏四线驱动模块
;    1114 	1、可直接嵌入到项目中使用
;    1115 	2、晶振频率：1M
;    1116 	3、如晶振提高低层驱动延时要作相应修改
;    1117 	4、程序在原来基础上只使用了PB口的高四位，即PB4—PB7，PB0—PB3可以做它用
;    1118 
;    1119 	SunnyTrip
;    1120 	www.iccavr.com
;    1121 */
;    1122 
;    1123 #include "config.h"
;    1124 	#ifndef __SLEEP_DEFINED__
	#ifndef __SLEEP_DEFINED__
;    1125 	#define __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
;    1126 	.EQU __se_bit=0x40
	.EQU __se_bit=0x40
;    1127 	.EQU __sm_mask=0xB0
	.EQU __sm_mask=0xB0
;    1128 	.EQU __sm_powerdown=0x20
	.EQU __sm_powerdown=0x20
;    1129 	.EQU __sm_powersave=0x30
	.EQU __sm_powersave=0x30
;    1130 	.EQU __sm_standby=0xA0
	.EQU __sm_standby=0xA0
;    1131 	.EQU __sm_ext_standby=0xB0
	.EQU __sm_ext_standby=0xB0
;    1132 	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_adc_noise_red=0x10
;    1133 	.SET power_ctrl_reg=mcucr
	.SET power_ctrl_reg=mcucr
;    1134 	#endif
	#endif
;    1135 
;    1136 #define RS_CLR	PORTA.1=0
;    1137 #define RS_SET	PORTA.1=1
;    1138 
;    1139 #define RW_CLR	PORTA.2=0
;    1140 #define RW_SET	PORTA.2=1
;    1141 
;    1142 #define EN_CLR	PORTA.3=0
;    1143 #define EN_SET	PORTA.3=1
;    1144 
;    1145 
;    1146 void LCDsetxy(unsigned char x , unsigned char y)
;    1147  {
_LCDsetxy:
;    1148   	 if (y == 0)
;	x -> Y+1
;	y -> Y+0
	LD   R30,Y
	CPI  R30,0
	BRNE _0xD6
;    1149  	 	{
;    1150     	 LCD_write_com(0x80 + x);
	LDD  R30,Y+1
	SUBI R30,-LOW(128)
	RJMP _0xE5
;    1151     	}
;    1152     else
_0xD6:
;    1153 		{
;    1154     	 LCD_write_com(0xC0 + x);
	LDD  R30,Y+1
	SUBI R30,-LOW(192)
_0xE5:
	ST   -Y,R30
	RCALL _LCD_write_com
;    1155     	}
;    1156 
;    1157  }
	ADIW R28,2
	RET
;    1158 
;    1159 /*显示屏命令写入函数*/
;    1160 void LCD_write_com(unsigned char com) {
_LCD_write_com:
;    1161 
;    1162 	RS_CLR;
;	com -> Y+0
	CBI  0x1B,1
;    1163 	RW_CLR;
	CALL SUBOPT_0x15
;    1164 	EN_SET;
;    1165 	PORTA&=0x0f;
;    1166 	PORTA|= com&0xf0;
;    1167 	com = com<<4;
;    1168 	EN_CLR;
;    1169 	delay_us( 10);
;    1170 	EN_SET;
;    1171 	PORTA&=0x0f;
;    1172 	PORTA|= com&0xf0;
;    1173 	EN_CLR;
;    1174 	delay_us(10);
	__DELAY_USB 3
;    1175 }
	RJMP _0xDC
;    1176 
;    1177 /*显示屏命令写入函数*/
;    1178 void LCD_write_data(unsigned char data) {
_LCD_write_data:
;    1179 
;    1180 	RS_SET;
;	data -> Y+0
	SBI  0x1B,1
;    1181 	RW_CLR;
	CALL SUBOPT_0x15
;    1182 	EN_SET;
;    1183 	PORTA&=0x0f;
;    1184 	PORTA|= data&0xf0;
;    1185 	data = data<<4;
;    1186 	EN_CLR;
;    1187 	delay_us( 10);
;    1188 	EN_SET;
;    1189 	PORTA&=0x0f;
;    1190 	PORTA|= data&0xf0;
;    1191 	EN_CLR;
;    1192 }
_0xDC:
	ADIW R28,1
	RET
;    1193 
;    1194 /*显示屏清空显示*/
;    1195 void LCD_clear(void) {
_LCD_clear:
;    1196 
;    1197 	LCD_write_com(0x01);
	LDI  R30,LOW(1)
	CALL SUBOPT_0x16
;    1198 	delay_ms( 15);
;    1199 }
	RET
;    1200 
;    1201 /*显示屏字符串写入函数*/
;    1202 void LCD_write_str(unsigned char x,unsigned char y,unsigned char flash *s) {
_LCD_write_str:
;    1203 
;    1204     uchar i=0;
;    1205     LCDsetxy( x ,  y);
	ST   -Y,R17
;	x -> Y+4
;	y -> Y+3
;	*s -> Y+1
;	i -> R17
	LDI  R17,0
	LDD  R30,Y+4
	ST   -Y,R30
	LDD  R30,Y+4
	ST   -Y,R30
	CALL _LCDsetxy
;    1206     while (*s) {
_0xD8:
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	LPM  R30,Z
	CPI  R30,0
	BREQ _0xDA
;    1207     	LCD_write_data(*s);
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	LPM  R30,Z
	ST   -Y,R30
	CALL _LCD_write_data
;    1208     	i++;
	SUBI R17,-1
;    1209     	s ++;
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	ADIW R30,1
	STD  Y+1,R30
	STD  Y+1+1,R31
;    1210     	}
	RJMP _0xD8
_0xDA:
;    1211 }
	LDD  R17,Y+0
	ADIW R28,5
	RET
;    1212 
;    1213 /*显示屏单字符写入函数*/
;    1214 void LCD_write_char(unsigned char x,unsigned char y,unsigned char data) {
_LCD_write_char:
;    1215 
;    1216     LCDsetxy( x ,  y);
;	x -> Y+2
;	y -> Y+1
;	data -> Y+0
	LDD  R30,Y+2
	ST   -Y,R30
	LDD  R30,Y+2
	ST   -Y,R30
	CALL _LCDsetxy
;    1217 
;    1218     LCD_write_data( data);
	LD   R30,Y
	ST   -Y,R30
	CALL _LCD_write_data
;    1219 }
	ADIW R28,3
	RET
;    1220 /*void CursorScroll(void)
;    1221 {
;    1222 
;    1223     LCD_write_com(0x10);	//光标左移
;    1224 
;    1225 }
;    1226 void ScreenScroll(void)
;    1227 {
;    1228 
;    1229 	LCD_write_com(0x18);		//画面左滚
;    1230 	delay_ms(255);
;    1231 
;    1232 } */
;    1233 
;    1234 /*显示屏初始化函数*/
;    1235 void LCD_init(void)
;    1236  {
_LCD_init:
;    1237 
;    1238 							/*I/O口方向设置*/
;    1239 	LCD_write_com(0x28);				/*显示模式设置*/
	LDI  R30,LOW(40)
	CALL SUBOPT_0x16
;    1240 	delay_ms( 15);
;    1241 	LCD_write_com(0x28);
	CALL SUBOPT_0x17
;    1242 	delay_ms( 5);
;    1243 	LCD_write_com(0x28);
	CALL SUBOPT_0x17
;    1244 	delay_ms( 5);
;    1245 	LCD_write_com(0x28);
	LDI  R30,LOW(40)
	ST   -Y,R30
	CALL _LCD_write_com
;    1246 
;    1247 	LCD_write_com(0x08);				/*显示关闭*/
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _LCD_write_com
;    1248 	LCD_write_com(0x01);				/*显示清屏*/
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _LCD_write_com
;    1249 	LCD_write_com(0x06);				/*显示光标移动设置*/
	LDI  R30,LOW(6)
	ST   -Y,R30
	CALL _LCD_write_com
;    1250 	delay_ms( 5);
	LDI  R30,LOW(5)
	LDI  R31,HIGH(5)
	CALL SUBOPT_0x4
;    1251 	LCD_write_com(0x0c);				/*显示开及光标设置*/
	LDI  R30,LOW(12)
	ST   -Y,R30
	CALL _LCD_write_com
;    1252 }
	RET
;    1253 #include  "config.h"
;    1254 	#ifndef __SLEEP_DEFINED__
	#ifndef __SLEEP_DEFINED__
;    1255 	#define __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
;    1256 	.EQU __se_bit=0x40
	.EQU __se_bit=0x40
;    1257 	.EQU __sm_mask=0xB0
	.EQU __sm_mask=0xB0
;    1258 	.EQU __sm_powerdown=0x20
	.EQU __sm_powerdown=0x20
;    1259 	.EQU __sm_powersave=0x30
	.EQU __sm_powersave=0x30
;    1260 	.EQU __sm_standby=0xA0
	.EQU __sm_standby=0xA0
;    1261 	.EQU __sm_ext_standby=0xB0
	.EQU __sm_ext_standby=0xB0
;    1262 	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_adc_noise_red=0x10
;    1263 	.SET power_ctrl_reg=mcucr
	.SET power_ctrl_reg=mcucr
;    1264 	#endif
	#endif
;    1265 
;    1266 #define    sec    0X02
;    1267 #define   miu    0X03
;    1268 #define   com   0X00
;    1269 
;    1270  /* uchar Read_PCF8563T(uchar Address)
;    1271  {
;    1272    uchar temp;
;    1273    temp=TWI_Receive(0xA2, Address);
;    1274    return temp;
;    1275  }  */
;    1276 
;    1277   void Write_PCF8563T(uchar Address , uchar date)
;    1278  {
_Write_PCF8563T:
;    1279 
;    1280   TWI_Send(0xA2, Address, date);
;	Address -> Y+1
;	date -> Y+0
	LDI  R30,LOW(162)
	ST   -Y,R30
	LDD  R30,Y+2
	ST   -Y,R30
	LDD  R30,Y+2
	ST   -Y,R30
	CALL _TWI_Send
;    1281 
;    1282  }
	ADIW R28,2
	RET
;    1283 void start_PCF( void )
;    1284 {
_start_PCF:
;    1285  Write_PCF8563T( com , 0x00 );
	RCALL SUBOPT_0xA
	RCALL SUBOPT_0x18
;    1286  delay_ms(2);
;    1287  Write_PCF8563T( sec , 0x00 );
	LDI  R30,LOW(2)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	RCALL SUBOPT_0x18
;    1288  delay_ms(2);
;    1289  Write_PCF8563T( miu , 0x00 );
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	RCALL _Write_PCF8563T
;    1290 }
	RET
;    1291 
;    1292 uchar Read_sec( void )
;    1293 {
_Read_sec:
;    1294    uchar temp;
;    1295    temp=TWI_Receive(0xA2, sec);
	ST   -Y,R17
;	temp -> R17
	LDI  R30,LOW(162)
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	CALL _TWI_Receive
	MOV  R17,R30
;    1296    return temp;
	MOV  R30,R17
	RJMP _0xDB
;    1297 }
;    1298 uchar Read_miu( void )
;    1299 {
_Read_miu:
;    1300    uchar temp;
;    1301    temp=TWI_Receive(0xA2, miu);
	ST   -Y,R17
;	temp -> R17
	LDI  R30,LOW(162)
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	CALL _TWI_Receive
	MOV  R17,R30
;    1302    return temp;
	MOV  R30,R17
_0xDB:
	LD   R17,Y+
	RET
;    1303 }


;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x0:
	RCALL _Read_sec
	STS  _Timer,R30
	__GETB1MN _Timer,2
	LDS  R26,_Timer
	CP   R30,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:31 WORDS
SUBOPT_0x1:
	LDS  R30,_BalanceTime
	SUBI R30,-LOW(1)
	STS  _BalanceTime,R30
	SUBI R30,LOW(1)
	LDI  R30,LOW(13)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	LDS  R26,_BalanceTime
	LDI  R30,LOW(10)
	CALL __MODB21U
	SUBI R30,-LOW(48)
	ST   -Y,R30
	CALL _LCD_write_char
	LDI  R30,LOW(12)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	LDS  R26,_BalanceTime
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	ST   -Y,R30
	CALL _LCD_write_char
	LDS  R30,_Timer
	__PUTB1MN _Timer,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x2:
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	LDS  R30,_write
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3:
	SUBI R30,-LOW(48)
	ST   -Y,R30
	JMP  _LCD_write_char

;OPTIMIZER ADDED SUBROUTINE, CALLED 24 TIMES, CODE SIZE REDUCTION:43 WORDS
SUBOPT_0x4:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x5:
	IN   R30,0x2F
	ORI  R30,0x40
	OUT  0x2F,R30
	IN   R30,0x2F
	ORI  R30,0x10
	OUT  0x2F,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x6:
	IN   R30,0x2F
	ANDI R30,0xBF
	OUT  0x2F,R30
	IN   R30,0x2F
	ANDI R30,0xEF
	OUT  0x2F,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x7:
	LDI  R19,LOW(0)
	LDI  R30,LOW(2)
	ST   -Y,R30
	LDI  R30,LOW(255)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x8:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(700)
	LDI  R31,HIGH(700)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x9:
	LDI  R30,LOW(900)
	LDI  R31,HIGH(900)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xA:
	LDI  R30,LOW(0)
	ST   -Y,R30
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 18 TIMES, CODE SIZE REDUCTION:31 WORDS
SUBOPT_0xB:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _LCD_write_str

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0xC:
	LDI  R30,LOW(0)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD:
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	RJMP SUBOPT_0x4

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0xE:
	LDI  R31,0
	LSL  R30
	ROL  R31
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETW1P
	RJMP SUBOPT_0xB

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF:
	CALL _beep
	RJMP _start_PCF

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x10:
	LDI  R30,LOW(2)
	OUT  0x2B,R30
	LDI  R30,LOW(255)
	OUT  0x2A,R30
	LDI  R30,LOW(2)
	OUT  0x29,R30
	LDI  R30,LOW(255)
	OUT  0x28,R30
	RJMP SUBOPT_0x5

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x11:
	LDI  R30,LOW(1)
	OUT  0x2B,R30
	LDI  R30,LOW(255)
	OUT  0x2A,R30
	LDI  R30,LOW(1)
	OUT  0x29,R30
	LDI  R30,LOW(255)
	OUT  0x28,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x12:
	CALL _Findbenlen
	CBI  0x15,2
	LDS  R30,_flage
	SUBI R30,-LOW(128)
	STS  _flage,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x13:
	LDI  R30,LOW(6)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	__POINTW1FN _0,150
	RJMP SUBOPT_0xB

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x14:
	CBI  0x12,0
	CBI  0x12,1
	LDI  R30,LOW(10)
	OUT  0x25,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x15:
	CBI  0x1B,2
	SBI  0x1B,3
	IN   R30,0x1B
	ANDI R30,LOW(0xF)
	OUT  0x1B,R30
	IN   R30,0x1B
	MOV  R26,R30
	LD   R30,Y
	ANDI R30,LOW(0xF0)
	OR   R30,R26
	OUT  0x1B,R30
	LD   R30,Y
	SWAP R30
	ANDI R30,0xF0
	ST   Y,R30
	CBI  0x1B,3
	__DELAY_USB 3
	SBI  0x1B,3
	IN   R30,0x1B
	ANDI R30,LOW(0xF)
	OUT  0x1B,R30
	IN   R30,0x1B
	MOV  R26,R30
	LD   R30,Y
	ANDI R30,LOW(0xF0)
	OR   R30,R26
	OUT  0x1B,R30
	CBI  0x1B,3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x16:
	ST   -Y,R30
	CALL _LCD_write_com
	LDI  R30,LOW(15)
	LDI  R31,HIGH(15)
	RJMP SUBOPT_0x4

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x17:
	LDI  R30,LOW(40)
	ST   -Y,R30
	CALL _LCD_write_com
	LDI  R30,LOW(5)
	LDI  R31,HIGH(5)
	RJMP SUBOPT_0x4

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x18:
	RCALL _Write_PCF8563T
	LDI  R30,LOW(2)
	LDI  R31,HIGH(2)
	RJMP SUBOPT_0x4

_delay_ms:
	ld   r30,y+
	ld   r31,y+
	adiw r30,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA
	wdr
	sbiw r30,1
	brne __delay_ms0
__delay_ms1:
	ret

__DIVB21U:
	CLR  R0
	LDI  R25,8
__DIVB21U1:
	LSL  R26
	ROL  R0
	SUB  R0,R30
	BRCC __DIVB21U2
	ADD  R0,R30
	RJMP __DIVB21U3
__DIVB21U2:
	SBR  R26,1
__DIVB21U3:
	DEC  R25
	BRNE __DIVB21U1
	MOV  R30,R26
	MOV  R26,R0
	RET

__MODB21U:
	RCALL __DIVB21U
	MOV  R30,R26
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

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
