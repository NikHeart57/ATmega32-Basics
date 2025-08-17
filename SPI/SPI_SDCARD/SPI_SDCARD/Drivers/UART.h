#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

/**
 * @file		UART.h
 * @brief		������� UART ��� AVR ���������������� AtMega32
 * @details		������������ ����������� ����� ������ � ���������� ��������
 * @note		-
 * @attention	��� ������� ��Ȩ�� ������ �����������
 * @warning		1. �� ������������� � ���������� ��� �������� "���������� ���������� �������� UART TX ISR(USART_TXC_vect)"
 * @warning		2. ��� �� ��� ��� �� ������� ��� �������� call-back �������
 * @todo		-
 * @bug			-
 * @author		������� ������, deepseek
 * @date		2025-08-03
 * @version		1.0
 */


//////////////////////////////////////////////////////////////////////////
//	�����������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ������� ���������� �� ��������� (1 ���)
 * @note ����� ���� �������������� ����� ���������� ����� �����
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/**
 * @brief �������� UART �� ��������� (9600 ���)
 */
#define BAUD 9600

/**
 * @brief �������� ��� �������� UBRR
 * @details �������������� �� �������: (F_CPU / (16 * BAUD)) - 1
 */
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

/**
 * @brief ������� ������� ����� � �������� (����)
 */
#define UART_RX_BUFFER_SIZE 64			// ������ ������ uart_rx_buffer[UART_RX_BUFFER_SIZE];
#define UART_TX_BUFFER_SIZE 64			// ������ ��������  uart_tx_buffer[UART_TX_BUFFER_SIZE];


//////////////////////////////////////////////////////////////////////////
//	������������ �������� UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ��������� UART
 */
typedef enum {
    UART_DISABLED = 0,
    UART_ENABLED
} UART_State;

/**
 * @brief ������ �������� ������
 */
typedef enum {
    UART_5_BITS = 0,
    UART_6_BITS,
    UART_7_BITS,
    UART_8_BITS,
    UART_9_BITS = 7  // ������ ������ ��� UCSZ2
} UART_DataBits;

/**
 * @brief ���������� ����-�����
 */
typedef enum {
    UART_1_STOP_BIT = 0,
    UART_2_STOP_BITS
} UART_StopBits;

/**
 * @brief ������ �������� ��������
 */
typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN = 2,
    UART_PARITY_ODD = 3
} UART_Parity;

/**
 * @brief ������ ����������
 */
typedef enum {
    UART_INTERRUPT_DISABLED = 0,
    UART_INTERRUPT_RX_ENABLED,
    UART_INTERRUPT_TX_ENABLED,
    UART_INTERRUPT_BOTH_ENABLED
} UART_Interrupt;


//////////////////////////////////////////////////////////////////////////
//	Callback-������� ��� UART RX
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ��� callback-������� ��� ��������� �������� ������
 * @param[in] c �������� ������
 */
typedef void (*UART_RxCallback)(char c);


//////////////////////////////////////////////////////////////////////////
//	��������� ������� UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ������������� UART � ���������� �����������
 * @param[in] state ��������� UART (���/����)
 * @param[in] dataBits ���������� ��� ������
 * @param[in] stopBits ���������� ����-�����
 * @param[in] parity �������� ��������
 * @param[in] interrupt ����� ����������
 */
void UART_Init(UART_State state, UART_DataBits dataBits, UART_StopBits stopBits, UART_Parity parity, UART_Interrupt interrupt);

/**
 * @brief ���������� ������������� UART
 * @param[in] state ��������� UART (���/����)
 * @note ������������ ��������� �� ���������: 8 ��� ������, 1 ����-���, ��� �������� ��������
 */
void UART_Init(UART_State state);

/**
 * @brief ��������� UART
 */
void UART_Enable(void);

/**
 * @brief ���������� UART
 */
void UART_Disable(void);

/**
 * @brief ��������� �������� UART
 * @param[in] baud �������� � �����
 */
void UART_SetBaudRate(uint32_t baud);

/**
 * @brief �������� ������ �������
 * @param[in] c ������ ��� ��������
 */
void UART_SendChar(char c);

/**
 * @brief �������� ������
 * @param[in] str ��������� �� ������ (����������� ����� \n)
 */
void UART_SendString(const char *str);

/**
 * @brief �������� ������� ������
 * @param[in] buffer ��������� �� ����� ������
 * @param[in] length ����� ������
 */
void UART_SendBuffer(const uint8_t *buffer, uint16_t length);

/**
 * @brief ����� ������ �������
 * @return �������� ������
 * @note ����������� ������� (���� ��������� ������)
 */
char UART_ReceiveChar(void);

/**
 * @brief ����� ������� ������
 * @param[out] buffer ����� ��� ������ ������
 * @param[in] length ������������ ����� ������
 * @return ���������� ���������� �������� ����
 */
uint8_t UART_ReceiveBuffer(uint8_t *buffer, uint16_t length);

/**
 * @brief �������� ������� ������ � ������
 * @return ���������� ��������� ��� ������ ����
 */
uint8_t UART_DataAvailable(void);

/**
 * @brief ������� ������ ������
 */
void UART_FlushRxBuffer(void);

/**
 * @brief ��������� callback-������� ��� ������ ������
 * @param[in] callback ������� ��������� ������
 * @note ���� callback �� ����������, ������������ ����������� ��������� ����� �����
 */
void UART_SetRxCallback(UART_RxCallback callback);

#endif /* UART_H_ */