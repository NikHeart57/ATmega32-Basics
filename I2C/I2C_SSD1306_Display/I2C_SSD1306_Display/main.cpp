#include "main.h"
using namespace std;

void setup()
{
	DDRA = 0x00;			// (выход - 1; вход - 0)
		
	ADCSRA |= (1 << ADEN);                                // Разрешаем работу АЦП (1)
	ADCSRA |= (1 << ADSC);                                // Запуск АЦП (1)
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);     // Задаю делитель N для определения частоты дискретизации f_ацп = f_цпу / N;
	// при ADPS0..2 = 111       => f_ацп = 7372800 / 128 = 57600Гц
	ADMUX  |= (1 << REFS1)|(1 << REFS0);                  // Задаю ИОН, REFS0..1 = 11 => внутренний ИОН, U_ион = 2,56В
	ADMUX  |= (0 << ADLAR);                               // Правосторонее выравнивание ADLAR = 0; ADCH, ADCL
	ADMUX  |= (0 << MUX4)|(0 << MUX3)|(0 << MUX2)|(0 << MUX1)|(0 << MUX0);  // Выбор пина АЦП; MX0..4 = 00000 => ADC0(PA0);
	
}

int main(void)
{	
	setup();
	
	ssd1306_Display Display(0b01111000, 0, 0);
	Display.Init();
	Display.Buffer_Fill(0);
	
	int x = 0;
	int xOld = 0;
	int yOld = 0;
		
	while (1)
	{	
		_delay_ms(50);
		if (ADCSRA & (1 << ADIF))        // Если флаг ADIF = 0 (т.е.ADCSRA&(1<<ADIF)=true) значит преобразование выполнилось
		{
			//PORTD = ADCL;                // Вывод состояния регистра ADC в порты, чтобы наглядно видеть как работает АЦП
			//PORTC = ADCH;                //
			ADCSRA |= (1 << ADIF);       // Установка флага ADIF, чтобы позволить дальнейшую работу АЦП
			ADCSRA |= (1 << ADSC);       // Запуск АЦП
		}
		
		Display.Xposconsole = 0;
		Display.Yposconsole = 0;
		
		float mvoltage = (ADC * 2.5f);
		
		if(mvoltage >= 1000)
		{
			Display.PrintInt((unsigned int)(mvoltage / 1000));
			Display.Print(",");
			Display.PrintInt((unsigned int)(mvoltage - 1000));
			Display.Print("V");
		}
		else if(mvoltage < 1000)
		{
			Display.PrintInt((unsigned int)(0));
			Display.Print(",");
			Display.PrintInt((unsigned int)(mvoltage));
			Display.Print("V");
		}
		
		//int pixel = ((ADC >> 4) * 5) - 128;
		
		int pixel = (ADC >> 4);
		
		if (pixel > 0 && pixel < 127)
		{
			Display.Buffer_SetLine(xOld, yOld, x, pixel);
		}
		else if (pixel <= 0)
		{
			Display.Buffer_SetLine(xOld, yOld, x, 0);
		}
		else
		{
			Display.Buffer_SetLine(xOld, yOld, x, 127);
		}

		xOld = x;
		x++;
		yOld = pixel;
		
		if (x > 128)
		{
			Display.Buffer_Fill(0);
			for (int x = 0; x < 16; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					Display.Buffer_SetPixel(x * 8, y * 8);
				}
			}
			
			for (int x = 0; x < 32; x++)
			{
				Display.Buffer_SetPixel(x * 4, 32);
			}
			
			x = 0;
			xOld = 0;
		}
		
		Display.Buffer_Send();

	}
}
