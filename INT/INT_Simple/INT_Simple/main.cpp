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


void setup(void)
{
	sei();																// ���������� ����������
	
	GICR |= (1 << INT1)|(1 << INT0)|(1 << INT2);						// General Interrupt Control Register - ��������� ����� INT1, INT0 ��� INT2 ��������� ���������� ��� ������������� ������� �� ��������������� ������ ���������������� AVR, � ����� � ���������.
	MCUCR |= (1 << ISC11)|(0 << ISC10)|(1 << ISC01)|(0 << ISC00);		// 10	- ��������� �� ���������� ������ INT0, INT1
	MCUCSR |= (0 << ISC2);												// 0	- ��������� �� ���������� ������ INT2
	
	DDRA = 0xff;
	PORTA = 0;
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
	PORTA++;
}

ISR(INT1_vect)
{
	PORTA--;
}

ISR(INT2_vect)
{
	PORTA = ~PORTA;
}