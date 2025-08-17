#include "INT.h"

static Int_Callback int0_callback = NULL;
static Int_Callback int1_callback = NULL;
static Int_Callback int2_callback = NULL;

// INT0 функции
void INT0_Init(Int_State state)
{
	DDRD &= ~(1 << PD2);      // PD2 (INT0) как вход
	PORTD |= (1 << PD2);      // Подтяжка к VCC (опционально)
	
	switch (state) {
		case INT_ENABLED:
		GICR |= (1 << INT0);
		break;
		case INT_DISABLED:
		GICR &= ~(1 << INT0);
		break;
		default:
		break;
	}
}

void INT0_Init(Int_State state, Int_TriggerMode mode)
{
	DDRD &= ~(1 << PD2);      // PD2 (INT0) как вход
	PORTD |= (1 << PD2);      // Подтяжка к VCC (опционально)
	
	INT0_Init(state);
	
	// Настройка триггера
	switch (mode) {
		case INT_TRIGGER_LOW_LEVEL:
		MCUCR &= ~(1 << ISC00) & ~(1 << ISC01);    // Низкий уровень
		break;
		case INT_TRIGGER_ANY_EDGE:
		MCUCR |= (1 << ISC00);                     // Любое изменение
		MCUCR &= ~(1 << ISC01);
		break;
		case INT_TRIGGER_RISING_EDGE:
		MCUCR |= (1 << ISC00) | (1 << ISC01);     // Нарастающий фронт
		break;
		case INT_TRIGGER_FALLING_EDGE:
		MCUCR &= ~(1 << ISC00);                   // Спадающий фронт
		MCUCR |= (1 << ISC01);
		break;
		default:
		MCUCR |= (1 << ISC00);                    // По умолчанию любое изменение
		MCUCR &= ~(1 << ISC01);
		break;
	}
}

void INT0_SetCallback(Int_Callback callback)
{
	int0_callback = callback;
}

// INT1 функции
void INT1_Init(Int_State state)
{
	DDRD &= ~(1 << PD3);      // PD3 (INT1) как вход
	PORTD |= (1 << PD3);      // Подтяжка к VCC (опционально)
	
	switch (state) {
		case INT_ENABLED:
		GICR |= (1 << INT1);
		break;
		case INT_DISABLED:
		GICR &= ~(1 << INT1);
		break;
		default:
		break;
	}
}

void INT1_Init(Int_State state, Int_TriggerMode mode)
{
	DDRD &= ~(1 << PD3);      // PD3 (INT1) как вход
	PORTD |= (1 << PD3);      // Подтяжка к VCC (опционально)
	
	INT1_Init(state);
	
	// Настройка триггера
	switch (mode) {
		case INT_TRIGGER_LOW_LEVEL:
		MCUCR &= ~(1 << ISC10) & ~(1 << ISC11);    // Низкий уровень
		break;
		case INT_TRIGGER_ANY_EDGE:
		MCUCR |= (1 << ISC10);                     // Любое изменение
		MCUCR &= ~(1 << ISC11);
		break;
		case INT_TRIGGER_RISING_EDGE:
		MCUCR |= (1 << ISC10) | (1 << ISC11);      // Нарастающий фронт
		break;
		case INT_TRIGGER_FALLING_EDGE:
		MCUCR &= ~(1 << ISC10);                   // Спадающий фронт
		MCUCR |= (1 << ISC11);
		break;
		default:
		MCUCR |= (1 << ISC10);                    // По умолчанию любое изменение
		MCUCR &= ~(1 << ISC11);
		break;
	}
}

void INT1_SetCallback(Int_Callback callback)
{
	int1_callback = callback;
}

// INT2 функции
void INT2_Init(Int_State state)
{
	DDRD &= ~(1 << PD2);      // PD2 (INT2) как вход
	PORTD |= (1 << PD2);      // Подтяжка к VCC (опционально)
	
	switch (state) {
		case INT_ENABLED:
		GICR |= (1 << INT2);
		break;
		case INT_DISABLED:
		GICR &= ~(1 << INT2);
		break;
		default:
		break;
	}
}

void INT2_Init(Int_State state, Int_TriggerMode mode)
{
	DDRD &= ~(1 << PD2);      // PD2 (INT2) как вход
	PORTD |= (1 << PD2);      // Подтяжка к VCC (опционально)
	
	INT2_Init(state);
	
	// Настройка триггера
	switch (mode) {
		case INT_TRIGGER_LOW_LEVEL:
		MCUCSR &= ~(1 << ISC2);            // Низкий уровень
		break;
		case INT_TRIGGER_FALLING_EDGE:
		MCUCSR |= (1 << ISC2);            // Спадающий фронт
		break;
		default:
		MCUCSR |= (1 << ISC2);            // По умолчанию спадающий фронт
		break;
	}
}

void INT2_SetCallback(Int_Callback callback)
{
	int2_callback = callback;
}

/*
// Обработчики прерываний
ISR(INT0_vect)
{
	if (int0_callback != NULL)
	{
		int0_callback();
	}
}

ISR(INT1_vect)
{
	if (int1_callback != NULL)
	{
		int1_callback();
	}
}

ISR(INT2_vect)
{
	if (int2_callback != NULL)
	{
		int2_callback();
	}
}
*/