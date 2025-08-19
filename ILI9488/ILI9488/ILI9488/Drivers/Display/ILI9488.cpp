#include "ILI9488.h"

//////////////////////////////////////////////////////////////////////////
//  ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ СОСТОЯНИЯ
//////////////////////////////////////////////////////////////////////////

uint16_t ili9488_cursor_x = 0;									///< Текущая позиция X для текста
uint16_t ili9488_cursor_y = 0;									///< Текущая позиция Y для текста
uint16_t ili9488_font_size = ILI9488_FONT;						///< Текущий размер шрифта
uint16_t ili9488_font_color =  ILI9488_FONT_COLOR;				///< Текущий цвет шрифта
uint16_t ili9488_font_bg_color = ILI9488_FONT_BACKGROUND;		///< Текущий цвет фона текста

//////////////////////////////////////////////////////////////////////////
//  МАССИВ СТАНДАРТНЫХ ЦВЕТОВ
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Массив стандартных цветов в формате RGB565
 * @note Индексы должны соответствовать ILI9488_ColorIndex
 */
const uint16_t ili9488_colors[ILI9488_COLOR_COUNT] = {
    0xF800, // RED:    11111 000000 00000
    0x07E0, // GREEN:  00000 111111 00000
    0x001F, // BLUE:   00000 000000 11111
    0xFFE0, // YELLOW: 11111 111111 00000
    0x07FF, // CYAN:   00000 111111 11111
    0xF81F, // MAGENTA:11111 000000 11111
    0xFFFF, // WHITE:  11111 111111 11111
    0x0000  // BLACK:  00000 000000 00000
};

//////////////////////////////////////////////////////////////////////////
//  НИЗКОУРОВНЕВЫЕ ФУНКЦИИ УПРАВЛЕНИЯ ДИСПЛЕЕМ
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Настройка линий ввода-вывода МК для работы с дисплеем
 * @note Инициализирует порты данных и управления как выходы с начальным уровнем 0.
 */
void ILI9488_Setup(void)
{
    // Настройка порта данных (D0-D7) на выход
    ILI9488_DATA_DDR    |=   (1 << ILI9488_DATA_DB0) | (1 << ILI9488_DATA_DB1) | (1 << ILI9488_DATA_DB2) | 
                        (1 << ILI9488_DATA_DB3) | (1 << ILI9488_DATA_DB4) | (1 << ILI9488_DATA_DB5) | 
                        (1 << ILI9488_DATA_DB6) | (1 << ILI9488_DATA_DB7);
    ILI9488_DATA_PORT   &= ~((1 << ILI9488_DATA_DB0) | (1 << ILI9488_DATA_DB1) | (1 << ILI9488_DATA_DB2) | 
                        (1 << ILI9488_DATA_DB3) | (1 << ILI9488_DATA_DB4) | (1 << ILI9488_DATA_DB5) | 
                        (1 << ILI9488_DATA_DB6) | (1 << ILI9488_DATA_DB7));
    
    // Настройка порта управления (CS, RS, WR, RST) на выход
    ILI9488_CMD_DDR     |=   (1 << ILI9488_CMD_CS) | (1 << ILI9488_CMD_RS) | (1 << ILI9488_CMD_WR) | (1 << ILI9488_CMD_RST);
    ILI9488_CMD_PORT    &= ~((1 << ILI9488_CMD_CS) | (1 << ILI9488_CMD_RS) | (1 << ILI9488_CMD_WR) | (1 << ILI9488_CMD_RST));    
}

//////////////////////////////////////////////////////////////////////////
//  ФУНКЦИИ ИНИЦИАЛИЗАЦИИ ДИСПЛЕЯ
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Внутренняя функция инициализации дисплея
 * @param[in] state Состояние дисплея
 * @param[in] orientation Ориентация дисплея
 * @param[in] ili9488_font_size Размер шрифта
 * @param[in] ili9488_font_color Цвет шрифта
 * @param[in] bg_color Цвет фона
 * @return 1 в случае успеха, 0 при ошибке
 * @note Внутренняя реализация, используется всеми публичными функциями Init
 */
