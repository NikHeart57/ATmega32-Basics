#include "main.h"
#include "ST7789.cpp"


char XCursor = 0;
char YCursor = 0;

unsigned char time[3] = {5, 47, 58};		// hour, min, sec
unsigned char compTime[3];
char buffer[3];



void setup(void)
{
	//////////// SPI ////////////
	// ��������� ����� SPI
	DDRB  |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(0 << MISO);		// ������������ ���������! ��� ���� 4-� ����� SPI!
	PORTB |= (1 << MOSI)|(1 << SCK)|(1 << SS)|(1 << MISO);		//
	
	// ��������� ������� SPI
	// ���������� SPI - (SPE), ������� ��� ������ - (DORD), ������ - (MSTR), ����� ? ((1 << CPHA)|(0 << SPR1)), �������� ������� - 64 (SPR1, SPR0) (~230���)
	SPCR = (1 << SPE)|(0 << DORD)|(1 << MSTR)|(1 << CPOL)|(1 << CPHA)|(0 << SPR1)|(0 << SPR0);	// (1 << CPHA)|(1 << SPR1) - ����������� ��� st7789!!!
	SPSR = (1 << SPI2X);
	
	// ��������� ���� RES ST7789
	DDRC  |= (1 << RES);									// ������ (1)
	PORTC |= (1 << RES);									// ���������� ������� ������� RES
	
	// ��������� ���� ������� ST7789
	DDRC  |= (1 << DC);										// ������ (1)
	PORTC |= (1 << DC);										// ���������� ������� ������� DC
	
	
	//////////// ������ 1 (16 ���) ���� ////////////
	OCR1A = 14398;	// ������ �������� ���������� CTC ������������ �� ������������� �������
	//OCR1A = 500;
	TCCR1A |= (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(0 << FOC1A)|(0 << FOC1B)|(0 << WGM11)|(0 << WGM10);	// COM - �����, FOC - ?, WGM - ����� CTC, CS - ���������
	TCCR1B |= (0 <<  ICNC1)|(0 <<  ICES1)|(0 <<  WGM13)|(1 <<  WGM12)|(1 <<  CS12)|(0 <<  CS11)|(1 <<  CS10);				// IC - ��������� ���� ICP1 (PD6)
	TIMSK  |= (0 << TICIE1)|(1 << OCIE1A)|(0 << OCIE1B)|(0 << TOIE1);														// OCIE1A - ���������� �� ���������� ���������


	//////////// ������ 0 (8 ���) PWM ////////////
	OCR0 |= 127;	// ���������� - �������� ��������� f = fcpu/(N*256) = 7372800/256 = 28800 || = 14745600/256 = 57600 | 248 -> 97.1% | 249 -> 97.5% | 250 -> 97.9% | 251 -> 98.3% | 252 -> 98.7%  | *�� ������� ���� 97,5 - 98,6%
	TCCR0 |= (0 << FOC0)|(1  << WGM01)|(1 << WGM00)|(1 << COM01)|(0 << COM00)|(0 << CS02)|(0 << CS01)|(1 << CS00);	// WGM - fast PWM, COM - clear on compare, CS - ���������, FOC - ?
	
	DDRB |= 0b00001000;		// ����� ��� - PB3(OC0)
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


int main(void)
{
	_delay_ms(100);
	setup();
	ST7789_Init();
	ST7789_Fill(0, 0, 0);
	sei();
	
	
	
	while (1)
	{
		while (compTime[2] == time[2]){}
		compTime[2] = time[2];

		XCursor = 0;
		YCursor = 0;
		PrintClock(time, &XCursor, &YCursor, rand() % 60, rand() % 60, rand() % 60, 5);

	}
}


ISR(TIMER1_COMPA_vect)
{
	cli();
	time[2]++;						// ��������� �������
	
	if (time[2] >= 60)
	{
		time[1]++;					// ��������� ������
		time[2] = 0;
		
		if (time[1] >= 60)
		{
			time[0]++;
			time[1] = 0;			// ��������� ����
			
			if (time[0] >= 24)		// �����
			{
				time[2] = 0;
				time[1] = 0;
				time[0] = 0;
			}
		}
	}
			
	sei();
}

