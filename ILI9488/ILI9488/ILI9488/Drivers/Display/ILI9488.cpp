#include "ILI9488.h"

uint16_t Xcursor = 0;
uint16_t Ycursor = 0;
uint16_t font_size = FONT;
uint16_t font_color = FONT_COLOR;
uint16_t font_background = FONT_BACKGROUND;

void ILI9488_Setup(void)
{
	// Настройка порта данных
	// Все пины как выходы с лог нулём
	LCD_DATA_DDR	|=   (1 << LCD_DATA_DB0) | (1 << LCD_DATA_DB1) | (1 << LCD_DATA_DB2) | (1 << LCD_DATA_DB3) | (1 << LCD_DATA_DB4) | (1 << LCD_DATA_DB5) | (1 << LCD_DATA_DB6) | (1 << LCD_DATA_DB7);
	LCD_DATA_PORT	&= ~((1 << LCD_DATA_DB0) | (1 << LCD_DATA_DB1) | (1 << LCD_DATA_DB2) | (1 << LCD_DATA_DB3) | (1 << LCD_DATA_DB4) | (1 << LCD_DATA_DB5) | (1 << LCD_DATA_DB6) | (1 << LCD_DATA_DB7));
	
	// Настройка порта команд
	// Все пины как выходы с лог нулём
	// CS(Чип селект), RS(Данные/команда), WR(Строб), LCD_RST(Ресет) как выходы с ог нулём
	LCD_CMD_DDR	    |=   (1 << LCD_CMD_CS) | (1 << LCD_CMD_RS) | (1 << LCD_CMD_WR) | (1 << LCD_CMD_RST);
	LCD_CMD_PORT	&= ~((1 << LCD_CMD_CS) | (1 << LCD_CMD_RS) | (1 << LCD_CMD_WR) | (1 << LCD_CMD_RST));	
}



void ILI9488_Init(void)
{
	// RESET экрана
	ILI9488_RESET();

	// Начальная инициализация
	LCD_CMD_PORT &= ~(1 << LCD_CMD_CS);		// Активируем чип (CS = LOW)

	ILI9488_WriteCommand(0x01);				// Software Reset
	_delay_ms(10);

	ILI9488_WriteCommand(0x11);				// Exit Sleep
	_delay_ms(10);
	
	ILI9488_WriteCommand(0x3A);				// Команда 0x3A (COLMOD — Pixel Format Set) устанавливает, в каком формате дисплей ожидает данные о цвете (*подробнее внизу файла)
	ILI9488_WriteData(0x55);				// 16-bit color RGB565
	_delay_ms(10);

	ILI9488_WriteCommand(0x36);				// Memory Access Control
	ILI9488_WriteData(0b00101000);			// Настройка ориентации (*подробности см внизу файла)
	_delay_ms(10);
	
	ILI9488_WriteCommand(0x29);				// Display ON
	_delay_ms(10);

	LCD_CMD_PORT |= (1 << LCD_CMD_CS);		// Деактивируем чип (CS = HIGH)
}



void ILI9488_RESET(void)
{
	LCD_CMD_PORT  &= ~(1 << LCD_CMD_RST);			// RESET = 0
	_delay_us(100);
	LCD_CMD_PORT  |= (1 << LCD_CMD_RST);			// RESET = 1
	_delay_us(900);
}



void ILI9488_WriteCommand(uint8_t cmd) 
{
	// 1/3 Подготовка строба
	LCD_CMD_PORT |=  (1 << LCD_CMD_WR);				// Строб	WR высокий (фронт)
	
	// 2/3 Пока готовится стоб - загрузка данных
	LCD_CMD_PORT &= ~(1 << LCD_CMD_RS);				// RS = 0 (команда)
	LCD_DATA_PORT =  cmd;							// Зарузка данных в порт
	
	// 3/3 Строб
	LCD_CMD_PORT &= ~(1 << LCD_CMD_WR);				// Строб	WR низкий (спад фронта)
	LCD_CMD_PORT |=  (1 << LCD_CMD_WR);
}



