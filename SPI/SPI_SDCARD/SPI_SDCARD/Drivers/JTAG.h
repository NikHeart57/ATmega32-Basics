#ifndef JTAG_H_
#define JTAG_H_

#include <avr/io.h>

/**
 * @brief ������ ������ JTAG
 */
typedef enum {
    JTAG_DISABLED = 0,  ///< ������ ���������� JTAG (����������� ����)
    JTAG_ENABLED,       ///< ��������� JTAG (�� ��������� ����� ������)
} JTAG_State;



/**
 * @brief ������������� JTAG � ��������� �������
 * @param mode ����� ������ (JTAG_DISABLED, JTAG_ENABLED � ��.)
 * @note ��� ���������� JTAG ��������� ������� ������ � �������, ��� ����������� � �������
 * @return 0 ���� JTAG ����������, 1 ���� ���������
 */
uint8_t JTAG_Init(JTAG_State state);

/**
 * @brief �������� �������� ��������� JTAG
 * @return 0 ���� JTAG ��������, 1 ���� �������
 */
uint8_t JTAG_IsWorking(void);

#endif /* JTAG_H_ */