static uint8_t ILI9488_Init_Internal(ILI9488_DisplayState state,  ILI9488_Orientation orientation, uint16_t ili9488_font_size, uint16_t ili9488_font_color, uint16_t bg_color)
{
    // Аппаратный сброс дисплея
    ILI9488_RESET();

    // Активация чипа (CS = LOW)
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_CS);        

    // Последовательность команд инициализации
    ILI9488_WriteCommand(0x01);    // Software Reset
    _delay_ms(10);

    ILI9488_WriteCommand(0x11);    // Exit Sleep
    _delay_ms(10);
    
    // Установка цветового формата (только 16-битный RGB565)
    ILI9488_WriteCommand(0x3A);    // Pixel Format Set
    ILI9488_WriteData(0x55);       // 16-bit color (RGB565)
    _delay_ms(10);

    // Установка ориентации
    ILI9488_WriteCommand(0x36);    // Memory Access Control
    ILI9488_WriteData(orientation);
    _delay_ms(10);
    
    // Включение/выключение дисплея
    if (state == ILI9488_DISPLAY_ON) 
	{
        ILI9488_WriteCommand(0x29);    // Display ON
    } else 
	{
        ILI9488_WriteCommand(0x28);    // Display OFF
    }
    _delay_ms(10);

    // Деактивация чипа (CS = HIGH)
    ILI9488_CMD_PORT |= (1 << ILI9488_CMD_CS);        
    
    // Установка параметров текста
	ili9488_font_size = ili9488_font_size;
	ili9488_font_color = ili9488_font_color;
    ili9488_font_bg_color = bg_color;
    
    // Сброс позиции курсора
    ili9488_cursor_x = 0;
    ili9488_cursor_y = 0;
    
    return 1; // Успешная инициализация
}

/**
 * @brief Базовая инициализация контроллера дисплея ILI9488
 * @param[in] state Состояние дисплея (вкл/выкл)
 * @return 1 в случае успеха, 0 при ошибке
 */
uint8_t ILI9488_Init(ILI9488_DisplayState state)
{
    return ILI9488_Init_Internal(state, ILI9488_ORIENTATION_LANDSCAPE, ILI9488_FONT, ili9488_font_color, ili9488_font_bg_color);
}

/**
 * @brief Инициализация контроллера дисплея ILI9488 с выбором ориентации
 * @param[in] state Состояние дисплея (вкл/выкл)
 * @param[in] orientation Ориentation дисплея
 * @return 1 в случае успеха, 0 при ошибке
 */
uint8_t ILI9488_Init(ILI9488_DisplayState state, ILI9488_Orientation orientation)
{
    return ILI9488_Init_Internal(state, orientation, ILI9488_FONT, ili9488_font_color, ili9488_font_bg_color);
}

/**
 * @brief Расширенная инициализация контроллера дисплея ILI9488
 * @param[in] state Состояние дисплея (вкл/выкл)
 * @param[in] orientation Ориентация дисплея
 * @param[in] ili9488_font_size Размер шрифта по умолчанию
 * @param[in] ili9488_font_color Цвет шрифта по умолчанию
 * @param[in] bg_color Цвет фона по умолчанию
 * @return 1 в случае успеха, 0 при ошибке
 */
uint8_t ILI9488_Init(ILI9488_DisplayState state, 
                    ILI9488_Orientation orientation,
                    uint16_t ili9488_font_size, 
                    uint16_t ili9488_font_color, 
                    uint16_t bg_color)
{
    return ILI9488_Init_Internal(state, 
                                orientation,
                                ili9488_font_size,
                                ili9488_font_color,
                                bg_color);
}



/**
 * @brief Аппаратный сброс (Reset) дисплея
 * @note Формирует импульс низкого уровня (~100 мкс) на линии RST.
 */
