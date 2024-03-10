#include "main.h"

int main(void)
{
	ssd1306_Display Display(0b01111000, 0, 0);
	Display.Init();
	int i = 0;

	while (1)
	{		
		Display.Buffer_Fill(0);
		
		for(int j = 0; j < 3; j++)
		{
			Display.Buffer_SetTriangle(rand() % 127,rand() % 63, rand() % 127,rand() % 63, rand() % 127,rand() % 63);
		}
		
		Display.Buffer_SetPixel(i, 0);
		Display.Buffer_SetPixel(i, 1);
		Display.Buffer_SetPixel(i, 2);
		Display.Buffer_SetPixel(i, 3);
		Display.Buffer_SetPixel(i, 5);
		Display.Buffer_SetPixel(i, 7);
		i++;
		
		if(i >= 128)
		{
			i = 0;
		}
		
		Display.Buffer_Send();
	}
	
	
	/*
	// Тест 10 треугольников
	
		Display.Buffer_Fill(0);
			
		for(int j = 0; j < 10; j++)
		{
			Display.Buffer_SetTriangle(rand() % 127,rand() % 63, rand() % 127,rand() % 63, rand() % 127,rand() % 63);
		}
			
		Display.Buffer_SetPixel(i, 0);
		Display.Buffer_SetPixel(i, 1);
		Display.Buffer_SetPixel(i, 2);
		Display.Buffer_SetPixel(i, 3);
		Display.Buffer_SetPixel(i, 5);
		Display.Buffer_SetPixel(i, 7);
		i++;
			
		if(i >= 128)
		{
			i = 0;
		}
			
		Display.Buffer_Send();
	*/
	
	
	/*
	Звездочки
	for(int i = 0; i < 2; i++)
	{
		Display.Buffer_SetPixel(rand() % 127,rand() % 63);
	}
	
	for(int i = 0; i < 220; i++)
	{
		Display.Buffer_RemovePixel(rand() % 127,rand() % 63);
	}

	Display.Buffer_Send();
	*/
	
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