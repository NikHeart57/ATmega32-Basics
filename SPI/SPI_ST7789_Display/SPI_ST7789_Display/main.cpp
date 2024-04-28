#include "main.h"
#include "ST7789.cpp"


char XCursor = 0;
char YCursor = 0;

char time[3] = {5, 47, 58};		// hour, min, sec
char minOld;
float decHour = (float)time[0] + ((float)time[1] / (float)60);

char buffer[3];

char dawnBegin = 6;
char dawnEnd = 10;
char duskBegin = 18;
char duskEnd = 21;

char bottomLevel = 150;
char topLevel = 100;

void setup(void)
{
	//////////// SPI ////////////
	// Настройка пинов SPI
	DDRB  |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(0 << MISO);		// Обязательная настройка! Для всех 4-х пинов SPI!
	PORTB |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(1 << MISO);		//
	
	// Настройка реистра SPI
	// Разрешение SPI - (SPE), старший бит вперед - (DORD), мастер - (MSTR), режим ? ((1 << CPHA)|(0 << SPR1)), делитель частоты - 64 (SPR1, SPR0) (~230кГц)
	SPCR = (1 << SPE)|(0 << DORD)|(1 << MSTR)|(1 << CPOL)|(1 << CPHA)|(0 << SPR1)|(0 << SPR0);	// (1 << CPHA)|(1 << SPR1) - обязательно для st7789!!!
	SPSR = (1 << SPI2X);
	
	// Настройка пина RES ST7789
	DDRC  |= (1 << RES);									// Выходы (1)
	PORTC |= (1 << RES);									// Изначально высокий уровень RES
	
	// Настройка пина защёлки ST7789
	DDRC  |= (1 << DC);										// Выходы (1)
	PORTC |= (1 << DC);										// Изначально высокий уровень DC
	
	
	//////////// Таймер 1 (16 бит) Часы ////////////
	OCR1A = 14398;	// Запись значения прерывания CTC производится до инициализации таймера
	OCR1A = 500;
	TCCR1A |= (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(0 << FOC1A)|(0 << FOC1B)|(0 << WGM11)|(0 << WGM10);	// COM - порты, FOC - ?, WGM - режим CTC, CS - прескелер
	TCCR1B |= (0 <<  ICNC1)|(0 <<  ICES1)|(0 <<  WGM13)|(1 <<  WGM12)|(1 <<  CS12)|(0 <<  CS11)|(1 <<  CS10);				// IC - настройки пина ICP1 (PD6)
	TIMSK  |= (0 << TICIE1)|(1 << OCIE1A)|(0 << OCIE1B)|(0 << TOIE1);														// OCIE1A - Прерывание по совпадению разрешено


	//////////// Таймер 0 (8 бит) PWM ////////////
	OCR0 |= 128;	// Скважность - Значение сравнения f = fcpu/(N*256) = 7372800/256 = 28800 || = 14745600/256 = 57600 | 248 -> 97.1% | 249 -> 97.5% | 250 -> 97.9% | 251 -> 98.3% | 252 -> 98.7%  | *по расчету надо 97,5 - 98,6%
	TCCR0 |= (0 << FOC0)|(1  << WGM01)|(1 << WGM00)|(1 << COM01)|(0 << COM00)|(0 << CS02)|(0 << CS01)|(1 << CS00);	// WGM - fast PWM, COM - clear on compare, CS - прескелер, FOC - ?
	
	DDRB |= 0b00001000;		// Вывод ШИМ - PB3(OC0)
}

void NumbersLine(void)
{
	cli();
	char xPos = 0;
	char yPos = bottomLevel + 10;
	
	for(char i = 0; i < 24; i++)
	{
		if(i % 3 == 0)
		{
			itoa(i, buffer, 10);
			xPos = i * 10;
			ST7789_ASCIPrintString(buffer, &xPos, &yPos);
		}
	}
	
	sei();
}


void TimeLine()
{
	cli();
	
	ST7789_DrawLine(0,				bottomLevel,	10 * dawnBegin,	bottomLevel,	41, 41, 41);
	ST7789_DrawLine(10 * dawnBegin,	bottomLevel,	10 * dawnEnd,	topLevel,		41, 41, 41);
	ST7789_DrawLine(10 * dawnEnd,	topLevel,		10 * duskBegin,	topLevel,		41, 41, 41);
	ST7789_DrawLine(10 * duskBegin,	topLevel,		10 * duskEnd,	bottomLevel,	41, 41, 41);
	ST7789_DrawLine(10 * duskEnd,	bottomLevel,	240,			bottomLevel,	41, 41, 41);
	
	char PixelHour	= 10.0 * ((float)time[0] + ((float)time[1] / 60.0));
	for (int y = (topLevel - 10); y < (bottomLevel + 10); y++)
	{
		ST7789_DrawPixel(PixelHour - 1, y, 0, 0, 0);
		ST7789_DrawPixel(PixelHour, y, 41, 41, 41);
	}
	sei();
};

int main(void)
{
	_delay_ms(50);
	setup();
	ST7789_Init();
	ST7789_Fill(0, 0, 0);
	sei();
	
	NumbersLine();
	
	while (1)
	{
		if (minOld != time[2])		// Отрисовка графика
		{
			TimeLine();
		}
	}
}


ISR(TIMER1_COMPA_vect)
{
	cli();
	time[2]++;						// Инкремент секунды
	
	if (time[2] >= 60)
	{
		time[1]++;					// Инкремент минуты
		time[2] = 0;
		
		// decHour
		char XCursor = 0;
		char YCursor = 10;

		decHour = ((float)time[0] + ((float)time[1] / 60.f)) * 1000.f;
		itoa(decHour, buffer, 10);
		char temp[4];
		temp[0] = buffer[0];
		temp[1] = buffer[1];
		temp[2] = ',';
		temp[3] = '\0';
		ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
		
		temp[0] = buffer[2];
		temp[1] = buffer[3];
		temp[2] = buffer[4];
		temp[3] = '\0';
		ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
		
		
		// скважность
		XCursor = 0;
		YCursor = 20;
		decHour = (float)time[0] + ((float)time[1] / 60.f);		
		if ((float)decHour < (float)dawnBegin)
		{
			ST7789_ASCIPrintString("NIGHT", &XCursor, &YCursor);
			
			OCR0 = 0;
			itoa(OCR0, temp, 10);
			XCursor = 0;
			YCursor = 30;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
			
			itoa(((float)OCR0 / 255.f) * 100.f, temp, 10);
			XCursor = 0;
			YCursor = 40;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
		}
		else if ((float)decHour < (float)dawnEnd)
		{
			ST7789_ASCIPrintString("SUNRISE", &XCursor, &YCursor);
			
			OCR0 = 255 * (decHour - duskBegin)/(duskEnd - duskBegin);
			itoa(OCR0, temp, 10);
			XCursor = 0;
			YCursor = 30;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
			
			itoa(((float)OCR0 / 255.f) * 100.f, temp, 10);
			XCursor = 0;
			YCursor = 40;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
		}
		else if ((float)decHour < (float)duskBegin)
		{
			ST7789_ASCIPrintString("DAY", &XCursor, &YCursor);
			
			OCR0 = 255;
			itoa(OCR0, temp, 10);
			XCursor = 0;
			YCursor = 30;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
			
			itoa(((float)OCR0 / 255.f) * 100.f, temp, 10);
			XCursor = 0;
			YCursor = 40;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
		}
		else if ((float)decHour < (float)duskEnd)
		{
			ST7789_ASCIPrintString("SUNSET", &XCursor, &YCursor);
			
			OCR0 = 255 * (1 - (decHour - dawnBegin)/(dawnEnd - dawnBegin));
			itoa(OCR0, temp, 10);
			XCursor = 0;
			YCursor = 30;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
			
			itoa(((float)OCR0 / 255.f) * 100.f, temp, 10);
			XCursor = 0;
			YCursor = 40;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
		}
		else
		{
			ST7789_ASCIPrintString("NIGHT", &XCursor, &YCursor);
			
			OCR0 = 0;
			itoa(OCR0, temp, 10);
			XCursor = 0;
			YCursor = 30;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
			
			itoa(((float)OCR0 / 255.f) * 100.f, temp, 10);
			XCursor = 0;
			YCursor = 40;
			ST7789_ASCIPrintString(temp, &XCursor, &YCursor);
		}
		
		
		if (time[1] >= 60)
		{
			time[0]++;
			time[1] = 0;			// Инкремент часа
			
			if (time[0] >= 24)		// Сутки
			{
				time[2] = 0;
				time[1] = 0;
				time[0] = 0;
			}
		}
	}
	
	// Отрисовка часов
	char XCursor = 0;
	char YCursor = 0;
	
	for(char i = 0; i < 3; i++)
	{
		itoa(time[i], buffer, 10);
		
		if (time[i] < 10)
		{
			ST7789_ASCIPrintString("0", &XCursor, &YCursor);
		}

		ST7789_ASCIPrintString(buffer, &XCursor, &YCursor);
		
		if (i < 2)
		{
			ST7789_ASCIPrintString(":", &XCursor, &YCursor);
		}
	}
		
	sei();
}

