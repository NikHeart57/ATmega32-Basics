#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// SPI
#define SS		PB4
#define MOSI	PB5
#define MISO	PB6
#define SCK		PB7

// ST7789
#define DC			PC2		// ��� DC
#define DDR_DC		DDRC	// ���� �� ������� ����� DC
#define PORT_DC     PORTC	// ���� �� ������� ����� DC

#define RES			PC3		// ��� RES
#define DDR_RES		DDRC	// ���� �� ������� ����� RES
#define PORT_RES	PORTC	// ���� �� ������� ����� RES

#define DC_HIGH     PORT_DC |=  (1 << DC);		// DC_HIGH -
#define DC_LOW      PORT_DC &=~ (1 << DC);
#define RESET_HIGH  PORT_RES |=  (1 << RES);
#define RESET_LOW	PORT_RES &=~ (1 << RES);


void setup(void);
