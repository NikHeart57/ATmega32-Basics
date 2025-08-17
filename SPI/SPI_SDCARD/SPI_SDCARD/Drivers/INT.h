#ifndef INT_H
#define INT_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

/**
 * @brief ������ ������ ����������
 */
typedef enum {
	INT_DISABLED = 0,			//
    INT_ENABLED,				//
} Int_State;


/**
 * @brief ���� ��������� ����������
 */
typedef enum {
	INT_TRIGGER_LOW_LEVEL = 0,	// ������ �������
	INT_TRIGGER_FALLING_EDGE,	// ��������� �����
	INT_TRIGGER_RISING_EDGE,	// ����������� �����
	INT_TRIGGER_ANY_EDGE		// ����� ���������
} Int_TriggerMode;


/**
 * @brief Callback-������� ��� ����������
 */
typedef void (*Int_Callback)(void);


// ��������� Callback-�������
void INT0_SetCallback(Int_Callback callback);
void INT1_SetCallback(Int_Callback callback);
void INT2_SetCallback(Int_Callback callback);

// ��������� �������
// ��������� INT0
void INT0_Init(Int_State state);
void INT0_Init(Int_State state, Int_TriggerMode mode);

// ��������� INT1
void INT1_Init(Int_State state);
void INT1_Init(Int_State state, Int_TriggerMode mode);

// ��������� INT2
void INT2_Init(Int_State state);
void INT2_Init(Int_State state, Int_TriggerMode mode);

#endif // INT_H