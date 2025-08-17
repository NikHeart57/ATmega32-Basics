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

#include "SPI.h"


//////////////////////////////////////////////////////////////////////////
//	Функции инициализации SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Полная версия инициализации SPI
 * @details Настраивает все параметры SPI интерфейса
 */
void SPI_Init(SPI_State state, SPI_Role role, SPI_Mode mode, SPI_ClockRate clockRate, SPI_DataOrder dataOrder, SPI_Interrupt interrupt)
{
	// Настройка пинов
	if(role == SPI_MASTER) {
		SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
		SPI_DDR &= ~(1 << SPI_MISO);
		SPI_PORT |= (1 << SPI_SS);
		} else {
		SPI_DDR |= (1 << SPI_MISO);
		SPI_DDR &= ~((1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS));
	}

	// Настройка регистров
	SPCR = ((state == SPI_ENABLED) << SPE) |
	((role == SPI_MASTER) << MSTR) |
	((dataOrder == SPI_LSB_FIRST) << DORD) |
	((mode & 0x02) << CPOL) |
	((mode & 0x01) << CPHA) |
	((interrupt == SPI_INTERRUPT_ENABLE) << SPIE);

	// Настройка скорости (только для Master)
	if(role == SPI_MASTER) {
		switch(clockRate) {
			case SPI_CLK_DIV2:
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR |= (1 << SPI2X);
			break;
			case SPI_CLK_DIV4:
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR &= ~(1 << SPI2X);
			break;
			case SPI_CLK_DIV8:
			SPCR &= ~(1 << SPR1);
			SPCR |= (1 << SPR0);
			SPSR |= (1 << SPI2X);
			break;
			case SPI_CLK_DIV16:
			SPCR &= ~(1 << SPR1);
			SPCR |= (1 << SPR0);
			SPSR &= ~(1 << SPI2X);
			break;
			case SPI_CLK_DIV32:
			SPCR |= (1 << SPR1);
			SPCR &= ~(1 << SPR0);
			SPSR |= (1 << SPI2X);
			break;
			case SPI_CLK_DIV64:
			SPCR |= (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
			case SPI_CLK_DIV128:
			SPCR |= (1 << SPR1) | (1 << SPR0);
			SPSR &= ~(1 << SPI2X);
			break;
		}
	}
}


/**
 * @brief Упрощенная инициализация SPI (Master режим)
 * @details Используется для быстрой настройки SPI в режиме Master
 */
void SPI_Init(SPI_Mode mode, SPI_ClockRate clockRate, SPI_DataOrder dataOrder)
{
	// Настройка пинов SPI
	SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);  // Выходы
	SPI_DDR &= ~(1 << SPI_MISO);                                  // Вход
	SPI_PORT |= (1 << SPI_SS);                                    // SS высокий уровень (неактивен)
	
	// Настройка регистра SPI
	SPCR = (1 << SPE) |               // Разрешение SPI
	(1 << MSTR) |               // Режим мастера
	((dataOrder == SPI_LSB_FIRST) << DORD) |  // Порядок битов
	((mode & 0x02) << CPOL) |  // Настройка CPOL (бит 2 mode)
	((mode & 0x01) << CPHA);   // Настройка CPHA (бит 1 mode)
	
	// Настройка скорости SPI
	switch(clockRate) {
		case SPI_CLK_DIV2:
		SPCR &= ~((1 << SPR1) | (1 << SPR0));
		SPSR |= (1 << SPI2X);
		break;
		case SPI_CLK_DIV4:
		SPCR &= ~((1 << SPR1) | (1 << SPR0));
		SPSR &= ~(1 << SPI2X);
		break;
		case SPI_CLK_DIV8:
		SPCR &= ~(1 << SPR1);
		SPCR |= (1 << SPR0);
		SPSR |= (1 << SPI2X);
		break;
		case SPI_CLK_DIV16:
		SPCR &= ~(1 << SPR1);
		SPCR |= (1 << SPR0);
		SPSR &= ~(1 << SPI2X);
		break;
		case SPI_CLK_DIV32:
		SPCR |= (1 << SPR1);
		SPCR &= ~(1 << SPR0);
		SPSR |= (1 << SPI2X);
		break;
		case SPI_CLK_DIV64:
		SPCR |= (1 << SPR1);
		SPSR &= ~(1 << SPI2X);
		break;
		case SPI_CLK_DIV128:
		SPCR |= (1 << SPR1) | (1 << SPR0);
		SPSR &= ~(1 << SPI2X);
		break;
	}
}


/**
 * @brief Минимальная инициализация SPI
 * @details Только включение/выключение SPI без изменения других параметров
 */
void SPI_Init(SPI_State state)
{
	if(state == SPI_ENABLED) {
		SPCR |= (1 << SPE);
		} else {
		SPCR &= ~(1 << SPE);
	}
}


//////////////////////////////////////////////////////////////////////////
//	Функции управления SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Включение SPI
 */
void SPI_Enable(void) 
{ 
	SPCR |= (1 << SPE); 
}


/**
 * @brief Выключение SPI
 */
void SPI_Disable(void) 
{ 
	SPCR &= ~(1 << SPE); 
}


/**
 * @brief Установка режима работы (Master/Slave)
 * @param[in] role Режим работы
 */
void SPI_SetRole(SPI_Role role) {
	if(role == SPI_MASTER) {
		SPCR |= (1 << MSTR);
		SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
		} else {
		SPCR &= ~(1 << MSTR);
		SPI_DDR &= ~((1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS));
		SPI_DDR |= (1 << SPI_MISO);
	}
}



//////////////////////////////////////////////////////////////////////////
//	Функции работы с данными
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Передача одного байта
 * @param[in] data Байт для передачи
 */
void SPI_SendByte(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
}

/**
 * @brief Прием одного байта
 * @return Принятый байт
 */
uint8_t SPI_ReceiveByte(void)
{
	// Для приема данных нужно отправить "пустой" байт
	SPDR = 0xFF;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}

/**
 * @brief Обмен одним байтом (передача + прием)
 * @param[in] data Байт для передачи
 * @return Принятый байт
 */
uint8_t SPI_TransferByte(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}










