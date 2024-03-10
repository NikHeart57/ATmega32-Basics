#ifndef I2C_H_
#define I2C_H_

#include "ssd1306.cpp"

//////////////////////////////////////////////////////////////////////////
// Прототипы функций

	void Init(void);
	void Buffer_Fill(char byte);
	void Buffer_Send(void);
	void Buffer_SetPixel(int x, int y);
	void Buffer_RemovePixel(int x, int y);
	void Buffer_SetLine(float xa, float ya, float xb, float yb);
	void Buffer_SetTriangle(float xa, float ya, float xb, float yb, float xc, float yc);

#endif /* I2C_H_ */
