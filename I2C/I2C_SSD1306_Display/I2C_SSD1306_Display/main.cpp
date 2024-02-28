#include "main.h"

int main(void)
{
	ssd1306_Display Display(0b01111000, 5, -5);
	Display.Init();
	
	// Линии
	for(int x = 0; x < 128; x++)
	{
		Display.Buffer_SetPixel(x, 0);
		Display.Buffer_SetPixel(x, 31);
	}
	
	for(int y = 0; y < 32; y++)
	{
		Display.Buffer_SetPixel(0, y);
		Display.Buffer_SetPixel(127, y);
	}
	
	
	// Точки
	for(int x = 0; x < 128; x = x + 8)
	{
		for (int y = 0; y < 32; y = y + 8)
		{
			Display.Buffer_SetPixel(x, y);
		}
	}
	
	Display.Buffer_SetPixel(64, 14);
	Display.Buffer_SetPixel(64, 15);
	Display.Buffer_SetPixel(64, 17);
	Display.Buffer_SetPixel(64, 18);
	
	Display.Buffer_SetPixel(62, 16);
	Display.Buffer_SetPixel(63, 16);
	Display.Buffer_SetPixel(65, 16);
	Display.Buffer_SetPixel(66, 16);	
	
	Display.Buffer_Send();
	
}