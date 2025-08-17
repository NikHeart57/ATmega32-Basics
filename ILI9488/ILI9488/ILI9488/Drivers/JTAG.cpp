#include "JTAG.h"

uint8_t JTAG_Init(JTAG_State state)
{
	switch(state) {
		// ����������� ���������� JTAG
		case JTAG_DISABLED:
		MCUCSR = (1 << JTD);
		MCUCSR = (1 << JTD);
		// ��������� ��� JTAG ������������� ����������
		if(MCUCSR & (1 << JTD)) {
			return 1; // ����� (��� ����������)
		}
		return 0; // ������ ����������
		break;
		
		case JTAG_ENABLED:
		// ��������� JTAG (����� ���� JTD)
		MCUCSR &= ~(1 << JTD);
		MCUCSR &= ~(1 << JTD);
		// ��������� ��� JTAG ������������� ���������
		if(!(MCUCSR & (1 << JTD))) {
			return 1; // ����� (��� �������)
		}
		return 0; // ������ ���������
		break;
	}
	
	return 0; // ������������ �����
}

uint8_t JTAG_IsWorking(void)
{
	return !((MCUCSR & (1 << JTD)) ? 1 : 0);
}