#include "main.h"

int main(void)
{
	ssd1306_Display Display(0b01111000, 0, 0);
	Display.Init();
	
	// Звездочки
	while (1)
	{
		for(int i = 0; i < 1; i++)
		{
			Display.Buffer_SetPixel(rand() % 127,rand() % 31);
		}
		
		for(int i = 0; i < 150; i++)
		{
			Display.Buffer_RemovePixel(rand() % 127,rand() % 31);
		}
		
		Display.Buffer_Send();
	}
	
	
	
	/*
	// Вывод функции	
	// Линии OX, OY
	Display.Buffer_SetLine(-64, 0, 63, 0);
	Display.Buffer_SetLine(0, 16, 0, -15);
			
	for (float x = -64; x < 64; x += 0.5)
	{
		Display.Buffer_SetPixel(x, -13*cos(x/4));
	}
			
	Display.Buffer_Send();
	*/
		
}