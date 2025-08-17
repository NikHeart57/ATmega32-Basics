#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

/**
 * @file		SPI.h
 * @brief		������� SPI ��� AVR ���������������� AtMega32
 * @details		������������ ������ Master/Slave � ���������� ���������� ��������
 * @note		��� ������ ��������� ��������� ��������� ���� SPI � �������
 * @attention	������� ��������/������ ������ �������� ������������
 * @warning		� ������ Slave ���������� ������� ���������� ������ SS
 * @todo		�� ����������� ����� ����������� SPI
 * @bug
 * @author		������� ������, deepseek
 * @date		2025-08-03
 * @version		1.0
 */


//////////////////////////////////////////////////////////////////////////
//	����������� ������ � ����� SPI
//////////////////////////////////////////////////////////////////////////
#define SPI_DDR     DDRB
#define SPI_PORT    PORTB
#define SPI_PIN     PINB
#define SPI_SS      PB4
#define SPI_MOSI    PB5
#define SPI_MISO    PB6
#define SPI_SCK     PB7


//////////////////////////////////////////////////////////////////////////
//	������������ �������� SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ��������� SPI (���/����)
 */
typedef enum {
	SPI_DISABLED = 0,
	SPI_ENABLED
} SPI_State;

/**
 * @brief ����� ������ (Master/Slave)
 */
typedef enum {
	SPI_SLAVE = 0,
	SPI_MASTER
} SPI_Role;


/**
 * @brief ������ SPI (���� � ���������� ��������� �������)
 */
typedef enum {
	SPI_MODE0 = 0,  // CPOL=0, CPHA=0
	SPI_MODE1,      // CPOL=0, CPHA=1
	SPI_MODE2,      // CPOL=1, CPHA=0
	SPI_MODE3       // CPOL=1, CPHA=1
} SPI_Mode;

/**
 * @brief �������� SPI (�������� �������)
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
 * @brief ������� �������� �����
 */
typedef enum {
	SPI_MSB_FIRST = 0,
	SPI_LSB_FIRST
} SPI_DataOrder;

/**
 * @brief ��������� ���������� SPI
 */
typedef enum {
	SPI_INTERRUPT_DISABLE = 0,
	SPI_INTERRUPT_ENABLE
} SPI_Interrupt;


//////////////////////////////////////////////////////////////////////////
//	��������� ������� SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ������ ������������� SPI
 * @param[in] state ��������� SPI (���/����)
 * @param[in] role ����� ������ (Master/Slave)
 * @param[in] mode ����� SPI (����/����������)
 * @param[in] clockRate �������� SPI (������ ��� Master)
 * @param[in] dataOrder ������� �����
 * @param[in] interrupt ���������� ����������
 */
void SPI_Init(SPI_State state, SPI_Role role, SPI_Mode mode, SPI_ClockRate clockRate, SPI_DataOrder dataOrder, SPI_Interrupt interrupt);

/**
 * @brief ���������� ������������� SPI (Master �����)
 * @param[in] mode ����� SPI
 * @param[in] clockRate �������� SPI
 * @param[in] dataOrder ������� �����
 */
void SPI_Init(SPI_Mode mode, SPI_ClockRate clockRate, SPI_DataOrder dataOrder);

/**
 * @brief ����������� ������������� SPI
 * @param[in] state ��������� SPI (���/����)
 */
void SPI_Init(SPI_State state);

/**
 * @brief ��������� SPI
 */
void SPI_Enable(void);

/**
 * @brief ���������� SPI
 */
void SPI_Disable(void);

/**
 * @brief ��������� ������ ������ (Master/Slave)
 * @param[in] role ����� ������
 */
void SPI_SetRole(SPI_Role role);

/*
void SPI_SetDataOrder(SPI_DataOrder dataOrder);
void SPI_SetClockRate(SPI_ClockRate clockRate);
void SPI_SetMode(SPI_Mode mode);
void SPI_InterruptEnable(SPI_Interrupt interrupt);
*/

/**
 * @brief �������� ������ �����
 * @param[in] data ���� ��� ��������
 */
void SPI_SendByte(uint8_t data);

/**
 * @brief ����� ������ �����
 * @return �������� ����
 */
uint8_t SPI_ReceiveByte(void);

/**
 * @brief ����� ����� ������ (�������� + �����)
 * @param[in] data ���� ��� ��������
 * @return �������� ����
 */
uint8_t SPI_TransferByte(uint8_t data);

/*
void SPI_SendBuffer(uint8_t *buffer, uint16_t length);
void SPI_ReceiveBuffer(uint8_t *buffer, uint16_t length);
void SPI_TransferBuffer(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t length);
*/

#endif /* SPI_H_ */