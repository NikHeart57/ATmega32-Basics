#include "main.h"


//====================== Slave Reciever ======================	

void i2c_SR_init(char adr, bool global_call_enabled_TWGCE)
{
	// TWGCE = 1 - слейв будет реагировать на любой вызов
	// TWEA = 1  - всегда 1 чтобы отвечать 9 битом		// Скорее всего это нужно но по ситуации в зависимости от того идет трансляция последнего бита или нет но хз..
	TWAR = (adr);										// Запись адреса слейва и разрешения вызова на общий вызов (бит 0 )
	TWAR |= (global_call_enabled_TWGCE << TWGCE);						
	TWCR = (1 << TWEA)|(1 << TWEN);						// (1 << TWEA) - ; (1 << TWEN) - включение i2c
}


//====================== Master Trasmitter ======================

void i2c_MT_init(void)
{
	TWBR = 66;										// Задает скорость соединения. Чем больше тем медленнее; 255 -> 28кбит/с; 66 -> 100кбит/с
	TWSR |= (0 << TWPS1)|(0 << TWPS0);				// Аналогично (00 - 11)
}

void i2c_MT_start(void)
{
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);	// Send START condition
	while (!(TWCR & (1 << TWINT)));					// Wait for TWINT Flag set. This indicates that the START condition has been transmitted
}

void i2c_MT_send(char data)
{
	TWDR = data;									// Load data into TWDR Register.
	TWCR = (1 << TWINT)|(1 << TWEN);				// Clear TWINT bit in TWCR to start transmission of data
	while (!(TWCR & (1 << TWINT)));					// ожидание завершения операции
}

void i2c_MT_stop(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);	// Transmit STOP condition
}


//====================== Master Reciever ======================	

void i2c_MR_init(void)
{
	TWBR = 66;										// Задает скорость соединения. Чем больше тем медленнее; 255 -> 28кбит/с; 66 -> 100кбит/с
	TWSR |= (0 << TWPS1)|(0 << TWPS0);				// Аналогично (00 - 11)
}

void i2c_MR_start(void)
{
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);	// Send START condition
	while (!(TWCR & (1 << TWINT)));					// Wait for TWINT Flag set. This indicates that the START condition has been transmitted
}

void i2c_MR_send(char data)
{
	TWDR = data;									// Load data into TWDR Register.
	TWCR = (1 << TWINT)|(1 << TWEN);				// Clear TWINT bit in TWCR to start transmission of data
	while (!(TWCR & (1 << TWINT)));					// ожидание завершения операции
}

unsigned char i2c_MR_Read(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWEA);
	while (!(TWCR & (1 << TWINT)));					//ожидание установки бита TWIN
	return TWDR;									//читаем регистр данных
}

unsigned char i2c_MR_ReadLast(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN);
	while (!(TWCR & (1 << TWINT)));					//ожидание установки бита TWIN
	return TWDR;									//читаем регистр данных
}

void i2c_MR_stop(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);	// Transmit STOP condition
}