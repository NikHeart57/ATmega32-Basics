#include "TIM.h"

//////////////////////////////////////////////////////////////////////////
//  ПРОВЕРКА ПОДДЕРЖКИ МИКРОКОНТРОЛЛЕРА (ATmega32 ONLY)
//////////////////////////////////////////////////////////////////////////

#ifndef __AVR_ATmega32__
    #error "TIM driver requires ATmega32 MCU! Check your target device."
#endif

// ========== Timer 0 Functions (8-bit) ==========

/**
 * @brief Инициализация таймера 0 с указанным предделителем
 * @param state Состояние таймера (вкл/выкл)
 * @param prescaler Коэффициент деления частоты
 * @note При выключении полностью сбрасывает регистры таймера
 * @warning Не поддерживает канал B, только канал A
 */
void TIM0_Init(TIM_State state, TIM_Prescaler prescaler) 
{
    if (state == TIM_DISABLED) 
    {
        // Полное отключение таймера и прерываний
        TCCR0 = 0;
        TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));
        return;
    }
    
    // Базовая инициализация с предделителем 1
    TCCR0 = (1 << CS00);
    TIM_SetPrescaler(0, prescaler);
}

/**
 * @brief Упрощенная инициализация таймера 0
 * @param state Состояние таймера
 * @note Использует предделитель 64 по умолчанию
 */
void TIM0_Init(TIM_State state) {
    TIM0_Init(state, TIM_PRESCALER_64);
}

/**
 * @brief Установка режима работы таймера 0
 * @param mode Режим работы (Normal, CTC, PWM)
 * @note Сбрасывает предыдущие настройки режима
 */
void TIM0_SetMode(TIM_Mode mode) {
    TCCR0 &= ~((1 << WGM01) | (1 << WGM00));
    switch(mode) {
        case TIM_CTC:
            TCCR0 |= (1 << WGM01);
            break;
        case TIM_PWM_PHASE_CORRECT:
            TCCR0 |= (1 << WGM00);
            break;
        case TIM_PWM_FAST:
            TCCR0 |= (1 << WGM01) | (1 << WGM00);
            break;
        default: // Normal mode
            break;
    }
}

/**
 * @brief Установка значения сравнения для таймера 0
 * @param value Значение сравнения (0-255)
 * @note Определяет период в режиме CTC или скважность в PWM
 */
void TIM0_SetCompareValue(uint8_t value) {
    OCR0 = value;
}

/**
 * @brief Настройка ШИМ на таймере 0
 * @param duty Коэффициент заполнения (0-255)
 * @param channel Канал ШИМ (только A для Timer0)
 * @note Устанавливает non-inverting PWM режим
 */
void TIM0_SetPWM(uint8_t duty, TIM_Channel channel) {
    if (channel == TIM_CHANNEL_A) {
        TCCR0 |= (1 << COM01); // Non-inverting PWM
        OCR0 = duty;
    }
}

// ========== Timer 1 Functions (16-bit) ==========

/**
 * @brief Инициализация 16-битного таймера 1
 * @param state Состояние таймера
 * @param prescaler Коэффициент деления частоты
 * @note Поддерживает оба канала и input capture
 */
void TIM1_Init(TIM_State state, TIM_Prescaler prescaler) {
    if (state == TIM_DISABLED) {
        // Полное отключение таймера 1
        TCCR1A = TCCR1B = 0;
        TIMSK &= ~((1 << TOIE1) | (1 << OCIE1A) | (1 << OCIE1B));
        return;
    }
    
    // Базовая инициализация с предделителем 1
    TCCR1B = (1 << CS10);
    TIM_SetPrescaler(1, prescaler);
}

/**
 * @brief Упрощенная инициализация таймера 1
 * @param state Состояние таймера
 */
void TIM1_Init(TIM_State state) {
    TIM1_Init(state, TIM_PRESCALER_64);
}

/**
 * @brief Установка режима работы таймера 1
 * @param mode Режим работы
 * @note Поддерживает сложные режимы PWM
 */
