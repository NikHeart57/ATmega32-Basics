/**
 * @note        Комментарий сгенерирован нейросетью deepseek 19.08.25
 *
 * @file        INT.cpp
 * @brief       Реализация драйвера внешних прерываний для AVR ATmega32
 * @details     Содержит низкоуровневые функции для работы с внешними прерываниями
 *              и обработчики прерываний с callback-механизмом.
 * 
 * @note        Особенности реализации:
 *              - Автоматическая настройка пинов как входов с подтяжкой
 *              - Поддержка всех режимов триггеров для INT0/INT1
 *              - Ограниченная поддержка режимов для INT2
 *              - Механизм callback-функций для удобства использования
 * 
 * @warning     Особенности использования:
 *              - Callback-функции выполняются в контексте прерывания
 *              - INT2 поддерживает только 2 режима из 4
 *              - Требуется ручное подключение обработчиков в основном коде
 * 
 * @author      Николай Куркин, deepseek
 * @date        2025-08-19
 * @version     1.0
 * 
 * @copyright   MIT License
 */

#include "INT.h"

//////////////////////////////////////////////////////////////////////////
//  СТАТИЧЕСКИЕ ПЕРЕМЕННЫЕ ДЛЯ CALLBACK-ФУНКЦИЙ
//////////////////////////////////////////////////////////////////////////

static Int_Callback int0_callback = NULL;  ///< Callback для INT0
static Int_Callback int1_callback = NULL;  ///< Callback для INT1
static Int_Callback int2_callback = NULL;  ///< Callback для INT2

//////////////////////////////////////////////////////////////////////////
//  ФУНКЦИИ ДЛЯ ПРЕРЫВАНИЯ INT0
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Базовая инициализация прерывания INT0
 * @param[in] state Состояние прерывания (вкл/выкл)
 * 
 * @details Настраивает пин PD2 как вход с подтяжкой и включает прерывание
 * с режимом по умолчанию (любое изменение).
 */
void INT0_Init(Int_State state)
{
    DDRD &= ~(1 << PD2);      // PD2 (INT0) как вход
    PORTD |= (1 << PD2);      // Подтяжка к VCC
    
    switch (state) {
        case INT_ENABLED:
            GICR |= (1 << INT0);  // Включение прерывания INT0
            break;
        case INT_DISABLED:
            GICR &= ~(1 << INT0); // Выключение прерывания INT0
            break;
        default:
            break;
    }
}

/**
 * @brief Расширенная инициализация прерывания INT0
 * @param[in] state Состояние прерывания (вкл/выкл)
 * @param[in] mode Режим триггера (условие срабатывания)
 * 
 * @details Настраивает пин и режим срабатывания прерывания INT0.
 * Поддерживает все 4 режима триггеров.
 */
void INT0_Init(Int_State state, Int_TriggerMode mode)
{
    DDRD &= ~(1 << PD2);      // PD2 (INT0) как вход
    PORTD |= (1 << PD2);      // Подтяжка к VCC
    
    INT0_Init(state);
    
    // Настройка режима триггера
    switch (mode) {
        case INT_TRIGGER_LOW_LEVEL:
            MCUCR &= ~(1 << ISC00) & ~(1 << ISC01);  // Низкий уровень
            break;
        case INT_TRIGGER_ANY_EDGE:
            MCUCR |= (1 << ISC00);                   // Любое изменение
            MCUCR &= ~(1 << ISC01);
            break;
        case INT_TRIGGER_RISING_EDGE:
            MCUCR |= (1 << ISC00) | (1 << ISC01);    // Нарастающий фронт
            break;
        case INT_TRIGGER_FALLING_EDGE:
            MCUCR &= ~(1 << ISC00);                  // Спадающий фронт
            MCUCR |= (1 << ISC01);
            break;
        default:
            MCUCR |= (1 << ISC00);                   // По умолчанию любое изменение
            MCUCR &= ~(1 << ISC01);
            break;
    }
}

/**
 * @brief Установка callback-функции для INT0
 * @param[in] callback Функция обратного вызова
 */
void INT0_SetCallback(Int_Callback callback)
{
    int0_callback = callback;
}

//////////////////////////////////////////////////////////////////////////
//  ФУНКЦИИ ДЛЯ ПРЕРЫВАНИЯ INT1
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Базовая инициализация прерывания INT1
 * @param[in] state Состояние прерывания (вкл/выкл)
 */
void INT1_Init(Int_State state)
{
    DDRD &= ~(1 << PD3);      // PD3 (INT1) как вход
    PORTD |= (1 << PD3);      // Подтяжка к VCC
    
    switch (state) {
        case INT_ENABLED:
            GICR |= (1 << INT1);  // Включение прерывания INT1
            break;
        case INT_DISABLED:
            GICR &= ~(1 << INT1); // Выключение прерывания INT1
            break;
        default:
            break;
    }
}

