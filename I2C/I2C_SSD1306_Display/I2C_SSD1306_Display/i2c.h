#ifndef I2C_H_
#define I2C_H_

#include "i2c.cpp"

//====================== Slave Reciever ======================	
void i2c_SR_init(char adr, bool global_call_enabled_TWGCE);	// Инициализация Слейва Ресивера

//====================== Master Trasmitter ======================	
void i2c_MT_init(void);										// Инициализация Мастера Трансмиттера и задание скорости соединения
void i2c_MT_start(void);									// Задание условия START Мастером Трансмиттером
void i2c_MT_send(char data);								// Отправка байта Мастером Трансмиттером
void i2c_MT_stop(void);										// Задание условия STOP Мастером Трансмиттером

//====================== Master Reciever ======================	
void i2c_MR_init(void);										// Инициализация Мастера Ресивера и задание скорости соединения
void i2c_MR_start(void);									// Задание условия START Мастером Ресивером
void i2c_MR_send(char data);
unsigned char i2c_MR_Read(void);							// Получение байта Мастером Ресивером и ответ ACK
unsigned char i2c_MR_ReadLast(void);						// Получение байта Мастером Ресивером и ответ NOACK
void i2c_MR_stop(void);										// Задание условия STOP Мастером Ресивером

#endif /* I2C_H_ */
