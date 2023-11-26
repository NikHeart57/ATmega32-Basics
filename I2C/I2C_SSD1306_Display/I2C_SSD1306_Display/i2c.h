#ifndef I2C_H_
#define I2C_H_

#include "i2c.cpp"

//====================== Slave Reciever ======================	
void i2c_SR_init(char adr, bool global_call_enabled_TWGCE);	// ������������� ������ ��������

//====================== Master Trasmitter ======================	
void i2c_MT_init(void);										// ������������� ������� ������������ � ������� �������� ����������
void i2c_MT_start(void);									// ������� ������� START �������� �������������
void i2c_MT_send(char data);								// �������� ����� �������� �������������
void i2c_MT_stop(void);										// ������� ������� STOP �������� �������������

//====================== Master Reciever ======================	
void i2c_MR_init(void);										// ������������� ������� �������� � ������� �������� ����������
void i2c_MR_start(void);									// ������� ������� START �������� ���������
void i2c_MR_send(char data);
unsigned char i2c_MR_Read(void);							// ��������� ����� �������� ��������� � ����� ACK
unsigned char i2c_MR_ReadLast(void);						// ��������� ����� �������� ��������� � ����� NOACK
void i2c_MR_stop(void);										// ������� ������� STOP �������� ���������

#endif /* I2C_H_ */