void ILI9488_WriteData(uint8_t data) 
{	
	// 1/3 Подготовка строба
	LCD_CMD_PORT |=  (1 << LCD_CMD_WR);				// Строб	WR высокий (фронт)
	
	// 2/3 Пока готовится стоб - загрузка данных
	LCD_CMD_PORT |=  (1 << LCD_CMD_RS);				// RS = 1 (данные)
	LCD_DATA_PORT =  data;							// Зарузка данных в порт
	
	// 3/3 Строб
	LCD_CMD_PORT &= ~(1 << LCD_CMD_WR);				// Строб	WR низкий (спад фронта)
	LCD_CMD_PORT |=  (1 << LCD_CMD_WR);
}



void ILI9488_FillScreen(uint16_t color) 
{
	uint8_t hi = color >> 8;
	uint8_t lo = color & 0xFF;

	LCD_CMD_PORT  &= ~(1 << LCD_CMD_CS);	// CS = LOW

	ILI9488_WriteCommand(0x2A);				// Column Address Set
	ILI9488_WriteData(0x00);
	ILI9488_WriteData(0x00);
	ILI9488_WriteData(0x01);
	ILI9488_WriteData(0xDF);				// 480 rows (0x01DF)

	ILI9488_WriteCommand(0x2B);				// Page Address Set
	ILI9488_WriteData(0x00);
	ILI9488_WriteData(0x00);
	ILI9488_WriteData(0x01);
	ILI9488_WriteData(0x3F);				// 320 columns (0x013F)

	ILI9488_WriteCommand(0x2C);				// Memory Write

	for (uint16_t i = 0; i < SCREEN_WIDTH; i++) 
	{
		for (uint16_t j = 0; j < SCREEN_HEIGHT; j++)
		{
			ILI9488_WriteData(hi);
			ILI9488_WriteData(lo);
		}
	}

	LCD_CMD_PORT  |= (1 << LCD_CMD_CS);   // CS = HIGH
}



void ILI9488_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	uint8_t hi = color >> 8;
	uint8_t lo = color & 0xFF;

	LCD_CMD_PORT &= ~(1 << LCD_CMD_CS);  // CS = LOW

	// Установка области (1x1 пиксель)
	ILI9488_WriteCommand(0x2A);  // Column Address Set
	ILI9488_WriteData(x >> 8);
	ILI9488_WriteData(x & 0xFF);
	ILI9488_WriteData(x >> 8);
	ILI9488_WriteData(x & 0xFF);

	ILI9488_WriteCommand(0x2B);  // Page Address Set
	ILI9488_WriteData(y >> 8);
	ILI9488_WriteData(y & 0xFF);
	ILI9488_WriteData(y >> 8);
	ILI9488_WriteData(y & 0xFF);

	ILI9488_WriteCommand(0x2C);  // Memory Write
	ILI9488_WriteData(hi);
	ILI9488_WriteData(lo);

	LCD_CMD_PORT |= (1 << LCD_CMD_CS);  // CS = HIGH
}



void ILI9488_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) 
{
	int16_t dx = abs(x1 - x0);
	int16_t dy = abs(y1 - y0);
	int16_t sx = (x0 < x1) ? 1 : -1;
	int16_t sy = (y0 < y1) ? 1 : -1;
	int16_t err = dx - dy;

	while (1) 
	{
		ILI9488_DrawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		
		int16_t e2 = 2 * err;
		
		if (e2 > -dy) 
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}



void ILI9488_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint8_t filled) 
{
	if (filled) 
	{
		for (uint16_t i = y; i < y + h; i++) 
		{
			ILI9488_DrawLine(x, i, x + w - 1, i, color);
		} 
	}
	else 
	{
		ILI9488_DrawLine(x, y, x + w - 1, y, color);					// Верхняя
		ILI9488_DrawLine(x, y + h - 1, x + w - 1, y + h - 1, color);	// Нижняя
		ILI9488_DrawLine(x, y, x, y + h - 1, color);					// Левая
		ILI9488_DrawLine(x + w - 1, y, x + w - 1, y + h - 1, color);	// Правая
	}
}



