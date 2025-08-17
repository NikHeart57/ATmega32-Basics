#ifndef INT_H
#define INT_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

/**
 * @brief Режимы работы прерываний
 */
typedef enum {
	INT_DISABLED = 0,			//
    INT_ENABLED,				//
} Int_State;


/**
 * @brief Типы триггеров прерываний
 */
typedef enum {
	INT_TRIGGER_LOW_LEVEL = 0,	// Низкий уровень
	INT_TRIGGER_FALLING_EDGE,	// Спадающий фронт
	INT_TRIGGER_RISING_EDGE,	// Нарастающий фронт
	INT_TRIGGER_ANY_EDGE		// Любое изменение
} Int_TriggerMode;


/**
 * @brief Callback-функция для прерывания
 */
typedef void (*Int_Callback)(void);


// Прототипы Callback-функций
void INT0_SetCallback(Int_Callback callback);
void INT1_SetCallback(Int_Callback callback);
void INT2_SetCallback(Int_Callback callback);

// Прототипы функций
// Настройка INT0
void INT0_Init(Int_State state);
void INT0_Init(Int_State state, Int_TriggerMode mode);

// Настройка INT1
void INT1_Init(Int_State state);
void INT1_Init(Int_State state, Int_TriggerMode mode);

// Настройка INT2
void INT2_Init(Int_State state);
void INT2_Init(Int_State state, Int_TriggerMode mode);

#endif // INT_H