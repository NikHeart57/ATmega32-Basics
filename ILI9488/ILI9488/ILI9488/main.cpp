#include "main.h"


char itoa_buffer[64] = {0};
	
int main(void)
{
	cli();
	JTAG_Init(JTAG_DISABLED);
	ADC_Init(ADC_DISABLED);
	UART_Init(UART_ENABLED, UART_8_BITS, UART_1_STOP_BIT, UART_PARITY_NONE, UART_INTERRUPT_RX_ENABLED);
	
	ILI9488_Setup();
	ILI9488_Init(ILI9488_DISPLAY_ON, ILI9488_ORIENTATION_LANDSCAPE_FLIP);
	ILI9488_FillScreen(BLACK);
	
	XPT2046_Setup();
	XPT2046_Init(&xpt2046_current_calibration, INT0_Init, INT_TRIGGER_LOW_LEVEL);
	sei();

	while (1)
	{
	}
	
	return 1;
}


ISR(INT0_vect)
{
	if(XPT2046_ReadCoordinates(xpt2046_touch_x, xpt2046_touch_y))
	{
		uint16_t current_pressure = XPT2046_ReadPressure(); 
		
		if(current_pressure > 500)
		{
			 ILI9488_DrawPixel(xpt2046_touch_x, xpt2046_touch_y, WHITE);
			 
			 UART_SendString("X = ");
			 UART_SendString(itoa(xpt2046_touch_x, itoa_buffer, 10));
			 UART_SendString(" Y = ");
			 UART_SendString(itoa(xpt2046_touch_y, itoa_buffer, 10));
			 UART_SendString(" P = ");
			 UART_SendString(itoa(current_pressure, itoa_buffer, 10));
			 UART_SendString("\r\n");
		 }
		 else
		 {
			UART_SendString("Filtered: X = ");
			  UART_SendString(itoa(xpt2046_touch_x, itoa_buffer, 10));
			  UART_SendString(" Y = ");
			  UART_SendString(itoa(xpt2046_touch_y, itoa_buffer, 10));
			  UART_SendString(" P = ");
			  UART_SendString(itoa(current_pressure, itoa_buffer, 10));
			  UART_SendString("\r\n");
		 }
	}
}