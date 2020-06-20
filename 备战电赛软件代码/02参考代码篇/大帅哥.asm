DIRECT EQU 20H.0
IN1    EQU p1.0
IN2    EQU P1.1
IN3    EQU P1.2
IN4    EQU P1.3
LED1   EQU P2.4
LED2   EQU P2.5
LED3   EQU P2.6
LED4   EQU P2.7
LED5   EQU P1.4
V1     EQU P1.5
V2     EQU P1.6
KEY    EQU P1.7
buzzer EQU P2.0


;**********************AT89S52*****************
ORG 0000H
    JB KEY,TIAO
    LJMP MAIN
TIAO:LJMP MAIN3
ORG 000BH
LJMP INTT0
;*******************�������ֳ���*******************************
MAIN:    MOV IE,#82H
         MOV TMOD,#1
         MOV TH0,#3CH
         MOV  TL0,#0B0H
         SETB TR0
         MOV 40H,#00H
         MOV 41H,#00H
         MOV 42H,#00H
         MOV 70H,#00H
          SETB DIRECT
          CLR BUZZER
          MOV R7,#0
          mov R3,#08H
          LCALL ZHAOPH
;*************ǰ��������*****************
MAIN1:    LCALL QIANJIN
          LCALL  DELAY10MS
          LCALL STOP
          LCALL DELAY30MS
          LCALL HEIDAI

          JNB DIRECT,MAIN2
HEAR1:    AJMP MAIN1
;*************����������***************
MAIN2:    LCALL HOUTUI
          LCALL  DELAY10MS
          LCALL STOP
          LCALL DELAY30MS
          LCALL HEIDAI1

HEAR2:   AJMP MAIN2

;*************�ж�С��λ�ó���******************
PANDUAN:  INC  R7
  NEXT0:  CJNE R7, #1 ,NEXT1
          LCALL  BDIAN
  NEXT1:  CJNE R7 ,#3 ,NEXT2
          LCALL  ADIAN
  NEXT2:  RET

 ;**************A�㴦�����*********************
 ADIAN:LCALL STOP
       LCALL  XIANG
       CLR TR0
       ;MOV A,40H
       ;  ADD A,#05H
       ;  MOV 40H,A
       ;  LJMP DIYI
;DIYI2:   INC 52H
 ;        MOV 51H,#00H
  ;       RET
;DIYI1:   INC 51H
 ;        MOV 50H,#00H
  ;       RET
;DIYI:    INC 50H
 ;        MOV  R5,50H
  ;       CJNE R5,#10 ,BIJIAO
   ;      LCALL DIYI1
;BIJIAO:  DJNZ 40H,DIYI
;DIYI0:   INC 51H
 ;        MOV R6,51H
  ;       CJNE R6,#10,BIJIAO1
   ;      LCALL DIYI2
;BIJIAO1: DJNZ 41H,DIYI0
;DIYI3  :INC 52H
 ;       DJNZ 42H,DIYI3
  ;
   ;      MOV 40H,50H
    ;     MOV 41H,51H
     ;    MOV 42H,52H
      ;   LCALL DISPLAY
TING:    AJMP TING
;**************B�㴦�����*********************
BDIAN:    LCALL STOP
          LCALL XIANG
          CLR TR0

          LCALL JISHI
          LCALL XIANG               ;Ҫ��ʱ5���Ӷ�ʱ
         ; LCALL HOUTUI
         ; LCALL DELAY30MS
         SETB TR0
         ;MOV A,40H
        ; ADD A,#05H
         ;ADD A,50H

        ; MOV 50H,A
        ; MOV A,41H
        ; ADDC A,51H
        ; MOV 51H,A
        ; MOV A,42H
        ; ADDC A,52H
        ; MOV 52H,A
         CLR DIRECT
         RET


;***************����������ƽ���ӳ���****************
ZHAOPH:;11 00  01

       MOV A,P1
       JB ACC.5,AHEAD
       JNB ACC.6,BACK1
       LCALL STOP
       djnz r3,jxz
       ljmp quit1
jxz:   lcall delay
       lcall delay
       lcall delay
       lcall delay
       lcall delay
       lcall delay
       MOV A,P1
       JB  ACC.5,AHEAD
       JNB ACC.6,BACK1
quit1: LCALL STOP
       LCALL XIANG
       CLR TR0
       LCALL  JISHI
       LCALL XIANG
       SETB TR0
       ;MOV 50H,40H
      ; MOV 51H,41H
       ;MOV 52H,42H
       LJMP  MAIN1
BACK1: LCALL HOUTUI
       LCALL DELAY10MS
       LCALL STOP
       LCALL DELAY30MS
       LCALL HEIDAI1F
       LJMP ZHAOPH
