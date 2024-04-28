﻿using namespace std;

class ssd1306_Display
{
	
private:
	// Набор команд для настройки работы экрана
	const char ssd1306config[49] = {
		
		// Выключить дисплей
		0x80, 0xAE,			// Код - 0xAE - Выключение экрана (!0xAF)
		
		// Частота дисплея
		0x80, 0xD5, 0x80,	// Код - 0xD5 - Установка делителя частоты							Параметр 0x80 - по умолчанию
		0x80, 0xA8, 0x3F,	// Код - 0xA8 - Устанавливает отношение мультиплексирования (MUX)	Параметр 0x3F - по умолчанию
		
		// Смещение дисплея
		0x80, 0xD3, 0x00,	// Код - 0xD3 - Устанавливаем смещение дисплея						Параметр 0x00 - по умолчанию
		0x80, 0x40,			// Код - 0x40 - Начало строки разверстки. Т.е. устанавливаем начальную линию. По сути двигает изображение вверх и вниз
		
		// Конфигурация портов (ВАЖНО Т.К. ОПРЕДЕЛЯЕТСЯ РАЗРЕШЕНИЕ ЭКРАНА)
		0x80, 0xDA, 0x12,	// Код - 0xDA - Устанавливаем конфигурацию выводов COM-сигналов в соответствии с компоновкой оборудования OLED - панели					Параметр (0x12 for 128x64, 0x02 = 0b00000010 for 128x32)
		0x80, 0x20, 0x00,	// Код - 0x20 - Режим автоматической адресации						Параметр 0-по горизонтали с переходом на новую страницу (строку); 1 - по вертикали с переходом на новую строку; 2 - только по выбранной странице без перехода
		
		// Установка адресации и каретки
		0x80, 0x21, 0, 127,	// Код - 0x21 - Установка стартовой и конечной колонки, для режима горизонтальной адресации		Параметр - Старовая колонка 0 - 127		Параметр - Конечная колонка 0 - 127
		0x80, 0x22, 0, 7,	// Код - 0x22 - Установка стартовой и конечной страницы, для режима горизонтальной адресации	Параметр - Старовая страница 0 - 7		Параметр - Конечная страница 0 - 7
		
		// Симметрия OX по OY
		0x80, 0xA1,			// Код - 0xXX - Режим разверки по странице (по X)	(Симметрия относительно OY)										Код - A1 - нормальный режим (слева/направо)			Код - A0 - обратный (справа/налево)
		0x80, 0xC0,			// Код - 0xXX - Режим сканирования озу дисплея		(Симметрия относительно OX) для изменения системы координат		Код - С0 - снизу/верх (начало нижний левый угол)	Код - С8 - сверху/вниз (начало верний левый угол)
		
		// Яркость
		0x80, 0x81, 255,	// Код - 0x81 - Установка контрастности								Параметр - 0 - 255 Чем больше тем ярче
		0x80, 0xDB, 0x40,	// Код - 0xDB - Установка уровня VcomH								Параметр - 0x00..0x70 Влияет на яркость дисплея
		
		// Что-то техническое про питание
		0x80, 0xD9, 0xF1,	// Код - 0xD9 - Настройка фаз DC/DC преоразователя					Параметр - 0x22 - VCC подается извне / 0xF1 для внутренего
		0x80, 0x8D, 0x14,	// Код - 0x8D - Управление внутреним преобразователем				Параметр - 0x10 - отключить (VCC подается извне) / 0x14 - запустить внутрений DC/DC
		
		// RAM; отображение и инверсия
		0x80, 0xA4,			// Код - 0xA4 - Возобновление отображения содержимого RAM
		0x80, 0xA6,			// Код - 0xA6 - Отображение в нормальном виде, без инверсии (!0xA7)
		
		// Включение
		0x80, 0xAF			// Код - 0xAF - Включение дисплея в нормальном режиме (!0xAE)
	};
	