void ILI9488_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint8_t filled) 
{

	int16_t x = r;
	int16_t y = 0;
	int16_t err = 0;

	while (x >= y) 
	{
		if (filled) 
		{
			ILI9488_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
			ILI9488_DrawLine(x0 - y, y0 + x, x0 + y, y0 + x, color);
			ILI9488_DrawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
			ILI9488_DrawLine(x0 - y, y0 - x, x0 + y, y0 - x, color);
		} 
		else
		{
			ILI9488_DrawPixel(x0 + x, y0 + y, color);
			ILI9488_DrawPixel(x0 + y, y0 + x, color);
			ILI9488_DrawPixel(x0 - y, y0 + x, color);
			ILI9488_DrawPixel(x0 - x, y0 + y, color);
			ILI9488_DrawPixel(x0 - x, y0 - y, color);
			ILI9488_DrawPixel(x0 - y, y0 - x, color);
			ILI9488_DrawPixel(x0 + y, y0 - x, color);
			ILI9488_DrawPixel(x0 + x, y0 - y, color);
		}

		if (err <= 0) 
		{
			y++;
			err += 2 * y + 1;
		}
		
		if (err > 0) 
		{
			x--;
			err -= 2 * x + 1;
		}
	}

}






/*
void ILI9488_PrintChar(char c)
{
	if (c == '\a')			// звонок
	{
		return;	// не реализовано
	}
	else if (c == '\b')		// backspace
	{
		// Возвращаем курсор на ширину одного символа назад
		if (Xcursor >= font_size * 6) 
		{
			Xcursor -= font_size * 6;
		} 
		else 
		{
			// Если мы в начале строки, переходим на предыдущую строку
			if (Ycursor >= font_size * 8) 
			{
				Ycursor -= font_size * 8;
				Xcursor = SCREEN_WIDTH - font_size * 6; // Конец предыдущей строки
			} 
			else 
			{
				// Если мы в самом начале экрана, ничего не делаем
				Xcursor = 0;
			}
		}
		// Стираем символ (рисуем черный прямоугольник)
		for (uint8_t x = 0; x < 6; x++) 
		{
			for (uint8_t y = 0; y < 8; y++) 
			{
				ILI9488_DrawRect(Xcursor + x*font_size, Ycursor + y*font_size, font_size, font_size, BLACK, true);
			}
		}
		return;
	}
	else if (c == '\t')		// табуляция
	{
		// Перемещаем курсор к следующей позиции табуляции (каждые 8 символа)
		uint16_t tab_size = 8 * 6 * font_size; // 4 символа * 6 пикселей * масштаб
		uint16_t tab_pos = ((Xcursor / tab_size) + 1) * tab_size;
		
		// Если табуляция выходит за границы экрана, переносим на следующую строку
		if (tab_pos + (font_size * 6) > SCREEN_WIDTH) {
			Xcursor = 0;
			Ycursor += font_size * 8;
			
			// Проверка переполнения по вертикали
			if (Ycursor + (font_size * 8) > SCREEN_HEIGHT) {
				Ycursor = 0;
			}
			} else {
			Xcursor = tab_pos;
		}
		return;
	}
	else if (c == '\n')		// перенос каретки
	{
		Ycursor += font_size * 8;
	}
	else if (c == '\r')		// возврат каретки
	{
		Xcursor = 0;
	}
	else if (c >= 32 && c <= 127) // печатаемые символы
	{
		for(uint8_t x = 0; x < 6; x++)
		{
			for (uint8_t y = 0; y < 8; y++)
			{
				uint8_t mask = 1;
				
				if(monocraft[(uint8_t)c - 32][x] & (mask << (7 - y)))
				{
					ILI9488_DrawRect(Xcursor, Ycursor + y*font_size, font_size, font_size, font_color, true);
				}
				else
				{
					ILI9488_DrawRect(Xcursor, Ycursor  + y*font_size, font_size, font_size, font_background, true);
				}
			}
			Xcursor += font_size;
		}
	}
	
	
	
	// Проверка переполнения по горизонтали
	if (Xcursor + (font_size * 6) > SCREEN_WIDTH)
	{
		Xcursor = 0;
		Ycursor += font_size * 8;
	}
	
	// Проверка переполнения по вертикали
	if (Ycursor + (font_size * 8) > SCREEN_HEIGHT)
	{
		Xcursor = 0;
		Ycursor = 0;
	}
}


void ILI9488_PrintStr(char* str)
{
	while(*str)
	{
		ILI9488_PrintChar(*str++);
	}
}


void ILI9488_PrintChar_S(char c)
{
	cli();
	ILI9488_PrintChar(c);
	sei();
}


void ILI9488_PrintStr_S(char* str)
{
	cli();
	ILI9488_PrintStr(str);
	sei();
}
*/

uint16_t ILI9488_RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

