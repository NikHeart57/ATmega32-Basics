#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

/**
 * @file		SPI.h
 * @brief		Драйвер SPI для AVR микроконтроллера AtMega32
 * @details		Поддерживает режимы Master/Slave с настройкой параметров передачи
 * @note		Для работы требуется правильно настроить пины SPI в системе
 * @attention	Функции передачи/приема данных являются блокирующими
 * @warning		В режиме Slave необходимо внешнее управление линией SS
 * @todo		Не реализована часть функционала SPI
 * @bug
 * @author		Николай Куркин, deepseek
 * @date		2025-08-03
 * @version		1.0
 */


//////////////////////////////////////////////////////////////////////////
//	Определения портов и пинов SPI
//////////////////////////////////////////////////////////////////////////
#define SPI_DDR     DDRB
#define SPI_PORT    PORTB
#define SPI_PIN     PINB
#define SPI_SS      PB4
#define SPI_MOSI    PB5
#define SPI_MISO    PB6
#define SPI_SCK     PB7


//////////////////////////////////////////////////////////////////////////
//	Перечисление настроек SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Состояние SPI (вкл/выкл)
 */
typedef enum {
	SPI_DISABLED = 0,
	SPI_ENABLED
} SPI_State;

/**
 * @brief Режим работы (Master/Slave)
 */
typedef enum {
	SPI_SLAVE = 0,
	SPI_MASTER
} SPI_Role;


/**
 * @brief Режимы SPI (фаза и полярность тактового сигнала)
 */
typedef enum {
	SPI_MODE0 = 0,  // CPOL=0, CPHA=0
	SPI_MODE1,      // CPOL=0, CPHA=1
	SPI_MODE2,      // CPOL=1, CPHA=0
	SPI_MODE3       // CPOL=1, CPHA=1
} SPI_Mode;

/**
 * @brief Скорость SPI (делитель частоты)
 */
typedef enum {
	SPI_CLK_DIV4 = 0,
	SPI_CLK_DIV16,
	SPI_CLK_DIV64,
	SPI_CLK_DIV128,
	SPI_CLK_DIV2,
	SPI_CLK_DIV8,
	SPI_CLK_DIV32
} SPI_ClockRate;

/**
 * @brief Порядок передачи битов
 */
typedef enum {
	SPI_MSB_FIRST = 0,
	SPI_LSB_FIRST
} SPI_DataOrder;

/**
 * @brief Состояние прерываний SPI
 */
typedef enum {
	SPI_INTERRUPT_DISABLE = 0,
	SPI_INTERRUPT_ENABLE
} SPI_Interrupt;


//////////////////////////////////////////////////////////////////////////
//	Прототипы функций SPI
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
 * @brief Упрощенная инициализация SPI (Master режим)
 * @param[in] mode Режим SPI
 * @param[in] clockRate Скорость SPI
 * @param[in] dataOrder Порядок битов
 */
void SPI_Init(SPI_Mode mode, SPI_ClockRate clockRate, SPI_DataOrder dataOrder);

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

/*
void SPI_SetDataOrder(SPI_DataOrder dataOrder);
void SPI_SetClockRate(SPI_ClockRate clockRate);
void SPI_SetMode(SPI_Mode mode);
void SPI_InterruptEnable(SPI_Interrupt interrupt);
*/

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

/*
void SPI_SendBuffer(uint8_t *buffer, uint16_t length);
void SPI_ReceiveBuffer(uint8_t *buffer, uint16_t length);
void SPI_TransferBuffer(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t length);
*/

#endif /* SPI_H_ */