#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define LATCH	PC0
#define SCK		PC1
#define DATA	PC2
#define OE		PC3

void setup(void)
{
	DDRC  |= (1 << LATCH)|(1 << SCK)|(1 << DATA)|(1 << OE);			// Выходы (1)
	PORTC |= (1 << LATCH);											// Изначально высокий уровень LATCH (ST_CP)
	PORTC &= ~((1 << SCK)|(1 << DATA)|(1 << OE));

}

void Send1(void)
{
	// Отправка 1
	PORTC |= (1 << DATA);		// Установка бита
	_delay_ms(10);
	PORTC |= (1 << SCK);		// Такт
	_delay_ms(20);				//
	PORTC &= ~(1 << SCK);		// Конец такта
	_delay_ms(10);
}


void Send0(void)
{
	// Отправка 0
	PORTC &= ~(1 << DATA);		// Установка бита в ноль
	_delay_ms(10);
	PORTC |= (1 << SCK);		// Такт
	_delay_ms(20);				//
	PORTC &= ~(1 << SCK);		// Конец такта
	_delay_ms(10);
}

int main(void)
{
	setup();
	
	PORTC &= ~(1 << LATCH);
	_delay_ms(10);
	
	Send0();
	Send1();
	Send1();	
	Send0();
	Send0();
	Send1();
	Send1();
	Send0();
	
	PORTC |= (1 << LATCH);
	_delay_ms(10);
}


