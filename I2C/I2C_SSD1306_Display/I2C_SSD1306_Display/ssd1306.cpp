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
	
		//// Выключить дисплей
		i2c_MT_send(0x80);						// Передача кода команды (0b10000000 = 0x80) озачающего что далее выполняется передача команд 		// (7(Co bit)->0 - дальнейшие биты это только DATA,		6(D/C# bit)->0, дальнейший бит это команда; ->1 дальнейший бит это DATA)
		i2c_MT_send(0xAE);						// Код - 0xAE Выключение экрана (!0xAF)
	
		//// Частота дисплея
		i2c_MT_send(0x80);
		i2c_MT_send(0xD5);						// Код - 0xD5 - Установка делителя частоты
		i2c_MT_send(0x80);						// Параметр 0x80 - по умолчанию
	
		i2c_MT_send(0x80);
		i2c_MT_send(0xA8);						// Код - 0xA8 - MUX - Устанавливает отношение мультиплексирования
		i2c_MT_send(0x3F);						// Параметр 0x3F - по умолчанию
	
	
		//// Смещение дисплея хз
		i2c_MT_send(0b10000000);
		i2c_MT_send(0xD3);				// 0xD3 = 0b11010011 - Устанавливаем смещение дисплея
		i2c_MT_send(0x00);				// Параметр 0x00

		i2c_MT_send(0x80);
		i2c_MT_send(0x40);				// Код - 0x40 Начало строки разверстки. Т.е. устанавливаем начальную линию. По сути двигает изображение вверх и вниз
	
		
		//// Конфигурация портов (ВАЖНО Т.К. ОПРЕДЕЛЯЕТСЯ РАЗРЕШЕНИЕ ЭКРАНА)
		i2c_MT_send(0x80);
		i2c_MT_send(0xDA);				// Устанавливаем конфигурацию выводов COM-сигналов в соответствии с компоновкой оборудования OLED - панели (0x12 for 128x64, 0x02 for 128x32)
		//i2c_MT_send(0x02);			// Параметр (0x12 for 128x64, 0x02 = 0b00000010 for 128x32)	ВЕРНО!
		i2c_MT_send(0x12);
	
	
		//// Установка адресации и каретки
		i2c_MT_send(0x80);
		i2c_MT_send(0x20);				// Код - 0x20 - Режим автоматической адресации
		i2c_MT_send(0x00);				// 0-по горизонтали с переходом на новую страницу (строку)
										// 1 - по вертикали с переходом на новую строку
										// 2 - только по выбранной странице без перехода
		i2c_MT_send(0x80);
		i2c_MT_send(0x21);				// Код(3) - 0x21 - установка стартовой и конечной колонки, для режима горизонтальной адресации
		i2c_MT_send(0);					// Старовая колонка 0 - 127
		i2c_MT_send(127);				// Конечная колонка 0 - 127
		
		i2c_MT_send(0x80);
		i2c_MT_send(0x22);				// Код(3) - 0x22 установка стартовой и конечной страницы, для режима горизонтальной адресации
		i2c_MT_send(0);					// Старовая страница 0 - 7
		i2c_MT_send(7);					// Конечная страница 0 - 7		
		
	
		//// Симметрия OX по OY		
		i2c_MT_send(0x80);				
		i2c_MT_send(0xA1);				// Режим разверки по странице (по X)	(Симметрия относительно OY)
										// A1 - нормальный режим (слева/направо) A0 - обратный (справа/налево)
		i2c_MT_send(0X80);
		i2c_MT_send(0xC0);				// Режим сканирования озу дисплея		(Симметрия относительно OX)
										// для изменения системы координат
										// С0 - снизу/верх (начало нижний левый угол)
										// С8 - сверху/вниз (начало верний левый угол)	
		
		//// Яркость
		i2c_MT_send(0x80);
		i2c_MT_send(0x81);				// Код - 0x81 Установка контрастности 
		i2c_MT_send(255);				// Параметр контрастности 0 - 255. Чем больше тем ярче
	
		i2c_MT_send(0x80);
		i2c_MT_send(0xDB);				// Код - Установка уровня VcomH
		i2c_MT_send(0x40);				// Влияет на яркость дисплея 0x00..0x70
	
	
		//// Что-то техническое про питание
		i2c_MT_send(0x80);
		i2c_MT_send(0xD9);				// Код 0xD9 - Настройка фаз DC/DC преоразователя
		i2c_MT_send(0xF1);				// 0x22 - VCC подается извне / 0xF1 для внутренего
		
		i2c_MT_send(0b10000000);
		i2c_MT_send(0x8D);				// Управление внутреним преобразователем
		i2c_MT_send(0x14);				// 0x10 - отключить (VCC подается извне) 0x14 - запустить внутрений DC/DC
	

		//// RAM; отображение и инверсия
		i2c_MT_send(0x80);
		i2c_MT_send(0xA4);				// Код 0xA4 - Возобновление отображения содержимого RAM
	
		i2c_MT_send(0x80);
		i2c_MT_send(0xA6);				// Код 0xA6 - Отображение в нормальном виде, без инверсии (!0xA7)
	
	
		//// Включение
		i2c_MT_send(0x80);
		i2c_MT_send(0xAF);				// Код 0xAF - Включение дисплея в нормальном режиме (!0xAE)
	
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
		if (x >= (127 - Xshift) || x <= -(127 - Xshift) || y >= (63 - Yshift) || y <= -(63 - Yshift)){return;}
		// Функция
		Buffer[(int)((((-y - Yshift) / 8) * 128) + x + Xshift) + (int)((y + (float)Yshift) / 8) * 256] |= (1 << ((y + Yshift) % 8));		
	}
	
	void Buffer_RemovePixel(int x, int y)
	{
		// Граничные условия экрана
		if (x > (127 - Xshift) || x < -(127 - Xshift) || y > (63 - Yshift) || y < -(63 - Yshift)){return;}
		// Функция
		Buffer[(int)((((-y - Yshift) / 8) * 128) + x + Xshift) + (int)((y + (float)Yshift) / 8) * 256] &= ~(1 << ((y + Yshift) % 8));
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
			k = 10000.0;
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
		for(signed char x = xa; x <= xb; x++)
		{
			Buffer_SetPixel(x, (int)((float)k * x + b));
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
		for(signed char y = ya; y <= yb; y++)
		{
			Buffer_SetPixel((int)((float)k_inverse * y - b_inverse), y);
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
};