#ifndef TIM_H_
#define TIM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

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


//////////////////////////////////////////////////////////////////////////
//	Перечисление настроек UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Состояния таймера
 */
typedef enum {
    TIM_DISABLED = 0,
    TIM_ENABLED
} TIM_State;

/**
 * @brief Предделители таймера
 */
typedef enum {
    TIM_NO_PRESCALER = 1,
    TIM_PRESCALER_8 = 2,
    TIM_PRESCALER_64 = 3,
    TIM_PRESCALER_256 = 4,
    TIM_PRESCALER_1024 = 5
} TIM_Prescaler;

/**
 * @brief Режимы работы таймера
 */
typedef enum {
	TIM_NORMAL = 0,
	TIM_CTC,
	TIM_PWM_PHASE_CORRECT,
	TIM_PWM_FAST
} TIM_Mode;

/**
 * @brief 
 */
typedef enum {
	TIM_CHANNEL_A = 0,
	TIM_CHANNEL_B
} TIM_Channel;


//////////////////////////////////////////////////////////////////////////
//	Прототипы функций TIM
//////////////////////////////////////////////////////////////////////////

/// ========== Timer 0 ==========
void TIM0_Init(TIM_State state, TIM_Prescaler prescaler);
void TIM0_Init(TIM_State state);
void TIM0_SetMode(TIM_Mode mode);
void TIM0_SetCompareValue(uint8_t value);
void TIM0_SetPWM(uint8_t duty, TIM_Channel channel);

// ========== Timer 1 ==========
void TIM1_Init(TIM_State state, TIM_Prescaler prescaler);
void TIM1_Init(TIM_State state);
void TIM1_SetMode(TIM_Mode mode);
void TIM1_SetCompareValue(uint16_t value, TIM_Channel channel);
void TIM1_SetPWM(uint16_t duty, TIM_Channel channel);
void TIM1_InputCaptureInit(void);
uint16_t TIM1_GetInputCaptureValue(void);

// ========== Timer 2 ==========
void TIM2_Init(TIM_State state, TIM_Prescaler prescaler);
void TIM2_Init(TIM_State state);
void TIM2_SetMode(TIM_Mode mode);
void TIM2_SetCompareValue(uint8_t value);
void TIM2_SetPWM(uint8_t duty, TIM_Channel channel);

// ========== Common ==========
void TIM_SetPrescaler(uint8_t timer_num, TIM_Prescaler prescaler);
void TIM_EnableInterrupt(uint8_t timer_num, uint8_t interrupt_type);
void TIM_DisableInterrupt(uint8_t timer_num, uint8_t interrupt_type);

#endif /* TIM_H_ */