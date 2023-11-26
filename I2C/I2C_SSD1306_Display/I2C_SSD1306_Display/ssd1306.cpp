#include "main.h"
using namespace std;

class ssd1306_Display
{
	public:
	unsigned char adr;
	signed char Xshift;
	signed char Yshift;
	const int BufferSize = 1024;
	unsigned char *Buffer;
			
	ssd1306_Display(unsigned char adr, signed char Xshift, signed char Yshift)
	{
		this->adr = adr;
		this->Xshift = Xshift;
		this->Yshift = Yshift;
		for (int i = 0; i < 1024; i++)
		{
			this -> Buffer[i] = 0x00;
		}
		
		this -> Buffer[0] = 0xff;
		this -> Buffer[1] = 0x00000001;
		this -> Buffer[2] = 0x00000001;
	
		this -> Buffer[509] = 0b10000000;
		this -> Buffer[510] = 0b10000000;
		this -> Buffer[511] = 0b10101010;
	}
	
	~ssd1306_Display()
	{
		delete[] Buffer;
	}	
	
	void Init(void)
	{
	// Адрес дисплея запись			0b01111000
		_delay_ms(10);

		i2c_MT_init();
		i2c_MT_start();					// Задание стартового условия мастером	// 00001 = 0x08 - A START condition has been transmitted
		i2c_MT_send(adr);		// Передача адреса						// 00011 = 0x18 - SLA+W has been transmitted;		ACK has been received
	
		i2c_MT_send(0b10000000);		// Задаем команды 0b10					// (7(Co bit)->0 - дальнейшие биты это только DATA,		6(D/C# bit)->0, дальнейший бит это команда; ->1 дальнейший бит это DATA)
		i2c_MT_send(0xAE);				// SLEEP MODE
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xD5);				// 0xD5 = 0b11010101 - Установка делителя частоты
		i2c_MT_send(0x80);				// Параметр
	
		//////////////////
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xA8);				// 0xA8 = 0b10101000 - MUX - Устанавливает отношение мультиплексирования (0x3F for 128x64, 0x1F for 128x32)
		i2c_MT_send(0x3f);				// Параметр (0x3F for 128x64, 0x1F for 128x32)
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xC0);				// устанавливаем направление сканирования строк C8 - remapped mode. Scan from COM[N-1] to COM0	
		
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xD3);				// 0xD3 = 0b11010011 - Устанавливаем смещение дисплея
		i2c_MT_send(0x00);				// Параметр
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0x40);				// 0x40 = 0b01000000? - устанавливаем начальную линию | 0x00?
	
	////////////////////////////////	
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xDA);				// Устанавливаем конфигурацию выводов COM-сигналов в соответствии с компоновкой оборудования OLED - панели (0x12 for 128x64, 0x02 for 128x32)
		i2c_MT_send(0x02);				// Параметр (0x12 for 128x64, 0x02 for 128x32)	
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0x8d);				// разрешаем charge pump
		i2c_MT_send(0x14);				// Параметр
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0x20);						// устанавливаем горизонтальный режим памяти
		i2c_MT_send(0b00000000);				// Параметр 00-Horizontal Addressing Mode; 10-Page Addressing Mode
		
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xA0);				// устанавливаем remapping столбцов seg0??seg127 | 0x00?
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0x81);				// Устанавливаем контраст (т.е. яркость)
		i2c_MT_send(0xCF);				// Параметр
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xD9);				// Установка предзаряда
		i2c_MT_send(0xF1);				// Параметр
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xDB);				// Установка Vcomp deselect level
		i2c_MT_send(0x40);				// Параметр
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xA4);				// Возобновление отображения содержимого оперативной памяти
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xA6);				// Отображение в нормальном виде
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xAF);				// Включение дисплея в нормальном режиме
	
		i2c_MR_stop();
	
	
		_delay_ms(10);
	
	
		i2c_MT_init();
		i2c_MT_start();					// Задание стартового условия мастером	// 00001 = 0x08 - A START condition has been transmitted
		i2c_MT_send(adr);		// Передача адреса						// 00011 = 0x18 - SLA+W has been transmitted;		ACK has been received
		i2c_MT_send(0b01000000);		// 10 - ND/Comand(setp)	// 11 - ND/DATA(nerabotaet)		// 00 - DataOnly/Comand		// 01 - DataOnly/DATA(steam)	
		i2c_MR_stop();
	}
	
	void Buffer_Fill(char byte)
	{
		for (int i = 0; i < BufferSize; i++)
		{
			Buffer[i] = byte;
		}
	}
	
	void Buffer_Send(void)
	{
		i2c_MT_init();
		i2c_MT_start();					// Задание стартового условия мастером	// 00001 = 0x08 - A START condition has been transmitted
		i2c_MT_send(0b01111000);		// Передача адреса						// 00011 = 0x18 - SLA+W has been transmitted;		ACK has been received
		i2c_MT_send(0b01000000);		// 10 - ND/Comand(setp)	// 11 - ND/DATA(nerabotaet)		// 00 - DataOnly/Comand		// 01 - DataOnly/DATA(steam)
	
		for (int i = 0; i < 1024; i++)
		{
			i2c_MT_send(Buffer[i]);
		}
		i2c_MT_stop();		
	}
	
	void Buffer_SetPixel(int x, int y)
	{
		if (y >= 0 && y <= 16)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift)] |= (1 << (8 - (y % 8)));	//+16 для смещения Y // +64 для смещения X
		}
		else if (y >= -16 && y < 0)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift)] |= (1 << ((-y) % 8));		//+16 для смещения Y // +64 для смещения X
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Особый случай для 8 бита
		if (y % 8 == 0 && y / 8 == 1)
		{
			Buffer[(int)((((-y + 16) / 8) * 128) + x + 64)] |= (1 << 0);
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Особый случай для оси OX		
		if (y == 0)
		{
			Buffer[(int)((((-y + 16) / 8) * 128) + x + 64)] |= (1 << 0);
		}
	}
	
	void Buffer_SetLine(float xa, float ya, float xb, float yb)
	{	
		//////////////////////////////////////////////////////////////////////////
		// Особый случай если линия вертикальная
		if (xa == xb)
		{
			if (yb <= ya)
			{
				int ytemp = ya;
				ya = yb;
				yb = ytemp;
			}
			
			int x = xa;
			
			for (int y = ya; y < yb; y++)
			{			
				Buffer_SetPixel(x, y);
			}
			
			return;
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Особый случай если xa > xb
		if (xb < xa)
		{
			float xtemp;
			float ytemp;
			
			xtemp = xa;
			xa = xb;
			xb = xtemp;
			
			ytemp = ya;
			ya = yb;
			yb = ytemp;			
		}		
			
		for(float x = xa; x <= xb; x += 0.5)
		{			
			float temp = (x - xa)*(yb - ya) + ya*(xb - xa);
			int y = temp / (xb - xa);
			Buffer_SetPixel(x, y);
		}
	}
	
	void Buffer_SetSquare(float xa, float ya, float xb, float yb)
	{
		//////////////////////////////////////////////////////////////////////////
		// Особый случай если xa > xb
		if (xa < xb)
		{
			float temp = xa;
			xa = xb;
			xb = temp;
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Особый случай если ya > yb		
		if (ya < yb)
		{
			float temp = ya;
			ya = yb;
			yb = temp;
		}
			
		Buffer_SetLine(xa, ya, xb, ya);
		Buffer_SetLine(xa, yb, xb, yb);
		Buffer_SetLine(xa, ya, xa, yb);
		Buffer_SetLine(xb, ya, xb, yb);
	}
};