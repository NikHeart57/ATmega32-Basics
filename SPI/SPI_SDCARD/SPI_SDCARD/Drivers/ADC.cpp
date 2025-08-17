#include "ADC.h"

void ADC_Init(ADC_State state, ADC_Reference ref, ADC_Prescaler prescaler, ADC_Channel channel, ADC_Alignment align)
{
	// 1. Настройка управления АЦП
	ADCSRA = (state == ADC_ENABLED) ? (1 << ADEN) | prescaler : 0;
	
	// 2. Настройка мультиплексора и опорного напряжения
	ADMUX = ref | align | (channel & 0x07);
	
	// 3. Для внутреннего ИОН - рекомендуемая задержка
	if(ref == ADC_REF_INTERNAL) {
		_delay_us(120);
	}
	/*
	Почему тут нужна задержка?

	1. Физический процесс стабилизации
	Внутренний ИОН - это аналоговая схема, которой требуется время (~50-100 мкс) для выхода на стабильное напряжение после включения. Без задержки первые несколько преобразований могут быть неточными.

	2. Технические требования datasheet
	В документации ATmega32 (раздел "ADC") явно указано:
	"When switching to internal reference, a delay of at least 100?s should be given before starting conversion"

	3. Конденсатор подзаряда
	Внутренний ИОН использует конденсатор, который должен зарядиться до рабочего напряжения.
	
	!! Примечание. Это нужно только для  внутреннего ИОН
	Для ADC_REF_AVCC и ADC_REF_AREF задержка не требуется.
	*/
}

void ADC_Init(ADC_State state) 
{
	if(state == ADC_ENABLED) 
	{
		ADCSRA |= (1 << ADEN);  // Включение АЦП
		} else {
		ADCSRA &= ~(1 << ADEN); // Отключение АЦП
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
	ADCSRA |= (1 << ADIF); // Сброс флага
	return ADC;
}

uint16_t ADC_Read(ADC_Channel ch) {
	ADMUX = (ADMUX & ~0x07) | (ch & 0x07); // Выбор канала
	ADC_StartConversion();
	return ADC_GetResult();
}