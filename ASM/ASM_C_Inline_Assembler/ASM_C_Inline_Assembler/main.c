#include "main.h"

extern void wait(uint8_t param);

int main(void)
{
	init();
	while(1)
	{
		PORTB ^= 0x20;	// �������������/���������� ��� 5 (���� ��������� �� 0) PORTB �������� (����������/������ ���������� Arduino ���������)
		wait(50);
	}
	
	return 0;
}

static void init(void)
{
	DDRB = 0x20;			// ������������� � ��� 5 � DDRB (Data Direction Register B) 1 ��� ������� Arduino, ��� pin 13 �� ����� B ����� �������� ��������.
}
