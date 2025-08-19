/**
 * @file		TIM.h
 * @brief		Драйвер таймера 0 для AVR микроконтроллера AtMega32
 * @details		Поддерживает работу с таймером 0
 * @note		-
 * @attention	-
 * @warning		Сделано нейросетью! Не протестировано!
 * @todo		Добавить работу с таймерами 1 и 2.
 * @bug			-
 * @author		deepseek
 * @date		-
 * @version		0.0
 */

#include "TIM.h"


// ========== Timer 0 Functions ==========
void TIM0_Init(TIM_State state, TIM_Prescaler prescaler) 
{
	if (state == TIM_DISABLED) 
	{
		TCCR0 = 0;
		TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));
		return;
	}
	
	TCCR0 = (1 << CS00); // Default prescaler 1
	TIM_SetPrescaler(0, prescaler);
}

void TIM0_Init(TIM_State state) {
	TIM0_Init(state, TIM_PRESCALER_64);
}

void TIM0_SetMode(TIM_Mode mode) {
	TCCR0 &= ~((1 << WGM01) | (1 << WGM00));
	switch(mode) {
		case TIM_CTC:
		TCCR0 |= (1 << WGM01);
		break;
		case TIM_PWM_PHASE_CORRECT:
		TCCR0 |= (1 << WGM00);
		break;
		case TIM_PWM_FAST:
		TCCR0 |= (1 << WGM01) | (1 << WGM00);
		break;
		default: // Normal mode
		break;
	}
}

void TIM0_SetCompareValue(uint8_t value) {
	OCR0 = value;
}

void TIM0_SetPWM(uint8_t duty, TIM_Channel channel) {
	if (channel == TIM_CHANNEL_A) {
		TCCR0 |= (1 << COM01); // Non-inverting PWM
		OCR0 = duty;
	}
}

// ========== Timer 1 Functions ==========

void TIM1_Init(TIM_State state, TIM_Prescaler prescaler) {
	if (state == TIM_DISABLED) {
		TCCR1A = TCCR1B = 0;
		TIMSK &= ~((1 << TOIE1) | (1 << OCIE1A) | (1 << OCIE1B));
		return;
	}
	
	TCCR1B = (1 << CS10); // Default prescaler 1
	TIM_SetPrescaler(1, prescaler);
}

void TIM1_Init(TIM_State state) {
	TIM1_Init(state, TIM_PRESCALER_64);
}

void TIM1_SetMode(TIM_Mode mode) {
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
	TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
	
	switch(mode) {
		case TIM_CTC:
		TCCR1B |= (1 << WGM12);
		break;
		case TIM_PWM_PHASE_CORRECT:
		TCCR1A |= (1 << WGM10);
		break;
		case TIM_PWM_FAST:
		TCCR1A |= (1 << WGM11) | (1 << WGM10);
		TCCR1B |= (1 << WGM12);
		break;
		default: // Normal mode
		break;
	}
}

void TIM1_SetCompareValue(uint16_t value, TIM_Channel channel) {
	if (channel == TIM_CHANNEL_A) OCR1A = value;
	else OCR1B = value;
}

void TIM1_SetPWM(uint16_t duty, TIM_Channel channel) {
	if (channel == TIM_CHANNEL_A) {
		TCCR1A |= (1 << COM1A1);
		OCR1A = duty;
		} else {
		TCCR1A |= (1 << COM1B1);
		OCR1B = duty;
	}
}

void TIM1_InputCaptureInit(void) {
	TCCR1B |= (1 << ICNC1); // Noise canceler
	TCCR1B |= (1 << ICES1); // Rising edge
	TIMSK |= (1 << TICIE1); // Enable interrupt
}

uint16_t TIM1_GetInputCaptureValue(void) {
	return ICR1;
}

// ========== Timer 2 Functions ==========

void TIM2_Init(TIM_State state, TIM_Prescaler prescaler) {
	if (state == TIM_DISABLED) {
		TCCR2 = 0;
		TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));
		return;
	}
	
	TCCR2 = (1 << CS20); // Default prescaler 1
	TIM_SetPrescaler(2, prescaler);
}

void TIM2_Init(TIM_State state) {
	TIM2_Init(state, TIM_PRESCALER_64);
}

