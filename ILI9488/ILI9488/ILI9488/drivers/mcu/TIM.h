/**
 * @file        TIM.h
 * @brief       ������� �������� 0, 1, 2 ��� AVR ���������������� AtMega32
 * @details     ������������� ������ �������� ��� ��������� ����������������:
 *              - ��������� ������� ������ (Normal, CTC, PWM)
 *              - ���������� ��������������
 *              - ������ � �������� ���������
 *              - ���������� ������������
 *              - ��������� Input Capture (Timer1)
 * 
 * @note        ����������� ����������:
 *              - ������ ��������� ���� ���� �������� (0, 1, 2)
 *              - ������ ��������� ��� ���� ��������
 *              - ��������� ��������� ������ ������������
 * 
 * @warning     ������ �����������:
 *              - Timer0 � Timer2: 8-������, ������������ �������� 255
 *              - Timer1: 16-������, ������������ �������� 65535
 *              - ������ PWM ������� ���������� ��������� ������� OCRx
 *              - Input Capture �������� ������ �� Timer1
 * 
 * @attention   ���������� ��� ������:
 *              - ����������� <avr/io.h> � <avr/interrupt.h>
 *              - ���������� ��������� FUSE-����� (���� ���������)
 *              - ���������� ������������� ���������� � ������� ���������
 * 
 * @todo        ����������� ���������:
 *              - ���������� ��������� �������� � sleep-�������
 *              - ����������� ����������� ������ ������������
 *              - ��������� �������������� ������� PWM
 *              - ����������� ��� ����������������
 * 
 * @bug         ��������� ��������:
 *              - �������� �������� ���������� ���������� � ��������� ��������
 *              - ������������ ��������� ����������� ������� (Timer2)
 *              - ��������� ������������ �� ��������� �������� CPU
 * 
 * @author      deepseek
 * @date        2025-08-18
 * @version     1.0
 * 
 * @copyright   MIT License
 */

#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//////////////////////////////////////////////////////////////////////////
// ������������ �������� ��������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ��������� ������� (�������/��������)
 * @note ��� ���������� ������� ������������ ��� ��������� � ����������
 */
typedef enum {
    TIM_DISABLED = 0,    ///< ������ ��������� ��������, ����������������
    TIM_ENABLED          ///< ������ ������� � ����� � ������
} TIM_State;

/**
 * @brief ������������ ������� ������� (������������)
 * @note ����� ������������ ������ �� �������� � �������� �������
 * @warning ������������ ������������ ����� �������� � ������������������� �������
 */
typedef enum {
    TIM_NO_PRESCALER = 1,    ///< ��� ������� (F_CPU)
    TIM_PRESCALER_8 = 2,     ///< ������� �� 8
    TIM_PRESCALER_64 = 3,    ///< ������� �� 64
    TIM_PRESCALER_256 = 4,   ///< ������� �� 256
    TIM_PRESCALER_1024 = 5   ///< ������� �� 1024
} TIM_Prescaler;

/**
 * @brief ������ ������ �������
 * @note ������ ����� ����� ������������� ����������:
 *       - Normal: ������� �������
 *       - CTC: Clear Timer on Compare, ��������� ������ ����������
 *       - PWM: ���-��������� ��� ���������� ���������
 */
typedef enum {
    TIM_NORMAL = 0,              ///< ������� ����� ����� (overflow)
    TIM_CTC,                     ///< ����� �� ���������� (������ ���������)
    TIM_PWM_PHASE_CORRECT,       ///< ���������� ��� (������������)
    TIM_PWM_FAST                 ///< ������� ��� (������� �������)
} TIM_Mode;

/**
 * @brief ������ ������� ��� ���������/PWM
 * @note �� ��� ������� ������������ ��� ������
 * @warning Timer0 � Timer2 ������������ ������ ����� A
 */
typedef enum {
    TIM_CHANNEL_A = 0,   ///< ����� A (OCRxA)
    TIM_CHANNEL_B        ///< ����� B (OCRxB) - ������ Timer1
} TIM_Channel;

//////////////////////////////////////////////////////////////////////////
// ��������� ������� ��������
//////////////////////////////////////////////////////////////////////////

/**
 * @group Timer 0 Functions (8-bit)
 * @brief ������� ��� ������ � 8-������ �������� 0
 */

/**
 * @brief ������������� ������� 0 � ��������� �������������
 * @param state ��������� ������� (���/����)
 * @param prescaler ����������� ������� �������
 * @note ��� ���������� ������������ ��� ��������� � ����������
 * @warning �� ������������ ����� B (������ ����� A)
 */
void TIM0_Init(TIM_State state, TIM_Prescaler prescaler);

/**
 * @brief ������������� ������� 0 � ������������� �� ��������� (64)
 * @param state ��������� �������
 * @note ���������� ������ � ������������ �����������
 */
void TIM0_Init(TIM_State state);

