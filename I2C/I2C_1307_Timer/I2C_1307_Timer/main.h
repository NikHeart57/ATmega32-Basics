#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "i2c.h"

void Read(void);
void Write(void);

#endif /* MAIN_H_ */