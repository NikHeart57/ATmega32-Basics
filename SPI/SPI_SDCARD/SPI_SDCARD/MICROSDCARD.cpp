/**
 * @file        MICROSDCARD.cpp
 * @brief       Драйвер для работы с microSD картой через SPI
 * @details     Обеспечивает базовые операции чтения/записи с microSD картой
 * @note        Требуется предварительная инициализация SPI интерфейса
 * @attention   Для работы необходимо правильно подключить карту (CS на PB1)
 * @warning     Поддерживается только режим SPI (не SDIO)
 * @todo        Реализовать поддержку SDHC/SDXC карт
 * @bug
 * @author      Николай Куркин, deepseek
 * @date        2025-08-14
 * @version     1.0
 */

#include "MICROSDCARD.h"
#include <util/delay.h>

// Приватные функции
static void     SD_CS_Low(void);
static void     SD_CS_High(void);
static uint8_t  SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc);
static SD_Error SD_WaitReady(void);
static SD_Error SD_GetResponse(uint8_t response);
static void     SD_SendDummy(void);

// Команды SD карты
#define SD_CMD0     (0x40+0)    // GO_IDLE_STATE
#define SD_CMD8     (0x40+8)    // SEND_IF_COND
#define SD_CMD16    (0x40+16)   // SET_BLOCKLEN
#define SD_CMD17    (0x40+17)   // READ_SINGLE_BLOCK
#define SD_CMD24    (0x40+24)   // WRITE_BLOCK
#define SD_CMD55    (0x40+55)   // APP_CMD
#define SD_CMD58    (0x40+58)   // READ_OCR
#define SD_ACMD41   (0x40+41)   // SD_SEND_OP_COND (APP_CMD)

// Ответы карты
#define SD_R1_IDLE_STATE        0x01
#define SD_R1_ILLEGAL_COMMAND   0x04
#define SD_DATA_START_BLOCK     0xFE
#define SD_DATA_ACCEPTED        0x05
#define SD_TOKEN_DATA_ACCEPTED  0x05

// Таймауты
#define SD_INIT_TIMEOUT     200     // 200 попыток
#define SD_READ_TIMEOUT     300000  // ~300ms
#define SD_WRITE_TIMEOUT    300000  // ~300ms

//////////////////////////////////////////////////////////////////////////
// Функции управления CS
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Установка CS в низкий уровень (активация карты)
 */
static void SD_CS_Low(void)
{
    SD_CS_PORT &= ~(1 << SD_CS_PIN);
}

/**
 * @brief Установка CS в высокий уровень (деактивация карты)
 */
static void SD_CS_High(void)
{
    SD_CS_PORT |= (1 << SD_CS_PIN);
}

//////////////////////////////////////////////////////////////////////////
// Вспомогательные функции
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Отправка "пустых" байтов (для синхронизации)
 */
static void SD_SendDummy(void)
{
    SPI_TransferByte(0xFF);
}

/**
 * @brief Ожидание готовности карты
 * @return SD_OK - карта готова, SD_ERROR_CMD - таймаут
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
 * @brief Ожидание конкретного ответа от карты
 * @param[in] response Ожидаемый ответ
 * @return SD_OK - получен ожидаемый ответ, SD_ERROR_CMD - другой ответ или таймаут
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
 * @brief Отправка команды SD карте
 * @param[in] cmd Команда
 * @param[in] arg Аргумент команды
 * @param[in] crc CRC команды
 * @return Ответ R1 от карты
 */
static uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t res;
    
    // Ожидаем готовности карты
    SD_WaitReady();
    
    // Отправляем команду
    SPI_SendByte(cmd);                     // Команда
    SPI_SendByte((uint8_t)(arg >> 24));    // Аргумент [31..24]
    SPI_SendByte((uint8_t)(arg >> 16));    // Аргумент [23..16]
    SPI_SendByte((uint8_t)(arg >> 8));     // Аргумент [15..8]
    SPI_SendByte((uint8_t)arg);            // Аргумент [7..0]
    SPI_SendByte(crc);                     // CRC
    
    // Для CMD0 и CMD8 нужно отправить дополнительные пустые байты
    if(cmd == SD_CMD0 || cmd == SD_CMD8) {
        SD_SendDummy();
    }
    
    // Получаем ответ (может потребоваться несколько попыток)
    uint8_t n = 10;
    do {
        res = SPI_ReceiveByte();
    } while((res & 0x80) && --n);
    
    return res;
}

