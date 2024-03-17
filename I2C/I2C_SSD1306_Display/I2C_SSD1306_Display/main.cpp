#include "main.h"
using namespace std;


int main(void)
{	
	ssd1306_Display Display(0b01111000, 0, 0);
	Display.Init();
	
	
	Display.Buffer_Fill(0);
	
	
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Display.PrintASCII(x, y, (char)(16 * y + x));
		}
	}	
	
	char word[] = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D', '!'};
		
	Display.Print(word, 12);
	
	Display.Buffer_Send();
}
