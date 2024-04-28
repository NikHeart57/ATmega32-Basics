#pragma once 

void Init(void);
void Buffer_Fill(char byte);
void Buffer_Send(void);
void Buffer_SetPixel(int x, int y);
void Buffer_RemovePixel(int x, int y);
void Buffer_SetLine(float xa, float ya, float xb, float yb);
void Buffer_SetTriangle(float xa, float ya, float xb, float yb, float xc, float yc);
void PrintChar(char ascii);
void PrintChar(int x, int y, char ascii);
void PrintStr(char word[]);
//void PrintInt(unsigned int num);
	
#include "ssd1306.cpp"