//////////////////////////////////////////////////////////////////////////
// Публичные функции
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Инициализация microSD карты
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_Init(void)
{
    uint8_t res, n;
    SD_CardType card_type = SD_CARD_TYPE_UNKNOWN;
    
    // Настройка пина CS
    SD_CS_DDR |= (1 << SD_CS_PIN);
    SD_CS_High();
    
    // Отправляем 10 пустых байтов для синхронизации
    for(n = 0; n < 10; n++) {
        SD_SendDummy();
    }
    
    // Активируем карту
    SD_CS_Low();
    
    // Отправляем CMD0 (сброс карты)
    res = SD_SendCmd(SD_CMD0, 0, 0x95);
    if(res != 1) {
        SD_CS_High();
        return SD_ERROR_INIT;
    }
    
    // Отправляем CMD8 для проверки версии карты
    res = SD_SendCmd(SD_CMD8, 0x1AA, 0x87);
    if(res == 1) {
        // Получаем ответ (4 байта)
        uint8_t ocr[4];
        for(n = 0; n < 4; n++) {
            ocr[n] = SPI_ReceiveByte();
        }
        
        // Проверяем поддержку напряжения
        if(ocr[2] == 0x01 && ocr[3] == 0xAA) {
            // Карта SDv2
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
            
            // Проверяем тип карты (SDHC или обычная SDv2)
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
        // Карта SDv1 или MMC
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
    
    // Устанавливаем размер блока 512 байт
    if(SD_SendCmd(SD_CMD16, 512, 0x15) != 0) {
        SD_CS_High();
        return SD_ERROR_INIT;
    }
    
    SD_CS_High();
    return SD_OK;
}

/**
 * @brief Чтение одного блока данных (512 байт)
 * @param[in] block_num Номер блока
 * @param[out] buffer Буфер для данных (минимум 512 байт)
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_ReadBlock(uint32_t block_num, uint8_t* buffer)
{
    uint16_t n;
    uint8_t res;
    
    // Для SDHC адрес указывается в блоках, для SDSC - в байтах
    if(SD_SendCmd(SD_CMD17, block_num, 0xFF) != 0) {
        SD_CS_High();
        return SD_ERROR_READ;
    }
    
    // Ожидаем начало блока данных
    res = SPI_ReceiveByte();
    if(res != SD_DATA_START_BLOCK) {
        SD_CS_High();
        return SD_ERROR_READ;
    }
    
    // Читаем данные
    for(n = 0; n < 512; n++) {
        buffer[n] = SPI_ReceiveByte();
    }
    
    // Читаем CRC (2 байта, игнорируем)
    SPI_ReceiveByte();
    SPI_ReceiveByte();
    
    SD_CS_High();
    return SD_OK;
}

/**
 * @brief Запись одного блока данных (512 байт)
 * @param[in] block_num Номер блока
 * @param[in] buffer Буфер с данными (минимум 512 байт)
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_WriteBlock(uint32_t block_num, const uint8_t* buffer)
{
    uint16_t n;
    uint8_t res;
    
    // Для SDHC адрес указывается в блоках, для SDSC - в байтах
    if(SD_SendCmd(SD_CMD24, block_num, 0xFF) != 0)
	 
	{
        SD_CS_High();
        return SD_ERROR_WRITE;
    }
    
    // Отправляем маркер начала блока
    SPI_SendByte(SD_DATA_START_BLOCK);
    
    // Отправляем данные
    for(n = 0; n < 512; n++) 
	{
        SPI_SendByte(buffer[n]);
    }
    
    // Отправляем CRC (2 байта, фиктивные)
    SPI_SendByte(0xFF);
    SPI_SendByte(0xFF);
    
    // Получаем ответ карты
    res = SPI_ReceiveByte();
    if((res & 0x1F) != SD_TOKEN_DATA_ACCEPTED) 
	{
        SD_CS_High();
        return SD_ERROR_WRITE;
    }
    
    // Ожидаем завершения записи
    if(SD_WaitReady() != SD_OK) 
	{
        SD_CS_High();
        return SD_ERROR_WRITE;
    }
    
    SD_CS_High();
    return SD_OK;
}

/**
 * @brief Получение информации о карте
 * @param[out] info Указатель на структуру с информацией
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_GetCardInfo(SD_CardInfo* info)
{
    // Заглушка - в реальной реализации нужно читать регистры карты
    // и заполнять структуру информацией
    info->type = SD_CARD_TYPE_SD2;
    info->size = 4096; // Пример размера - 4GB
    info->oem_id = 0;
    info->manufacturer_id = 0;
    
    return SD_OK;
}

/**
 * @brief Проверка наличия карты
 * @return true - карта присутствует, false - отсутствует
 */
uint8_t SD_IsPresent(void)
{
    // В реальной реализации можно добавить проверку наличия карты
    // через детектор карты (если он есть в схеме)
    return 1;
}