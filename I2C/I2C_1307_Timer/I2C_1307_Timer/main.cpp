#include "main.h"

unsigned char sec,min,hour,day,date,month,year;

void Read(void);
void Write(void);


int main(void)
{
	Write();
	
	DDRA = 0xff;
	while(1)
	{
		Read();
		PORTA = sec;
	}
}


void Write()
{
	// ����� ��						0b10101010
	// ����� ����� ����� ������ W	0b11010000
	// ����� ����� ����� ������	R	0b11010001
	
	i2c_MT_init();					// ������������� �������				// 11111 = 0xf8 - No relevant state information	available; TWINT = �0�
	i2c_MT_start();					// ������� ���������� ������� ��������	// 00001 = 0x08 - A START condition has been transmitted
	i2c_MT_send(0b11010000);		// �������� ������						// 00011 = 0x18 - SLA+W has been transmitted;		ACK has been received
	i2c_MT_send(0x00);				// �������� ������(������ ������)		// 00101 = 0x28 - Data byte has been transmitted;	ACK has been received
	// �������� ������ DATA					// 00101 = 0x28 - Data byte has been transmitted;	ACK has been received
	i2c_MT_send(0b00000000);		// 0x00	7-CH	6-10Sec		5-10Sec		4-10Sec		3-Sec	2-Sec	1-Sec	0-Sec	(CH-0 - ��� ���������, CH-1 - ���� ���������)
	i2c_MT_send(0b00000000);		// 0x01	7-0		6-10Min		5-10Min		4-10Min		3-Min	2-Min	1-Min	0-Min
	i2c_MT_send(0b00000000);		// 0x02	7-0		6-12/24		5-10H/AMPM	4-10Hour	3-Hour	2-Hour	1-Hour	0-Hour
	i2c_MT_send(0b00000000);		// 0x03	7-0		6-0			5-0			4-0			3-0		2-Day	1-Day	0-Day	(���� ������)
	i2c_MT_send(0b00000000);		// 0x04	7-0		6-0			5-10Date	4-10Date	3-Date	2-Date	1-Date	0-Date	(���� ������)
	i2c_MT_send(0b00000000);		// 0x05	7-0		6-0			5-0			4-10Month	3-Month	2-Month	1-Month	0-Month
	i2c_MT_send(0b00000000);		// 0x06	7-10Y	6-10Y		5-10Y		4-10Y		3-Y		2-Y		1-Y		0-Y
	i2c_MT_send(0b00010000);		// 0x07	7-OUT	6-0			5-0			4-SQWE		3-0		2-0		1-RS1	0-RS0	(OUT - ������ �� ������; SQWE - ��������� �� ������; RS1..0 - ������������ ������� ���������� SQWE)
	
	i2c_MT_stop();					// ���� �� �������						// 11111 = 0xf8 - No relevant state information	available; TWINT = �0�
}


void Read(void)
{
	// ����� ��						0b10101010
	// ����� ����� ����� ������		0b11010000
	// ����� ����� ����� ������		0b11010001
	
	// ===== ������ � ��������� ���������� ������ � �������� ���������� ������ =====
	i2c_MR_init();					// ������������� �������				// 11111 = 0xf8 - No relevant state information	available; TWINT = �0�
	i2c_MR_start();					// ������� ���������� ������� ��������	// 00001 = 0x08 - A START condition has been transmitted
	i2c_MR_send(0b11010000);		// �������� ������ ������
	i2c_MR_send(0x00);				// �������� ������ ������
	i2c_MR_start();					// ��������� �����
	i2c_MR_send(0b11010001);		// �������� ������ ������				// 01000 = 0x40 - SLA+R has been transmitted;	ACK has been received

	sec		= i2c_MR_Read();		// ������ ������   ACK					// 01010 = Data byte has been received;			ACK has been returned
	min		= i2c_MR_Read();
	hour	= i2c_MR_Read();
	day		= i2c_MR_Read();
	date	= i2c_MR_Read();
	month	= i2c_MR_Read();
	year	= i2c_MR_ReadLast();	// ������ ������ NOACK					// 01011 = 0x58 - Data byte has been received;	NOT ACK has been returned
	
	i2c_MR_stop();					// ���� �� �������						// 11111 = 0xf8 - No relevant state information	available; TWINT = �0�
}

