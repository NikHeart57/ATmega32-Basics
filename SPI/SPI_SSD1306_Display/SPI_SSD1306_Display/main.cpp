#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CS		PC0
#define SS		PB4
#define MOSI	PB5
#define MISO	PB6
#define SCK		PB7


void setup(void)
{
	DDRC  |= (1 << PC1);
	// Настройка пина CS (Вместо этого пина можно использовать пин SS или любой другой, по желанию)
	DDRC  |= (1 << CS);										// Выходы (1)
	PORTC |= (1 << CS);										// Изначально высокий уровень CS
	
	// Настройка пинов SPI
	DDRB  |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(0 << MISO);	// Обязательная настройка! Для всех 4-х пинов SPI!
	PORTB |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(1 << MISO);	//

	// Разрешение SPI (SPE), старший бит вперед (DORD), мастер (MSTR), режим 0 (CPOL, CPHA), делитель частоты - 128 (SPR1, SPR0)
	SPCR = (1 << SPE)|(0 << DORD)|(1 << MSTR)|(0 << CPOL)|(0 << CPHA)|(1 << SPR1)|(0 << SPR0);
	SPSR = (0 << SPI2X);
}

void ssd1306SendCommand(uint8_t data)
{
	PORTB &= ~(1 << SS);				// Проверяй порты!!!
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
	PORTB |= (1 << SS);
}

int main(void)
{
	setup();
	_delay_ms(1000);						// Не обязательно
	
	
	ssd1306SendCommand(0xAE); //display off
	ssd1306SendCommand(0xD5); //Set Memory Addressing Mode
	ssd1306SendCommand(0x80); //00,Horizontal Addressing Mode;01,Vertical
	ssd1306SendCommand(0xA8); //Set Page Start Address for Page Addressing
	ssd1306SendCommand(0x3F); //Set COM Output Scan Direction
	ssd1306SendCommand(0xD3); //set low column address
	ssd1306SendCommand(0x00); //set high column address
	ssd1306SendCommand(0x40); //set start line address
	ssd1306SendCommand(0x8D); //set contrast control register
	ssd1306SendCommand(0x14);
	ssd1306SendCommand(0x20); //set segment re-map 0 to 127
	ssd1306SendCommand(0x00); //set normal display
	ssd1306SendCommand(0xA1); //set multiplex ratio(1 to 64)
	ssd1306SendCommand(0xC8); //
	ssd1306SendCommand(0xDA); //0xa4,Output follows RAM
	ssd1306SendCommand(0x12); //set display offset
	ssd1306SendCommand(0x81); //not offset
	ssd1306SendCommand(0x8F); //set display clock divide ratio/oscillator frequency
	ssd1306SendCommand(0xD9); //set divide ratio
	ssd1306SendCommand(0xF1); //set pre-charge period
	ssd1306SendCommand(0xDB);
	ssd1306SendCommand(0x40); //set com pins hardware configuration
	ssd1306SendCommand(0xA4);
	ssd1306SendCommand(0xA6); //set vcomh
	ssd1306SendCommand(0xAF); //0x20,0.77xVcc
	
	PORTC |= (1 << PC1);
	_delay_ms(1);
	PORTC &= ~(1 << PC1);
	_delay_ms(100);
	
	PORTC |= (1 << PC1);
	_delay_ms(1);
	PORTC &= ~(1 << PC1);
	_delay_ms(100);
	
	PORTC |= (1 << PC1);
	_delay_ms(1);
	PORTC &= ~(1 << PC1);
	_delay_ms(100);	
}
