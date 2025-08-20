/**
 * @note        ����������� ������������ ���������� deepseek 19.08.25
 *
 * @file        SPI.h
 * @brief       ������� SPI ��� AVR ���������������� AtMega32
 * @details     ������������� ������ ����� ������� ��� ������ � ����������� SPI
 *              � ������� Master/Slave � ���������� ���� ���������� ��������.
 * 
 * @note        �����������:
 *              - ��������� ������� Master � Slave
 *              - ��������� ��������, ������ ������, ������� �����
 *              - ����������� ������� ��������/������ ������
 *              - ��������� ����������
 * 
 * @warning     �����������:
 *              - ������� ��������/������ �������� ������������
 *              - � ������ Slave ��������� ������� ���������� ������ SS
 *              - ������������ �������� ������
 * 
 * @attention   ��� ������ ����������:
 *              - ����������� <avr/io.h> � <stdint.h>
 *              - ���������� ��������� ����� SPI � �������
 * 
 * @todo        ����������� ���������:
 *              - ������������� ������� � �������������� DMA
 *              - ����������� ����������� ������
 *              - ��������� ����� ������� ��������� ��������
 * 
 * @bug         ��������� ��������:
 *              - 
 * 
 * @author      ������� ������, deepseek
 * @date        2025-08-03
 * @version     0.9
 * 
 * @copyright   MIT License
 */
#pragma once

#include <avr/io.h>
#include <stdint.h>

//////////////////////////////////////////////////////////////////////////
//  ����������� ������ � ����� SPI
//////////////////////////////////////////////////////////////////////////

#define SPI_DDR     DDRB	///< ������� ����������� ������ ����� SPI
#define SPI_PORT    PORTB	///< ������� ������ ����� SPI
#define SPI_PIN     PINB	///< ������� ����� ����� SPI
#define SPI_SS      PB4		///< ��� ������ ���������� (Slave Select)
#define SPI_MOSI    PB5		///< ��� �������� ������ (Master Out Slave In)
#define SPI_MISO    PB6		///< ��� ������ ������ (Master In Slave Out)
#define SPI_SCK     PB7		///< ��� ��������� ������� (Serial Clock)

//////////////////////////////////////////////////////////////////////////
//  ������������ �������� SPI
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ��������� SPI (���/����)
 */
typedef enum {
    SPI_DISABLED = 0,  ///< SPI �������� (����������������)
    SPI_ENABLED         ///< SPI ������� � ����� � ������
} SPI_State;

/**
 * @brief ����� ������ (Master/Slave)
 */
typedef enum {
    SPI_SLAVE = 0,  ///< ����� �������� (Slave)
    SPI_MASTER       ///< ����� �������� (Master)
} SPI_Role;

/**
 * @brief ������ SPI (���� � ���������� ��������� �������)
 */
typedef enum {
    SPI_MODE0 = 0,  ///< CPOL=0, CPHA=0
    SPI_MODE1,      ///< CPOL=0, CPHA=1
    SPI_MODE2,      ///< CPOL=1, CPHA=0
    SPI_MODE3       ///< CPOL=1, CPHA=1
} SPI_Mode;

/**
 * @brief �������� SPI (�������� �������)
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
 * @brief ������� �������� �����
 */
typedef enum {
    SPI_MSB_FIRST = 0,  ///< ������� ��� ������ (��������)
    SPI_LSB_FIRST        ///< ������� ��� ������
} SPI_DataOrder;

/**
 * @brief ��������� ���������� SPI
 */
typedef enum {
    SPI_INTERRUPT_DISABLE = 0,  ///< ���������� ���������
    SPI_INTERRUPT_ENABLE         ///< ���������� ��������
} SPI_Interrupt;

//////////////////////////////////////////////////////////////////////////
//  ��������� ������� SPI
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
