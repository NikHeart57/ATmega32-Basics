#include "../main.h" 

int8_t Ball::activeBallId = -1;


void Ball::UpdatePos(float dt)
{
	// ��������� ��� ���������� ������
	const float STOP_THRESHOLD = 0.05f;    // ����� ��������� (��������/���)
	const float FRICTION = 0.97f;          // ����������� ������ (3% ������)
	
	// ��������� ���������� �������� ���������
	float abs_vx = fabs(vx);
	float abs_vy = fabs(vy);
	
	// ���� ��� �������� ���� ������ - ������ ���������
	if (abs_vx < STOP_THRESHOLD && abs_vy < STOP_THRESHOLD)
	{
		vx = 0.0f;
		vy = 0.0f;
		return;
	}
	
	// ��������� ������ � ������ ���������� ��������
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
	
	
	// ��������� �������
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
	// ����� �����
	if (x <= r) {
		// ����������� ������ ���� �������� �����
		if (vx < 0) {
			vx = -vx;
			x = r; // ������������ �������
		}
	}
	// ������ �����
	else if (x >= 480 - r) {
		// ����������� ������ ���� �������� ������
		if (vx > 0) {
			vx = -vx;
			x = 480 - r;
		}
	}
		
	// ������� �����
	if (y <= r) {
		// ����������� ������ ���� �������� �����
		if (vy < 0) {
			vy = -vy;
			y = r;
		}
	}
	// ������ �����
	else if (y >= 320 - r) {
		// ����������� ������ ���� �������� ����
		if (vy > 0) {
			vy = -vy;
			y = 320 - r;
		}
	}
}
	
	
bool Ball::IsTouched(uint16_t touch_x, uint16_t touch_y)
{
	// ��������� ���������� �� ����� ������� �� ������ ������
	float dx = touch_x - this->x;
	float dy = touch_y - this->y;
	
	float distance = sqrt(dx * dx + dy * dy);
	
	// ���� ���������� ������ ������� � ������� ������ ������
	return (distance <= this->r);
}


void Ball::CheckBallsCollision(Ball& ball1, Ball& ball2)
{
	// ��������� ���������� ����� ��������
	float dx = ball2.x - ball1.x;
	float dy = ball2.y - ball1.y;
	
	float distance = sqrt(dx*dx + dy*dy);
	if (distance == 0) return;		// �� ������ ������ �������� ������� �� ����
	
	// ��������� ������������
	if (distance < ball1.r + ball2.r)
	{
		// ����������� ������ ����������
		float nx = dx / distance;
		float ny = dy / distance;
		
		// ��������� ������������� ��������
		float dvx = ball2.vx - ball1.vx;
		float dvy = ball2.vy - ball1.vy;
		
		// �������� �������� �� ����� �������
		float velocity_along_normal = dvx * nx + dvy * ny;
		
		// ���� ���� ��������� ���� �� ����� - �� ������������ ������������
		if (velocity_along_normal > 0) return;
		
		// ������� ������������ (������� ����������)
		float impulse = 2.0f * velocity_along_normal / (1.0f + 1.0f); // ����� ����������
		
		// ��������� �������
		ball1.vx += impulse * nx;
		ball1.vy += impulse * ny;
		ball2.vx -= impulse * nx;
		ball2.vy -= impulse * ny;
		
		// ��������� ����, ����� ��� �� ��������
		float overlap = (ball1.r + ball2.r) - distance;
		ball1.x -= overlap * nx * 0.5f;
		ball1.y -= overlap * ny * 0.5f;
		ball2.x += overlap * nx * 0.5f;
		ball2.y += overlap * ny * 0.5f;
	}
}


bool Ball::CheckHoleCollision(Ball& ball) 
{
	// ���� ��� ��� ��� ���� (����� � ����� �����)
	if (ball.x < 0 && ball.y < 0) {
		return false;
	}

	for (uint8_t i = 0; i < HOLE_COUNT; i++) {
		float dx = holes[i].x - ball.x;
		float dy = holes[i].y - ball.y;
		float distance_sq = dx*dx + dy*dy; // ������� ���������� (�����������)
		float min_distance = (ball.r + holes[i].r) * 0.9f; // 90% ����� ��������
		
		if (distance_sq < (min_distance * min_distance)) {
			#ifdef DEBUG
			char buf[32];
			snprintf(buf, sizeof(buf), "Ball %d in hole %d\n\r", ball.id, i);
			UART_SendString(buf);
			#endif
			
			// ������������ ���
			ball.Deactivate();
			
			// ���� ��� ��� �������� ��� - ���������� �����
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