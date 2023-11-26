/* 
 * ������ DC-DC �������, � ���������� ��������� ���������� �������� PC1 � PC0
 * ��� ��� - PA7(ADC7). � HV ������ ������� ��������� �������� ���������� �� 101 (R1 = 1���, R2 = 10���, K = 101)
 * ��� ������� ���������� �� ��������, � � ������������ � ��� ������ ���������� ��� ������� (OCR1B)
 * �� ���� ���� �������� ���������� �������.
 * ��� PD4 - PWM, ���� PC2..4 - �������� ��� ��������� ��������� ���.
 * AREF, AVCC, ADC7 - �������������� ���������� � �����, AVCC - ���������� � VCC ����� ��������
 */

#define F_CPU 7372800
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <stdio.h>
#include <stdlib.h>

const unsigned int ADC_tolerance = 2;     
unsigned int       voltage = 60;          // ������� ��� 245
unsigned int	   ADC_target = 1;
unsigned int	   PushPC0 = 10;
unsigned int	   PushPC1 = 10;


void setup()
{
	DDRC = 0b00011100;				// ������ ��� ����.(PC2), ���.(PC3), �����.(PC4) �������� (1); ������ PC1 � PC0;
	PORTC |= (1 << PC1)|(1 << PC0); // ������ PC1 � PC0;
	DDRD = 0b00010000;				// ����� PWM - PD4 (1)
	DDRA = 0b00000000;				// ��� �����  (0)
	
	
	ADCSRA |= (1 << ADEN);                                // ��������� ������ ��� (1)
	ADCSRA |= (1 << ADSC);                                // ������ ��� (1)
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);     // ����� �������� N ��� ����������� ������� ������������� f_��� = f_��� / N;
	                                                      // ��� ADPS2..0 = 111       => f_��� = 7372800 / 128 = 57600��
	ADMUX  |= (1 << REFS1)|(1 << REFS0);                  // ����� ���, REFS2..0 = 11 => ���������� ���, U_��� = 2,56�
	ADMUX  |= (0 << ADLAR);                               // ������������� ������������ ADLAR = 0; ADCH, ADCL
	ADMUX  |= (0 << MUX4)|(0 << MUX3)|(1 << MUX2)|(1 << MUX1)|(1 << MUX0);  // ����� ���� ���; MX4..0 = 00111 => ADC7(PA7);
	
	
	sei();
	TCCR1A |= (0 << COM1A1)|(0 << COM1A0)|(1 << COM1B1)|(0 << COM1B0)|(0 << FOC1A)|(0 << FOC1B)|(0 << WGM11)|(0 << WGM10);
	TCCR1B |= (0 << ICNC1) |(0 << ICES1) |(1 << WGM13) |(0 << WGM12) |(0 << CS12) |(0 << CS11) |(1 << CS10);
	ICR1 = 255;		// ������� ��������, �� �������� ���� ����
	OCR1B = 1;		// ������� ���������, ��� ���������� � ������� �������� ��������� ������ OC1B
}

void ADC_reboot()
{
	if (ADCSRA & (1 << ADIF))			// ���� ���� ADIF = 0 (�.�.ADCSRA&(1<<ADIF)=true) ������ �������������� �����������
	{
		ADCSRA |= (1 << ADIF);			// ��������� ����� ADIF, ����� ��������� ���������� ������ ���
		ADCSRA |= (1 << ADSC);			// ������ ���
	}	
}

void ADC_calculation()
{
	float float_target = 1;
	float_target = float_target * 1024 * voltage;
	float_target = float_target / 98.5;					// 98.5 - ��� ������������� �������� �������� (��������� - 101 = 1���/10���)
	float_target = float_target / 2.56;
	ADC_target = (int)float_target;
			
	if (ADC_target >= 1024 - ADC_tolerance)
	{
		ADC_target = 1024 - ADC_tolerance;
	}
}

void ButtonPC0()
{
	if (!(PINC & (1 << PINC0)))
	{
		if (PushPC0 == 0 && voltage > 20)
		{
			// ��� �������� ������
			PushPC0 = 20;
			voltage--;
		}
	}
	else
	{
		if (PushPC0 > 0)
		{
			PushPC0--;
		}
	}
}

void ButtonPC1()
{
	if (!(PINC & (1 << PINC1)))
	{
		if (PushPC1 == 0 && voltage < 240)
		{
			// ��� �������� ������
			PushPC1 = 20;
			voltage++;
		}
	}
	else
	{
		if (PushPC1 > 0)
		{
			PushPC1--;
		}
	}
}


int main(void)
{
	setup();
	
	while (1)
	{
		ButtonPC0();
		ButtonPC1();
		
		ADC_reboot();								// ������� ���
		ADC_calculation();							// ������ ��� � ������������� � ��������� �����������
		
		// ������ ���
		if (ADC < ADC_target - ADC_tolerance)												// ��������� ������ ����� (PC2)
		{
			PORTC = (1 << PC1)|(1 << PC0)|(1 << PC2)|(0 << PC3)|(0 << PC4);
			if (OCR1B < (ICR1 - 2))
			{
				OCR1B++;
			}
		}
		else if	(ADC >= ADC_target - ADC_tolerance && ADC <= ADC_target + ADC_tolerance)	// ��������� ������� ����� (PC3)
		{
			PORTC = (1 << PC1)|(1 << PC0)|(0 << PC2)|(1 << PC3)|(0 << PC4);
		}
		else																				// ��������� ������� ����� (PC4)
		{
			PORTC = (1 << PC1)|(1 << PC0)|(0 << PC2)|(0 << PC3)|(1 << PC4);
			if (OCR1B > 1)
			{
				OCR1B--;
			}
		}					
	}
}

