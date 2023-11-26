.DEVICE		ATmega32		; ��������� ".DEVICE - ����������� ���������� ��� �������� ������� ���"					
							; ��������� ".INCLUDE" - ����������� ������������ ������
.INCLUDE	"m32def.inc"		; ��������� ������������ ���� � ������� �������� ������ � ������������� ����� ��������� (����: PORTA, DDRA, TIMSK � �.�.)											
							; ��������� ".DEF" - ���������� �������������� ����� ��������		
.DEF		temp1 = r16			; ���������� �������������� ����� �������� r16 
.DEF		temp2 = r17			; ���������� �������������� ����� �������� r17
.DEF		temp3 = r18			; ���������� �������������� ����� �������� r18
.DEF		temp4 = r19			; ���������� �������������� ����� �������� r18
.DEF		null = r25			; ���������� �������������� ����� �������� r25	*��� �������: � AtMega32 �������� ������ ���������� - r16-r25								
							; ��������� ".DSEG" - ������ �������� ������
.DSEG							; ��������� ".BYTE" - ����������� ����� � ��� (�������� ������: $0060 - $085F)
.ORG		$0060				; ��������� .ORG ������������� ������� ��������� ������ �������� ��������, ������� ��������� ��� ��������
Second:		.BYTE 1				; �������������� 1 ���� ��� Second �� ������	$0060
.ORG		$0061
Minute:		.BYTE 1				; �������������� 1 ���� ��� Minute �� ������	$0061
.ORG		$0062
Hour:		.BYTE 1				; �������������� 1 ���� ��� Hour �� ������		$0062

