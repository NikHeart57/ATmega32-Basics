/*

 ** ������ ������! **
 
 // ������������� ���:
 // - �������
 // - ���������� ��� 2.56�
 // - �������� 128
 // - ����� 0 (PA0)
 // - ������ ������������
 ADC_Init(ADC_ENABLED, ADC_REF_INTERNAL, ADC_DIV128, ADC_CH0, ADC_ALIGN_RIGHT);

 // ������ ��������
 uint16_t value = ADC_Read(ADC_CH0);

 // ������� ������ � �������� ������
 ADC_StartConversion();
 value = ADC_GetResult();
 
 */

#ifndef ADC_H_
#define ADC_H_

#ifndef F_CPU
#define F_CPU			1000000
#endif

#include <avr/io.h>
#include <util/delay.h>

typedef enum {
    ADC_DISABLED = 0,
    ADC_ENABLED
} ADC_State;

typedef enum {
    ADC_REF_AREF = 0,
    ADC_REF_AVCC = (1 << REFS0),
    ADC_REF_INTERNAL = (1 << REFS1) | (1 << REFS0)
} ADC_Reference;

typedef enum {
    ADC_DIV2 = (1 << ADPS0),
    ADC_DIV4 = (1 << ADPS1),
    ADC_DIV8 = (1 << ADPS1) | (1 << ADPS0),
    ADC_DIV16 = (1 << ADPS2),
    ADC_DIV32 = (1 << ADPS2) | (1 << ADPS0),
    ADC_DIV64 = (1 << ADPS2) | (1 << ADPS1),
    ADC_DIV128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)
} ADC_Prescaler;

typedef enum {
    ADC_CH0 = 0,
    ADC_CH1,
    ADC_CH2,
    ADC_CH3,
    ADC_CH4,
    ADC_CH5,
    ADC_CH6,
    ADC_CH7
} ADC_Channel;

typedef enum {
    ADC_ALIGN_RIGHT = 0,
    ADC_ALIGN_LEFT = (1 << ADLAR)
} ADC_Alignment;

// ��������� �������
/**
 * @brief ������������� ��� � ���������� �����������
 * @param state ���������/���������� (ADC_DISABLED/ADC_ENABLED)
 * @param ref �������� �������� ����������
 * @param prescaler �������� �������
 * @param channel ����� ���
 * @param align ������������ ����������
 */
void ADC_Init(ADC_State state, ADC_Reference ref, ADC_Prescaler prescaler, ADC_Channel channel, ADC_Alignment align);
/**
 * @brief ���������/���������� ���
 * @param state ADC_DISABLED ��� ADC_ENABLED
 */
void ADC_Init(ADC_State state);


// �������������� �������
void ADC_Enable(void);
void ADC_Disable(void);
void ADC_StartConversion(void);
uint16_t ADC_GetResult(void);
uint16_t ADC_Read(ADC_Channel ch);

#endif /* ADC_H_ */