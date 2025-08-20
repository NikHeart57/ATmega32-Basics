#ifndef XPT2046_H
#define XPT2046_H

/*
void XPT2046_Setup(void); - ����������� chip select � �� ��� ����������
*/


#ifndef F_CPU
#define F_CPU			1000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "../mcu/SPI.h"
#include "../display/ILI9488.h"


// ��������� ����� (����� �������� ��� ���� �����)
#define XPT2046_CS_DDR   DDRD
#define XPT2046_CS_PORT  PORTD
#define XPT2046_CS_PIN   PD3	// Chip Select ��� ���-�����������

// ������� ��� XPT2046
#define XPT2046_CMD_X     0x90  // ��������� X
#define XPT2046_CMD_Y     0xD0  // ��������� Y
#define XPT2046_CMD_Z1    0xB0  // ��������� Z1
#define XPT2046_CMD_Z2    0xC0  // ��������� Z2

// ��������� �������
void XPT2046_Setup(void);
uint16_t XPT2046_ReadData(uint8_t command);
void XPT2046_ReadCoordinates(uint16_t &x, uint16_t &y);


#endif // XPT2046_H