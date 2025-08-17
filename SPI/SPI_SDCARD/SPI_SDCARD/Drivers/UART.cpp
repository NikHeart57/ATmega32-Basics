/**
 * @file		UART.h
 * @brief		������� UART ��� AVR ���������������� AtMega32
 * @details		������������ ����������� ����� ������ � ���������� ��������
 * @note		-
 * @attention	��� ������� ��Ȩ��/�������� ������ �����������
 * @warning		1. �� ������������� � ���������� ��� �������� "���������� ���������� �������� UART TX ISR(USART_TXC_vect)"
 * @warning		2. ��� �� ��� ��� �� ������� ��� �������� call-back �������
 * @todo		-
 * @bug			-
 * @author		������� ������
 * @date		2025-08-03
 * @version		1.0
 */

#include "UART.h"


//////////////////////////////////////////////////////////////////////////
//	���������� ��������� ������� ��� ������ RX � �������� TX
//////////////////////////////////////////////////////////////////////////

/// @brief ����� ������ ������
static volatile uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];

/// @brief ����� �������� ������
static volatile uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];

/// @brief ������ ������ ������ ������
static volatile uint8_t uart_rx_head = 0;

/// @brief ������ ������ ������ ������
static volatile uint8_t uart_rx_tail = 0;

/// @brief ������ ������ ������ ��������
static volatile uint8_t uart_tx_head = 0;

/// @brief ������ ������ ������ ��������
static volatile uint8_t uart_tx_tail = 0;


//////////////////////////////////////////////////////////////////////////
//	���������� Callback-������� ��� UART RX
//////////////////////////////////////////////////////////////////////////

/// @brief Callback-������� ��� ��������� �������� ������
static UART_RxCallback uart_rx_callback = NULL;


//////////////////////////////////////////////////////////////////////////
//	������� UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ����������� ������������� UART � ��������� ���� ����������
 * @param state ��������� UART (���/����)
 * @param dataBits ���������� ��� ������
 * @param stopBits ���������� ����-�����
 * @param parity �������� ��������
 * @param interrupt ����� ����������
 */
void UART_Init(UART_State state, UART_DataBits dataBits, UART_StopBits stopBits, UART_Parity parity, UART_Interrupt interrupt)
{
	// ������ ������������ ��������
	uint16_t ubrr = UBRR_VALUE;
	UBRRH = (uint8_t)(ubrr >> 8);
	UBRRL = (uint8_t)ubrr;
	
	// ��������� ������� �����
	UCSRC = (1 << URSEL) | (parity << UPM0) | (stopBits << USBS) | (dataBits << UCSZ0);
	
	// ��������� ���������� � ��������� UART
	UCSRB = (1 << RXEN) | (1 << TXEN);
	
	// ��� 9-������� ������
	if(dataBits == UART_9_BITS)
	{
		UCSRB |= (1 << UCSZ2);
	}
	
	// ��������� ����������
	switch(interrupt) {
		case UART_INTERRUPT_RX_ENABLED:
		UCSRB |= (1 << RXCIE);
		break;
		case UART_INTERRUPT_TX_ENABLED:
		UCSRB |= (1 << TXCIE);
		break;
		case UART_INTERRUPT_BOTH_ENABLED:
		UCSRB |= (1 << RXCIE) | (1 << TXCIE);
		break;
		default:
		break;
	}
	
	// ���������/���������� UART
	if(state == UART_DISABLED) {
		UART_Disable();
	}
}

/**
 * @brief ���������� ������������� UART
 * @param state ��������� UART (���/����)
 * @note ���������� ��������� �� ���������: 8 ���, 1 ����-���, ��� �������� ��������
 */
void UART_Init(UART_State state)
{
	if(state == UART_ENABLED) {
		// ������ ������������ �������� ��� 9600 ���
		uint16_t ubrr = (F_CPU / (16UL * 9600)) - 1;
		UBRRH = (uint8_t)(ubrr >> 8);
		UBRRL = (uint8_t)ubrr;
		
		// ��������� ������� �����: 8 ���, 1 ����-���, ��� �������� ��������
		UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
		
		// ��������� ��������� � ����������� + ���������� �� ������
		UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
		} else {
		// ������ ���������� UART
		UCSRB = 0;  // ��������� �����, �������� � ����������
	}
}

