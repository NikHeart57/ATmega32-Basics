#ifndef MICROSDCARD_H_
#define MICROSDCARD_H_

#include <avr/io.h>
#include <stdint.h>
#include "Drivers/SPI.h"

/**
 * @file        MICROSDCARD.h
 * @brief       ������� ��� ������ � microSD ������ ����� SPI
 * @details     ������������ ������� �������� ������/������ � microSD ������
 * @note        ��������� ��������������� ������������� SPI ����������
 * @attention   ��� ������ ���������� ��������� ���������� ����� (CS �� PB1)
 * @warning     �������������� ������ ����� SPI (�� SDIO)
 * @todo        ����������� ��������� SDHC/SDXC ����
 * @bug
 * @author      ������� ������, deepseek
 * @date        2025-08-14
 * @version     1.0
 */

// ����������� ���� CS ��� microSD �����
#define SD_CS_PORT  PORTB
#define SD_CS_DDR   DDRB
#define SD_CS_PIN   PB1

// ���� ����
typedef enum {
    SD_CARD_TYPE_UNKNOWN = 0,
    SD_CARD_TYPE_SD1,
    SD_CARD_TYPE_SD2,
    SD_CARD_TYPE_SDHC
} SD_CardType;

// ������ ������ � ������
typedef enum {
    SD_OK = 0,
    SD_ERROR_INIT,
    SD_ERROR_CMD,
    SD_ERROR_READ,
    SD_ERROR_WRITE,
    SD_ERROR_NOT_PRESENT,
    SD_ERROR_BAD_CSD
} SD_Error;

// ��������� ���������� � �����
typedef struct {
    SD_CardType type;
    uint32_t size;      // ������ � ����������
    uint16_t oem_id;
    uint8_t  manufacturer_id;
} SD_CardInfo;

//////////////////////////////////////////////////////////////////////////
// ��������� �������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ������������� microSD �����
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_Init(void);

/**
 * @brief ��������� ���������� � �����
 * @param[out] info ��������� �� ��������� � �����������
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_GetCardInfo(SD_CardInfo* info);

/**
 * @brief ������ ������ ����� ������ (512 ����)
 * @param[in] block_num ����� �����
 * @param[out] buffer ����� ��� ������ (������� 512 ����)
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_ReadBlock(uint32_t block_num, uint8_t* buffer);

/**
 * @brief ������ ������ ����� ������ (512 ����)
 * @param[in] block_num ����� �����
 * @param[in] buffer ����� � ������� (������� 512 ����)
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_WriteBlock(uint32_t block_num, const uint8_t* buffer);

/**
 * @brief �������� ������� �����
 * @return true - ����� ������������, false - �����������
 */
uint8_t SD_IsPresent(void);

#endif /* MICROSDCARD_H_ */