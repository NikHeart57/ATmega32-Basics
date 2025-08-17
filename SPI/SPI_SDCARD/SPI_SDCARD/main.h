#ifndef MAIN_H_
#define MAIN_H_

#define DEBUG
#define F_CPU 16000000UL

// Подключение библиотек
#include <avr/io.h>
#include <string.h>

// Подключение драйверов устройств микроконтрллера
#include "Drivers/JTAG.h"
#include "Drivers/ADC.h"
#include "Drivers/UART.h"
#include "Drivers/SPI.h"
#include "Drivers/INT.h"
#include "Drivers/TIM.h"

// Подключение драйверов внешних устройств
#include "Drivers/Display/ILI9488.h"
#include "Drivers/Display/XPT2046.h"
#include "MICROSDCARD.h"


#endif /* MAIN_H_ */