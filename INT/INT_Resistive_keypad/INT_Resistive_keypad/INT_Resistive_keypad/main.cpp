/*
 *	(INT0) PD2
 *	R1 = 100 kOm
 *	R2 = 220 Om	
 *	C1 = 0.1 uF (�� ����� �������� 1uF)
 *	����� ����������� � ������ � ����� � ��������
 */
#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>	
#include <util/delay.h>


void setup(void)
{
	sei();											// ���������� ����������
	GICR |= (0 << INT1)|(1 << INT0)|(0 << INT2);	// General Interrupt Control Register - ��������� ����� INT1, INT0 ��� INT2 ��������� ���������� ��� ������������� ������� �� ��������������� ������ ���������������� AVR, � ����� � ���������.
	MCUCR |= (1 << ISC01)|(0 << ISC00);				// 1,0 - ��������� �� ���������� ������
	
	DDRC = 0xff;
	PORTC = 0;
}


int main(void)
{
	setup();
	
    
    while (1) 
    {
    }
}

ISR(INT0_vect)
{
	PORTC++;
}

