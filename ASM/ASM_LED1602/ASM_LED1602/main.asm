;*************************************************************************
;*************** �������� ��������� ��� ���������� LED1602 ***************
;*************************************************************************
;*
;*  �������:
;*	����� �������� ���������� ������ ������
;*	������������ ����� �������� ������ ������
;*	����� �������� flash-������ � ����������� ������ ������
;*	�������� ������ � ��������� ������ ����������� �������� � �������� ����� �����
;*	�������� ���������� ������� ������������� ����� ���
;*		
;*	��� ������ � ������������ ������� ������� SREG ����������� ���������� � ���� ����:
;*		 1. ��������� ����� ����������� �� ������� ����������
;*		 2. � ������������ ���������� ��������� ������� ��� ���������� �������� (������ �� ���� Z)
;*	������ ��������	SREG	- �������� in
;*	������ � �������� SREG	- �������� out
;*	
;*  ��� �������������� � ������������� ������������ ������� ��������� ���������� (cli), 
;*	� ������ ��������� ������ ������� � �������� ��������� ������� ������� � ����
;*	������������� ������������ ���������: 
;*		 1. ����� �����-������ PORTX, PIN, DDRX
;*		 2. ���������� � ������ �������������� �� �������� ���������� INTX
;*		 3. �������, �������� � ��������� ������ TIMX
;*		 4. ���������� ���������� AC
;*		 5. �������-�������� ��������������� ADC
;*		 6. ������������� ���������������� ���������������� USART
;*		 7. ���������������� ������������ ��������� SPI
;*		 8. ������������� ���������������� ��������� TWI
;*		 9. ��������� JTAG
;*		10. ���������� �������� ���������
;*		11. ������� ��������� �������
;*  ����� ������� ��������� ���������� � ������ ������� �������� �� ������� ����������
;*	��! ���������� ���������� �� ��������� ���� ���������� ������� ��� ��������� ����������
;*
;*
;*	�������
;*	� �� ����� 2 ���� ������: 
;*		1. ��� - ��� ��: RAM, SRAM, ��� ��� ����������� ������	(.dseg)
;*		2. ��� - ��� ��: ROM, flash, ��� ������ ��������		(.cseg)
;*	EEPROM �� ��������� ������� ��, ��� ��� �������� ������������ �����������
;*	���� � �������� ������ � ��� �������						(.eseg)
;*
;*	�������� ������ ���������� (�������), �������� ������������ ��������� � ����, ��� ��� ����������� � ���
;*  �� �������:
;*		�������� ������ ���������� (r0 - r31)								- $0000 - $001F
;*		�������� ������������ ��������� (TWBR, TWI, PORT, SP, SREG, � ��.)	- $0020 - $005F
;*		������� SRAM														- $0060 - $085F
;*  ���� ���������� � ����� SRAM. ���������� ����� ���� � ����� ������ �� �� ������.
;*	��� ������ � ���� ��������� ���������� �������� ���� �������� SP.
;*	� �������� SP �������� ������ � ������
;*	���� ���������� ������� ������� � ��������� (CALL, RCALL, ICALL, RET, RETI)
;*
;*************************************************************************

; ***** ������� ������������� ********************************************
.device		ATmega32
.include	"m32def.inc"
.list

; ***** ����������� ������������� ���� ��� �������� *****							
.equ		E			= 0b00000001
.equ		RW			= 0b00000010
.equ		RS			= 0b00000100
.equ		MSG_HW_SIZE	= 16

; ***** ����������� ������������� ���� ��������� CPU *****
.def		temp0		= r16						; ������������ ������������� ���� ��������� ������ �������	
.def		temp1		= r17						; ��������� temp0 - temp5 ������������� ���������� � �����
.def		temp2		= r18						; �������� insvar0 - insvar3 � ���� �� �����������, � ������������ ��� ����������� ���������� ������ �����������
.def		temp3		= r19						
.def		temp4		= r20
.def		temp5		= r21
.def		intvar0		= r22
.def		intvar1		= r23
.def		intvar2		= r24
.def		intvar3		= r25