.CSEG						; ��������� ".CSEG" - ������ ����������� �������� (����)
.ORG		$000				; ��������� ".ORG" - ��������� �������� ��� ���������. // ����� ������� ���������� Reset (������ ����� - 1, ����� - $000, �������� - RESET, ������������� ��� - RESET)
	JMP		RESET				; (����������� JMP(RJMP - ��� ������) ������ ������� ����� ��������� �� ������ $000 
.ORG		$008				; ����� ������� ���������� TIM2_COMP - (������ ����� - 5, ����� - $008, �������� - TIMER2 COMP ������������� ��� - TIM2_COMP)
	JMP		TIM2_COMP			; (����������� JMP(RJMP - ��� ������) ������ ������� ����� ��������� �� ������ $008
.ORG		$00E				; ����� ������� ���������� TIM1_COMPA - (������ ����� - 8, ����� - $00E, �������� - TIMER1 COMPA, ������������� ��� - TIM1_COMPA)
	JMP		TIM1_COMPA			; (����������� JMP(RJMP - ��� ������) ������ ������� ����� ��������� �� ������ $014
.ORG		INT_VECTORS_SIZE    ; ����� ������� ����������

RESET:						; ����� RESET // ������ ���� ������� RESET
	; ����
	LDI		temp1,	Low(RAMEND)	; ������������� ����� - �����������!!!
	OUT		SPL,	temp1		; ������������� �����..
	LDI		temp1,	High(RAMEND); ������������� �����..
	OUT		SPH,	temp1		; ������������� �����

	; ����� I/O
	LDI		temp1,	0b00111111	; ������ ����� � ������� temp1 (r16)	*(1 - �����, 0 - ����)
	OUT		DDRC,	temp1		; �������� �������� temp1 � ������� DDRC
	LDI		temp1,	0b00001000	
	OUT		DDRB,	temp1

	; ���������� ���������� ��� �������� 2 � 1 �� ����������
	LDI		temp1,	(1 << OCIE2)|(0 << TOIE2)|(0 << TICIE1)|(1 << OCIE1A)|(0 << OCIE1B)|(0 << TOIE1)|(0 << OCIE0)|(0 << TOIE0) ; OCIE2 OCIE1A - 11 - ���������� ���������
	OUT		TIMSK,	temp1
	SEI							; ���������� ����������

	; ������ 2 - ������������ ���������
	LDI		temp1,	119			; ����� �� �������� ��������� ����������
	OUT		OCR2,	temp1
	LDI		temp1,	(0 << FOC2)|(1 << WGM21)|(0 <<  WGM20)|(0 << COM21)|(0 << COM20)|(1 << CS22)|(1 << CS21)|(0 << CS20) ; COM - 00, ���� ���������; WGM - 10 - CTC; CS - 110 - �������� 256
	OUT		TCCR2,	temp1

	; ������ 1 - ����
	LDI		temp1,	High(14398)	; ������ �������� ����� 7199 || 14398
	LDI		temp2,	Low(14398)	; ������ �������� �����
	OUT		OCR1AH,	temp1		; OCR1AH[15:8]	- ������ ���� ��������� ��� ��� - ������������, ������� H, ����� L. ������ �������� ���������� CTC ������������ �� ������������� �������
	OUT		OCR1AL,	temp2		; OCR1AL[7:0]
	LDI		temp1,	(0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(0 << FOC1A)|(0 << FOC1B)|(0 << WGM11)|(0 << WGM10) ; COM - �����, FOC - ?, WGM - ����� CTC, CS - ���������
	OUT		TCCR1A,	temp1
	LDI		temp1,	(0 <<  ICNC1)|(0 <<  ICES1)|(0 <<  WGM13)|(1 <<  WGM12)|(1 <<  CS12)|(0 <<  CS11)|(1 <<  CS10) ; IC - ��������� ���� ICP1 (PD6)
	OUT		TCCR1B,	temp1

	; ������ 0 - PWM
	LDI		temp1,	250			; | 248 -> 97.1% | 249 -> 97.5% | 250 -> 97.9% | 251 -> 98.3% | 252 -> 98.7%  | *�� ������� ���� 97,5 - 98,6%
	OUT		OCR0,	temp1		; �������� ��������� f = fcpu/(N*256) = 7372800/256 = 28800 || = 14745600/256 = 57600
	LDI		temp1,	(0 << FOC0) |(1  << WGM01)|(1 << WGM00)|(1 << COM01)|(0 << COM00)|(0 << CS02)|(0 << CS01)|(1 << CS00) ; WGM - fast PWM, COM - clear on compare, CS - ���������, FOC - ?
	OUT		TCCR0,	temp1

	LDI		null,		0		; ������������� �������� null(r25)
	STS		Second,		null	; ��������� Second
	STS		Minute,		null	; ��������� Minute
	STS		Hour,		null	; ���������	Hour
	CLR		temp1				; ������� �������� temp1
	CLR		temp2				; ������� �������� temp2
	JMP		Start				; ������� � ���������


	; ������������
Start:						; ������ ���� ���������
	//LDS		temp1, Second		; ��������� �� I/O-�������� � ������� (60 = 0b 0011 1100; 24 = 0b 0001 1000)
	//OUT		PORTC, temp1
	NOP
	NOP
	NOP
	NOP
	NOP
	JMP		Start				; ������������� ������� (������ Start)


	; ������������ ������� 2 - ������������ ���������
TIM2_COMP:
	CLI							; ��������� ����������, ����� ������ ������� ���������� �� ����������
	PUSH	temp1				; ��������� �������� �������� � ����
	PUSH	temp3				; ��������� �������� �������� � ����
	IN		temp1,	SREG		; ��������� �� ������������ �����/������ (�����, �������, ���������������� �������� � �.�.) � ������� ������ ����������
	PUSH	temp1				; ��������� �������� �������� � ����

	IN		temp3, PORTC
	INC		temp3
	OUT		PORTC, temp3

	POP		temp1				; ������� �������� �������� �� �����
	OUT		SREG,	temp1		; ����� � ������������ �����/������
	POP		temp3				; ������� �������� �������� �� �����
	POP		temp1				; ������� �������� �������� �� �����
	SEI							; ���������� ����������
	RETI


	; ������������ ������� 1 - ����
TIM1_COMPA:					; ��� ������� ���������� 
	CLI							; ��������� ����������, ����� ������ ������� ���������� �� ����������
	PUSH	temp1				; ��������� �������� �������� � ����
	IN		temp1,	SREG		; ��������� �� ������������ �����/������ (�����, �������, ���������������� �������� � �.�.) � ������� ������ ����������
	PUSH	temp1				; ��������� �������� �������� � ����
INC_Second:	
	LDS		temp1,	Second	; ��������� �� I/O-�������� � �������
	INC		temp1				; ���������
	CPI		temp1, 60			; ���� temp1 >= 60	
	BRSH	INC_Minute
	STS		Second, temp1	; ������ ���������� (� ���)
	INC_Minute_Ret:				; ������ � PORTC � ������������(��)
	POP		temp1				; ������� �������� �������� �� �����
	OUT		SREG,	temp1		; ����� � ������������ �����/������
	POP		temp1				; ������� �������� �������� �� �����
	SEI							; ���������� ����������
	RETI						; ������� �� ����������

INC_Minute:
	STS		Second, null		; ��������� Second
	LDS		temp2,	Minute		; �������� Minute
	INC		temp2				; ��������� Minute
	CPI		temp2, 60			; ��������� Minute � 60
	BRSH	INC_Hour
	STS		Minute, temp2		; �������� Minute (< 60)
	INC_Hour_Ret:
	JMP		INC_Minute_Ret

INC_Hour:
	STS		Minute, null		; ��������� Minute
	LDS		temp3,	Hour		; �������� Hour
	INC		temp3				; ��������� Hour
	CPI		temp3, 24			; ��������� Hour � 24
	BRSH	Null_Hour
	STS		Hour, temp3			; �������� Hour (< 24)
	Null_Hour_Ret:
	JMP		INC_Hour_Ret

Null_Hour:
	STS		Hour, null
	JMP		Null_Hour_Ret
