#include "../main.h" 

uint32_t seconds = 0;
uint8_t fps = 0;

uint16_t touch_x = 0;
uint16_t touch_y = 0;
uint16_t prev_touch_x = 0;
uint16_t prev_touch_y = 0;
uint32_t last_touch_time = 0;


void TIMER1_COMPA_vect_handler()
{
	static uint8_t counter_200ms = 0;
	counter_200ms++;
	
	// Пример: каждую секунду (5 * 200 мс) отправлять сообщение
	if (counter_200ms >= 5)
	{		
		counter_200ms = 0;
		seconds++;
		
	#ifdef DEBUG
		UART_SendString("Timer: ");
		UART_SendString(itoa(seconds, itoa_buffer, 10));
		UART_SendString(" \tFPS: ");
		UART_SendString(itoa(fps, itoa_buffer, 10));
		UART_SendString("\n\r");
	#endif
	
		fps = 0;
	}
}



void INT0_vect_flag_handler()
{
	// Если касание есть
	if (!(PIND & (1 << PD2)))
	{
		// Считываем координаты касания
		XPT2046_ReadCoordinates(touch_x, touch_y);
		
		// Проверяем, не вышли ли координаты за пределы экрана
		if (touch_x >= ILI9488_SCREEN_WIDTH - 1 || touch_y >= ILI9488_SCREEN_HEIGHT - 1)
		{
			#ifdef DEBUG
			UART_SendString("Out of screen range\n\r");
			#endif
			return;
		}
		
		// Если уже есть активный шар, обновляем его позицию и записываем историю
		if (Ball::activeBallId != -1)
		{
			// Обновляем позицию
			balls[Ball::activeBallId].x = touch_x;
			balls[Ball::activeBallId].y = touch_y;
			
			// Записываем историю перемещений (кольцевой буфер)
			static uint8_t history_index = 0;
			touchHistory[history_index].x = touch_x;
			touchHistory[history_index].y = touch_y;
			touchHistory[history_index].counter = 5; // Счетчик активности
			history_index = (history_index + 1) % 5;
		}
		// Ищем новый шар для нажатия
		else
		{
			for (uint8_t i = 0; i < BALL_COUNT; i++)
			{
				if (balls[i].IsTouched(touch_x, touch_y))
				{
					#ifdef DEBUG
					UART_SendString("Touched ball ID: ");
					UART_SendString(itoa(balls[i].id, itoa_buffer, 10));
					UART_SendString("\n\r");
					#endif
					
					balls[i].x = touch_x;
					balls[i].y = touch_y;
					Ball::activeBallId = i;
					
					// Очищаем историю при новом касании
					memset(touchHistory, 0, sizeof(touchHistory));
					break;
				}
			}
		}
	}
	else // Если касания нет (палец убран)
	{
		if (Ball::activeBallId != -1)
		{
			// Рассчитываем скорость только если есть история перемещений
			uint8_t valid_points = 0;
			int16_t sum_dx = 0, sum_dy = 0;
			
			// Анализируем историю перемещений
			for (uint8_t i = 0; i < 4; i++)
			{
				if (touchHistory[i].counter > 0 && touchHistory[i+1].counter > 0)
				{
					sum_dx += (int16_t)touchHistory[i].x - touchHistory[i+1].x;
					sum_dy += (int16_t)touchHistory[i].y - touchHistory[i+1].y;
					valid_points++;
					touchHistory[i].counter--; // Уменьшаем счетчик
				}
			}
			
			// Если есть данные для расчета скорости
			if (valid_points > 0)
			{
				// Усредняем перемещение и устанавливаем скорость
				balls[Ball::activeBallId].vx = sum_dx / (float)valid_points * 2.0f;
				balls[Ball::activeBallId].vy = sum_dy / (float)valid_points * 2.0f;
				
				#ifdef DEBUG
				UART_SendString("Ball ");
				UART_SendString(itoa(Ball::activeBallId, itoa_buffer, 10));
				UART_SendString(" velocity: ");
				UART_SendString(itoa((int)balls[Ball::activeBallId].vx, itoa_buffer, 10));
				UART_SendString(", ");
				UART_SendString(itoa((int)balls[Ball::activeBallId].vy, itoa_buffer, 10));
				UART_SendString("\n\r");
				#endif
			}
		}
		
		Ball::activeBallId = -1;
	}
}




void Engine()
{
	// Отрисовка лунок
	for (uint8_t i = 0; i < HOLE_COUNT; i++)
	{
		cli();
		holes[i].Draw_Static();
		sei();
	}
	
	// Движение шариков
	for (uint8_t i = 0; i < BALL_COUNT; i++)
	{
		balls[i].UpdatePos(7.0f);
	}
	
	// Проверка столкновений шариков со стенками
	for (uint8_t i = 0; i < BALL_COUNT; i++)
	{
		balls[i].CheckWallCollision();
	}
	
	// Проверка столкновений между шарами
	for (uint8_t i = 0; i < BALL_COUNT; i++)
	{
		for (uint8_t j = i + 1;  j < BALL_COUNT; j++)
		{
			Ball::CheckBallsCollision(balls[i], balls[j]);
		}
	}
	
	// Отрисовка шариков
	for (uint8_t i = 0; i < BALL_COUNT; i++)
	{
		cli();
		balls[i].Draw();
		sei();
	}
	
	// Проверка попадания шаров в лунки
	for (uint8_t i = 0; i < BALL_COUNT; i++)
	{
		Ball::CheckHoleCollision(balls[i]);
	}
}