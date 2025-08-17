#include "XPT2046.h"



// ������������� ���-�����������
void XPT2046_Setup(void) 
{
	// ��������� ���� CS ��� ������
	XPT2046_CS_DDR |= (1 << XPT2046_CS_PIN);
	
	// ������������ ���-���������� (CS � HIGH)
	XPT2046_CS_PORT |= (1 << XPT2046_CS_PIN);
	
	// ��������� �������� ��� ������������
	_delay_ms(10);
}


uint16_t XPT2046_ReadData(uint8_t command) 
{
	XPT2046_CS_PORT &= ~(1 << XPT2046_CS_PIN);  // ���������� CS (������ �������)
	_delay_ms(1); 
	
	SPI_SendByte(command);						// ���������� ������� (8 ���)
	uint8_t high_byte = SPI_ReceiveByte();  // ������ ������� 8 ���
	uint8_t low_byte = SPI_ReceiveByte();   // ������ ������� 4 ���� (��������� ����������)
	
	XPT2046_CS_PORT |= (1 << XPT2046_CS_PIN);  // ������������ CS (������� �������)
	
	// ���������� 12 ��� (������ 4 ���� low_byte - �����)
	return ((high_byte << 8) | low_byte) >> 3;
}


void XPT2046_ReadCoordinates(uint16_t &x, uint16_t &y)
{
	const uint8_t samples = 16; // ����������� ���������� ������� ��� ������ ���������� 16
	const uint8_t discard = 4;  // ���������� ����������� � ������������ �������� ��� ������������ 4
	
	uint16_t raw_x[samples];
	uint16_t raw_y[samples];
	
	// ���� ������
	for (uint8_t i = 0; i < samples; i++)
	{
		raw_x[i] = 4096 - XPT2046_ReadData(XPT2046_CMD_X);
		raw_y[i] = 4096 - XPT2046_ReadData(XPT2046_CMD_Y);
		_delay_ms(1); // �������� �������� ������ 1 ��
	}
	
	// ���������� �������� ��� ���������� �������
	for (uint8_t i = 0; i < samples-1; i++) {
		for (uint8_t j = i+1; j < samples; j++) 
		{
			if (raw_x[i] > raw_x[j]) {
				uint16_t temp = raw_x[i];
				raw_x[i] = raw_x[j];
				raw_x[j] = temp;
			}
			if (raw_y[i] > raw_y[j]) 
			{
				uint16_t temp = raw_y[i];
				raw_y[i] = raw_y[j];
				raw_y[j] = temp;
			}
		}
	}
	
	// ���������� �������� ��� ����� discard ����������� � ������������ ��������
	uint32_t sum_x = 0, sum_y = 0;
	for (uint8_t i = discard; i < samples-discard; i++) 
	{
		sum_x += raw_x[i];
		sum_y += raw_y[i];
	}
	
	uint16_t avg_x = sum_x / (samples - 2*discard);
	uint16_t avg_y = sum_y / (samples - 2*discard);
	
	// �������� �� ���������� (���� ������ �� ��������)
	/*
	if (avg_x < 100 || avg_x > 3900 || avg_y < 100 || avg_y > 3900) 
	{
		x = SCREEN_WIDTH + 1; // ������������ ����������
		y = SCREEN_HEIGHT + 1;
		return;
	}
	*/
	
	// �������������� � �������� ���������� � �����������
	float real_x = (float)avg_x * (float)SCREEN_WIDTH / 4096.0f;
	float real_y = (float)avg_y * (float)SCREEN_HEIGHT / 4096.0f;
	
	// ���������� ������������� ������������� (��������� ���� ��������)
	uint16_t corrected_x = (uint16_t)(real_x * 1.097f - 19.6f);
	uint16_t corrected_y = (uint16_t)(real_y * 1.12f - 21.56f);
	
	// ����������� ���������
	x = (corrected_x >= SCREEN_WIDTH) ? SCREEN_WIDTH-1 : corrected_x;
	y = (corrected_y >= SCREEN_HEIGHT) ? SCREEN_HEIGHT-1 : corrected_y;
}



/*
// ������ ������ � ���-�����������
uint16_t XPT2046_ReadData(uint8_t command) 
{
	// ���������� ���-���������� (CS � LOW)
	XPT2046_CS_PORT &= ~(1 << XPT2046_CS_PIN);
		
		
		
	// ���������� ������� � ������ ��������� (12 ���)
	uint16_t data = SPI_TransferByte(command) << 8;
	data |= SPI_TransferByte(0x00);
	
	
	
	// ������������ ���-���������� (CS � HIGH)
	XPT2046_CS_PORT |= (1 << XPT2046_CS_PIN);
	
	// ��������� 12 ���, �����������
	//return (data >> 3) & 0xFFF;
	return data >> 3;
}
*/




