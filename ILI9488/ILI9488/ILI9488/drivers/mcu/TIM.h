/**
 * @file        TIM.h
 * @brief       Драйвер таймеров 0, 1, 2 для AVR микроконтроллера AtMega32
 * @details     Предоставляет полный контроль над таймерами микроконтроллера:
 *              - Настройка режимов работы (Normal, CTC, PWM)
 *              - Управление предделителями
 *              - Работа с каналами сравнения
 *              - Управление прерываниями
 *              - Поддержка Input Capture (Timer1)
 * 
 * @note        Особенности реализации:
 *              - Полная поддержка всех трех таймеров (0, 1, 2)
 *              - Единый интерфейс для всех таймеров
 *              - Подробная обработка ошибок конфигурации
 * 
 * @warning     Важные ограничения:
 *              - Timer0 и Timer2: 8-битные, максимальное значение 255
 *              - Timer1: 16-битный, максимальное значение 65535
 *              - Режимы PWM требуют корректной настройки выводов OCRx
 *              - Input Capture доступен только на Timer1
 * 
 * @attention   Требования для работы:
 *              - Подключение <avr/io.h> и <avr/interrupt.h>
 *              - Корректная настройка FUSE-битов (если требуется)
 *              - Правильная инициализация прерываний в главной программе
 * 
 * @todo        Планируемые улучшения:
 *              - Добавление поддержки таймеров в sleep-режимах
 *              - Расширенная диагностика ошибок конфигурации
 *              - Поддержка дополнительных режимов PWM
 *              - Оптимизация для энергосбережения
 * 
 * @bug         Известные проблемы:
 *              - Неполная проверка валидности параметров в некоторых функциях
 *              - Ограниченная поддержка асинхронных режимов (Timer2)
 *              - Требуется тестирование на различных частотах CPU
 * 
 * @author      deepseek
 * @date        2025-08-18
 * @version     1.0
 * 
 * @copyright   MIT License
 */

#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//////////////////////////////////////////////////////////////////////////
// Перечисление настроек таймеров
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Состояния таймера (включен/выключен)
 * @note При выключении таймера сбрасываются все настройки и прерывания
 */
typedef enum {
    TIM_DISABLED = 0,    ///< Таймер полностью отключен, энергосбережение
    TIM_ENABLED          ///< Таймер включен и готов к работе
} TIM_State;

/**
 * @brief Коэффициенты деления частоты (предделители)
 * @note Выбор предделителя влияет на точность и диапазон таймера
 * @warning Некорректный предделитель может привести к неработоспособности таймера
 */
typedef enum {
    TIM_NO_PRESCALER = 1,    ///< Без деления (F_CPU)
    TIM_PRESCALER_8 = 2,     ///< Деление на 8
    TIM_PRESCALER_64 = 3,    ///< Деление на 64
    TIM_PRESCALER_256 = 4,   ///< Деление на 256
    TIM_PRESCALER_1024 = 5   ///< Деление на 1024
} TIM_Prescaler;

/**
 * @brief Режимы работы таймера
 * @note Каждый режим имеет специфическое применение:
 *       - Normal: базовый счетчик
 *       - CTC: Clear Timer on Compare, генерация точных интервалов
 *       - PWM: ШИМ-модуляция для управления мощностью
 */
typedef enum {
    TIM_NORMAL = 0,              ///< Обычный режим счета (overflow)
    TIM_CTC,                     ///< Сброс по совпадению (точные интервалы)
    TIM_PWM_PHASE_CORRECT,       ///< Корректный ШИМ (симметричный)
    TIM_PWM_FAST                 ///< Быстрый ШИМ (высокая частота)
} TIM_Mode;

/**
 * @brief Каналы таймера для сравнения/PWM
 * @note Не все таймеры поддерживают оба канала
 * @warning Timer0 и Timer2 поддерживают только канал A
 */
typedef enum {
    TIM_CHANNEL_A = 0,   ///< Канал A (OCRxA)
    TIM_CHANNEL_B        ///< Канал B (OCRxB) - только Timer1
} TIM_Channel;

//////////////////////////////////////////////////////////////////////////
// Прототипы функций таймеров
//////////////////////////////////////////////////////////////////////////

/**
 * @group Timer 0 Functions (8-bit)
 * @brief Функции для работы с 8-битным таймером 0
 */

/**
 * @brief Инициализация таймера 0 с указанным предделителем
 * @param state Состояние таймера (вкл/выкл)
 * @param prescaler Коэффициент деления частоты
 * @note При выключении сбрасываются все настройки и прерывания
 * @warning Не поддерживает канал B (только канал A)
 */
void TIM0_Init(TIM_State state, TIM_Prescaler prescaler);

/**
 * @brief Инициализация таймера 0 с предделителем по умолчанию (64)
 * @param state Состояние таймера
 * @note Упрощенная версия с стандартными настройками
 */
void TIM0_Init(TIM_State state);

