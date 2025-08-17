#ifndef XPT2046_H
#define XPT2046_H

/*
void XPT2046_Setup(void); - настраивает chip select а не пин прерывания
*/


#ifndef F_CPU
#define F_CPU			1000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "../SPI.h"
#include "../Display/ILI9488.h"

#include "../UART.h"

// Настройки пинов (можно изменить под вашу схему)
#define XPT2046_CS_DDR   DDRB
#define XPT2046_CS_PORT  PORTB
#define XPT2046_CS_PIN   PB0	// Chip Select для тач-контроллера

// Настройки пинов (можно изменить под вашу схему)
#define XPT2046_IRQ_DDR   DDRD
#define XPT2046_IRQ_PORT  PORTD
#define XPT2046_IRQ_PIN   PD2

// Команды для XPT2046
#define XPT2046_CMD_X     0x90  // Измерение X
#define XPT2046_CMD_Y     0xD0  // Измерение Y
#define XPT2046_CMD_Z1    0xB0  // Измерение Z1
#define XPT2046_CMD_Z2    0xC0  // Измерение Z2

// Прототипы функций
void XPT2046_Setup(void);
uint16_t XPT2046_ReadData(uint8_t command);
void XPT2046_ReadCoordinates(uint16_t &x, uint16_t &y);


#endif // XPT2046_H