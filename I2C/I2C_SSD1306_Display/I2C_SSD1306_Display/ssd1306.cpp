#include "main.h"
using namespace std;

class ssd1306_Display
{
	public:
	unsigned char adr;									// I2C адрес экранчика
	signed char Xshift;									// Сдвиг точки начала координат по X
	signed char Yshift;									// Сдвиг точки начала координат по Y
	unsigned char Buffer[1024];							// Буффер памяти
	
	
	public:		
	ssd1306_Display(unsigned char adr, signed char Xshift, signed char Yshift)
	{
		this->adr = adr;
		this->Xshift = Xshift;
		this->Yshift = Yshift;
		for (int i = 0; i < 1024; i++)
		{
			this -> Buffer[i] = 0x00;
		}
		
		/*
		this -> Buffer[0] = 0xff;
		this -> Buffer[1] = 0x00000001;
		this -> Buffer[2] = 0x00000001;
	
		this -> Buffer[509] = 0b10000000;
		this -> Buffer[510] = 0b10000000;
		this -> Buffer[511] = 0b10101010;
		*/
		
	}
	
	~ssd1306_Display()
	{
	}	
		
	void Init(void)
	{
		_delay_ms(1);
		i2c_MT_init();					// Инициализация протокола I2C
		i2c_MT_start();					// Задание стартового условия мастером			// Код - 00001 = 0x08 - A START condition has been transmitted
		i2c_MT_send(adr);				// Вызов ведомого								// Код - 00011 = 0x18 - SLA+W has been transmitted;		ACK has been received
	
		i2c_MT_send(0x80);						// Передача кода команды (0b10000000 = 0x80) озачающего что далее выполняется передача команд 		// (7(Co bit)->0 - дальнейшие биты это только DATA,		6(D/C# bit)->0, дальнейший бит это команда; ->1 дальнейший бит это DATA)
		i2c_MT_send(0xAE);						// Код - 0xAE Выключение экрана (!0xAF)
	
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
		i2c_MT_send(0x00);				// Параметр 0x00
		

		i2c_MT_send(0x80);
		i2c_MT_send(0b01000000);				// Коды - 0x40 = 0b01000000 ~ 0x7F = 0b01111111 - Set Display Start Line устанавливаем начальную линию. По сути двигает изображение вверх и вниз
	
	////////////////////////////////	
	
		i2c_MT_send(0x80);
		i2c_MT_send(0xDA);						// Устанавливаем конфигурацию выводов COM-сигналов в соответствии с компоновкой оборудования OLED - панели (0x12 for 128x64, 0x02 for 128x32)
		i2c_MT_send(0x02);				// Параметр (0x12 for 128x64, 0x02 = 0b00000010 for 128x32)	ХЗ на самом деле
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0x8d);				// разрешаем charge pump. не знаю зачем, возможно это что-то hardware
		i2c_MT_send(0x14);				// Параметр 0x14
	
		i2c_MT_send(0x80);
		i2c_MT_send(0x20);						// Код 0x20 - Устанавливка режима адресации в памяти
		i2c_MT_send(0b00000000);				// Параметр 00-Horizontal Addressing Mode; 10-Page Addressing Mode
		
		// Установка каретки
		i2c_MT_send(0x80);
		i2c_MT_send(0x21);						// Три команды Код - 0x21 установка стартовой и конечной колонки, для режима горизонтальной адресации
		i2c_MT_send(0);							// Старовая колонка 0 - 127
		i2c_MT_send(127);						// Конечная колонка 0 - 127
		
		i2c_MT_send(0x80);
		i2c_MT_send(0x22);						// Три команды Код - 0x22 установка стартовой и конечной страницы, для режима горизонтальной адресации
		i2c_MT_send(0);							// Старовая страница 0 - 7
		i2c_MT_send(7);							// Конечная страница 0 - 7		
		
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xA0);				// устанавливаем remapping столбцов seg0??seg127 | 0x00?
	
		i2c_MT_send(0x80);
		i2c_MT_send(0x81);						// Двойная команда Код - 0x81 Установка контрастности 
		i2c_MT_send(255);						// Параметр контрастности 0 - 255. Чем больше тем ярче
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xD9);				// Установка предзаряда
		i2c_MT_send(0xF1);				// Параметр
	
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xDB);				// Установка Vcomp deselect level
		i2c_MT_send(0x40);				// Параметр

	
		i2c_MT_send(0x80);
		i2c_MT_send(0xA4);						// Код 0xA4 - Возобновление отображения содержимого RAM
	
		i2c_MT_send(0x80);
		i2c_MT_send(0xA6);						// Код 0xA6 - Отображение в нормальном виде, без инверсии (!0xA7)
	
		i2c_MT_send(0x80);
		i2c_MT_send(0xAF);						// Код 0xAF - Включение дисплея в нормальном режиме (!0xAE)
	
		i2c_MT_stop();					// Завершение передачи
	
		_delay_ms(1);
	
		i2c_MT_init();
		i2c_MT_start();					// Задание стартового условия мастером			// 00001 = 0x08 - A START condition has been transmitted
		i2c_MT_send(adr);				// Передача адреса								// 00011 = 0x18 - SLA+W has been transmitted;		ACK has been received
		i2c_MT_send(0b01000000);		// 10 - ND/Comand(setp)	// 11 - ND/DATA(nerabotaet)		// 00 - DataOnly/Comand		// 01 - DataOnly/DATA(steam)	
		i2c_MT_stop();
	}
		
	void Buffer_Fill(char byte)
	{
		for (int i = 0; i < 1024; i++)
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
		if ((y - (float)Yshift) < 8) 
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift)	    ] |= (1 << ((y - Yshift) % 8));
		}
		else if ((y - (float)Yshift) < 16)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift) + 256] |= (1 << ((y - Yshift) % 8));
		}
		else if ((y - (float)Yshift) < 24)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift) + 512] |= (1 << ((y - Yshift) % 8));
		}
		else if ((y - (float)Yshift) < 32)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift) + 768] |= (1 << ((y - Yshift) % 8));
		}		
	}
	
	void Buffer_RemovePixel(int x, int y)
	{
		if ((y - (float)Yshift) < 8)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift)	    ] &= ~(1 << ((y - Yshift) % 8));
		}
		else if ((y - (float)Yshift) < 16)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift) + 256] &= ~(1 << ((y - Yshift) % 8));
		}
		else if ((y - (float)Yshift) < 24)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift) + 512] &= ~(1 << ((y - Yshift) % 8));
		}
		else if ((y - (float)Yshift) < 32)
		{
			Buffer[(int)((((-y + Yshift) / 8) * 128) + x + Xshift) + 768] &= ~(1 << ((y - Yshift) % 8));
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
		// Особый случай если xa > xb - тогда надо переставить их местами
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
		
		// Собственно функция
		for(float x = xa; x <= xb; x += 0.5)
		{
			float temp = ((x - 1) - xa)*(yb - ya) + ya*(xb - xa);		// Без temp почему-то не работает. Не получается сделать одну функцию, пришлось разбить на 2
			int y = temp / (xb - xa) + 0.9;								// -1 и +0.9 - Эмперические поправки, с ними картинка болеее симетричная относительно центра
			Buffer_SetPixel(x, y);
		}
	}
};