AHEAD:LCALL QIANJIN
       LCALL DELAY10MS
       LCALL STOP
       LCALL DELAY30MS
       LCALL HEIDAIF
       LJMP ZHAOPH

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;T0�жϷ����ӳ������ڼ�ʱ
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
INTT0:;INC 50H
      ;MOV A,50H
      ;DA A
      ;CJNE A,#99,QUIT
      ;INC 51H
      ;MOV 50H,#00H
       MOV TH0,#3CH
       MOV  TL0,#0B0H
      INC 70H
      MOV R1,70H
      CJNE R1,#14H,QUIT
      MOV 70H,#00H
      LCALL YIMIAO
      RETI
YIMIAO:INC 40H
       MOV R4,40H
       CJNE R4,#10,JI11
       LJMP JI1
JI11:  LCALL DISPLAY
       RETI
JI1:   INC 41H
       MOV 40H,#00H
       MOV R4,41H
       CJNE  R4,#10,JI22
       LJMP JI2
JI22:  LCALL DISPLAY
       RETI
JI2:   INC 42H
       MOV 41H,#00H
       LCALL DISPLAY
QUIT: RETI

;*********�ڴ����************
HEIDAI:   JB LED5, CL
          JB LED1,XIANGZUO  ;ǰ�����⵽�ڴ���ת
          JB LED2,XIANGYOU  ;ǰ�ҹ���⵽�ڴ���ת
          RET
CL:       LCALL qianjin
          lcall delay10ms
          LCALL STOP
          lcall  delay30ms
          JB LED5, CL
          LCALL PANDUAN
          RET
XIANGZUO:
          LCALL ZUOZHUAN
          LCALL DELAY10MS

          LCALL STOP
          LCALL DELAY30MS

          JB LED1,XIANGZUO
       	  RET
XIANGYOU: LCALL YOUZHUAN
          LCALL DELAY10MS

          LCALL STOP
          LCALL DELAY30MS

          JB LED2,XIANGYOU
	  RET
HEIDAI1:  JB LED5, CL1
          JB LED3,XIANGHZ  ;ǰ�����⵽�ڴ���ת
          JB LED4,XIANGHY  ;ǰ�ҹ���⵽�ڴ���ת
          RET
CL1:       LCALL HOUTUI
          lcall  delay10ms
          LCALL STOP
          lcall delay30ms
          JB LED5, CL1
          LCALL PANDUAN
          RET
XIANGHZ: LCALL HOUZUOZHUAN
          LCALL DELAY10MS
          LCALL STOP
          LCALL DELAY30MS

          JB LED3,XIANGHZ
       	  RET
XIANGHY: LCALL HOUYOUZHUAN
          LCALL DELAY10MS
          LCALL STOP
          LCALL DELAY30MS

          JB LED4,XIANGHY
          RET

;*****************�����������*******************

STOP:	     CLR IN1
     	     CLR IN2
     	     CLR IN3
             CLR IN4
             LCALL  DELAY30MS
             RET
ZUOZHUAN:    CLR  IN1
             CLR  IN2
             SETB IN3
             CLR  IN4
             LCALL  DELAY30MS
             RET
YOUZHUAN:    SETB IN1
             CLR  IN2
             CLR  IN3
             CLR  IN4
             LCALL   DELAY30MS
             RET
QIANJIN:     SETB IN1
             CLR  IN2
             SETB IN3
             CLR  IN4
             LCALL  DELAY30MS
             RET
HOUTUI:      CLR  IN1
             SETB IN2
             CLR  IN3
             SETB IN4
             LCALL  DELAY30MS
             RET
HOUYOUZHUAN:	CLR  IN1
     		CLR  IN2
     		CLR  IN3
     		SETB IN4
                LCALL  DELAY30MS
    		RET
HOUZUOZHUAN:	CLR  IN1
    		SETB IN2
     		CLR  IN3
     		CLR  IN4
                LCALL  DELAY30MS
                RET

;**********����������****************
XIANG:SETB BUZZER
      LCALL DELAY
      CLR BUZZER
      RET

;************��ʱ30MS����-*****************

DELAY30MS:
    MOV     R5,#20
DD1: MOV     R6,#50
 DD2: DJNZ    R6,$
    DJNZ    R5,DD1
    RET
;*************10MS��ʱ*****************

 DELAY10MS:
    MOV     R5,#15
DDD1: MOV     R6,#50
 DDD2: DJNZ    R6,$
    DJNZ    R5,DDD1
    RET