void TIM2_SetMode(TIM_Mode mode) {
	TCCR2 &= ~((1 << WGM21) | (1 << WGM20));
	switch(mode) {
		case TIM_CTC:
		TCCR2 |= (1 << WGM21);
		break;
		case TIM_PWM_PHASE_CORRECT:
		TCCR2 |= (1 << WGM20);
		break;
		case TIM_PWM_FAST:
		TCCR2 |= (1 << WGM21) | (1 << WGM20);
		break;
		default: // Normal mode
		break;
	}
}

void TIM2_SetCompareValue(uint8_t value) {
	OCR2 = value;
}

void TIM2_SetPWM(uint8_t duty, TIM_Channel channel) {
	if (channel == TIM_CHANNEL_A) {
		TCCR2 |= (1 << COM21); // Non-inverting PWM
		OCR2 = duty;
	}
}

// ========== Common Functions ==========

void TIM_SetPrescaler(uint8_t timer_num, TIM_Prescaler prescaler) {
	switch(timer_num) {
		case 0: // Timer 0
		TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
		switch(prescaler) {
			case TIM_NO_PRESCALER:  TCCR0 |= (1 << CS00); break;
			case TIM_PRESCALER_8:   TCCR0 |= (1 << CS01); break;
			case TIM_PRESCALER_64:  TCCR0 |= (1 << CS01) | (1 << CS00); break;
			case TIM_PRESCALER_256: TCCR0 |= (1 << CS02); break;
			case TIM_PRESCALER_1024:TCCR0 |= (1 << CS02) | (1 << CS00); break;
		}
		break;
		
		case 1: // Timer 1
		TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		switch(prescaler) {
			case TIM_NO_PRESCALER:  TCCR1B |= (1 << CS10); break;
			case TIM_PRESCALER_8:   TCCR1B |= (1 << CS11); break;
			case TIM_PRESCALER_64:  TCCR1B |= (1 << CS11) | (1 << CS10); break;
			case TIM_PRESCALER_256: TCCR1B |= (1 << CS12); break;
			case TIM_PRESCALER_1024:TCCR1B |= (1 << CS12) | (1 << CS10); break;
		}
		break;
		
		case 2: // Timer 2
		TCCR2 &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
		switch(prescaler) {
			case TIM_NO_PRESCALER:  TCCR2 |= (1 << CS20); break;
			case TIM_PRESCALER_8:   TCCR2 |= (1 << CS21); break;
			case TIM_PRESCALER_64:  TCCR2 |= (1 << CS21) | (1 << CS20); break;
			case TIM_PRESCALER_256: TCCR2 |= (1 << CS22); break;
			case TIM_PRESCALER_1024:TCCR2 |= (1 << CS22) | (1 << CS20); break;
		}
		break;
	}
}

void TIM_EnableInterrupt(uint8_t timer_num, uint8_t interrupt_type) {
	switch(timer_num) {
		case 0:
		if (interrupt_type & 0x01) TIMSK |= (1 << TOIE0);
		if (interrupt_type & 0x02) TIMSK |= (1 << OCIE0);
		break;
		case 1:
		if (interrupt_type & 0x01) TIMSK |= (1 << TOIE1);
		if (interrupt_type & 0x02) TIMSK |= (1 << OCIE1A);
		if (interrupt_type & 0x04) TIMSK |= (1 << OCIE1B);
		if (interrupt_type & 0x08) TIMSK |= (1 << TICIE1);
		break;
		case 2:
		if (interrupt_type & 0x01) TIMSK |= (1 << TOIE2);
		if (interrupt_type & 0x02) TIMSK |= (1 << OCIE2);
		break;
	}
}

void TIM_DisableInterrupt(uint8_t timer_num, uint8_t interrupt_type) {
	switch(timer_num) {
		case 0:
		if (interrupt_type & 0x01) TIMSK &= ~(1 << TOIE0);
		if (interrupt_type & 0x02) TIMSK &= ~(1 << OCIE0);
		break;
		case 1:
		if (interrupt_type & 0x01) TIMSK &= ~(1 << TOIE1);
		if (interrupt_type & 0x02) TIMSK &= ~(1 << OCIE1A);
		if (interrupt_type & 0x04) TIMSK &= ~(1 << OCIE1B);
		if (interrupt_type & 0x08) TIMSK &= ~(1 << TICIE1);
		break;
		case 2:
		if (interrupt_type & 0x01) TIMSK &= ~(1 << TOIE2);
		if (interrupt_type & 0x02) TIMSK &= ~(1 << OCIE2);
		break;
	}
}
