/**
 * @file		UART.h
 * @brief		Драйвер UART для AVR микроконтроллера AtMega32
 * @details		Поддерживает асинхронный режим работы с кольцевыми буферами
 * @note		-
 * @attention	ВСЕ ФУНКЦИИ ПРИЁМА/ПЕРЕДАЧИ ДАННЫХ БЛОКИРУЮЩИЕ
 * @warning		1. Не протестирован и неизвестно как работает "Обработчик прерывания передачи UART TX ISR(USART_TXC_vect)"
 * @warning		2. Мне до сих пор не понятно как работает call-back функция
 * @todo		-
 * @bug			-
 * @author		Николай Куркин
 * @date		2025-08-03
 * @version		1.0
 */

#include "UART.h"


//////////////////////////////////////////////////////////////////////////
//	Объявление кольцевых буферов для приема RX и передачи TX
//////////////////////////////////////////////////////////////////////////

/// @brief Буфер приема данных
static volatile uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];

/// @brief Буфер передачи данных
static volatile uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];

/// @brief Индекс головы буфера приема
static volatile uint8_t uart_rx_head = 0;

/// @brief Индекс хвоста буфера приема
static volatile uint8_t uart_rx_tail = 0;

/// @brief Индекс головы буфера передачи
static volatile uint8_t uart_tx_head = 0;

/// @brief Индекс хвоста буфера передачи
static volatile uint8_t uart_tx_tail = 0;


//////////////////////////////////////////////////////////////////////////
//	Объявление Callback-функции для UART RX
//////////////////////////////////////////////////////////////////////////

/// @brief Callback-функция для обработки принятых данных
static UART_RxCallback uart_rx_callback = NULL;


//////////////////////////////////////////////////////////////////////////
//	Функции UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Расширенная инициализация UART с указанием всех параметров
 * @param state Состояние UART (вкл/выкл)
 * @param dataBits Количество бит данных
 * @param stopBits Количество стоп-битов
 * @param parity Контроль четности
 * @param interrupt Режим прерываний
 */
void UART_Init(UART_State state, UART_DataBits dataBits, UART_StopBits stopBits, UART_Parity parity, UART_Interrupt interrupt)
{
	// Расчет коэффициента делителя
	uint16_t ubrr = UBRR_VALUE;
	UBRRH = (uint8_t)(ubrr >> 8);
	UBRRL = (uint8_t)ubrr;
	
	// Настройка формата кадра
	UCSRC = (1 << URSEL) | (parity << UPM0) | (stopBits << USBS) | (dataBits << UCSZ0);
	
	// Настройка прерываний и включение UART
	UCSRB = (1 << RXEN) | (1 << TXEN);
	
	// Для 9-битного режима
	if(dataBits == UART_9_BITS)
	{
		UCSRB |= (1 << UCSZ2);
	}
	
	// Настройка прерываний
	switch(interrupt) {
		case UART_INTERRUPT_RX_ENABLED:
		UCSRB |= (1 << RXCIE);
		break;
		case UART_INTERRUPT_TX_ENABLED:
		UCSRB |= (1 << TXCIE);
		break;
		case UART_INTERRUPT_BOTH_ENABLED:
		UCSRB |= (1 << RXCIE) | (1 << TXCIE);
		break;
		default:
		break;
	}
	
	// Включение/выключение UART
	if(state == UART_DISABLED) {
		UART_Disable();
	}
}

/**
 * @brief Упрощенная инициализация UART
 * @param state Состояние UART (вкл/выкл)
 * @note Использует настройки по умолчанию: 8 бит, 1 стоп-бит, без контроля четности
 */
void UART_Init(UART_State state)
{
	if(state == UART_ENABLED) {
		// Расчет коэффициента делителя для 9600 бод
		uint16_t ubrr = (F_CPU / (16UL * 9600)) - 1;
		UBRRH = (uint8_t)(ubrr >> 8);
		UBRRL = (uint8_t)ubrr;
		
		// Настройка формата кадра: 8 бит, 1 стоп-бит, без контроля четности
		UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
		
		// Включение приемника и передатчика + прерывание по приему
		UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
		} else {
		// Полное отключение UART
		UCSRB = 0;  // Отключаем прием, передачу и прерывания
	}
}