	// Шрифт Monocraft
	const char letter[128][6] =
	{
		//0x00 - 0x0F
		{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
		
		//0x10 - 0x1F
		{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
		
		//0x20 - 0x2F
		{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x20 - 32 - * Пробел *
		{0b00000000, 0b11111010, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x21 - 33 - !
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{0b00000000, 0b00000001, 0b00000010, 0b00000000, 0b00000000, 0b00000000},	// 0x2C - 44 - , *Запятая*
		{0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00000000, 0b00000000},	// 0x2D - 45 - - *Тире*
		{0b00000000, 0b00000000, 0b00000010, 0b00000000, 0b00000000, 0b00000000},	// 0x2E - 46 - . *Точка*
		{},
		
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
		{0b00000000, 0b00100100, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x3A - 58 - :
		{0b00000010, 0b00100100, 0b00000000, 0b00000000, 0b00000000, 0b00000000},	// 0x3B - 59 - ;
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
		{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
		
		//0x70 - 0x7F
		{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
	};
		
		
private:
	unsigned char adr;								// I2C адрес экранчика
	signed char Xshift;								// Сдвиг точки начала координат по X
	signed char Yshift;								// Сдвиг точки начала координат по Y
	
public:	
	unsigned char Buffer[1024];						// Буффер памяти
	signed char Xcursor = 0;						// Каретка консоли по X
	signed char Ycursor = 0;						// Каретка консоли по Y (строка)

public:		
	ssd1306_Display(unsigned char adr, signed char Xshift, signed char Yshift)
	{
		this->adr = adr;
		this->Xshift = Xshift;
		this->Yshift = Yshift;
		this -> Buffer[1024] = {0};	
	}
		
	void Init(void)
	{
		_delay_ms(1);
		i2c_MT_init();					// Инициализация протокола I2C
		i2c_MT_start();					// Задание стартового условия мастером			// Код - 00001 = 0x08 - A START condition has been transmitted
		i2c_MT_send(adr);				// Вызов ведомого								// Код - 00011 = 0x18 - SLA+W has been transmitted;		ACK has been received
		
		for (int i = 0; i < 49; i++)
		{
			i2c_MT_send(ssd1306config[i]);
		}		
		
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
		// Граничные условия экрана
		if (x > (128 - Xshift) || x < -(128 - Xshift) || y > (64 - Yshift) || y < -(64 - Yshift)){return;}
		// Функция
		Buffer[( -(((y + Yshift) >> 3) << 7) + x + Xshift) + (((y + Yshift) >> 3) << 8)] |= (1 << (((y + Yshift) % 8)));
		
		// Старая // Buffer[(int)((((-y - Yshift) / 8) * 128) + x + Xshift) + (int)((y + (float)Yshift) / 8) * 256] |= (1 << ((y + Yshift) % 8));		
	}
	
	void Buffer_RemovePixel(int x, int y)
	{
		// Граничные условия экрана
		if (x >= (128 - Xshift) || x <= -(128 - Xshift) || y >= (64 - Yshift) || y <= -(64 - Yshift)){return;}
		// Функция
		Buffer[( -(((y + Yshift) >> 3) << 7) + x + Xshift) + (((y + Yshift) >> 3) << 8)] &= ~(1 << (((y + Yshift) % 8)));
		// Старая // Buffer[(int)((((-y - Yshift) / 8) * 128) + x + Xshift) + (int)((y + (float)Yshift) / 8) * 256] &= ~(1 << ((y + Yshift) % 8));
	}	
		
	void Buffer_SetLine(float xa, float ya, float xb, float yb)
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
			k = 100000.0;
		}
		
		// Расчет k^(-1) и особый случай если (yb - ya) = 0
		float k_inverse;
		if (yb != ya)
		{
			k_inverse = (xb - xa)/(yb - ya);
		}
		else
		{
			k_inverse = 10000.0;
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
			Buffer_SetPixel(x, (int)(k * x + b));
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
			Buffer_SetPixel((int)(k_inverse * y - b_inverse), y);
		}
	}
	
	void Buffer_SetTriangle(float xa, float ya, float xb, float yb, float xc, float yc)
	{
		float temparray[3][4] = 
		{
			{xa, ya, xb, yb},
			{xb, yb, xc, yc},
			{xc, yc, xa, ya}
		};

		for (int i = 0; i < 3; i++)
		{
			Buffer_SetLine(temparray[i][0], temparray[i][1], temparray[i][2], temparray[i][3]);
		}
	}
		
	void PrintChar(char ascii)
	{							
		for (int i = 0; i < 6; i++)
		{
			Buffer[i + 6 * Xcursor + 128 * (7 - Ycursor)] = letter[(int)ascii][i];
		}
	}
	
	void PrintChar(int x, int y, char ascii)
	{
		for (int i = 0; i < 6; i++)
		{
			Buffer[i + 6 * x + 128 * (7 - y)] = letter[(int)ascii][i];
		}
	}	
	
	
	void PrintStr(char word[])
	{
		int counter = 0;
		
		while(word[counter] != '\0')
		{	
			if (word[counter] == '\n')			// Особый случай если только \n
			{
				Xcursor = -1;					// Сборс Xpos
				Ycursor++;						// Переход на строку вниз
				if (Ycursor >= 8)
				{
					Ycursor = 0;
				}
			}
				
			PrintChar(Xcursor, Ycursor, word[counter]);
			counter++;
			Xcursor++;
			
			if (Xcursor >= 21)
			{
				Xcursor = 0;
				Ycursor++;
			}
			
			if (Ycursor >= 8)
			{
				Xcursor = 0;
				Ycursor = 0;
			}
		}
	}
	
	/*
	template<typename T> T PrintNum(T num)
	{
		// Определение длинны числа
		T tempnum = num;
		int lentgh = 1;
		
		while(tempnum > 0)
		{
			tempnum /= 10;
			lentgh++;
		}
		
		// Создание массива
		int counter = 0;
		char temp[lentgh] = {};
		
		if (num == 0)
		{
			temp[0] = {'0'};
			temp[1] = {'\0'};
			PrintStr(temp);
			return 0;
		}
		
		if(num > 0)
		{
			tempnum = num;
			
			while(tempnum > 0)
			{
				temp[(lentgh - 2) - counter] = ((tempnum % 10) + 48);
				tempnum /= 10;
				counter++;
			}
			temp[lentgh - 1] = {'\0'};
		}
		
		if(num < 0)
		{
			tempnum = -num;
			
			temp[lentgh - 2] = {'A'};
			temp[lentgh - 1] = {'\0'};
		}

		
		PrintStr(temp);
		return 0;
	}
	*/


	template<typename T> T PrintNum(T num)
	{
		// Определение длинны числа
		T tempnum = num;
		int lentgh = 1;
		
		if (num < 0)
		{
			tempnum = -num;
		}
		
		while(tempnum > 0)
		{
			tempnum /= 10;
			lentgh++;
		}
		
		
		// Массив
		int counter = 0;
		char temp[lentgh + 1] = {};
		
		if(num > 0)
		{
			while(num > 0)
			{
				temp[(lentgh - 2) - counter] = ((num % 10) + 48);
				num /= 10;
				counter++;
			}
			temp[lentgh - 1] = {'\0'};
		}
		else if (num < 0)
		{
			num = -num;
			while(num > 0)
			{
				temp[(lentgh - 1) - counter] = ((num % 10) + 48);
				num /= 10;
				counter++;
			}
			temp[0] = {'-'};
			temp[lentgh - 0] = {'\0'};
		}
		else
		{
			temp[0] = {'0'};
			temp[1] = {'\0'};
			PrintStr(temp);
			return 0;
		}
		
		PrintStr(temp);
		return 0;
	}

	
	void PrintInt(int num)
	{		
		// Длинна
		int tempnum = num;
		int lentgh = 1;
		
		if (num < 0)
		{
			tempnum = -num;
		}
		
		while(tempnum > 0)
		{
			tempnum /= 10;
			lentgh++;
		}
		
		// Массив
		int counter = 0;
		char temp[lentgh + 1] = {};
		
		if(num > 0)
		{
			while(num > 0)
			{
				temp[(lentgh - 2) - counter] = ((num % 10) + 48);
				num /= 10;
				counter++;
			}
			temp[lentgh - 1] = {'\0'};
		}
		else if (num < 0)
		{
			num = -num;
			while(num > 0)
			{
				temp[(lentgh - 1) - counter] = ((num % 10) + 48);
				num /= 10;
				counter++;
			}
			temp[0] = {'-'};
			temp[lentgh - 0] = {'\0'};
		}
		else
		{
			temp[0] = {'0'};
			temp[1] = {'\0'};
			PrintStr(temp);
			return;
		}
		
		PrintStr(temp);
	}
};