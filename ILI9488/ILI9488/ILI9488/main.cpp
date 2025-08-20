#include "main.h"

Ball balls[BALL_COUNT];
Ball holes[HOLE_COUNT];
TouchHistory touchHistory[5] = {0};
	
char itoa_buffer[64]; 

bool TIMER1_COMPA_vect_flag = false;
bool INT0_vect_flag = false;


// todo 
// drivers mcu исправлены
// display и touch надо допиливать

int main(void)
{	
	cli();
	_delay_ms(10);
	
	// Инициализация устройств микроконтроллера
	JTAG_Init(JTAG_DISABLED);
	ADC_Init(ADC_DISABLED);
	UART_Init(UART_ENABLED, UART_8_BITS, UART_1_STOP_BIT, UART_PARITY_NONE, UART_INTERRUPT_RX_ENABLED);
	SPI_Init(SPI_ENABLED, SPI_MASTER, SPI_MODE0, SPI_CLK_DIV8, SPI_MSB_FIRST, SPI_INTERRUPT_DISABLE);
	
	INT0_Init(INT_ENABLED, INT_TRIGGER_LOW_LEVEL);
	
	TIM1_Init(TIM_ENABLED, TIM_PRESCALER_1024);		// Настройка таймера 1 на прерывание каждые 200 мс
	TIM1_SetMode(TIM_CTC);
	TIM1_SetCompareValue(3125, TIM_CHANNEL_A);
	TIM_EnableInterrupt(1, 0x02);
	
	
	// Настройка выводов МК и инициализация внешнего экрана ILI9488
	ILI9488_Setup();	
	ILI9488_Init(ILI9488_DISPLAY_ON, ILI9488_ORIENTATION_LANDSCAPE_FLIP);
	ILI9488_FillScreen(BLACK);
	ILI9488_FillScreen(BLACK);
	
	// Настройка выводов МК и инициализация внешнего контроллера сенсора XPT2046
	XPT2046_Setup();

#ifdef DEBUG
	UART_SendString("\n\rScreen is ON\n\r");
#endif

	sei();	
	

		
	for (uint8_t i = 0; i < BALL_COUNT; i++)
	{
		balls[i].id = i;
		balls[i].r = 15.0f;
		balls[i].x = 0;
		balls[i].y = 0;
		balls[i].old_x = balls[i].x; // Инициализация
		balls[i].old_y = balls[i].y; // Инициализация
		balls[i].vx = 0;
		balls[i].vy = 0;
	}
	
	// Белый шар (биток) - отдельно слева
	balls[0].x = 120;
	balls[0].y = 160;
	balls[0].color = WHITE;

	// Цветные шары (пирамида справа)
	// Первый ряд (1 шар)
	balls[1].x = 360;
	balls[1].y = 160;
	balls[1].color = RED;

	// Второй ряд (2 шара)
	balls[2].x = 400;
	balls[2].y = 130;
	balls[2].color = GREEN;

	balls[3].x = 400;
	balls[3].y = 190;
	balls[3].color = BLUE;

	// Третий ряд (3 шара)
	balls[4].x = 440;
	balls[4].y = 100;
	balls[4].color = YELLOW;

	balls[5].x = 440;
	balls[5].y = 160;
	balls[5].color = MAGENTA;

	balls[6].x = 440;
	balls[6].y = 220;
	balls[6].color = CYAN;
	
	
	
	// Белый шар (биток)
	balls[0].x = 100; balls[0].y = 160;

	// Пирамида (шаг ~30px вместо 40px)
	balls[1].x = 380; balls[1].y = 160;  // Вершина

	balls[2].x = 410; balls[2].y = 145;   // Второй ряд
	balls[3].x = 410; balls[3].y = 175;

	balls[4].x = 440; balls[4].y = 130;   // Третий ряд
	balls[5].x = 440; balls[5].y = 160;
	balls[6].x = 440; balls[6].y = 190;
		
	
	for(uint8_t i = 0; i <  HOLE_COUNT; i++)
	{
		balls[i].id = i;
		holes[i].r = 16.0f;
		holes[i].color = WHITE;	
		holes[i].vx = 0;
		holes[i].vy = 0;
	}
	
	holes[0].x = 0;
	holes[0].y = 0;
	
	holes[1].x = 240;
	holes[1].y = 0;
	
	holes[2].x = 480;
	holes[2].y = 0;
	
	holes[3].x = 0;
	holes[3].y = 320;
	
	holes[4].x = 240;
	holes[4].y = 320;
	
	holes[5].x = 480;
	holes[5].y = 320;
	
	
	// Первичная отрисовка поля и шариков
	for (uint8_t i = 0; i < HOLE_COUNT; i++) {holes[i].Draw_Static();}
	for (uint8_t i = 0; i < BALL_COUNT; i++) {balls[i].Draw_Static();}
			
		
    while (1) 
    {	
		//////////////////////////////////////////////////////////////////////////
		//	Обработчики прерываний
		//////////////////////////////////////////////////////////////////////////
		if (TIMER1_COMPA_vect_flag)
		{
			TIMER1_COMPA_vect_flag = false;
			TIMER1_COMPA_vect_handler();
		}
		
		if (INT0_vect_flag)
		{
			INT0_vect_flag = false;
			INT0_vect_flag_handler();
			INT0_Init(INT_ENABLED, INT_TRIGGER_LOW_LEVEL);
		}
			
		
		//////////////////////////////////////////////////////////////////////////
		//	Игра
		//////////////////////////////////////////////////////////////////////////
		Engine();
		fps++;		
    }
	
	return 1;
}









//////////////////////////////////////////////////////////////////////////
//	Обработка векторов прерываний
//////////////////////////////////////////////////////////////////////////


// Обработчик прерывания таймера 1
ISR(TIMER1_COMPA_vect)
{
	TIMER1_COMPA_vect_flag = true;
}


// Обработчик прерывания по выводу INT0 
ISR(INT0_vect)
{
	INT0_vect_flag = true;
	INT0_Init(INT_DISABLED);
}


