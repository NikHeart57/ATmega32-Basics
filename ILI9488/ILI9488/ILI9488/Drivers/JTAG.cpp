#include "JTAG.h"

uint8_t JTAG_Init(JTAG_State state)
{
	switch(state) {
		// Стандартное отключение JTAG
		case JTAG_DISABLED:
		MCUCSR = (1 << JTD);
		MCUCSR = (1 << JTD);
		// Проверяем что JTAG действительно отключился
		if(MCUCSR & (1 << JTD)) {
			return 1; // Успех (бит установлен)
		}
		return 0; // Ошибка отключения
		break;
		
		case JTAG_ENABLED:
		// Включение JTAG (сброс бита JTD)
		MCUCSR &= ~(1 << JTD);
		MCUCSR &= ~(1 << JTD);
		// Проверяем что JTAG действительно включился
		if(!(MCUCSR & (1 << JTD))) {
			return 1; // Успех (бит сброшен)
		}
		return 0; // Ошибка включения
		break;
	}
	
	return 0; // Некорректный режим
}

uint8_t JTAG_IsWorking(void)
{
	return !((MCUCSR & (1 << JTD)) ? 1 : 0);
}