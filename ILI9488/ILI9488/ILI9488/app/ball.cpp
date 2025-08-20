#include "../main.h" 

int8_t Ball::activeBallId = -1;


void Ball::UpdatePos(float dt)
{
	// Константы для физической модели
	const float STOP_THRESHOLD = 0.05f;    // Порог остановки (пикселей/сек)
	const float FRICTION = 0.97f;          // Коэффициент трения (3% потерь)
	
	// Вычисляем абсолютные значения скоростей
	float abs_vx = fabs(vx);
	float abs_vy = fabs(vy);
	
	// Если обе скорости ниже порога - полная остановка
	if (abs_vx < STOP_THRESHOLD && abs_vy < STOP_THRESHOLD)
	{
		vx = 0.0f;
		vy = 0.0f;
		return;
	}
	
	// Применяем трение к каждой компоненте скорости
	if (abs_vx > STOP_THRESHOLD) 
	{
		vx *= FRICTION;	
	} 
	else 
	{
		vx = 0.0f;
	}
	
	if (abs_vy > STOP_THRESHOLD) 
	{
		vy *= FRICTION;
	} 
	else 
	{
		vy = 0.0f;
	}
	
	
	// Обновляем позицию
	x += vx * dt;
	y += vy * dt;
}




void Ball::Draw()
{
	if (x != old_x || y != old_y) 
	{
		ILI9488_DrawCircle(old_x, old_y, r, BLACK, 0);
		ILI9488_DrawCircle(x, y, r, color, 0);
		old_x = x;
		old_y = y;
	}
}
	
	
void Ball::Draw_Static()
{
	ILI9488_DrawCircle(x, y, r, color, 0);
}

	
void Ball::CheckWallCollision(void)
{
	// Левая стена
	if (x <= r) {
		// Инвертируем только если движемся влево
		if (vx < 0) {
			vx = -vx;
			x = r; // Корректируем позицию
		}
	}
	// Правая стена
	else if (x >= 480 - r) {
		// Инвертируем только если движемся вправо
		if (vx > 0) {
			vx = -vx;
			x = 480 - r;
		}
	}
		
	// Верхняя стена
	if (y <= r) {
		// Инвертируем только если движемся вверх
		if (vy < 0) {
			vy = -vy;
			y = r;
		}
	}
	// Нижняя стена
	else if (y >= 320 - r) {
		// Инвертируем только если движемся вниз
		if (vy > 0) {
			vy = -vy;
			y = 320 - r;
		}
	}
}
	
	
bool Ball::IsTouched(uint16_t touch_x, uint16_t touch_y)
{
	// Вычисляем расстояние от точки касания до центра шарика
	float dx = touch_x - this->x;
	float dy = touch_y - this->y;
	
	float distance = sqrt(dx * dx + dy * dy);
	
	// Если расстояние меньше радиуса — касание внутри шарика
	return (distance <= this->r);
}


void Ball::CheckBallsCollision(Ball& ball1, Ball& ball2)
{
	// Вычисляем расстояние между центрами
	float dx = ball2.x - ball1.x;
	float dy = ball2.y - ball1.y;
	
	float distance = sqrt(dx*dx + dy*dy);
	if (distance == 0) return;		// На всякий случай проверка деления на ноль
	
	// Проверяем столкновение
	if (distance < ball1.r + ball2.r)
	{
		// Нормализуем вектор расстояния
		float nx = dx / distance;
		float ny = dy / distance;
		
		// Вычисляем относительную скорость
		float dvx = ball2.vx - ball1.vx;
		float dvy = ball2.vy - ball1.vy;
		
		// Проекция скорости на линию центров
		float velocity_along_normal = dvx * nx + dvy * ny;
		
		// Если шары удаляются друг от друга - не обрабатываем столкновение
		if (velocity_along_normal > 0) return;
		
		// Импульс столкновения (упругое соударение)
		float impulse = 2.0f * velocity_along_normal / (1.0f + 1.0f); // Массы одинаковые
		
		// Применяем импульс
		ball1.vx += impulse * nx;
		ball1.vy += impulse * ny;
		ball2.vx -= impulse * nx;
		ball2.vy -= impulse * ny;
		
		// Разделяем шары, чтобы они не залипали
		float overlap = (ball1.r + ball2.r) - distance;
		ball1.x -= overlap * nx * 0.5f;
		ball1.y -= overlap * ny * 0.5f;
		ball2.x += overlap * nx * 0.5f;
		ball2.y += overlap * ny * 0.5f;
	}
}


bool Ball::CheckHoleCollision(Ball& ball) 
{
	// Если шар уже вне игры (попал в лунку ранее)
	if (ball.x < 0 && ball.y < 0) {
		return false;
	}

	for (uint8_t i = 0; i < HOLE_COUNT; i++) {
		float dx = holes[i].x - ball.x;
		float dy = holes[i].y - ball.y;
		float distance_sq = dx*dx + dy*dy; // Квадрат расстояния (оптимизация)
		float min_distance = (ball.r + holes[i].r) * 0.9f; // 90% суммы радиусов
		
		if (distance_sq < (min_distance * min_distance)) {
			#ifdef DEBUG
			char buf[32];
			snprintf(buf, sizeof(buf), "Ball %d in hole %d\n\r", ball.id, i);
			UART_SendString(buf);
			#endif
			
			// Деактивируем шар
			ball.Deactivate();
			
			// Если это был активный шар - сбрасываем выбор
			if (activeBallId == ball.id) {
				activeBallId = -1;
			}
			
			return true;
		}
	}
	return false;
}


void Ball::Deactivate() 
{	
	ILI9488_DrawCircle(x, y, r, BLACK, 0);
	x = -100;
	y = -100;
	vx = 0;
	vy = 0;
	old_x = -100;
	old_y = -100;
}