void TIM1_SetMode(TIM_Mode mode) {
    TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
    TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
    
    switch(mode) {
        case TIM_CTC:
            TCCR1B |= (1 << WGM12);
            break;
        case TIM_PWM_PHASE_CORRECT:
            TCCR1A |= (1 << WGM10);
            break;
        case TIM_PWM_FAST:
            TCCR1A |= (1 << WGM11) | (1 << WGM10);
            TCCR1B |= (1 << WGM12);
            break;
        default: // Normal mode
            break;
    }
}

/**
 * @brief Установка значения сравнения для таймера 1
 * @param value Значение сравнения (0-65535)
 * @param channel Канал сравнения (A или B)
 */
void TIM1_SetCompareValue(uint16_t value, TIM_Channel channel) {
    if (channel == TIM_CHANNEL_A) OCR1A = value;
    else OCR1B = value;
}

/**
 * @brief Настройка ШИМ на таймере 1
 * @param duty Коэффициент заполнения (0-65535)
 * @param channel Канал ШИМ (A или B)
 * @note Устанавливает non-inverting PWM режим
 */
void TIM1_SetPWM(uint16_t duty, TIM_Channel channel) {
    if (channel == TIM_CHANNEL_A) {
        TCCR1A |= (1 << COM1A1);
        OCR1A = duty;
    } else {
        TCCR1A |= (1 << COM1B1);
        OCR1B = duty;
    }
}

/**
 * @brief Инициализация Input Capture для таймера 1
 * @note Настраивает шумоподавление и фронт срабатывания
 * @warning Требует внешнего сигнала на соответствующем выводе
 */
void TIM1_InputCaptureInit(void) {
    TCCR1B |= (1 << ICNC1); // Noise canceler
    TCCR1B |= (1 << ICES1); // Rising edge
    TIMSK |= (1 << TICIE1); // Enable interrupt
}

/**
 * @brief Получение значения Input Capture
 * @return Захваченное значение времени
 * @note Используется для измерения длительности импульсов
 */
uint16_t TIM1_GetInputCaptureValue(void) {
    return ICR1;
}

// ========== Timer 2 Functions (8-bit) ==========

/**
 * @brief Инициализация таймера 2
 * @param state Состояние таймера
 * @param prescaler Коэффициент деления частоты
 * @note Аналогичен Timer0 с возможностью асинхронной работы
 */
void TIM2_Init(TIM_State state, TIM_Prescaler prescaler) {
    if (state == TIM_DISABLED) {
        TCCR2 = 0;
        TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));
        return;
    }
    
    TCCR2 = (1 << CS20);
    TIM_SetPrescaler(2, prescaler);
}

/**
 * @brief Упрощенная инициализация таймера 2
 * @param state Состояние таймера
 */
void TIM2_Init(TIM_State state) {
    TIM2_Init(state, TIM_PRESCALER_64);
}

/**
 * @brief Установка режима работы таймера 2
 * @param mode Режим работы
 */
void TIM2_SetMode(TIM_Mode mode) {
    TCCR2 &= ~((1 << WGM21) | (1 << WGM20));
    switch(mode) {
        case TIM_CTC:
            TCCR2 |= (1 << WGM21);
            break;
        case TIM_PWM_PHASE_CORRECT:
            TCCR2 |= (1 << WGM20);
            break;
        case TIM_PWM_FAST:
            TCCR2 |= (1 << WGM21) | (1 << WGM20);
            break;
        default: // Normal mode
            break;
    }
}

/**
 * @brief Установка значения сравнения для таймера 2
 * @param value Значение сравнения (0-255)
 */
void TIM2_SetCompareValue(uint8_t value) {
    OCR2 = value;
}

/**
 * @brief Настройка ШИМ на таймера 2
 * @param duty Коэффициент заполнения (0-255)
 * @param channel Канал ШИМ (только A для Timer2)
 */
void TIM2_SetPWM(uint8_t duty, TIM_Channel channel) {
    if (channel == TIM_CHANNEL_A) {
        TCCR2 |= (1 << COM21); // Non-inverting PWM
        OCR2 = duty;
    }
}

// ========== Common Functions ==========

/**
 * @brief Установка предделителя для указанного таймера
 * @param timer_num Номер таймера (0, 1, 2)
 * @param prescaler Коэффициент деления частоты
 * @warning Отсутствует проверка валидности номера таймера
 */
