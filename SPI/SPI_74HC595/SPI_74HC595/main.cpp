#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LATCH	PC0
#define SS		PB4
#define MOSI	PB5
#define MISO	PB6
#define SCK		PB7


void setup(void)
{
	// ��������� ���� ������� (������ ����� ���� ����� ������������ ��� SS ��� ����� ������, �� �������)
	DDRC  |= (1 << LATCH);									// ������ (1)
	PORTC |= (1 << LATCH);									// ���������� ������� ������� LATCH (ST_CP)
	
	// ��������� ����� SPI
   DDRB  |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(0 << MISO);	// ������������ ���������! ��� ���� 4-� ����� SPI!
   PORTB |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(1 << MISO);	//

   // ���������� SPI (SPE), ������� ��� ������ (DORD), ������ (MSTR), ����� 0 (CPOL, CPHA), �������� ������� - 128 (SPR1, SPR0)
   SPCR = (1 << SPE)|(0 << DORD)|(1 << MSTR)|(0 << CPOL)|(0 << CPHA)|(1 << SPR1)|(1 << SPR0);
   SPSR = (0 << SPI2X);
}

void SPI_WriteByte(uint8_t data)
{
	//PORTB &= ~(1 << SS);				// ����� ������������ ����� ��� ��� ������ ����������
	PORTC &= ~(1 << LATCH);		
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
	//PORTB |= (1 << SS);
	PORTC |= (1 << LATCH);
}

int main(void)
{
	setup();
	_delay_ms(100);						// �� �����������
	
	while(1)
	{
		SPI_WriteByte(rand() % 255);
	}
}