;***********5���ӵ���ʱ����*********************
JISHI :
       MOV  40H ,#5
       MOV  41H ,#0
       MOV  42H ,#0
       LCALL DELAY
       LCALL DISPLAY
       MOV  40H ,#4
       MOV  41H ,#00H
       MOV  42H ,#00H
       LCALL DELAY
       LCALL DISPLAY
       MOV  40H ,#3
       MOV  41H ,#00H
       MOV  42H ,#00H
       LCALL DELAY
       LCALL DISPLAY
       MOV  40H ,#2
       MOV  41H ,#00H
       MOV  42H ,#00H
       LCALL DELAY
       LCALL DISPLAY
       MOV  40H ,#1
       MOV  41H ,#00H
       MOV  42H ,#00H
       LCALL DELAY
       LCALL DISPLAY
       MOV  40H ,#0
       MOV  41H ,#00H
       MOV  42H ,#00H
       LCALL DELAY
       LCALL DISPLAY
       RET
 ;******************1s***********************

DELAY:	MOV  45H,#10       ;��ʱһ��
D2:	MOV   47H,#200
D1:	MOV  46H ,#248
 	DJNZ  46H,$
	DJNZ  47H,D1
	DJNZ  45H,D2
        RET


;;**************�������ʾ�ӳ���**********************
;��ڲ�����40H��41H  ��42H
DISPLAY:
         MOV R2, #3
      MOV R0,#40H              ;ȡ����ʾ������ַƫ����
DL0: MOV A,@R0              ;ָ�����ͱ���ַ
      MOV DPTR,#TAB
      MOVC A,@A+DPTR         ;���
      MOV SBUF,A             ;������ʾ
      JNB TI,$            ;�ȴ�һ֡������
      CLR TI                 ;���жϱ�־��׼����������
      INC R0                 ;������ʾ��Ԫ
      DJNZ R2,DL0            ;3�����δ��ʾ�꣬����
      RET
TAB:DB 0C0H,0F9H,0A4H,0B0H,99H;0,1,2,3,4,
      DB 92H,82H,0F8H,80H,90H   ;5,6,7,8,9,


;************���Ӳ���*********************************
MAIN3:
         MOV IE,#82H
         MOV TMOD,#1
         MOV TH0,#3CH
         MOV  TL0,#0B0H
         SETB TR0
         CLR  BUZZER

         LCALL QIANJIN
         LCALL DELAY10MS
         LCALL STOP
         LCALL DELAY30MS
         LCALL HEIDAIF
         MOV A,P1
         JNB ACC.5 ,HEAR
         LJMP ZHAOPHF
HEAR:    AJMP MAIN3

;***********��ƽ��**********************************
ZHAOPHF:;11��ǰ��   00������  01��ƽ��
       MOV A,P1
       JB ACC.5,AHEADF
       JNB ACC.6,BACK1F
       LCALL STOP
       LCALL DELAY
       LCALL DELAY
       LCALL DELAY
       LCALL DELAY
       LCALL DELAY
       LCALL DELAY
       MOV A,P1
       JB ACC.5,AHEADF
       JNB ACC.6,BACK1F
       LCALL XIANG
       CLR TR0
       LCALL DELAY
       LCALL JISHI
       LCALL XIANG
       SETB TR0
       LJMP ZHAOPHF

BACK1F: LCALL HOUTUI
       LCALL DELAY10MS
       LCALL STOP
       LCALL DELAY30MS
       LCALL HEIDAI1
       LJMP ZHAOPHF

AHEADF:LCALL QIANJIN
       LCALL DELAY10MS
       LCALL STOP
       LCALL DELAY30MS
       LCALL HEIDAIF
       LJMP ZHAOPHF
 ;*********�ڴ����************
HEIDAIF:  JB LED1,XIANGZUOF  ;ǰ�����⵽�ڴ���ת
          JB LED2,XIANGYOUF  ;ǰ�ҹ���⵽�ڴ���ת
          RET

XIANGZUOF:
          LCALL ZUOZHUAN
          LCALL DELAY10MS

          LCALL STOP
          LCALL DELAY30MS

          JB LED1,XIANGZUOF
       	  RET
XIANGYOUF: LCALL YOUZHUAN
          LCALL DELAY10MS

          LCALL STOP
          LCALL DELAY30MS

          JB LED2,XIANGYOUF
	  RET
HEIDAI1F: JB LED3,XIANGHZF  ;ǰ�����⵽�ڴ���ת
          JB LED4,XIANGHYF  ;ǰ�ҹ���⵽�ڴ���ת
          RET
XIANGHZF: LCALL HOUZUOZHUAN
          LCALL DELAY10MS
          LCALL STOP
          LCALL DELAY30MS

          JB LED3,XIANGHZF
       	  RET
XIANGHYF: LCALL HOUYOUZHUAN
          LCALL DELAY10MS
          LCALL STOP
          LCALL DELAY30MS

          JB LED4,XIANGHYF
          RET