void TIM_SetPrescaler(uint8_t timer_num, TIM_Prescaler prescaler) {
    switch(timer_num) {
        case 0: // Timer 0
            TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
            switch(prescaler) {
                case TIM_NO_PRESCALER:  TCCR0 |= (1 << CS00); break;
                case TIM_PRESCALER_8:   TCCR0 |= (1 << CS01); break;
                case TIM_PRESCALER_64:  TCCR0 |= (1 << CS01) | (1 << CS00); break;
                case TIM_PRESCALER_256: TCCR0 |= (1 << CS02); break;
                case TIM_PRESCALER_1024:TCCR0 |= (1 << CS02) | (1 << CS00); break;
            }
            break;
        
        case 1: // Timer 1
            TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
            switch(prescaler) {
                case TIM_NO_PRESCALER:  TCCR1B |= (1 << CS10); break;
                case TIM_PRESCALER_8:   TCCR1B |= (1 << CS11); break;
                case TIM_PRESCALER_64:  TCCR1B |= (1 << CS11) | (1 << CS10); break;
                case TIM_PRESCALER_256: TCCR1B |= (1 << CS12); break;
                case TIM_PRESCALER_1024:TCCR1B |= (1 << CS12) | (1 << CS10); break;
            }
            break;
        
        case 2: // Timer 2
            TCCR2 &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
            switch(prescaler) {
                case TIM_NO_PRESCALER:  TCCR2 |= (1 << CS20); break;
                case TIM_PRESCALER_8:   TCCR2 |= (1 << CS21); break;
                case TIM_PRESCALER_64:  TCCR2 |= (1 << CS21) | (1 << CS20); break;
                case TIM_PRESCALER_256: TCCR2 |= (1 << CS22); break;
                case TIM_PRESCALER_1024:TCCR2 |= (1 << CS22) | (1 << CS20); break;
            }
            break;
    }
}

/**
 * @brief Включение прерываний таймера
 * @param timer_num Номер таймера (0, 1, 2)
 * @param interrupt_type Битовое поле прерываний
 */
void TIM_EnableInterrupt(uint8_t timer_num, uint8_t interrupt_type) {
    switch(timer_num) {
        case 0:
            if (interrupt_type & 0x01) TIMSK |= (1 << TOIE0);
            if (interrupt_type & 0x02) TIMSK |= (1 << OCIE0);
            break;
        case 1:
            if (interrupt_type & 0x01) TIMSK |= (1 << TOIE1);
            if (interrupt_type & 0x02) TIMSK |= (1 << OCIE1A);
            if (interrupt_type & 0x04) TIMSK |= (1 << OCIE1B);
            if (interrupt_type & 0x08) TIMSK |= (1 << TICIE1);
            break;
        case 2:
            if (interrupt_type & 0x01) TIMSK |= (1 << TOIE2);
            if (interrupt_type & 0x02) TIMSK |= (1 << OCIE2);
            break;
    }
}

/**
 * @brief Отключение прерываний таймера
 * @param timer_num Номер таймера (0, 1, 2)
 * @param interrupt_type Битовое поле прерываний
 */
void TIM_DisableInterrupt(uint8_t timer_num, uint8_t interrupt_type) {
    switch(timer_num) {
        case 0:
            if (interrupt_type & 0x01) TIMSK &= ~(1 << TOIE0);
            if (interrupt_type & 0x02) TIMSK &= ~(1 << OCIE0);
            break;
        case 1:
            if (interrupt_type & 0x01) TIMSK &= ~(1 << TOIE1);
            if (interrupt_type & 0x02) TIMSK &= ~(1 << OCIE1A);
            if (interrupt_type & 0x04) TIMSK &= ~(1 << OCIE1B);
            if (interrupt_type & 0x08) TIMSK &= ~(1 << TICIE1);
            break;
        case 2:
            if (interrupt_type & 0x01) TIMSK &= ~(1 << TOIE2);
            if (interrupt_type & 0x02) TIMSK &= ~(1 << OCIE2);
            break;
    }
}