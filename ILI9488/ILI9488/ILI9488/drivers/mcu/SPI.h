/**
 * @note        Комментарий сгенерирован нейросетью deepseek 19.08.25
 *
 * @file        SPI.h
 * @brief       Драйвер SPI для AVR микроконтроллера AtMega32
 * @details     Предоставляет полный набор функций для работы с интерфейсом SPI
 *              в режимах Master/Slave с настройкой всех параметров передачи.
 * 
 * @note        Особенности:
 *              - Поддержка режимов Master и Slave
 *              - Настройка скорости, режима работы, порядка битов
 *              - Блокирующие функции передачи/приема данных
 *              - Поддержка прерываний
 * 
 * @warning     Ограничения:
 *              - Функции передачи/приема являются блокирующими
 *              - В режиме Slave требуется внешнее управление линией SS
 *              - Ограниченная проверка ошибок
 * 
 * @attention   Для работы необходимо:
 *              - Подключение <avr/io.h> и <stdint.h>
 *              - Корректная настройка пинов SPI в системе
 * 
 * @todo        Планируемые улучшения:
 *              - Неблокирующие функции с использованием DMA
 *              - Расширенная диагностика ошибок
 *              - Поддержка более сложных сценариев передачи
 * 
 * @bug         Известные проблемы:
 *              - 
 * 
 * @author      Николай Куркин, deepseek
 * @date        2025-08-03
 * @version     0.9
 * 
 * @copyright   MIT License
 */
#pragma once

#include <avr/io.h>
#include <stdint.h>

//////////////////////////////////////////////////////////////////////////
//  ОПРЕДЕЛЕНИЯ ПОРТОВ И ПИНОВ SPI
//////////////////////////////////////////////////////////////////////////

#define SPI_DDR     DDRB	///< Регистр направления данных порта SPI
#define SPI_PORT    PORTB	///< Регистр данных порта SPI
#define SPI_PIN     PINB	///< Регистр ввода порта SPI
#define SPI_SS      PB4		///< Пин выбора устройства (Slave Select)
#define SPI_MOSI    PB5		///< Пин передачи данных (Master Out Slave In)
#define SPI_MISO    PB6		///< Пин приема данных (Master In Slave Out)
#define SPI_SCK     PB7		///< Пин тактового сигнала (Serial Clock)

//////////////////////////////////////////////////////////////////////////
//  ПЕРЕЧИСЛЕНИЕ НАСТРОЕК SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Состояние SPI (вкл/выкл)
 */
typedef enum {
    SPI_DISABLED = 0,  ///< SPI выключен (энергосбережение)
    SPI_ENABLED         ///< SPI включен и готов к работе
} SPI_State;

/**
 * @brief Режим работы (Master/Slave)
 */
typedef enum {
    SPI_SLAVE = 0,  ///< Режим ведомого (Slave)
    SPI_MASTER       ///< Режим ведущего (Master)
} SPI_Role;

/**
 * @brief Режимы SPI (фаза и полярность тактового сигнала)
 */
typedef enum {
    SPI_MODE0 = 0,  ///< CPOL=0, CPHA=0
    SPI_MODE1,      ///< CPOL=0, CPHA=1
    SPI_MODE2,      ///< CPOL=1, CPHA=0
    SPI_MODE3       ///< CPOL=1, CPHA=1
} SPI_Mode;

/**
 * @brief Скорость SPI (делитель частоты)
 */
typedef enum {
    SPI_CLK_DIV4 = 0,   ///< F_CPU/4
    SPI_CLK_DIV16,      ///< F_CPU/16
    SPI_CLK_DIV64,      ///< F_CPU/64
    SPI_CLK_DIV128,     ///< F_CPU/128
    SPI_CLK_DIV2,       ///< F_CPU/2
    SPI_CLK_DIV8,       ///< F_CPU/8
    SPI_CLK_DIV32       ///< F_CPU/32
} SPI_ClockRate;

/**
 * @brief Порядок передачи битов
 */
typedef enum {
    SPI_MSB_FIRST = 0,  ///< Старший бит первый (стандарт)
    SPI_LSB_FIRST        ///< Младший бит первый
} SPI_DataOrder;

/**
 * @brief Состояние прерываний SPI
 */
typedef enum {
    SPI_INTERRUPT_DISABLE = 0,  ///< Прерывания отключены
    SPI_INTERRUPT_ENABLE         ///< Прерывания включены
} SPI_Interrupt;

//////////////////////////////////////////////////////////////////////////
//  ПРОТОТИПЫ ФУНКЦИЙ SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Полная инициализация SPI
 * @param[in] state Состояние SPI (вкл/выкл)
 * @param[in] role Режим работы (Master/Slave)
 * @param[in] mode Режим SPI (фаза/полярность)
 * @param[in] clockRate Скорость SPI (только для Master)
 * @param[in] dataOrder Порядок битов
 * @param[in] interrupt Разрешение прерываний
 */
void SPI_Init(SPI_State state, SPI_Role role, SPI_Mode mode, SPI_ClockRate clockRate, SPI_DataOrder dataOrder, SPI_Interrupt interrupt);

/**
 * @brief Минимальная инициализация SPI
 * @param[in] state Состояние SPI (вкл/выкл)
 */
void SPI_Init(SPI_State state);

/**
 * @brief Включение SPI
 */
void SPI_Enable(void);

/**
 * @brief Выключение SPI
 */
void SPI_Disable(void);

/**
 * @brief Установка режима работы (Master/Slave)
 * @param[in] role Режим работы
 */
void SPI_SetRole(SPI_Role role);

/**
 * @brief Передача одного байта
 * @param[in] data Байт для передачи
 */
void SPI_SendByte(uint8_t data);

/**
 * @brief Прием одного байта
 * @return Принятый байт
 */
uint8_t SPI_ReceiveByte(void);

/**
 * @brief Обмен одним байтом (передача + прием)
 * @param[in] data Байт для передачи
 * @return Принятый байт
 */
uint8_t SPI_TransferByte(uint8_t data);
