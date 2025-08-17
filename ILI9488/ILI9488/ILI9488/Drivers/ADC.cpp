#include "ADC.h"

void ADC_Init(ADC_State state, ADC_Reference ref, ADC_Prescaler prescaler, ADC_Channel channel, ADC_Alignment align)
{
	// 1. ��������� ���������� ���
	ADCSRA = (state == ADC_ENABLED) ? (1 << ADEN) | prescaler : 0;
	
	// 2. ��������� �������������� � �������� ����������
	ADMUX = ref | align | (channel & 0x07);
	
	// 3. ��� ����������� ��� - ������������� ��������
	if(ref == ADC_REF_INTERNAL) {
		_delay_us(120);
	}
	/*
	������ ��� ����� ��������?

	1. ���������� ������� ������������
	���������� ��� - ��� ���������� �����, ������� ��������� ����� (~50-100 ���) ��� ������ �� ���������� ���������� ����� ���������. ��� �������� ������ ��������� �������������� ����� ���� ���������.

	2. ����������� ���������� datasheet
	� ������������ ATmega32 (������ "ADC") ���� �������:
	"When switching to internal reference, a delay of at least 100?s should be given before starting conversion"

	3. ����������� ���������
	���������� ��� ���������� �����������, ������� ������ ���������� �� �������� ����������.
	
	!! ����������. ��� ����� ������ ���  ����������� ���
	��� ADC_REF_AVCC � ADC_REF_AREF �������� �� ���������.
	*/
}

void ADC_Init(ADC_State state) 
{
	if(state == ADC_ENABLED) 
	{
		ADCSRA |= (1 << ADEN);  // ��������� ���
		} else {
		ADCSRA &= ~(1 << ADEN); // ���������� ���
	}
}

void ADC_Enable(void) {
	ADCSRA |= (1 << ADEN);
}

void ADC_Disable(void) {
	ADCSRA &= ~(1 << ADEN);
}

void ADC_StartConversion(void) {
	ADCSRA |= (1 << ADSC);
}

uint16_t ADC_GetResult(void) {
	while (!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF); // ����� �����
	return ADC;
}

uint16_t ADC_Read(ADC_Channel ch) {
	ADMUX = (ADMUX & ~0x07) | (ch & 0x07); // ����� ������
	ADC_StartConversion();
	return ADC_GetResult();
}