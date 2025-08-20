#pragma once

#define DEBUG
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <util/delay.h>

#include "drivers/mcu/JTAG.h"
#include "drivers/mcu/ADC.h"
#include "drivers/mcu/UART.h"
#include "drivers/mcu/SPI.h"
#include "drivers/mcu/INT.h"
#include "drivers/mcu/TIM.h"
#include "drivers/display/ILI9488.h"
#include "drivers/touch/XPT2046.h"

struct Ball
{
	uint8_t id;
	float r;
	float old_x;
	float old_y;
	float x;
	float y;
	float vx;
	float vy;
	uint16_t color;
	
	static int8_t activeBallId;
	
	
	void UpdatePos(float dt);
	void Draw();
	void Draw_Static();
	void CheckWallCollision(void);
	bool IsTouched(uint16_t touch_x, uint16_t touch_y);
	
	static void CheckBallsCollision(Ball& ball1, Ball& ball2);
	static bool CheckHoleCollision(Ball& ball);
	void Deactivate();
};


struct TouchHistory
{
	uint16_t x;
	uint16_t y;
	uint8_t counter;
};




extern char itoa_buffer[64]; 

const uint8_t BALL_COUNT = 7;
const uint8_t HOLE_COUNT = 6;

extern Ball balls[BALL_COUNT];
extern Ball holes[HOLE_COUNT];

extern TouchHistory touchHistory[5];

extern bool TIMER1_COMPA_vect_flag;
extern uint32_t seconds;
extern uint8_t fps;

extern bool INT0_vect_flag;
extern uint16_t touch_x;
extern uint16_t touch_y;
extern uint16_t prev_touch_x;
extern uint16_t prev_touch_y;
extern uint32_t last_touch_time;



void TIMER1_COMPA_vect_handler();
void INT0_vect_flag_handler();

void Engine();