; ***** ������� ��� (SRAM) ***********************************************
; ***** �������������� ������ � ��� *****
.dseg												; ��� ���-�� ������ �� ��������� � ��
.org		SRAM_START								; �������������� ����� � ������ ��� ��� ���������� (SRAM_START = 0x0060)
RAM_delay_loop0:		.byte 1						; ������� delay
RAM_delay_loop1:		.byte 1						; ������� delay
RAM_LED1602_ZL:			.byte 1						; ������� LCD1602_print_string
RAM_LED1602_ZH:			.byte 1						; ������� LCD1602_print_string
RAM_LED1602_byte:		.byte 1						; ������� LCD1602_send_cmd � LCD1602_send_data


; ***** ������� ���� *****************************************************
; ***** ������� �������� ���������� *****
.cseg
.org		$0000
  	jmp		RESET
.org		INT_VECTORS_SIZE						; INT_VECTORS_SIZE = 0d42 = 0x2A

; ***** ������� �������� ���������� *****
; ***** ������ ���������� RESET *****
RESET:
	cli
	ldi		temp0,	Low(RAMEND)						; ������������� ����� - �����������!!!
    out		SPL,	temp0							; SP - �������� ���� ��������
	ldi		temp0,	High(RAMEND)
	out		SPH,	temp0

	ldi		temp0,	0b11111111						; ����� I/O	; ������ ����� � ������� temp1 (r16)	*(1 - �����, 0 - ����)
	out		DDRA,	temp0							; �������� �������� temp1 � ������� DDRC
	ldi		temp0,	0b00000111	
	out		DDRB,	temp0

	sei
	jmp		main


; ***** ������� �������������� �������� �� FLASH ������ *****
msg_time_leters:
	.db		0b11100000, 0b11100000, 0b11110001, 0b11111011, 0b11110101, 0b11110001, 0b11110001, 0b11100000	 		; ����� 1 - �
	.db		0b11100000, 0b11100000, 0b11101111, 0b11110001, 0b11101111, 0b11110001, 0b11110001, 0b11100000	 		; ����� 2 - �
	.db		0b11100000, 0b11100000, 0b11101000, 0b11100000, 0b11100000, 0b11101000, 0b11100000, 0b11100000, 0, 0	; ����� 3 - :
msg_setting_leters:
	.db		0b11100000, 0b11100000, 0b11111111, 0b11100100, 0b11100100, 0b11100100, 0b11100100, 0b11100000			; ����� 1 - �
	.db		0b11100010, 0b11100100, 0b11110001, 0b11110011, 0b11110101, 0b11111001, 0b11110001, 0b11100000			; ����� 2 - �
	.db		0b11100000, 0b11100000, 0b11110001, 0b11110010, 0b11111100, 0b11110010, 0b11110001, 0b11100000			; ����� 3 - �
	.db		0b11100000, 0b11100000, 0b11110001, 0b11110001, 0b11101111, 0b11100001, 0b11100001, 0b11100000			; ����� 4 - �
	.db		0b11100000, 0b11100000, 0b11111110, 0b11110001, 0b11111110, 0b11110001, 0b11111110, 0b11100000, 0, 0	; ����� 5 - �

msg_time:
	.db		66, 112, 101, 1, 2, 3, 0, 0											; Bpe123 - �����:
msg_setting:
	.db		72, 97, 99, 1, 112, 111, 2, 3, 97, 32, 4, 97, 99, 111, 5, 0			; Hac1po23a 4aco5 - ��������� �����

; ***** ������� �������� *****
;.macro	macro_name
;	ldi		r16,	@0
;	nop
;.endmacro


