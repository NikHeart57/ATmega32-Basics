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
#define DC			PC2		// Пин DC
#define DDR_DC		DDRC	// Порт на котором будет DC
#define PORT_DC     PORTC	// Порт на котором будет DC

#define RES			PC3		// Пин RES
#define DDR_RES		DDRC	// Порт на котором будет RES
#define PORT_RES	PORTC	// Порт на котором будет RES

#define DC_HIGH     PORT_DC |=  (1 << DC);		// DC_HIGH -
#define DC_LOW      PORT_DC &=~ (1 << DC);
#define RESET_HIGH  PORT_RES |=  (1 << RES);
#define RESET_LOW	PORT_RES &=~ (1 << RES);


void setup(void);
