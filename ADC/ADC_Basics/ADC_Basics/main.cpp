/* 
 * � ���� ������� �������� ��� ��������� ��� � ���������������� AtMega32
 * ��� ��������� PA0. ���� �� PA0 ���������� ������ 1� �� �������� ������ ����� (PC2), 
 * ���� � ��������� �� 1 �� 1,5 �� �������� ������� (PC3), ���� ����� 1,5 �� �������� ������� (PC4).
 * ��� ����������� �������� �������� ADC ��������� �� ���� ADCL - PD0..7, ADCH - PC0..1.
 * � ������ ������������ ���������� ��� ��.
 * ���� AREF � AVCC �������������� ������� � ������, AVCC ��������� � VCC ����� ��������.
 * AREF � ����� �� ���������� !
 */

#define F_CPU 7372800
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

void setup()
{
	DDRD = 0xff;         // ��� ������ (1)
	DDRC = 0xff;         // ��� ������ (1)
	DDRA = 0b00000000;   // ��� �����  (0)
	
	ADCSRA |= (1 << ADEN);                                // ��������� ������ ��� (1)
	ADCSRA |= (1 << ADSC);                                // ������ ��� (1)
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);     // ����� �������� N ��� ����������� ������� ������������� f_��� = f_��� / N;
	                                                      // ��� ADPS0..2 = 111       => f_��� = 7372800 / 128 = 57600��
	ADMUX  |= (1 << REFS1)|(1 << REFS0);                  // ����� ���, REFS0..1 = 11 => ���������� ���, U_��� = 2,56�
	ADMUX  |= (0 << ADLAR);                               // ������������� ������������ ADLAR = 0; ADCH, ADCL
	ADMUX  |= (0 << MUX4)|(0 << MUX3)|(0 << MUX2)|(0 << MUX1)|(0 << MUX0);  // ����� ���� ���; MX0..4 = 00000 => ADC0(PA0);
}


int main(void)
{
	setup();
	
	while (1)
	{
		_delay_ms(50);
		if (ADCSRA & (1 << ADIF))        // ���� ���� ADIF = 0 (�.�.ADCSRA&(1<<ADIF)=true) ������ �������������� �����������
		{
			PORTD = ADCL;                // ����� ��������� �������� ADC � �����, ����� �������� ������ ��� �������� ���
			PORTC = ADCH;                //
			ADCSRA |= (1 << ADIF);       // ��������� ����� ADIF, ����� ��������� ���������� ������ ���
			ADCSRA |= (1 << ADSC);       // ������ ���
		}

		if (ADC < 400)                                 // ���� U_in < 1� (ADC < 400) - ��������� ������ ����� (PC2)
		{
			PORTC |= (1 << PC2)|(0 << PC3)|(0 << PC4);
		}
		else if	(ADC >= 400 && ADC <= 600)	           // ���� 1� (ADC > 400) >= U_in <= 1.5� (ADC < 600) - ��������� ������� ����� (PC3)
		{
			PORTC |= (0 << PC2)|(1 << PC3)|(0 << PC4);
		}
		else if (ADC >= 600)                           // ���� U_in > 1.5� (ADC > 600) - ��������� ������� ����� (PC4)
		{
			PORTC |= (0 << PC2)|(0 << PC3)|(1 << PC4);
		}
	}
}

