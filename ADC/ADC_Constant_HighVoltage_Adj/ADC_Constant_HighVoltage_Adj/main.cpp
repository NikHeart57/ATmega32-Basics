/* 
 * Пример DC-DC бустера, с настройкой выходного напряжения кнопками PC1 и PC0
 * Пин АЦП - PA7(ADC7). К HV выводу бустера подключен делитель напряжения на 101 (R1 = 1МОм, R2 = 10кОм, K = 101)
 * АЦП смотрит напряжение на делителе, и в соответствии с ним меняет скважность ШИМ бустера (OCR1B)
 * за счет чего меняется напряжение бустера.
 * Пин PD4 - PWM, пины PC2..4 - лампочки для индикации состояния АЦП.
 * AREF, AVCC, ADC7 - конденсаторами подключены к земле, AVCC - подключена к VCC через индукцию
 */

#define F_CPU 7372800
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <stdio.h>
#include <stdlib.h>

const unsigned int ADC_tolerance = 2;     
unsigned int       voltage = 60;          // Реально мах 245
unsigned int	   ADC_target = 1;
unsigned int	   PushPC0 = 10;
unsigned int	   PushPC1 = 10;


void setup()
{
	DDRC = 0b00011100;				// Выходы для желт.(PC2), зел.(PC3), красн.(PC4) лампочек (1); Кнопки PC1 и PC0;
	PORTC |= (1 << PC1)|(1 << PC0); // Кнопки PC1 и PC0;
	DDRD = 0b00010000;				// Выход PWM - PD4 (1)
	DDRA = 0b00000000;				// Все входы  (0)
	
	
	ADCSRA |= (1 << ADEN);                                // Разрешаем работу АЦП (1)
	ADCSRA |= (1 << ADSC);                                // Запуск АЦП (1)
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);     // Задаю делитель N для определения частоты дискретизации f_ацп = f_цпу / N;
	                                                      // при ADPS2..0 = 111       => f_ацп = 7372800 / 128 = 57600Гц
	ADMUX  |= (1 << REFS1)|(1 << REFS0);                  // Задаю ИОН, REFS2..0 = 11 => внутренний ИОН, U_ион = 2,56В
	ADMUX  |= (0 << ADLAR);                               // Правосторонее выравнивание ADLAR = 0; ADCH, ADCL
	ADMUX  |= (0 << MUX4)|(0 << MUX3)|(1 << MUX2)|(1 << MUX1)|(1 << MUX0);  // Выбор пина АЦП; MX4..0 = 00111 => ADC7(PA7);
	
	
	sei();
	TCCR1A |= (0 << COM1A1)|(0 << COM1A0)|(1 << COM1B1)|(0 << COM1B0)|(0 << FOC1A)|(0 << FOC1B)|(0 << WGM11)|(0 << WGM10);
	TCCR1B |= (0 << ICNC1) |(0 << ICES1) |(1 << WGM13) |(0 << WGM12) |(0 << CS12) |(0 << CS11) |(1 << CS10);
	ICR1 = 255;		// Верхнее значение, до которого идет счет
	OCR1B = 1;		// Регистр сравнения, при совпадении с которым меняется состояние вывода OC1B
}

void ADC_reboot()
{
	if (ADCSRA & (1 << ADIF))			// Если флаг ADIF = 0 (т.е.ADCSRA&(1<<ADIF)=true) значит преобразование выполнилось
	{
		ADCSRA |= (1 << ADIF);			// Установка флага ADIF, чтобы позволить дальнейшую работу АЦП
		ADCSRA |= (1 << ADSC);			// Запуск АЦП
	}	
}

void ADC_calculation()
{
	float float_target = 1;
	float_target = float_target * 1024 * voltage;
	float_target = float_target / 98.5;					// 98.5 - это эмперическкий кофицент делителя (расчетный - 101 = 1Мом/10кОм)
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
			// Код холодной кнопки
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
			// Код холодной кнопки
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
		
		ADC_reboot();								// Рестарт АЦП
		ADC_calculation();							// Расчет АЦП в соортветствии с требуемым напряжением
		
		// Логика АЦП
		if (ADC < ADC_target - ADC_tolerance)												// загорится желтая лампа (PC2)
		{
			PORTC = (1 << PC1)|(1 << PC0)|(1 << PC2)|(0 << PC3)|(0 << PC4);
			if (OCR1B < (ICR1 - 2))
			{
				OCR1B++;
			}
		}
		else if	(ADC >= ADC_target - ADC_tolerance && ADC <= ADC_target + ADC_tolerance)	// загорится зеленая лампа (PC3)
		{
			PORTC = (1 << PC1)|(1 << PC0)|(0 << PC2)|(1 << PC3)|(0 << PC4);
		}
		else																				// загорится красная лампа (PC4)
		{
			PORTC = (1 << PC1)|(1 << PC0)|(0 << PC2)|(0 << PC3)|(1 << PC4);
			if (OCR1B > 1)
			{
				OCR1B--;
			}
		}					
	}
}

