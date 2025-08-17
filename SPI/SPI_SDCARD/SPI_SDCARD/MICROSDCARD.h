#ifndef MICROSDCARD_H_
#define MICROSDCARD_H_

#include <avr/io.h>
#include <stdint.h>
#include "Drivers/SPI.h"

/**
 * @file        MICROSDCARD.h
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

// Определение пина CS для microSD карты
#define SD_CS_PORT  PORTB
#define SD_CS_DDR   DDRB
#define SD_CS_PIN   PB1

// Типы карт
typedef enum {
    SD_CARD_TYPE_UNKNOWN = 0,
    SD_CARD_TYPE_SD1,
    SD_CARD_TYPE_SD2,
    SD_CARD_TYPE_SDHC
} SD_CardType;

// Ошибки работы с картой
typedef enum {
    SD_OK = 0,
    SD_ERROR_INIT,
    SD_ERROR_CMD,
    SD_ERROR_READ,
    SD_ERROR_WRITE,
    SD_ERROR_NOT_PRESENT,
    SD_ERROR_BAD_CSD
} SD_Error;

// Структура информации о карте
typedef struct {
    SD_CardType type;
    uint32_t size;      // Размер в килобайтах
    uint16_t oem_id;
    uint8_t  manufacturer_id;
} SD_CardInfo;

//////////////////////////////////////////////////////////////////////////
// Прототипы функций
//////////////////////////////////////////////////////////////////////////

/**
 * @brief Инициализация microSD карты
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_Init(void);

/**
 * @brief Получение информации о карте
 * @param[out] info Указатель на структуру с информацией
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_GetCardInfo(SD_CardInfo* info);

/**
 * @brief Чтение одного блока данных (512 байт)
 * @param[in] block_num Номер блока
 * @param[out] buffer Буфер для данных (минимум 512 байт)
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_ReadBlock(uint32_t block_num, uint8_t* buffer);

/**
 * @brief Запись одного блока данных (512 байт)
 * @param[in] block_num Номер блока
 * @param[in] buffer Буфер с данными (минимум 512 байт)
 * @return Код ошибки (SD_Error)
 */
SD_Error SD_WriteBlock(uint32_t block_num, const uint8_t* buffer);

/**
 * @brief Проверка наличия карты
 * @return true - карта присутствует, false - отсутствует
 */
uint8_t SD_IsPresent(void);

#endif /* MICROSDCARD_H_ */