; ***** ������� ��������� *****
; ***** ������ ��������� *****
main:
	; ������������� ������ 
	ldi		temp0,	0b00111000						; 0b00111000 - 8bit line, 2 row mode, 5x8 font
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd

	ldi		temp0,	0b00001100						; 0b00001100 - Turns on display and cursor
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd

	ldi		temp0,	0b00000110						; 0b00000110 - Sets mode to increment the address by one and to shift the cursor to the right at the time of write to the DD/CGRAM.
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd

	ldi		temp0,	0b00000001						; 0b00000001 - Clear screen
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd
	; ����� ������������� ������



	; ������ �������. ������� �������� � ��� ������ ������� ��� ������, ����� ����� ������������ ������

	ldi		temp0,	0b01001000						; ����������� ������ 8 � ������ CGRAM 						
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd

	ldi		ZH,		high(msg_time_leters << 1)		; ��������� ����������� ����
	sts		RAM_LED1602_ZH,	ZH
	ldi		ZL,		low(msg_time_leters << 1)
	sts		RAM_LED1602_ZL,	ZL
	call	LCD1602_print_string
	
	ldi		temp0,	0b00000001						; 0b00000001 - Clear screen
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd

	ldi		ZH,		high(msg_time << 1)				; ������������� Z-���������. �������� � ���� �������� ������ �� �������� ����� ������
	sts		RAM_LED1602_ZH,	ZH
	ldi		ZL,		low(msg_time << 1)
	sts		RAM_LED1602_ZL,	ZL
	call	LCD1602_print_string


	ldi		temp0,	100
main_loop:
	ldi		intvar1,	255	
	sts		RAM_delay_loop0,	intvar1
	ldi		intvar1,	255
	sts		RAM_delay_loop1,	intvar1
	call	delay
	dec		temp0
	cpi		temp0,	0
	brne	main_loop


	ldi		temp0,	0b01001000						; ����������� ������ 8 � ������ CGRAM 						
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd

	ldi		ZH,		high(msg_setting_leters << 1)	; ��������� ����������� ����
	sts		RAM_LED1602_ZH,	ZH
	ldi		ZL,		low(msg_setting_leters << 1)
	sts		RAM_LED1602_ZL,	ZL
	call	LCD1602_print_string
	
	ldi		temp0,	0b00000001						; 0b00000001 - Clear screen
	sts		RAM_LED1602_byte, temp0
	call	LCD1602_send_cmd

	ldi		ZH,		high(msg_setting << 1)			; ������������� Z-���������. �������� � ���� �������� ������ �� �������� ����� ������
	sts		RAM_LED1602_ZH,	ZH
	ldi		ZL,		low(msg_setting << 1)
	sts		RAM_LED1602_ZL,	ZL
	call	LCD1602_print_string



main_end:
	jmp		main_end
; ***** ����� ��������� *****


; ***** ������� ����������� *****
; ***** ����������� ������ ������ �� ����� *****
; ��������� �� ��� ����� ������� � ������� ��� �� ��� ��� ���� �� ��������� �� ����-����������
; ����� �������	����������� �� �������: RAM_LED1602_ZL � RAM_LED1602_ZH
; *��������� ���������� �������� ������� ��� ��� ���������� ��������� ������ ������������
; *�� ��������� ���������� ���������� ��� ��� ��� ������� � ����������
LCD1602_print_string:
  /*cli
	push	temp0
	push	temp1
	push	temp2
	push	temp3
	push	temp4
	push	temp5*/
	in		intvar3,	sreg
	push	intvar3
	
	lds		ZL,	RAM_LED1602_ZL						; �������� ������ �� ���
	lds		ZH,	RAM_LED1602_ZH

LCD1602_print_string_loop:
	lpm		intvar0,	Z+							; �������� ����� �� ������ Z � ������� intvar0 c ���������������
	cpi		intvar0,	'\0'						; ��������� �������� � �������� intvar0 � ����-������������
	breq	LCD1602_print_string_exit				; ���� ��� ����-���������� �� ����� �� ������������
	sts		RAM_LED1602_byte,	intvar0				; � ���� �� ����-���������� �� ��������� ����� � ��� ���� ��� ������				
	call	LCD1602_send_data						; � ������� ������������ ������ �������
	brne	LCD1602_print_string_loop				; � ����� ��������� ����

LCD1602_print_string_exit:
	pop		intvar3
	out		SREG,	intvar3
  /*pop		temp5
	pop		temp4
	pop		temp3
	pop		temp2
	pop		temp1
	pop		temp0
	sei*/
	ret