void ILI9488_RESET(void)
{
    ILI9488_CMD_PORT  &= ~(1 << ILI9488_CMD_RST); // RESET = 0
    _delay_us(100);
    ILI9488_CMD_PORT  |= (1 << ILI9488_CMD_RST);  // RESET = 1
    _delay_us(900);
}

/**
 * @brief Отправка команды на дисплей
 * @param[in] cmd Код команды для отправки
 * @note Устанавливает RS=0 (режим команды) и формирует строб записи.
 */
void ILI9488_WriteCommand(uint8_t cmd) 
{
    // Подготовка строба (WR в HIGH)
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);             
    
    // Загрузка данных: режим команды (RS=0) и выставление кода команды на шину данных
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_RS);             
    ILI9488_DATA_PORT =  cmd;                           
    
    // Формирование строба (спадающий фронт WR)
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_WR);             
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);
}

/**
 * @brief Отправка данных на дисплей
 * @param[in] data Данные для отправки
 * @note Устанавливает RS=1 (режим данных) и формирует строб записи.
 */
void ILI9488_WriteData(uint8_t data) 
{    
    // Подготовка строба (WR в HIGH)
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);             
    
    // Загрузка данных: режим данных (RS=1) и выставление данных на шину
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_RS);             
    ILI9488_DATA_PORT =  data;                          
    
    // Формирование строба (спадающий фронт WR)
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_WR);             
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);
}

//////////////////////////////////////////////////////////////////////////
//  ФУНКЦИИ ОТРИСОВКИ ГРАФИЧЕСКИХ ПРИМИТИВОВ
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Заполнение всего экрана указанным цветом
 * @param[in] color Цвет заполнения в формате RGB565
 * @note Устанавливает область записи на весь экран и последовательно записывает цвет каждого пикселя.
 * @warning Может работать медленно на низких частотах МК.
 */
void ILI9488_FillScreen(uint16_t color) 
{
    uint8_t hi = color >> 8;   // Старший байт цвета (R+G)
    uint8_t lo = color & 0xFF; // Младший байт цвета (G+B)

    // Активация чипа
    ILI9488_CMD_PORT  &= ~(1 << ILI9488_CMD_CS);    

    // Установка адреса столбцов (X: 0-479)
    ILI9488_WriteCommand(0x2A); // Column Address Set
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x01);
    ILI9488_WriteData(0xDF);    // 480 columns (0x01DF = 479)

    // Установка адреса строк (Y: 0-319)
    ILI9488_WriteCommand(0x2B); // Page Address Set
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x01);
    ILI9488_WriteData(0x3F);    // 320 rows (0x013F = 319)

    // Команда начала записи в память
    ILI9488_WriteCommand(0x2C); // Memory Write

    // Заполнение экрана цветом
    for (uint16_t i = 0; i < ILI9488_SCREEN_WIDTH; i++) 
    {
        for (uint16_t j = 0; j < ILI9488_SCREEN_HEIGHT; j++)
        {
            ILI9488_WriteData(hi); // Старший байт
            ILI9488_WriteData(lo); // Младший байт
        }
    }

    // Деактивация чипа
    ILI9488_CMD_PORT  |= (1 << ILI9488_CMD_CS);   
}

/**
 * @brief Рисование одного пикселя
 * @param[in] x Координата X (0..SCREEN_WIDTH-1)
 * @param[in] y Координата Y (0..SCREEN_HEIGHT-1)
 * @param[in] color Цвет пикселя в формате RGB565
 */
void ILI9488_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    // Активация чипа
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_CS);  

    // Установка области (1x1 пиксель) по X
    ILI9488_WriteCommand(0x2A);  // Column Address Set
    ILI9488_WriteData(x >> 8);
    ILI9488_WriteData(x & 0xFF);
    ILI9488_WriteData(x >> 8);
    ILI9488_WriteData(x & 0xFF);

    // Установка области (1x1 пиксель) по Y
    ILI9488_WriteCommand(0x2B);  // Page Address Set
    ILI9488_WriteData(y >> 8);
    ILI9488_WriteData(y & 0xFF);
    ILI9488_WriteData(y >> 8);
    ILI9488_WriteData(y & 0xFF);

    // Запись цвета пикселя
    ILI9488_WriteCommand(0x2C);  // Memory Write
    ILI9488_WriteData(hi);
    ILI9488_WriteData(lo);

    // Деактивация чипа
    ILI9488_CMD_PORT |= (1 << ILI9488_CMD_CS);  
}