/**
 * @brief ��������� ������ ������ ������� 0
 * @param mode ����� ������ (Normal, CTC, PWM)
 * @note ����� PWM ������� �������������� ��������� �������
 */
void TIM0_SetMode(TIM_Mode mode);

/**
 * @brief ��������� �������� ��������� ��� ������� 0
 * @param value �������� ��������� (0-255)
 * @note ��� ������ CTC ���������� ������ �������
 */
void TIM0_SetCompareValue(uint8_t value);

/**
 * @brief ��������� ��� �� ������� 0
 * @param duty ����������� ���������� (0-255)
 * @param channel ����� ��� (������ A ��� Timer0)
 * @note ������������� non-inverting PWM �����
 */
void TIM0_SetPWM(uint8_t duty, TIM_Channel channel);

/**
 * @group Timer 1 Functions (16-bit)
 * @brief ������� ��� ������ � 16-������ �������� 1
 */

/**
 * @brief ������������� ������� 1 � ��������� �������������
 * @param state ��������� �������
 * @param prescaler ����������� ������� �������
 * @note ������������ ��� ������ � input capture
 */
void TIM1_Init(TIM_State state, TIM_Prescaler prescaler);

/**
 * @brief ������������� ������� 1 � ������������� �� ��������� (64)
 * @param state ��������� �������
 */
void TIM1_Init(TIM_State state);

/**
 * @brief ��������� ������ ������ ������� 1
 * @param mode ����� ������
 * @note ������������ ��� ������ ������� ������� PWM
 */
void TIM1_SetMode(TIM_Mode mode);

/**
 * @brief ��������� �������� ��������� ��� ������� 1
 * @param value �������� ��������� (0-65535)
 * @param channel ����� ��������� (A ��� B)
 */
void TIM1_SetCompareValue(uint16_t value, TIM_Channel channel);

/**
 * @brief ��������� ��� �� ������� 1
 * @param duty ����������� ���������� (0-65535)
 * @param channel ����� ��� (A ��� B)
 * @note ������������� non-inverting PWM �����
 */
void TIM1_SetPWM(uint16_t duty, TIM_Channel channel);

/**
 * @brief ������������� Input Capture ��� ������� 1
 * @note ����������� �������������� � ����� ������������
 * @warning ������� ����������� �������� ������� � ���������������� ������
 */
void TIM1_InputCaptureInit(void);

/**
 * @brief ��������� �������� Input Capture
 * @return �������� ������������ �������
 * @note ������������ ��� ��������� ������������ ���������
 */
uint16_t TIM1_GetInputCaptureValue(void);

/**
 * @group Timer 2 Functions (8-bit)
 * @brief ������� ��� ������ � 8-������ �������� 2
 */

/**
 * @brief ������������� ������� 2 � ��������� �������������
 * @param state ��������� �������
 * @param prescaler ����������� ������� �������
 * @note ���������� Timer0 �� � ������������ �������������
 */
void TIM2_Init(TIM_State state, TIM_Prescaler prescaler);

/**
 * @brief ������������� ������� 2 � ������������� �� ��������� (64)
 * @param state ��������� �������
 */
void TIM2_Init(TIM_State state);

/**
 * @brief ��������� ������ ������ ������� 2
 * @param mode ����� ������
 */
void TIM2_SetMode(TIM_Mode mode);

/**
 * @brief ��������� �������� ��������� ��� ������� 2
 * @param value �������� ��������� (0-255)
 */
void TIM2_SetCompareValue(uint8_t value);

/**
 * @brief ��������� ��� �� ������� 2
 * @param duty ����������� ���������� (0-255)
 * @param channel ����� ��� (������ A ��� Timer2)
 */
void TIM2_SetPWM(uint8_t duty, TIM_Channel channel);

/**
 * @group Common Timer Functions
 * @brief ����� ������� ��� ���� ��������
 */

/**
 * @brief ��������� ������������ ��� ���������� �������
 * @param timer_num ����� ������� (0, 1, 2)
 * @param prescaler ����������� ������� �������
 * @warning ������������ ����� ������� �������� � ��������������� ���������
 */
void TIM_SetPrescaler(uint8_t timer_num, TIM_Prescaler prescaler);

/**
 * @brief ��������� ���������� �������
 * @param timer_num ����� ������� (0, 1, 2)
 * @param interrupt_type ������� ���� ������������� ����������:
 *                      - 0x01: Overflow
 *                      - 0x02: Compare A
 *                      - 0x04: Compare B (Timer1)
 *                      - 0x08: Input Capture (Timer1)
 * @note ������� ���������� ��������������� ISR � ������� ���������
 */
void TIM_EnableInterrupt(uint8_t timer_num, uint8_t interrupt_type);

/**
 * @brief ���������� ���������� �������
 * @param timer_num ����� ������� (0, 1, 2)
 * @param interrupt_type ������� ���� ����������� ����������
 */
void TIM_DisableInterrupt(uint8_t timer_num, uint8_t interrupt_type);