/**
 * @brief ��������� UART
 */
void UART_Enable(void) 
{
    UCSRB |= (1 << RXEN) | (1 << TXEN);
}

/**
 * @brief ���������� UART
 */
void UART_Disable(void) 
{
    UCSRB &= ~((1 << RXEN) | (1 << TXEN));
}

/**
 * @brief ��������� �������� UART
 * @param[in] baud �������� � �����
 */
void UART_SetBaudRate(uint32_t baud) 
{
    uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;
}

/**
 * @brief �������� ������ �������
 * @param[in] c ������ ��� ��������
 */
void UART_SendChar(char c) 
{
    while(!(UCSRA & (1 << UDRE)));
    UDR = c;
}

/**
 * @brief �������� ������
 * @param[in] str ��������� �� ������ (����������� ����� \n)
 */
void UART_SendString(const char *str) 
{
    while(*str) {
        UART_SendChar(*str++);
    }
}

/**
 * @brief �������� ������� ������
 * @param[in] buffer ��������� �� ����� ������
 * @param[in] length ����� ������
 */
void UART_SendBuffer(const uint8_t *buffer, uint16_t length) 
{
    for(uint16_t i = 0; i < length; i++) {
        UART_SendChar(buffer[i]);
    }
}

/**
 * @brief ����� ������ �������
 * @return �������� ������
 * @note ����������� ������� (���� ��������� ������)
 */
char UART_ReceiveChar(void) 
{
    while(!UART_DataAvailable());
    uint8_t data = uart_rx_buffer[uart_rx_tail];
    uart_rx_tail = (uart_rx_tail + 1) % UART_RX_BUFFER_SIZE;
    return data;
}

/**
 * @brief ����� ������� ������
 * @param[out] buffer ����� ��� ������ ������
 * @param[in] length ������������ ����� ������
 * @return ���������� ���������� �������� ����
 */
uint8_t UART_ReceiveBuffer(uint8_t *buffer, uint16_t length) 
{
    uint16_t i;
    for(i = 0; i < length && UART_DataAvailable(); i++) {
        buffer[i] = uart_rx_buffer[uart_rx_tail];
        uart_rx_tail = (uart_rx_tail + 1) % UART_RX_BUFFER_SIZE;
    }
    return i;
}

/**
 * @brief �������� ������� ������ � ������
 * @return ���������� ��������� ��� ������ ����
 */
uint8_t UART_DataAvailable(void) 
{
    return (uart_rx_head != uart_rx_tail);
}

/**
 * @brief ������� ������ ������
 */
void UART_FlushRxBuffer(void) 
{
    uart_rx_head = uart_rx_tail = 0;
}

/**
 * @brief ��������� callback-������� ��� ������ ������
 * @param[in] callback ������� ��������� ������
 * @note ���� callback �� ����������, ������������ ����������� ��������� ����� �����
 */
void UART_SetRxCallback(UART_RxCallback callback)
{
	uart_rx_callback = callback;
}


//////////////////////////////////////////////////////////////////////////
//	������� ���������� UART
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ���������� ���������� ������ UART
 * @details ������������� ���������� ��� ��������� ������
 */
ISR(USART_RXC_vect)
{
	char c = UDR;
	
	if (uart_rx_callback != NULL)
	{
		uart_rx_callback(c);
	}
	else
	{
		// ����������� ��������� (���� callback �� ����������)
		if (uart_rx_head != ((uart_rx_tail - 1) % UART_RX_BUFFER_SIZE))
		{
			uart_rx_buffer[uart_rx_head] = c;
			uart_rx_head = (uart_rx_head + 1) % UART_RX_BUFFER_SIZE;
		}
	}
}

/**
 * @brief ���������� ���������� �������� UART
 * @details ������������� ���������� ��� ���������� �����������
 */
ISR(USART_TXC_vect) 
{
	if (uart_tx_head != uart_tx_tail)
	{
		UDR = uart_tx_buffer[uart_tx_tail];
		uart_tx_tail = (uart_tx_tail + 1) % UART_TX_BUFFER_SIZE;
	}
}