/**
 * @brief Установка режима работы таймера 0
 * @param mode Режим работы (Normal, CTC, PWM)
 * @note Режим PWM требует дополнительной настройки выводов
 */
void TIM0_SetMode(TIM_Mode mode);

/**
 * @brief Установка значения сравнения для таймера 0
 * @param value Значение сравнения (0-255)
 * @note Для режима CTC определяет период таймера
 */
void TIM0_SetCompareValue(uint8_t value);

/**
 * @brief Настройка ШИМ на таймере 0
 * @param duty Коэффициент заполнения (0-255)
 * @param channel Канал ШИМ (только A для Timer0)
 * @note Устанавливает non-inverting PWM режим
 */
void TIM0_SetPWM(uint8_t duty, TIM_Channel channel);

/**
 * @group Timer 1 Functions (16-bit)
 * @brief Функции для работы с 16-битным таймером 1
 */

/**
 * @brief Инициализация таймера 1 с указанным предделителем
 * @param state Состояние таймера
 * @param prescaler Коэффициент деления частоты
 * @note Поддерживает оба канала и input capture
 */
void TIM1_Init(TIM_State state, TIM_Prescaler prescaler);

/**
 * @brief Инициализация таймера 1 с предделителем по умолчанию (64)
 * @param state Состояние таймера
 */
void TIM1_Init(TIM_State state);

/**
 * @brief Установка режима работы таймера 1
 * @param mode Режим работы
 * @note Поддерживает все режимы включая сложные PWM
 */
void TIM1_SetMode(TIM_Mode mode);

/**
 * @brief Установка значения сравнения для таймера 1
 * @param value Значение сравнения (0-65535)
 * @param channel Канал сравнения (A или B)
 */
void TIM1_SetCompareValue(uint16_t value, TIM_Channel channel);

/**
 * @brief Настройка ШИМ на таймере 1
 * @param duty Коэффициент заполнения (0-65535)
 * @param channel Канал ШИМ (A или B)
 * @note Устанавливает non-inverting PWM режим
 */
void TIM1_SetPWM(uint16_t duty, TIM_Channel channel);

/**
 * @brief Инициализация Input Capture для таймера 1
 * @note Настраивает шумоподавление и фронт срабатывания
 * @warning Требует подключения внешнего сигнала к соответствующему выводу
 */
void TIM1_InputCaptureInit(void);

/**
 * @brief Получение значения Input Capture
 * @return Значение захваченного времени
 * @note Используется для измерения длительности импульсов
 */
uint16_t TIM1_GetInputCaptureValue(void);

/**
 * @group Timer 2 Functions (8-bit)
 * @brief Функции для работы с 8-битным таймером 2
 */

/**
 * @brief Инициализация таймера 2 с указанным предделителем
 * @param state Состояние таймера
 * @param prescaler Коэффициент деления частоты
 * @note Аналогичен Timer0 но с асинхронными возможностями
 */
void TIM2_Init(TIM_State state, TIM_Prescaler prescaler);

/**
 * @brief Инициализация таймера 2 с предделителем по умолчанию (64)
 * @param state Состояние таймера
 */
void TIM2_Init(TIM_State state);

/**
 * @brief Установка режима работы таймера 2
 * @param mode Режим работы
 */
void TIM2_SetMode(TIM_Mode mode);

/**
 * @brief Установка значения сравнения для таймера 2
 * @param value Значение сравнения (0-255)
 */
void TIM2_SetCompareValue(uint8_t value);

/**
 * @brief Настройка ШИМ на таймере 2
 * @param duty Коэффициент заполнения (0-255)
 * @param channel Канал ШИМ (только A для Timer2)
 */
void TIM2_SetPWM(uint8_t duty, TIM_Channel channel);

/**
 * @group Common Timer Functions
 * @brief Общие функции для всех таймеров
 */

/**
 * @brief Установка предделителя для указанного таймера
 * @param timer_num Номер таймера (0, 1, 2)
 * @param prescaler Коэффициент деления частоты
 * @warning Некорректный номер таймера приведет к неопределенному поведению
 */
void TIM_SetPrescaler(uint8_t timer_num, TIM_Prescaler prescaler);

/**
 * @brief Включение прерываний таймера
 * @param timer_num Номер таймера (0, 1, 2)
 * @param interrupt_type Битовое поле запрашиваемых прерываний:
 *                      - 0x01: Overflow
 *                      - 0x02: Compare A
 *                      - 0x04: Compare B (Timer1)
 *                      - 0x08: Input Capture (Timer1)
 * @note Требует реализации соответствующих ISR в главной программе
 */
void TIM_EnableInterrupt(uint8_t timer_num, uint8_t interrupt_type);

/**
 * @brief Отключение прерываний таймера
 * @param timer_num Номер таймера (0, 1, 2)
 * @param interrupt_type Битовое поле отключаемых прерываний
 */
void TIM_DisableInterrupt(uint8_t timer_num, uint8_t interrupt_type);

