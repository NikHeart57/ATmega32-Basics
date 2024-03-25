#include "main.h"
using namespace std;

void setup()
{
	DDRA = 0x00;			// (����� - 1; ���� - 0)
		
	ADCSRA |= (1 << ADEN);                                // ��������� ������ ��� (1)
	ADCSRA |= (1 << ADSC);                                // ������ ��� (1)
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);     // ����� �������� N ��� ����������� ������� ������������� f_��� = f_��� / N;
	// ��� ADPS0..2 = 111       => f_��� = 7372800 / 128 = 57600��
	ADMUX  |= (1 << REFS1)|(1 << REFS0);                  // ����� ���, REFS0..1 = 11 => ���������� ���, U_��� = 2,56�
	ADMUX  |= (0 << ADLAR);                               // ������������� ������������ ADLAR = 0; ADCH, ADCL
	ADMUX  |= (0 << MUX4)|(0 << MUX3)|(0 << MUX2)|(0 << MUX1)|(0 << MUX0);  // ����� ���� ���; MX0..4 = 00000 => ADC0(PA0);
	
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
		if (ADCSRA & (1 << ADIF))        // ���� ���� ADIF = 0 (�.�.ADCSRA&(1<<ADIF)=true) ������ �������������� �����������
		{
			//PORTD = ADCL;                // ����� ��������� �������� ADC � �����, ����� �������� ������ ��� �������� ���
			//PORTC = ADCH;                //
			ADCSRA |= (1 << ADIF);       // ��������� ����� ADIF, ����� ��������� ���������� ������ ���
			ADCSRA |= (1 << ADSC);       // ������ ���
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
