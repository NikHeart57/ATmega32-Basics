/*
 *	(INT0) PD2
 *	R1 = 100 kOm
 *	R2 = 220 Om	
 *	C1 = 0.1 uF (Но лучше наверное 1uF)
 *	Схема подключения и расчет в папке с проектом
 */
#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>	
#include <util/delay.h>


void setup(void)
{
	sei();											// Разрешение прерываний
	GICR |= (0 << INT1)|(1 << INT0)|(0 << INT2);	// General Interrupt Control Register - Установка битов INT1, INT0 или INT2 разрешает прерывания при возникновении события на соответствующем выводе микроконтроллера AVR, а сброс — запрещает.
	MCUCR |= (1 << ISC01)|(0 << ISC00);				// 1,0 - Перывание по спадающему фронту
	
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