/**
 * @brief Расширенная инициализация прерывания INT1
 * @param[in] state Состояние прерывания (вкл/выкл)
 * @param[in] mode Режим триггера (условие срабатывания)
 */
void INT1_Init(Int_State state, Int_TriggerMode mode)
{
    DDRD &= ~(1 << PD3);      // PD3 (INT1) как вход
    PORTD |= (1 << PD3);      // Подтяжка к VCC
    
    INT1_Init(state);
    
    // Настройка режима триггера
    switch (mode) {
        case INT_TRIGGER_LOW_LEVEL:
            MCUCR &= ~(1 << ISC10) & ~(1 << ISC11);  // Низкий уровень
            break;
        case INT_TRIGGER_ANY_EDGE:
            MCUCR |= (1 << ISC10);                   // Любое изменение
            MCUCR &= ~(1 << ISC11);
            break;
        case INT_TRIGGER_RISING_EDGE:
            MCUCR |= (1 << ISC10) | (1 << ISC11);    // Нарастающий фронт
            break;
        case INT_TRIGGER_FALLING_EDGE:
            MCUCR &= ~(1 << ISC10);                  // Спадающий фронт
            MCUCR |= (1 << ISC11);
            break;
        default:
            MCUCR |= (1 << ISC10);                   // По умолчанию любое изменение
            MCUCR &= ~(1 << ISC11);
            break;
    }
}

/**
 * @brief Установка callback-функции для INT1
 * @param[in] callback Функция обратного вызова
 */
void INT1_SetCallback(Int_Callback callback)
{
    int1_callback = callback;
}

//////////////////////////////////////////////////////////////////////////
//  ФУНКЦИИ ДЛЯ ПРЕРЫВАНИЯ INT2
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Базовая инициализация прерывания INT2
 * @param[in] state Состояние прерывания (вкл/выкл)
 * 
 * @note INT2 находится на пине PB2, а не PD2 (исправьте в коде)
 */
void INT2_Init(Int_State state)
{
    DDRB &= ~(1 << PB2);      // PB2 (INT2) как вход (ИСПРАВЛЕНО!)
    PORTB |= (1 << PB2);      // Подтяжка к VCC
    
    switch (state) {
        case INT_ENABLED:
            GICR |= (1 << INT2);  // Включение прерывания INT2
            break;
        case INT_DISABLED:
            GICR &= ~(1 << INT2); // Выключение прерывания INT2
            break;
        default:
            break;
    }
}

/**
 * @brief Расширенная инициализация прерывания INT2
 * @param[in] state Состояние прерывания (вкл/выкл)
 * @param[in] mode Режим триггера (условие срабатывания)
 * 
 * @note INT2 поддерживает только 2 режима: низкий уровень и спадающий фронт
 */
void INT2_Init(Int_State state, Int_TriggerMode mode)
{
    DDRB &= ~(1 << PB2);      // PB2 (INT2) как вход (ИСПРАВЛЕНО!)
    PORTB |= (1 << PB2);      // Подтяжка к VCC
    
    INT2_Init(state);
    
    // Настройка режима триггера (INT2 имеет только 2 режима)
    switch (mode) {
        case INT_TRIGGER_LOW_LEVEL:
            MCUCSR &= ~(1 << ISC2);  // Низкий уровень
            break;
        case INT_TRIGGER_FALLING_EDGE:
            MCUCSR |= (1 << ISC2);   // Спадающий фронт
            break;
        default:
            MCUCSR |= (1 << ISC2);   // По умолчанию спадающий фронт
            break;
    }
}

/**
 * @brief Установка callback-функции для INT2
 * @param[in] callback Функция обратного вызова
 */
void INT2_SetCallback(Int_Callback callback)
{
    int2_callback = callback;
}

//////////////////////////////////////////////////////////////////////////
//  ОБРАБОТЧИКИ ПРЕРЫВАНИЙ (РАСКОММЕНТИРУЙТЕ ПО НЕОБХОДИМОСТИ)
//////////////////////////////////////////////////////////////////////////

/*
// Обработчик прерывания INT0
ISR(INT0_vect)
{
    if (int0_callback != NULL)
    {
        int0_callback();
    }
}

// Обработчик прерывания INT1
ISR(INT1_vect)
{
    if (int1_callback != NULL)
    {
        int1_callback();
    }
}

// Обработчик прерывания INT2
ISR(INT2_vect)
{
    if (int2_callback != NULL)
    {
        int2_callback();
    }
}
*/