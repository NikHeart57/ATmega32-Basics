#ifndef MAIN_H_
#define MAIN_H_

#define DEBUG
#define F_CPU 16000000UL


// #include <stdlib.h>
#include <string.h>
#include <avr/wdt.h>
// #include <stdint.h>
// #include <ctype.h>
// #include <stdio.h>
#include <avr/io.h>	
#include <avr/interrupt.h>
// #include <util/delay.h>

#include "drivers/mcu/JTAG.h"
#include "drivers/mcu/ADC.h"
#include "drivers/mcu/UART.h"
#include "drivers/mcu/INT.h"
// #include "drivers/mcu/TIM.h"
// #include "drivers/mcu/SPI.h"

#include "drivers/display/ILI9488.h"
#include "drivers/touch/XPT2046.h"



#endif 