/**
 * @brief Включение UART
 */
void UART_Enable(void) 
{
    UCSRB |= (1 << RXEN) | (1 << TXEN);
}

/**
 * @brief Выключение UART
 */
void UART_Disable(void) 
{
    UCSRB &= ~((1 << RXEN) | (1 << TXEN));
}

/**
 * @brief Установка скорости UART
 * @param[in] baud Скорость в бодах
 */
void UART_SetBaudRate(uint32_t baud) 
{
    uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;
}

/**
 * @brief Передача одного символа
 * @param[in] c Символ для передачи
 */
void UART_SendChar(char c) 
{
    while(!(UCSRA & (1 << UDRE)));
    UDR = c;
}

/**
 * @brief Передача строки
 * @param[in] str Указатель на строку (завершается нулем \n)
 */
void UART_SendString(const char *str) 
{
    while(*str) {
        UART_SendChar(*str++);
    }
}

/**
 * @brief Передача массива данных
 * @param[in] buffer Указатель на буфер данных
 * @param[in] length Длина буфера
 */
void UART_SendBuffer(const uint8_t *buffer, uint16_t length) 
{
    for(uint16_t i = 0; i < length; i++) {
        UART_SendChar(buffer[i]);
    }
}

/**
 * @brief Прием одного символа
 * @return Принятый символ
 * @note Блокирующая функция (ждет появления данных)
 */
char UART_ReceiveChar(void) 
{
    while(!UART_DataAvailable());
    uint8_t data = uart_rx_buffer[uart_rx_tail];
    uart_rx_tail = (uart_rx_tail + 1) % UART_RX_BUFFER_SIZE;
    return data;
}

/**
 * @brief Прием массива данных
 * @param[out] buffer Буфер для приема данных
 * @param[in] length Максимальная длина приема
 * @return Количество фактически принятых байт
 */
uint8_t UART_ReceiveBuffer(uint8_t *buffer, uint16_t length) 
{
    uint16_t i;
    for(i = 0; i < length && UART_DataAvailable(); i++) {
        buffer[i] = uart_rx_buffer[uart_rx_tail];
        uart_rx_tail = (uart_rx_tail + 1) % UART_RX_BUFFER_SIZE;
    }
    return i;
}

/**
 * @brief Проверка наличия данных в буфере
 * @return Количество доступных для чтения байт
 */
uint8_t UART_DataAvailable(void) 
{
    return (uart_rx_head != uart_rx_tail);
}

/**
 * @brief Очистка буфера приема
 */
void UART_FlushRxBuffer(void) 
{
    uart_rx_head = uart_rx_tail = 0;
}

/**
 * @brief Установка callback-функции для приема данных
 * @param[in] callback Функция обратного вызова
 * @note Если callback не установлен, используется стандартная обработка через буфер
 */
void UART_SetRxCallback(UART_RxCallback callback)
{
	uart_rx_callback = callback;
}


//////////////////////////////////////////////////////////////////////////
//	Векторы прерываний UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Обработчик прерывания приема UART
 * @details Автоматически вызывается при получении данных
 */
ISR(USART_RXC_vect)
{
	char c = UDR;
	
	if (uart_rx_callback != NULL)
	{
		uart_rx_callback(c);
	}
	else
	{
		// Стандартная обработка (если callback не установлен)
		if (uart_rx_head != ((uart_rx_tail - 1) % UART_RX_BUFFER_SIZE))
		{
			uart_rx_buffer[uart_rx_head] = c;
			uart_rx_head = (uart_rx_head + 1) % UART_RX_BUFFER_SIZE;
		}
	}
}

/**
 * @brief Обработчик прерывания передачи UART
 * @details Автоматически вызывается при готовности передатчика
 */
ISR(USART_TXC_vect) 
{
	if (uart_tx_head != uart_tx_tail)
	{
		UDR = uart_tx_buffer[uart_tx_tail];
		uart_tx_tail = (uart_tx_tail + 1) % UART_TX_BUFFER_SIZE;
	}
}
