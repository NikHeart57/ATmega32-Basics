#include "SPI.h"

//////////////////////////////////////////////////////////////////////////
//  ФУНКЦИИ ИНИЦИАЛИЗАЦИИ SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Полная версия инициализации SPI
 * @param[in] state Состояние SPI (вкл/выкл)
 * @param[in] role Режим работы (Master/Slave)
 * @param[in] mode Режим SPI (фаза/полярность)
 * @param[in] clockRate Скорость SPI (только для Master)
 * @param[in] dataOrder Порядок битов
 * @param[in] interrupt Разрешение прерываний
 * 
 * @details Алгоритм инициализации:
 * 1. Настройка направления пинов SPI
 * 2. Конфигурация регистров управления SPI
 * 3. Установка скорости передачи (только для Master)
 * 4. Настройка прерываний (если требуется)
 * 
 * @warning Для режима Slave скорость игнорируется
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
 * @brief Минимальная инициализация SPI
 * @param[in] state Состояние SPI (вкл/выкл)
 * 
 * @details Только включение/выключение SPI без изменения других параметров.
 * Предполагается, что пины и режим уже настроены ранее.
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
//  ФУНКЦИИ УПРАВЛЕНИЯ SPI
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
//  ФУНКЦИИ РАБОТЫ С ДАННЫМИ
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Передача одного байта
 * @param[in] data Байт для передачи
 * 
 * @note Блокирует выполнение до завершения передачи
 */
void SPI_SendByte(uint8_t data)
{
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
}

/**
 * @brief Прием одного байта
 * @return Принятый байт
 * 
 * @note Блокирует выполнение до завершения приема
 * @note Для приема данных передается байт 0xFF
 */
uint8_t SPI_ReceiveByte(void)
{
    SPDR = 0xFF;
    while(!(SPSR & (1 << SPIF)));
    return SPDR;
}

/**
 * @brief Обмен одним байтом (передача + прием)
 * @param[in] data Байт для передачи
 * @return Принятый байт
 * 
 * @note Блокирует выполнение до завершения обмена
 */
uint8_t SPI_TransferByte(uint8_t data)
{
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
    return SPDR;
}