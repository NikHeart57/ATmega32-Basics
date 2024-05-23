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


void setup(void)
{
	sei();																// Разрешение прерываний
	
	GICR |= (1 << INT1)|(1 << INT0)|(1 << INT2);						// General Interrupt Control Register - Установка битов INT1, INT0 или INT2 разрешает прерывания при возникновении события на соответствующем выводе микроконтроллера AVR, а сброс — запрещает.
	MCUCR |= (1 << ISC11)|(0 << ISC10)|(1 << ISC01)|(0 << ISC00);		// 10	- Перывание по спадающему фронту INT0, INT1
	MCUCSR |= (0 << ISC2);												// 0	- Перывание по спадающему фронту INT2
	
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