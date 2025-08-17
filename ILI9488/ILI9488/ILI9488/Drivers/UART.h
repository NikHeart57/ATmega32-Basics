#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

/**
 * @file		UART.h
 * @brief		Драйвер UART для AVR микроконтроллера AtMega32
 * @details		Поддерживает асинхронный режим работы с кольцевыми буферами
 * @note		-
 * @attention	ВСЕ ФУНКЦИИ ПРИЁМА ДАННЫХ БЛОКИРУЮЩИЕ
 * @warning		1. Не протестирован и неизвестно как работает "Обработчик прерывания передачи UART TX ISR(USART_TXC_vect)"
 * @warning		2. Мне до сих пор не понятно как работает call-back функция
 * @todo		-
 * @bug			-
 * @author		Николай Куркин, deepseek
 * @date		2025-08-03
 * @version		1.0
 */


//////////////////////////////////////////////////////////////////////////
//	Определения
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Частота процессора по умолчанию (1 МГц)
 * @note Может быть переопределена перед включением этого файла
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/**
 * @brief Скорость UART по умолчанию (9600 бод)
 */
#define BAUD 9600

/**
 * @brief Значение для регистра UBRR
 * @details Рассчитывается по формуле: (F_CPU / (16 * BAUD)) - 1
 */
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

/**
 * @brief Размеры буферов приёма и передачи (байт)
 */
#define UART_RX_BUFFER_SIZE 64			// Буффер приёмки uart_rx_buffer[UART_RX_BUFFER_SIZE];
#define UART_TX_BUFFER_SIZE 64			// Буффер передачи  uart_tx_buffer[UART_TX_BUFFER_SIZE];


//////////////////////////////////////////////////////////////////////////
//	Перечисление настроек UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Состояния UART
 */
typedef enum {
    UART_DISABLED = 0,
    UART_ENABLED
} UART_State;

/**
 * @brief Режимы битности данных
 */
typedef enum {
    UART_5_BITS = 0,
    UART_6_BITS,
    UART_7_BITS,
    UART_8_BITS,
    UART_9_BITS = 7  // Особый случай для UCSZ2
} UART_DataBits;

/**
 * @brief Количество стоп-битов
 */
typedef enum {
    UART_1_STOP_BIT = 0,
    UART_2_STOP_BITS
} UART_StopBits;

/**
 * @brief Режимы контроля четности
 */
typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN = 2,
    UART_PARITY_ODD = 3
} UART_Parity;

/**
 * @brief Режимы прерываний
 */
typedef enum {
    UART_INTERRUPT_DISABLED = 0,
    UART_INTERRUPT_RX_ENABLED,
    UART_INTERRUPT_TX_ENABLED,
    UART_INTERRUPT_BOTH_ENABLED
} UART_Interrupt;


//////////////////////////////////////////////////////////////////////////
//	Callback-функция для UART RX
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Тип callback-функции для обработки принятых данных
 * @param[in] c Принятый символ
 */
typedef void (*UART_RxCallback)(char c);


//////////////////////////////////////////////////////////////////////////
//	Прототипы функций UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Инициализация UART с указанными параметрами
 * @param[in] state Состояние UART (вкл/выкл)
 * @param[in] dataBits Количество бит данных
 * @param[in] stopBits Количество стоп-битов
 * @param[in] parity Контроль четности
 * @param[in] interrupt Режим прерываний
 */
void UART_Init(UART_State state, UART_DataBits dataBits, UART_StopBits stopBits, UART_Parity parity, UART_Interrupt interrupt);

/**
 * @brief Упрощенная инициализация UART
 * @param[in] state Состояние UART (вкл/выкл)
 * @note Используются настройки по умолчанию: 8 бит данных, 1 стоп-бит, без контроля четности
 */
void UART_Init(UART_State state);

/**
 * @brief Включение UART
 */
void UART_Enable(void);

/**
 * @brief Выключение UART
 */
void UART_Disable(void);

/**
 * @brief Установка скорости UART
 * @param[in] baud Скорость в бодах
 */
void UART_SetBaudRate(uint32_t baud);

/**
 * @brief Передача одного символа
 * @param[in] c Символ для передачи
 */
void UART_SendChar(char c);

/**
 * @brief Передача строки
 * @param[in] str Указатель на строку (завершается нулем \n)
 */
void UART_SendString(const char *str);

/**
 * @brief Передача массива данных
 * @param[in] buffer Указатель на буфер данных
 * @param[in] length Длина буфера
 */
void UART_SendBuffer(const uint8_t *buffer, uint16_t length);

/**
 * @brief Прием одного символа
 * @return Принятый символ
 * @note Блокирующая функция (ждет появления данных)
 */
char UART_ReceiveChar(void);

/**
 * @brief Прием массива данных
 * @param[out] buffer Буфер для приема данных
 * @param[in] length Максимальная длина приема
 * @return Количество фактически принятых байт
 */
uint8_t UART_ReceiveBuffer(uint8_t *buffer, uint16_t length);

/**
 * @brief Проверка наличия данных в буфере
 * @return Количество доступных для чтения байт
 */
uint8_t UART_DataAvailable(void);

/**
 * @brief Очистка буфера приема
 */
void UART_FlushRxBuffer(void);

/**
 * @brief Установка callback-функции для приема данных
 * @param[in] callback Функция обратного вызова
 * @note Если callback не установлен, используется стандартная обработка через буфер
 */
void UART_SetRxCallback(UART_RxCallback callback);

#endif /* UART_H_ */