; ***** ����������� ������� ������ �� ����� *****
; ��������� �� ��� ���� ������ ��� ������ � ������� ��� �� �����
; *�� ��������� ���������� �������� ������� ��� ��� �� ���������� ��������� ������ ������������
; *��������� ���������� ���������� ��� ��� ���������� ������� � ���������� (PORTA, PORTB)
LCD1602_send_data:
	cli
	call	BF_check								; �������� ����� ���������

	lds		intvar0,	RAM_LED1602_byte			; �������� ������ �� ���
	out		PORTA,		intvar0

	ldi		intvar0,	RS							; ���� ����� 
	out		PORTB,		intvar0
	ldi		intvar0,	(RS|E)
	out		PORTB,		intvar0
	ldi		intvar0,	RS
	out		PORTB,		intvar0
	sei
	ret


; ***** ����������� ������� ������� �� ����� *****
; ��������� �� ��� ���� ������� ��� ��������� ������ � ���������� ��� � �����
; *�� ��������� ���������� �������� ������� ��� ��� �� ���������� ��������� ������ ������������
; *��������� ���������� ���������� ��� ��� ���������� ������� � ���������� (PORTA, PORTB)
LCD1602_send_cmd:
	cli
	call	BF_check								; �������� ����� ���������

	lds		intvar0,	RAM_LED1602_byte			; �������� ������ �� ���
	out		PORTA,		intvar0

	ldi		intvar0,	0							; ���� ����� 
	out		PORTB,		intvar0
	ldi		intvar0,	E
	out		PORTB,		intvar0
	ldi		intvar0,	0
	out		PORTB,		intvar0
	sei
	ret


; ***** ����������� �������� ����� ��������� ������ *****
; ������������ ��������� ��������� ���� BF � LED1602
; *�� ��������� ���������� �������� ������� ��� ��� �� ���������� ��������� ������ ������������
; *�� ��������� ���������� ���������� ��� ���, ���� � ���������� ������� � ���������� (PORTA, PORTB), ���������� ��� ������� � ������������ ������� LCD1602_send_cmd � LCD1602_send_data
BF_check:
	ldi		intvar1,	0b00000000					; ���� A �� ���� ������		
	out		DDRA,		intvar1

BF_check_loop:
	ldi		intvar1,	RW							; ���� ����� 
	out		PORTB,		intvar1
	ldi		intvar1,	(RW|E)
	out		PORTB,		intvar1
	ldi		intvar1,	RW
	out		PORTB,		intvar1

	in		intvar1,	PINA						; ������ ����� ������
	sbrc	intvar1,	7							; �������� ���� ����� ���������
	jmp		BF_check_loop							; ���� ���� ���� ��������� �� ��������� ��������

	ldi		intvar1,	0b11111111					; ���� ��� ����� ��������� �� ����� �� ������������		
	out		DDRA,		intvar1
	
	ldi		intvar1,	10							; ����� ������� �� ������������ ������� �������� �� 4 * RAM_delay_loop1 * RAM_delay_loop0 + 38 = 438 ������
	sts		RAM_delay_loop0,	intvar1
	ldi		intvar1,	10
	sts		RAM_delay_loop1,	intvar1
	call	delay
	ret


; ***** ����������� �������� *****
; �������� ������������� �� �������:  4 * RAM_delay_loop1 * RAM_delay_loop0 + 12
; *��������� ���������� �������� ������� ��� ��� ���������� ��������� ������ ������������
; *�� ��������� ���������� ���������� ��� ��� ��� ������� � ����������
delay:
	in		intvar3,	sreg
	push	intvar3
	
	lds		intvar1,	RAM_delay_loop1
delay_loop1:
	lds		intvar0,	RAM_delay_loop0
delay_loop0:
  	dec		intvar0
	cpi		intvar0,	0
	brne	delay_loop0
	dec		intvar1
	cpi		intvar1,	1
	brne	delay_loop1

	pop		intvar3
	out		SREG,	intvar3
	ret


; ***** ������� EEPROM *****************************************************
.eseg