/**
 * @brief Рисование линии по алгоритму Брезенхема
 * @param[in] x0 Начальная координата X (0..SCREEN_WIDTH-1)
 * @param[in] y0 Начальная координата Y (0..SCREEN_HEIGHT-1)
 * @param[in] x1 Конечная координата X (0..SCREEN_WIDTH-1)
 * @param[in] y1 Конечная координата Y (0..SCREEN_HEIGHT-1)
 * @param[in] color Цвет линии в формате RGB565
 * 
 * @note Особенности реализации:
 *  - Использует целочисленный алгоритм Брезенхема
 *  - Работает с любым направлением линии (включая диагонали)
 *  - Не выполняет проверку выхода за границы экрана
 *  - Рисует пиксели включая начальную и конечную точки
 * 
 * @warning Производительность зависит от длины линии
 *          (для длинных линий возможны задержки)
 */
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

/**
 * @brief Рисование прямоугольника или заполненного прямоугольника
 * @param[in] x Координата левого верхнего угла X (0..SCREEN_WIDTH-1)
 * @param[in] y Координата левого верхнего угла Y (0..SCREEN_HEIGHT-1)
 * @param[in] w Ширина прямоугольника в пикселях (>=1)
 * @param[in] h Высота прямоугольника в пикселях (>=1)
 * @param[in] color Цвет в формате RGB565
 * @param[in] filled Флаг заливки (0 - только контур, 1 - заполненный)
 * 
 * @note Особенности реализации:
 *  - Для контура рисуются 4 отдельные линии
 *  - Для заливки рисуются горизонтальные линии по всей высоте
 *  - Автоматически корректирует координаты (x+w-1, y+h-1)
 * 
 * @warning Не выполняет проверку на выход за границы экрана
 *          При больших размерах заполненного прямоугольника
 *          возможны значительные задержки
 */
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

/**
 * @brief Рисование окружности или заполненного круга
 * @param[in] x0 Координата центра X (0..SCREEN_WIDTH-1)
 * @param[in] y0 Координата центра Y (0..SCREEN_HEIGHT-1)
 * @param[in] r Радиус окружности в пикселях (>=1)
 * @param[in] color Цвет в формате RGB565
 * @param[in] filled Флаг заливки (0 - только контур, 1 - заполненный)
 * 
 * @note Особенности реализации:
 *  - Использует алгоритм Брезенхема для окружностей
 *  - Для контура рисует 8 симметричных точек на каждом шаге
 *  - Для заливки рисует горизонтальные линии между симметричными точками
 *  - Обеспечивает сглаживание окружности
 * 
 * @warning Производительность сильно зависит от радиуса
 *          Не выполняет проверку на выход за границы экрана
 *          Для больших радиусов возможны значительные задержки
 */
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

//////////////////////////////////////////////////////////////////////////
//  ФУНКЦИИ ВЫВОДА ТЕКСТА
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Вывод одного символа на экран
 * @param[in] c Символ для отображения
 * @note Поддерживает управляющие символы и автоматический перенос.
 */
