#ifndef JTAG_H_
#define JTAG_H_

#include <avr/io.h>

/**
 * @brief Режимы работы JTAG
 */
typedef enum {
    JTAG_DISABLED = 0,  ///< Полное отключение JTAG (освобождает пины)
    JTAG_ENABLED,       ///< Включение JTAG (по умолчанию после сброса)
} JTAG_State;



/**
 * @brief Инициализация JTAG с указанным режимом
 * @param mode Режим работы (JTAG_DISABLED, JTAG_ENABLED и др.)
 * @note Для отключения JTAG требуется двойная запись в регистр, что реализовано в функции
 * @return 0 если JTAG отключится, 1 если включится
 */
uint8_t JTAG_Init(JTAG_State state);

/**
 * @brief Проверка текущего состояния JTAG
 * @return 0 если JTAG отключен, 1 если включен
 */
uint8_t JTAG_IsWorking(void);

#endif /* JTAG_H_ */