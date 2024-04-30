﻿#include "main.h"

const char monocraft[96][6] =
{
	/*
	//0x00 - 0x0F
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},							// Управляющие символы 
	
	//0x10 - 0x1F
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},							// Управляющие символы 
	*/
	
	//0x20 - 0x2F
	{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x20 - 32 - * Пробел *
	{0b00000000, 0b11111010, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x21 - 33 - !
	{0b00000000, 0b01100000, 0b00000000, 0b01100000, 0b00000000, 0b00000000},	// 0x22 - 34 - "
	{0b00101000, 0b11111110, 0b00101000, 0b11111110, 0b00101000, 0b00000000},	// 0x23 - 35 - #
	{0b00100100, 0b01010100, 0b11010110, 0b01010100, 0b01001000, 0b00000000},	// 0x24 - 36 - $
	{0b11000010, 0b00001100, 0b00010000, 0b01100000, 0b10000110, 0b00000000},	// 0x25 - 37 - %
	{0b00001100, 0b01010010, 0b10111010, 0b01001100, 0b00010010, 0b00000000},	// 0x26 - 38 - &
	{0b00000000, 0b00000000, 0b01100000, 0b00000000, 0b00000000, 0b00000000},	// 0x27 - 39 - '
	{0b00000000, 0b00111000, 0b01000100, 0b10000010, 0b00000000, 0b00000000},	// 0x28 - 40 - (
	{0b00000000, 0b10000010, 0b01000100, 0b00111000, 0b00000000, 0b00000000},	// 0x29 - 41 - )
	{0b00000000, 0b01010000, 0b00100000, 0b01010000, 0b00000000, 0b00000000},	// 0x2A - 42 - *
	{0b00010000, 0b00010000, 0b01111100, 0b00010000, 0b00010000, 0b00000000},	// 0x2B - 43 - +
	{0b00000000, 0b00000001, 0b00000010, 0b00000000, 0b00000000, 0b00000000},	// 0x2C - 44 - ,
	{0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00000000, 0b00000000},	// 0x2D - 45 - -
	{0b00000000, 0b00000000, 0b00000010, 0b00000000, 0b00000000, 0b00000000},	// 0x2E - 46 - .
	{0b00000010, 0b00001100, 0b00010000, 0b01100000, 0b10000000, 0b00000000},	// 0x2F - 47 - /
	
	//0x30 - 0x3F
	{0b01111100, 0b10001010, 0b10010010, 0b10100010, 0b01111100, 0b00000000},	// 0x30 - 48 - 0
	{0b00000010, 0b01000010, 0b11111110, 0b00000010, 0b00000010, 0b00000000},	// 0x31 - 49 - 1
	{0b01000110, 0b10001010, 0b10010010, 0b10010010, 0b01100110, 0b00000000},	// 0x32 - 50 - 2
	{0b01000100, 0b10000010, 0b10010010, 0b10010010, 0b01101100, 0b00000000},	// 0x33 - 51 - 3
	{0b00011000, 0b00101000, 0b01001000, 0b10001000, 0b11111110, 0b00000000},	// 0x34 - 52 - 4
	{0b11100100, 0b10100010, 0b10100010, 0b10100010, 0b10011100, 0b00000000},	// 0x35 - 53 - 5
	{0b00111100, 0b01010010, 0b10010010, 0b10010010, 0b00001100, 0b00000000},	// 0x36 - 54 - 6
	{0b11000000, 0b10000000, 0b10001110, 0b10010000, 0b11100000, 0b00000000},	// 0x37 - 55 - 7
	{0b01101100, 0b10010010, 0b10010010, 0b10010010, 0b01101100, 0b00000000},	// 0x38 - 56 - 8
	{0b01100000, 0b10010010, 0b10010010, 0b10010100, 0b01111000, 0b00000000},	// 0x39 - 57 - 9
	{0b00000000, 0b00000000, 0b00100100, 0b00000000, 0b00000000, 0b00000000},	// 0x3A - 58 - :
	{0b00000000, 0b00000010, 0b00100100, 0b00000000, 0b00000000, 0b00000000},	// 0x3B - 59 - ;
	{0b00010000, 0b00101000, 0b01000100, 0b10000010, 0b00000000, 0b00000000},	// 0x3C - 60 - <
	{0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00000000, 0b00000000},	// 0x3D - 61 - =
	{0b10000010, 0b01000100, 0b00101000, 0b00010000, 0b00000000, 0b00000000},	// 0x3E - 62 - >
	{0b01000000, 0b10000000, 0b10001010, 0b10010000, 0b01100000, 0b00000000},	// 0x3F - 63 - ?
	
	//0x40 - 0x4F
	{0b00111100, 0b01000010, 0b01011010, 0b01001010, 0b00111010, 0b00000000},	// 0x40 - 64 - @
	{0b01111110, 0b10010000, 0b10010000, 0b10010000, 0b01111110, 0b00000000},	// 0x41 - 65 - A
	{0b11111110, 0b10010010, 0b10010010, 0b10010010, 0b01101100, 0b00000000},	// 0x42 - 66 - B
	{0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01000100, 0b00000000},	// 0x43 - 67 - C
	{0b11111110, 0b10000010, 0b10000010, 0b10000010, 0b01111100, 0b00000000},	// 0x44 - 68 - D
	{0b11111110, 0b10010010, 0b10010010, 0b10010010, 0b10000010, 0b00000000},	// 0x45 - 69 - E
	{0b11111110, 0b10010000, 0b10010000, 0b10010000, 0b10000000, 0b00000000},	// 0x46 - 70 - F
	{0b01111100, 0b10000010, 0b10000010, 0b10100010, 0b10111100, 0b00000000},	// 0x47 - 71 - G
	{0b11111110, 0b00010000, 0b00010000, 0b00010000, 0b11111110, 0b00000000},	// 0x48 - 72 - H
	{0b00000000, 0b10000010, 0b11111110, 0b10000010, 0b00000000, 0b00000000},	// 0x49 - 73 - I
	{0b00000100, 0b00000010, 0b00000010, 0b00000010, 0b11111100, 0b00000000},	// 0x4A - 74 - J
	{0b11111110, 0b00100000, 0b00100000, 0b01010000, 0b10001110, 0b00000000},	// 0x4B - 75 - K
	{0b11111110, 0b00000010, 0b00000010, 0b00000010, 0b00000000, 0b00000000},	// 0x4C - 76 - L
	{0b11111110, 0b01000000, 0b00100000, 0b01000000, 0b11111110, 0b00000000},	// 0x4D - 77 - M
	{0b11111110, 0b01000000, 0b00100000, 0b00010000, 0b11111110, 0b00000000},	// 0x4E - 78 - N
	{0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01111100, 0b00000000},	// 0x4F - 79 - O
	
	//0x50 - 0x5F
	{0b11111110, 0b10010000, 0b10010000, 0b10010000, 0b01100000, 0b00000000},	// 0x50 - 80 - P
	{0b01111100, 0b10000010, 0b10000010, 0b10000100, 0b01111010, 0b00000000},	// 0x51 - 81 - Q
	{0b11111110, 0b10010000, 0b10010000, 0b10010000, 0b01101110, 0b00000000},	// 0x52 - 82 - R
	{0b01100100, 0b10010010, 0b10010010, 0b10010010, 0b01001100, 0b00000000},	// 0x53 - 83 - S
	{0b10000000, 0b10000000, 0b11111110, 0b10000000, 0b10000000, 0b00000000},	// 0x54 - 84 - T
	{0b11111100, 0b00000010, 0b00000010, 0b00000010, 0b11111100, 0b00000000},	// 0x55 - 85 - U
	{0b11110000, 0b00001100, 0b00000010, 0b00001100, 0b11110000, 0b00000000},	// 0x56 - 86 - V
	{0b11111110, 0b00000100, 0b00001000, 0b00000100, 0b11111110, 0b00000000},	// 0x57 - 87 - W
	{0b11000110, 0b00101000, 0b00010000, 0b00101000, 0b11000110, 0b00000000},	// 0x58 - 88 - X
	{0b10000000, 0b01000000, 0b00111110, 0b01000000, 0b10000000, 0b00000000},	// 0x59 - 89 - Y
	{0b10000110, 0b10001010, 0b10010010, 0b10100010, 0b11000010, 0b00000000},	// 0x5A - 90 - Z
	{0b00000000, 0b11111110, 0b10000010, 0b10000010, 0b00000000, 0b00000000},	// 0x5B - 91 - [
	{0b10000000, 0b01100000, 0b00010000, 0b00001100, 0b00000010, 0b00000000},	// 0x5C - 92 - \	/* backslash */
	{0b00000000, 0b10000010, 0b10000010, 0b11111110, 0b00000000, 0b00000000},	// 0x5D - 93 - ]
	{0b00100000, 0b01000000, 0b10000000, 0b01000000, 0b00100000, 0b00000000},	// 0x5E - 94 - ^
	{0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000000},	// 0x5F - 95 - _

	//0x60 - 0x6F
	{0b01000000, 0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x60 - 96 - '
	{0b00000100, 0b00101010, 0b00101010, 0b00101010, 0b00011110, 0b00000000},	// 0x61 - 97 - a
	{0b01111110, 0b00010010, 0b00100010, 0b00100010, 0b00011100, 0b00000000},	// 0x62 - 98 - b
	{0b00011100, 0b00100010, 0b00100010, 0b00100010, 0b00010100, 0b00000000},	// 0x63 - 99 - c
	{0b00011100, 0b00100010, 0b00100010, 0b00010010, 0b01111110, 0b00000000},	// 0x64 - 100 - d
	{0b00011100, 0b00101010, 0b00101010, 0b00101010, 0b00011010, 0b00000000},	// 0x65 - 101 - e
	{0b00100000, 0b01111110, 0b10100000, 0b10100000, 0b00000000, 0b00000000},	// 0x66 - 102 - f
	{0b00011001, 0b00100101, 0b00100101, 0b00100101, 0b00111110, 0b00000000},	// 0x67 - 103 - g
	{0b11111110, 0b00010000, 0b00100000, 0b00100000, 0b00011110, 0b00000000},	// 0x68 - 104 - h
	{0b00000000, 0b00100000, 0b10111100, 0b00000010, 0b00000000, 0b00000000},	// 0x69 - 105 - i
	{0b00000100, 0b00000010, 0b00000010, 0b00000010, 0b10111100, 0b00000000},	// 0x6A - 106 - j
	{0b00000000, 0b11111110, 0b00001000, 0b00010100, 0b00100010, 0b00000000},	// 0x6B - 107 - k
	{0b00000000, 0b10000000, 0b11111100, 0b00000010, 0b00000010, 0b00000000},	// 0x6C - 108 - l
	{0b00111110, 0b00100000, 0b00011000, 0b00100000, 0b00011110, 0b00000000},	// 0x6D - 109 - m
	{0b00111110, 0b00100000, 0b00100000, 0b00100000, 0b00011110, 0b00000000},	// 0x6E - 110 - n
	{0b00011100, 0b00100010, 0b00100010, 0b00100010, 0b00011100, 0b00000000},	// 0x6F - 111 - o
	
	//0x70 - 0x7F
	{0b00111111, 0b00010100, 0b00100100, 0b00100100, 0b00011000, 0b00000000},	// 0x70 - 112 - p
	{0b00011000, 0b00100100, 0b00100100, 0b00010100, 0b00111111, 0b00000000},	// 0x71 - 113 - q
	{0b00111110, 0b00010000, 0b00100000, 0b00100000, 0b00010000, 0b00000000},	// 0x72 - 114 - r
	{0b00010010, 0b00101010, 0b00101010, 0b00101010, 0b00100100, 0b00000000},	// 0x73 - 115 - s
	{0b00000000, 0b00100000, 0b11111100, 0b00100010, 0b00000000, 0b00000000},	// 0x74 - 116 - t
	{0b00111100, 0b00000010, 0b00000010, 0b00000010, 0b00111110, 0b00000000},	// 0x75 - 117 - u
	{0b00111000, 0b00000100, 0b00000010, 0b00000100, 0b00111000, 0b00000000},	// 0x76 - 118 - v
	{0b00111100, 0b00000010, 0b00001110, 0b00000010, 0b00111110, 0b00000000},	// 0x77 - 119 - w
	{0b00100010, 0b00010100, 0b00001000, 0b00010100, 0b00100010, 0b00000000},	// 0x78 - 120 - x
	{0b00111001, 0b00000101, 0b00000101, 0b00000101, 0b00111110, 0b00000000},	// 0x79 - 121 - y
	{0b00100010, 0b00100110, 0b00101010, 0b00110010, 0b00100010, 0b00000000},	// 0x7A - 122 - z
	{0b00000000, 0b00010000, 0b01101100, 0b10000010, 0b00000000, 0b00000000},	// 0x7B - 123 - {
	{0b00000000, 0b00000000, 0b11111110, 0b00000000, 0b00000000, 0b00000000},	// 0x7C - 124 - |
	{0b00000000, 0b10000010, 0b01101100, 0b00010000, 0b00000000, 0b00000000},	// 0x7D - 125 - }
	{0b00011000, 0b00100000, 0b00010000, 0b00001000, 0b00110000, 0b00000000},	// 0x7E - 126 - ~
	{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x7F - 127 - DEL
};


void ST7789_SendData(char data)
{
	DC_HIGH;
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
}


void ST7789_SendCommand(char data)
{
	DC_LOW;
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
}


void ST7789_Init(void)
{
	RESET_HIGH;
	_delay_ms(200);
	RESET_LOW;
	_delay_ms(50);
	RESET_HIGH;
	_delay_ms(200);
	
	ST7789_SendCommand(0x01);	// Software Reset
	_delay_ms(10);
	ST7789_SendCommand(0x11);	// Sleep out - проснулись
	_delay_ms(10);
	
	ST7789_SendCommand(0x3A);		// режим цвета:
	ST7789_SendData(0x06);			// 18 бит			Write data for 18-bit/pixel (RGB-6-6-6-bit input), 262K-Colors, 3Ah=”06h”
	_delay_ms(10);
	
	ST7789_SendCommand(0x36);		// (36h) Memory data access control
	ST7789_SendData(0b00000000);		// 0x20 = 0b00100000; MY0(верх/низ по Y) MX0(право/лево по X) MV1(vertical addressing mode(развертка справа налево или сверху вниз) ML0 RGB0 0 0 0
	_delay_ms(10);
	
	ST7789_SendCommand(0x2A);		// Column address set
	ST7789_SendData(0);				// XS highpart	(Старшая часть начального положения Колонки)
	ST7789_SendData(0);				// XS lowpart	(Младшая часть начального положения Колонки)
	ST7789_SendData(0);				// XE highpart	(Старшая часть конечного положения Колонки)
	ST7789_SendData(240);			// XE highpart	(Младшая часть конечного положения Колонки)
	_delay_ms(10);
	
	ST7789_SendCommand(0x2B);		// Row address set
	ST7789_SendData(0);				// YS highpart	(Старшая часть начального положения Ряда)
	ST7789_SendData(0);				// YS lowpart	(Младшая часть начального положения Ряда)
	ST7789_SendData(0);				// YE highpart	(Старшая часть конечного положения Ряда)
	ST7789_SendData(240);			// YE highpart	(Младшая часть конечного положения Ряда)
	_delay_ms(10);
	
	ST7789_SendCommand(0x21);		// Display inversion - on
	_delay_ms(10);
	
	ST7789_SendCommand(0x29);		// Display on - включаем изображение
	_delay_ms(10);
}


void ST7789_DrawPixel(char x, char y, char red, char green, char blue)
{
	if (x > 240 || y > 240 || x < 0 || y < 0)
	{
		return;
	}
	
	ST7789_SendCommand(0x2A);
	ST7789_SendData(x >> 8);
	ST7789_SendData(x);
	ST7789_SendData((x + 1) >> 8);
	ST7789_SendData( x + 1);

	ST7789_SendCommand(0x2B);
	ST7789_SendData(y >> 8);
	ST7789_SendData(y);
	ST7789_SendData((y + 1) >> 8);
	ST7789_SendData(y + 1);

	ST7789_SendCommand(0x2C);
	ST7789_SendData(red << 2);
	ST7789_SendData(green << 2);
	ST7789_SendData(blue << 2);
}


void ST7789_DrawLine(float xa, float ya, float xb, float yb, char red, char green, char blue)
{
	// Расчет по формуле y = kx + b
	// Расчет k и особый случай если (xb - xa) = 0
	float k;
	if (xb != xa)
	{
		k = (yb - ya)/(xb - xa);
	}
	else
	{
		k = 1000.0;
	}
	
	// Расчет k^(-1) и особый случай если (yb - ya) = 0
	float k_inverse;
	if (yb != ya)
	{
		k_inverse = (xb - xa)/(yb - ya);
	}
	else
	{
		k_inverse = 1000.0;
	}
	
	// Расчет b и b_inverse = b/k - xa
	float b = ya - k * xa;
	float b_inverse = k_inverse * ya - xa;
	
	
	// Определение диапазона отрисовки
	if (xb < xa)
	{
		float xtemp;
		xtemp = xa;
		xa = xb;
		xb = xtemp;
	}
	
	// Отрисовка по x
	for(unsigned char x = xa; x < xb; x++)
	{
		ST7789_DrawPixel(x, (int)(k * x + b), red, green, blue);
	}
	
	
	// Определение диапазона отрисовки
	if (yb < ya)
	{
		float ytemp;
		ytemp = ya;
		ya = yb;
		yb = ytemp;
	}
	
	// Отрисовка по y
	for(unsigned char y = ya; y < yb; y++)
	{
		ST7789_DrawPixel((int)(k_inverse * y - b_inverse), y, red, green, blue);
	}
}


void ST7789_Fill(char red, char green, char blue)
{
	for (char x = 0; x < 240; x++)
	{
		for (char y = 0; y < 240; y++)
		{
			ST7789_DrawPixel(x, y, red, green, blue);
		}
	}
}


void ST7789_ASCIPrintChar(char letter, char* Xcursor, char* Ycursor, char red, char green, char blue, char size)
{
	for(char i = 0; i < 6; i++)
	{
		char mask;
		for (int y = 0; y < 8; y++)
		{
			mask = 1;
			if(monocraft[letter - 32][i] & (mask << (7 - y)))
			{
				for (char i = 0; i < size; i++)
				{
					for (char j = 0; j < size; j++)
					{
						ST7789_DrawPixel(*Xcursor + j, (*Ycursor + (y * size)) + i, red, green, blue);
					}
				}
			}
			else
			{
				for (char i = 0; i < size; i++)
				{
					for (char j = 0; j < size; j++)
					{
						ST7789_DrawPixel((*Xcursor) + j, (*Ycursor + (y * size)) + i, 0, 0, 0);
					}
				}
			}

		}
		
		*Xcursor += size;
		
		if (*Xcursor >= 240)
		{
			*Xcursor = 0;
			*Ycursor += 8 * size;
		}
		
		if (*Ycursor >= 240)
		{
			*Xcursor = 0;
			*Ycursor = 0;
		}
		
	}	
}


void ST7789_ASCIPrintString(char string[], char* Xcursor, char* Ycursor, char red, char green, char blue, char size)
{
	char counter = 0;
	
	while (string[counter] != '\0')
	{
		ST7789_ASCIPrintChar(string[counter], Xcursor, Ycursor, red, green, blue, size);
		counter++;
	}
}