void ILI9488_PrintChar(char c)
{
	if (c == '\a')			// звонок
	{
		return;	// не реализовано
	}
	else if (c == '\b')		// backspace
	{
		// Возвращаем курсор на ширину одного символа назад
		if (ili9488_cursor_x >= ili9488_font_size * 6) 
		{
			ili9488_cursor_x -= ili9488_font_size * 6;
		} 
		else 
		{
			// Если мы в начале строки, переходим на предыдущую строку
			if (ili9488_cursor_y >= ili9488_font_size * 8) 
			{
				ili9488_cursor_y -= ili9488_font_size * 8;
				ili9488_cursor_x = ILI9488_SCREEN_WIDTH - ili9488_font_size * 6; // Конец предыдущей строки
			} 
			else 
			{
				// Если мы в самом начале экрана, ничего не делаем
				ili9488_cursor_x = 0;
			}
		}
		// Стираем символ (рисуем черный прямоугольник)
		for (uint8_t x = 0; x < 6; x++) 
		{
			for (uint8_t y = 0; y < 8; y++) 
			{
				ILI9488_DrawRect(ili9488_cursor_x + x*ili9488_font_size, ili9488_cursor_y + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, BLACK, true);
			}
		}
		return;
	}
	else if (c == '\t')		// табуляция
	{
		// Перемещаем курсор к следующей позиции табуляции (каждые 8 символа)
		uint16_t tab_size = 8 * 6 * ili9488_font_size; // 4 символа * 6 пикселей * масштаб
		uint16_t tab_pos = ((ili9488_cursor_x / tab_size) + 1) * tab_size;
		
		// Если табуляция выходит за границы экрана, переносим на следующую строку
		if (tab_pos + (ili9488_font_size * 6) > ILI9488_SCREEN_WIDTH) {
			ili9488_cursor_x = 0;
			ili9488_cursor_y += ili9488_font_size * 8;
			
			// Проверка переполнения по вертикали
			if (ili9488_cursor_y + (ili9488_font_size * 8) > ILI9488_SCREEN_HEIGHT) {
				ili9488_cursor_y = 0;
			}
			} else {
			ili9488_cursor_x = tab_pos;
		}
		return;
	}
	else if (c == '\n')		// перенос каретки
	{
		ili9488_cursor_y += ili9488_font_size * 8;
	}
	else if (c == '\r')		// возврат каретки
	{
		ili9488_cursor_x = 0;
	}
	else if (c >= 32 && c <= 127) // печатаемые символы
	{
		for(uint8_t x = 0; x < 5; x++)
		{
			for (uint8_t y = 0; y < 8; y++)
			{
				uint8_t mask = 1;
				
				if(monocraft[(uint8_t)c - 32][x] & (mask << (7 - y)))
				{
					ILI9488_DrawRect(ili9488_cursor_x, ili9488_cursor_y + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, ili9488_font_color, true);
				}
				else
				{
					ILI9488_DrawRect(ili9488_cursor_x, ili9488_cursor_y  + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, ili9488_font_bg_color, true);
				}
			}
			ili9488_cursor_x += ili9488_font_size;
		}
		
		// Добавляем один пиксель межсимвольного интервала
		for (uint8_t y = 0; y < 8; y++)
		{
			ILI9488_DrawRect(ili9488_cursor_x, ili9488_cursor_y + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, ili9488_font_bg_color, true);
		}
		ili9488_cursor_x += ili9488_font_size;
	}
		
	// Проверка переполнения по горизонтали
	if (ili9488_cursor_x + (ili9488_font_size * 6) > ILI9488_SCREEN_WIDTH)
	{
		ili9488_cursor_x = 0;
		ili9488_cursor_y += ili9488_font_size * 8;
	}
	
	// Проверка переполнения по вертикали
	if (ili9488_cursor_y + (ili9488_font_size * 8) > ILI9488_SCREEN_HEIGHT)
	{
		ili9488_cursor_x = 0;
		ili9488_cursor_y = 0;
	}
}


/**
 * @brief Вывод строки на экран
 * @param[in] str Указатель на строку (нуль-терминированная)
 */
void ILI9488_PrintStr(char* str)
{
    while(*str)
    {
        ILI9488_PrintChar(*str++);
    }
}

//////////////////////////////////////////////////////////////////////////
//  ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Конвертация цвета из 24-битного RGB в 16-битный RGB565
 * @param[in] r Компонента Red (0-255)
 * @param[in] g Компонента Green (0-255)
 * @param[in] b Компонента Blue (0-255)
 * @return Цвет в формате RGB565
 */
uint16_t ILI9488_RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

