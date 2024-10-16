.text
	
.global wait								; ���������, ��� ������� (������) wait ���������� (�������� �� ������ ��������� ������)

; ��� ������� ��������� 1 8-� ������ �������� ����� �������, �������� AVR GCC Calling convention
wait:
	; ��������� � ���� �������� ���� ��������� ������� ����� �������������� � �������
	push r18	
	push r19
	push r24
	
	; ��������� ���������� �������� � �������� r18 � r19
	mov r18, r24
	mov r19, r24
_wait_loop:									; ����� wait �����
	dec  r18								; ��������� �������� � �������� r18 �� 1
    brne _wait_loop							; ��������� � ������ �����, ���� ������� r18 �� ����� 0
	dec  r19								; ��������� �������� � �������� r19 �� 1
    brne _wait_loop							; ��������� � ������ �����, ���� ������� r19 �� ����� 0
	dec  r24								; ��������� �������� � �������� r24 �� 1
    brne _wait_loop							; ��������� � ������ �����, ���� ������� r24 �� ����� 0
	
	; ��������������� �������� �������������� ��������� �� ����� (����������������� �������� � ������� �������� ������� �� ����������)
	pop r24
	pop r19
	pop r18									
	ret
	
.end										; ����� ���������