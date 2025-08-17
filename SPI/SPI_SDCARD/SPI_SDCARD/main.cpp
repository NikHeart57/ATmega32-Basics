#include "main.h"

char itoa_buffer[64];

// Буфер для работы с SD картой
uint8_t sd_buffer[512];

int main(void)
{
	PORTB |= (1 << PB1);
	DDRB |= (1 << PB1);
	
	cli();
	_delay_ms(100);
		
	// Инициализация устройств микроконтроллера
	JTAG_Init(JTAG_DISABLED);
	ADC_Init(ADC_DISABLED);
	UART_Init(UART_ENABLED, UART_8_BITS, UART_1_STOP_BIT, UART_PARITY_NONE, UART_INTERRUPT_RX_ENABLED);
	SPI_Init(SPI_ENABLED, SPI_MASTER, SPI_MODE0, SPI_CLK_DIV8, SPI_MSB_FIRST, SPI_INTERRUPT_DISABLE);
		
#ifdef DEBUG
	UART_SendString("\n\rDEBUG ON\n\r");
#endif


	// Инициализация SD карты
	SD_Error sd_status = SD_Init();
	
	if(sd_status != SD_OK) 
	{
		UART_SendString("SD Init Error: ");
		UART_SendString(itoa(sd_status, itoa_buffer, 16));		
		UART_SendString("\n\r");
		while(1);
	}
	
	UART_SendString("SD Card Initialized\n\r");
	
	
	// Получение информации о карте
	SD_CardInfo card_info;
	
	SD_GetCardInfo(&card_info);
	
	UART_SendString("Card Type: ");
	UART_SendString(itoa(card_info.type, itoa_buffer, 16));
	
	UART_SendString("\n\rSize: ");
	UART_SendString(itoa(card_info.size, itoa_buffer, 16));
		
	UART_SendString(" KB\n\r");
	
	sei();
	
		
    // Подготовка тестовых данных
    const char* test_data = "Hello from ATmega32! This is a test string written to SD card.";
    memset(sd_buffer, 0, sizeof(sd_buffer));
    strncpy((char*)sd_buffer, test_data, strlen(test_data));
    
    while(1)
    {
		UART_SendString("\n\r");
		
	    // Запись блока данных (блок 0)
	    sd_status = SD_WriteBlock(0, sd_buffer);
		
	    if(sd_status != SD_OK) 
		{
		    UART_SendString("Write Error: ");
			UART_SendString(itoa(sd_status, itoa_buffer, 16));
		    UART_SendString("\n\r");
		} 
		else 
		{
		    UART_SendString("Write OK\n\r");
	    }
	    
	    _delay_ms(1000);
	    
	    // Чтение блока данных (блок 0)
	    memset(sd_buffer, 0, sizeof(sd_buffer));
	    sd_status = SD_ReadBlock(0, sd_buffer);
		
	    if(sd_status != SD_OK) 
		{
		    UART_SendString("Read Error: ");
			UART_SendString(itoa(sd_status, itoa_buffer, 16));
		    UART_SendString("\n\r");
		} 
		else 
		{
		    UART_SendString("Read OK: ");
		    UART_SendString((char*)sd_buffer);
		    UART_SendString("\n\r");
	    }
	    
	    _delay_ms(3000);
    }
}

