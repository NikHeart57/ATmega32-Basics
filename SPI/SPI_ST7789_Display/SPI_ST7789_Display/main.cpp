#include "main.h"
#include "ST7789.cpp"


char XCursor = 0;
char YCursor = 0;
char size = 4;

unsigned char time[3] = {8, 51, 0};		// hour, min, sec
unsigned char compTime[3];
float decHour = 0;
char buffer[8];



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
	//OCR1A = 500;
	TCCR1A |= (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(0 << FOC1A)|(0 << FOC1B)|(0 << WGM11)|(0 << WGM10);	// COM - порты, FOC - ?, WGM - режим CTC, CS - прескелер
	TCCR1B |= (0 <<  ICNC1)|(0 <<  ICES1)|(0 <<  WGM13)|(1 <<  WGM12)|(1 <<  CS12)|(0 <<  CS11)|(1 <<  CS10);				// IC - настройки пина ICP1 (PD6)
	TIMSK  |= (0 << TICIE1)|(1 << OCIE1A)|(0 << OCIE1B)|(0 << TOIE1);														// OCIE1A - Прерывание по совпадению разрешено


	//////////// Таймер 0 (8 бит) PWM ////////////
	OCR0 |= 127;	// Скважность - Значение сравнения f = fcpu/(N*256) = 7372800/256 = 28800 || = 14745600/256 = 57600 | 248 -> 97.1% | 249 -> 97.5% | 250 -> 97.9% | 251 -> 98.3% | 252 -> 98.7%  | *по расчету надо 97,5 - 98,6%
	TCCR0 |= (0 << FOC0)|(1  << WGM01)|(1 << WGM00)|(1 << COM01)|(0 << COM00)|(0 << CS02)|(0 << CS01)|(1 << CS00);	// WGM - fast PWM, COM - clear on compare, CS - прескелер, FOC - ?
	
	DDRB |= 0b00001000;		// Вывод ШИМ - PB3(OC0)
}

void PrintClock(unsigned char time[], char* Xcursor, char* Ycursor, char red, char green, char blue, char size)
{
	for(char i = 0; i < 3; i++)
	{
		itoa(time[i], buffer, 10);
	
		if (time[i] < 10)
		{
			ST7789_ASCIPrintString("0", &XCursor, &YCursor, red, green, blue, size);
		}

		ST7789_ASCIPrintString(buffer, &XCursor, &YCursor, red, green, blue, size);
	
		if (i < 2)
		{
			ST7789_ASCIPrintString(":", &XCursor, &YCursor, red, green, blue, size);
		}
	}	
}

// Reverses a string 'str' of length 'len'
inline void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
inline int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.'; // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter
		// is needed to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

void PrintDecHour(float decHour, char* Xcursor, char* Ycursor, char red, char green, char blue, char size)
{
	ftoa(decHour, buffer, 3);
	if (decHour < 1)
	{
		ST7789_ASCIPrintString("00", &XCursor, &YCursor, red, green, blue, size);
	}
	else if (decHour < 10)
	{
		ST7789_ASCIPrintString("0", &XCursor, &YCursor, red, green, blue, size);
	}
	ST7789_ASCIPrintString(buffer, &XCursor, &YCursor, red, green, blue, size);
}


int main(void)
{
	_delay_ms(100);
	setup();
	ST7789_Init();
	ST7789_Fill(0, 0, 0);
	sei();
	
	/*
	допиливай сырое.  прочитал АЦП 10-20 раз с шагом 1-2мс нашел текущее средние , 
	сравнил с передушим значением , попал в окно dU принял решение о номере нажатой кнопки , т
	екущие средние записал как предыдущие. 
	*/
	
	while (1)
	{
		while (compTime[2] == time[2]){}
		compTime[2] = time[2];
		
		char red = rand() % 60;
		char green = rand() % 60;
		char blue = rand() % 60;
		
		XCursor = 0;
		YCursor = 0;
		PrintClock(time, &XCursor, &YCursor, red, green, blue, size);
		
		
		XCursor = 0;
		YCursor = 8 * size;
		PrintDecHour(decHour, &XCursor, &YCursor, red, green, blue, size);

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
	
	decHour = (float)time[0] + (float)time[1] / 60.0 + (float)time[2] / 3600.0;
			
	sei();
}

