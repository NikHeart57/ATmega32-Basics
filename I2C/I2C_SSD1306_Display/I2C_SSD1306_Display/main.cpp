#include "main.h"
using namespace std;


struct point				// Структура точки
{
	float x, y, z;
};

struct triangle				// Структура треугольника p
{
	point p[3];
};

void proj(point &pInput, point &pProj, float Vect[6])
{
	pProj.x = (Vect[4] * Vect[5] * pInput.x) / (pInput.z);
	pProj.y = (Vect[5] * pInput.y) / (pInput.z);
	pProj.z = (pInput.z * Vect[2]) - (Vect[0] * Vect[2]);
}



int main(void)
{	
	ssd1306_Display Display(0b01111000, 64, 32);
	Display.Init();
	Display.Buffer_Fill(0);
	
	const int AmmountOfPoints = 40;
		
	point arrPoints[AmmountOfPoints];
	
	for (int i = 0; i < AmmountOfPoints; i++)
	{
		arrPoints[AmmountOfPoints] = {(float)((rand() % 1280) - 640) / 10.0f, (float)((rand() % 640) - 320)/ 10.0f, (float)(rand() % 100) / 10.0f};
	}

	point arrTempPoints[AmmountOfPoints];
	
	//// Инициализация и заселение матрицы проекции и вычисление ее компонентов
	const float zNear = 0.1f;												// znear
	const float zFar = 10.0f;												// zfar
	const float q = (float)(zFar)/(float)(zFar - zFar);					// q
	const float Fov = 100.0f;												// theta (FOV)
	const float a = (float)64 / (float)128;								// a = h/w
	const float f = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);			// f = 1/(tan(theta/2))
	
	float projVect[6] = {zNear, zFar, q, Fov, a, f};
		
	while(1)
	{
		Display.Buffer_Fill(0);
		
		for (int i = 0; i < AmmountOfPoints; i++)
		{
			proj(arrPoints[i], arrTempPoints[i], projVect);
			Display.Buffer_SetPixel(arrTempPoints[i].x, arrTempPoints[i].y);
			arrPoints[i].z -= 0.1;
			if (arrPoints[i].z <= 0)
			{
				arrPoints[i] = {(float)((rand() % 1280) - 640) / 10.0f, (float)((rand() % 640) - 320)/ 10.0f, (float)((float)(rand() % 80) / 10.0f) + 2.0f};
			}
		}		
	
		Display.Buffer_Send();
	}
}