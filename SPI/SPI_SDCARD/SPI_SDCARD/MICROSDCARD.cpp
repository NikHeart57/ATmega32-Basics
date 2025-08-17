/**
 * @file        MICROSDCARD.cpp
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

#include "MICROSDCARD.h"
#include <util/delay.h>

// ��������� �������
static void     SD_CS_Low(void);
static void     SD_CS_High(void);
static uint8_t  SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc);
static SD_Error SD_WaitReady(void);
static SD_Error SD_GetResponse(uint8_t response);
static void     SD_SendDummy(void);

// ������� SD �����
#define SD_CMD0     (0x40+0)    // GO_IDLE_STATE
#define SD_CMD8     (0x40+8)    // SEND_IF_COND
#define SD_CMD16    (0x40+16)   // SET_BLOCKLEN
#define SD_CMD17    (0x40+17)   // READ_SINGLE_BLOCK
#define SD_CMD24    (0x40+24)   // WRITE_BLOCK
#define SD_CMD55    (0x40+55)   // APP_CMD
#define SD_CMD58    (0x40+58)   // READ_OCR
#define SD_ACMD41   (0x40+41)   // SD_SEND_OP_COND (APP_CMD)

// ������ �����
#define SD_R1_IDLE_STATE        0x01
#define SD_R1_ILLEGAL_COMMAND   0x04
#define SD_DATA_START_BLOCK     0xFE
#define SD_DATA_ACCEPTED        0x05
#define SD_TOKEN_DATA_ACCEPTED  0x05

// ��������
#define SD_INIT_TIMEOUT     200     // 200 �������
#define SD_READ_TIMEOUT     300000  // ~300ms
#define SD_WRITE_TIMEOUT    300000  // ~300ms

//////////////////////////////////////////////////////////////////////////
// ������� ���������� CS
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ��������� CS � ������ ������� (��������� �����)
 */
static void SD_CS_Low(void)
{
    SD_CS_PORT &= ~(1 << SD_CS_PIN);
}

/**
 * @brief ��������� CS � ������� ������� (����������� �����)
 */
static void SD_CS_High(void)
{
    SD_CS_PORT |= (1 << SD_CS_PIN);
}

//////////////////////////////////////////////////////////////////////////
// ��������������� �������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief �������� "������" ������ (��� �������������)
 */
static void SD_SendDummy(void)
{
    SPI_TransferByte(0xFF);
}

/**
 * @brief �������� ���������� �����
 * @return SD_OK - ����� ������, SD_ERROR_CMD - �������
 */
static SD_Error SD_WaitReady(void)
{
    uint32_t timeout = SD_WRITE_TIMEOUT;
    
    while(SPI_ReceiveByte() != 0xFF) {
        if(--timeout == 0) {
            return SD_ERROR_CMD;
        }
    }
    
    return SD_OK;
}

/**
 * @brief �������� ����������� ������ �� �����
 * @param[in] response ��������� �����
 * @return SD_OK - ������� ��������� �����, SD_ERROR_CMD - ������ ����� ��� �������
 */
static SD_Error SD_GetResponse(uint8_t response)
{
    uint16_t timeout = 0xFFFF;
    uint8_t res;
    
    do {
        res = SPI_ReceiveByte();
    } while((res == 0xFF) && (--timeout));
    
    if(res != response) {
        return SD_ERROR_CMD;
    }
    
    return SD_OK;
}

/**
 * @brief �������� ������� SD �����
 * @param[in] cmd �������
 * @param[in] arg �������� �������
 * @param[in] crc CRC �������
 * @return ����� R1 �� �����
 */
static uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t res;
    
    // ������� ���������� �����
    SD_WaitReady();
    
    // ���������� �������
    SPI_SendByte(cmd);                     // �������
    SPI_SendByte((uint8_t)(arg >> 24));    // �������� [31..24]
    SPI_SendByte((uint8_t)(arg >> 16));    // �������� [23..16]
    SPI_SendByte((uint8_t)(arg >> 8));     // �������� [15..8]
    SPI_SendByte((uint8_t)arg);            // �������� [7..0]
    SPI_SendByte(crc);                     // CRC
    
    // ��� CMD0 � CMD8 ����� ��������� �������������� ������ �����
    if(cmd == SD_CMD0 || cmd == SD_CMD8) {
        SD_SendDummy();
    }
    
    // �������� ����� (����� ������������� ��������� �������)
    uint8_t n = 10;
    do {
        res = SPI_ReceiveByte();
    } while((res & 0x80) && --n);
    
    return res;
}

