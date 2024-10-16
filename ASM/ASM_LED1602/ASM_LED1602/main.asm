.device		ATmega32
.include	"m32def.inc"
							
.equ		E			= 0b00000001				; ���������� ������������� ���� ��������
.equ		RW			= 0b00000010				; ������������ ������������� ������ ���������� ��������� ��������
.equ		RS			= 0b00000100
.equ		msg_hw_size	= 16
.def		temp1		= r16						; ������������ ������������� ���� ��������� ������ �������	

.dseg												; ������� ������ ��� (����)
.org		$0060									; �������������� ����� � ������ ��� ��� ����������
RAM_var:	.byte 1

.cseg												; ������� ����
.org		$000									; ������� �������� ����������
  	jmp		reset

.org		$030									; ������ ���������
reset:
	ldi		r16,	Low(RAMEND)						; ������������� ����� - �����������!!!
    out		SPL,	r16
	ldi		r16,	High(RAMEND)
	out		SPH,	r16

	ldi		r16,	0b11111111						; ����� I/O	; ������ ����� � ������� temp1 (r16)	*(1 - �����, 0 - ����)
	out		DDRA,	r16								; �������� �������� temp1 � ������� DDRC
	ldi		r16,	0b00000111	
	out		DDRB,	r16

	jmp		main

main:
	ldi		r16,	0b00111000						; 0b00111000 - 8bit line, 2 row mode, 5x8 font
	call	send_cmd_r16
	ldi		r16,	0b00001100						; 0b00001100 - Turns on display and cursor
	call	send_cmd_r16
	ldi		r16,	0b00000110						; 0b00000110 - Sets mode to increment the address by one and to shift the cursor to the right at the time of write to the DD/CGRAM.
	call	send_cmd_r16
	ldi		r16,	0b00000001						; 0b00000001 - Clear screen
	call	send_cmd_r16

	clr		ZL
	clr		ZH
	ldi		r17,	0

	ldi		ZH,		high(msg_hw << 1)				; ������������� Z-���������
	ldi		ZL,		low(msg_hw << 1)
loop:
	lpm		r16,	Z+								; load lower byte of var into r4 (0x34)
	call	send_data_r16
	inc		r17
	cpi		r17,	msg_hw_size
	brne	loop

end:
	jmp		end



send_cmd_r16:
	call	BF_check
	out		PORTA,	r16
	ldi		r16,	0
	out		PORTB,	r16
	ldi		r16,	E
	out		PORTB,	r16
	ldi		r16,	0
	out		PORTB,	r16
	ret

send_data_r16:
	call	BF_check
	out		PORTA,	r16
	ldi		r16,	0
	out		PORTB,	r16
	ldi		r16,	(RS|E)
	out		PORTB,	r16
	ldi		r16,	0
	out		PORTB,	r16
	ret

BF_check:
	push	r16
	ldi		r16,	0
	out		DDRA,	r16
BF_check_loop:
	ldi		r16,	RW
	out		PORTB,	r16
	ldi		r16,	(RW|E)
	out		PORTB,	r16
	ldi		r16,	RW
	out		PORTB,	r16
	in		r16,	PINA
	sbrc	r16,	7
	jmp		BF_check_loop	
	nop
	ldi		r16,	0b11111111
	out		DDRA,	r16
	call	delay
	pop		r16
	ret


delay:
	push	r16
	push	r17
	in		r16,	sreg
	push	r16
	ldi		r17,	10								; �������� ������������� ��� - 10*10*4 + 14 = 414
delay_d2:
	ldi		r16,	10
delay_d1:
  	dec		r16
	cpi		r16,	0
	brne	delay_d1
	dec		r17
	cpi		r17,	1
	brne	delay_d2
	pop		r16
	out		SREG,	r16
	pop		r17
	pop		r16
	ret

msg_hw:
   .db		"Hello, world!   "