//////////////////////////////////////////////////////////////////////////
// ��������� �������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ������������� microSD �����
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_Init(void)
{
    uint8_t res, n;
    SD_CardType card_type = SD_CARD_TYPE_UNKNOWN;
    
    // ��������� ���� CS
    SD_CS_DDR |= (1 << SD_CS_PIN);
    SD_CS_High();
    
    // ���������� 10 ������ ������ ��� �������������
    for(n = 0; n < 10; n++) {
        SD_SendDummy();
    }
    
    // ���������� �����
    SD_CS_Low();
    
    // ���������� CMD0 (����� �����)
    res = SD_SendCmd(SD_CMD0, 0, 0x95);
    if(res != 1) {
        SD_CS_High();
        return SD_ERROR_INIT;
    }
    
    // ���������� CMD8 ��� �������� ������ �����
    res = SD_SendCmd(SD_CMD8, 0x1AA, 0x87);
    if(res == 1) {
        // �������� ����� (4 �����)
        uint8_t ocr[4];
        for(n = 0; n < 4; n++) {
            ocr[n] = SPI_ReceiveByte();
        }
        
        // ��������� ��������� ����������
        if(ocr[2] == 0x01 && ocr[3] == 0xAA) {
            // ����� SDv2
            for(n = 0; n < SD_INIT_TIMEOUT; n++) {
                res = SD_SendCmd(SD_CMD55, 0, 0x65);
                if(res > 1) {
                    SD_CS_High();
                    return SD_ERROR_INIT;
                }
                
                res = SD_SendCmd(SD_ACMD41, 0x40000000, 0x77);
                if(res == 0) {
                    break;
                }
                _delay_ms(10);
            }
            
            if(n == SD_INIT_TIMEOUT) {
                SD_CS_High();
                return SD_ERROR_INIT;
            }
            
            // ��������� ��� ����� (SDHC ��� ������� SDv2)
            res = SD_SendCmd(SD_CMD58, 0, 0xFD);
            if(res != 0) {
                SD_CS_High();
                return SD_ERROR_INIT;
            }
            
            for(n = 0; n < 4; n++) {
                ocr[n] = SPI_ReceiveByte();
            }
            
            if(ocr[0] & 0x40) {
                card_type = SD_CARD_TYPE_SDHC;
            } else {
                card_type = SD_CARD_TYPE_SD2;
            }
        }
    } else {
        // ����� SDv1 ��� MMC
        if(SD_SendCmd(SD_CMD55, 0, 0x65) > 1) {
            card_type = SD_CARD_TYPE_SD1;
        } else {
            SD_CS_High();
            return SD_ERROR_INIT;
        }
        
        for(n = 0; n < SD_INIT_TIMEOUT; n++) {
            res = SD_SendCmd(SD_ACMD41, 0, 0x77);
            if(res == 0) {
                break;
            }
            _delay_ms(10);
        }
        
        if(n == SD_INIT_TIMEOUT) {
            SD_CS_High();
            return SD_ERROR_INIT;
        }
        
        card_type = SD_CARD_TYPE_SD1;
    }
    
    // ������������� ������ ����� 512 ����
    if(SD_SendCmd(SD_CMD16, 512, 0x15) != 0) {
        SD_CS_High();
        return SD_ERROR_INIT;
    }
    
    SD_CS_High();
    return SD_OK;
}

/**
 * @brief ������ ������ ����� ������ (512 ����)
 * @param[in] block_num ����� �����
 * @param[out] buffer ����� ��� ������ (������� 512 ����)
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_ReadBlock(uint32_t block_num, uint8_t* buffer)
{
    uint16_t n;
    uint8_t res;
    
    // ��� SDHC ����� ����������� � ������, ��� SDSC - � ������
    if(SD_SendCmd(SD_CMD17, block_num, 0xFF) != 0) {
        SD_CS_High();
        return SD_ERROR_READ;
    }
    
    // ������� ������ ����� ������
    res = SPI_ReceiveByte();
    if(res != SD_DATA_START_BLOCK) {
        SD_CS_High();
        return SD_ERROR_READ;
    }
    
    // ������ ������
    for(n = 0; n < 512; n++) {
        buffer[n] = SPI_ReceiveByte();
    }
    
    // ������ CRC (2 �����, ����������)
    SPI_ReceiveByte();
    SPI_ReceiveByte();
    
    SD_CS_High();
    return SD_OK;
}

/**
 * @brief ������ ������ ����� ������ (512 ����)
 * @param[in] block_num ����� �����
 * @param[in] buffer ����� � ������� (������� 512 ����)
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_WriteBlock(uint32_t block_num, const uint8_t* buffer)
{
    uint16_t n;
    uint8_t res;
    
    // ��� SDHC ����� ����������� � ������, ��� SDSC - � ������
    if(SD_SendCmd(SD_CMD24, block_num, 0xFF) != 0)
	 
	{
        SD_CS_High();
        return SD_ERROR_WRITE;
    }
    
    // ���������� ������ ������ �����
    SPI_SendByte(SD_DATA_START_BLOCK);
    
    // ���������� ������
    for(n = 0; n < 512; n++) 
	{
        SPI_SendByte(buffer[n]);
    }
    
    // ���������� CRC (2 �����, ���������)
    SPI_SendByte(0xFF);
    SPI_SendByte(0xFF);
    
    // �������� ����� �����
    res = SPI_ReceiveByte();
    if((res & 0x1F) != SD_TOKEN_DATA_ACCEPTED) 
	{
        SD_CS_High();
        return SD_ERROR_WRITE;
    }
    
    // ������� ���������� ������
    if(SD_WaitReady() != SD_OK) 
	{
        SD_CS_High();
        return SD_ERROR_WRITE;
    }
    
    SD_CS_High();
    return SD_OK;
}

/**
 * @brief ��������� ���������� � �����
 * @param[out] info ��������� �� ��������� � �����������
 * @return ��� ������ (SD_Error)
 */
SD_Error SD_GetCardInfo(SD_CardInfo* info)
{
    // �������� - � �������� ���������� ����� ������ �������� �����
    // � ��������� ��������� �����������
    info->type = SD_CARD_TYPE_SD2;
    info->size = 4096; // ������ ������� - 4GB
    info->oem_id = 0;
    info->manufacturer_id = 0;
    
    return SD_OK;
}

/**
 * @brief �������� ������� �����
 * @return true - ����� ������������, false - �����������
 */
uint8_t SD_IsPresent(void)
{
    // � �������� ���������� ����� �������� �������� ������� �����
    // ����� �������� ����� (���� �� ���� � �����)
    return 1;
}