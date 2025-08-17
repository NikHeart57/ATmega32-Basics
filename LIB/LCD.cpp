#include <avr/io.h>
#include "LCD.h"

#define PIN8
#define LCD2004		// У LCD1602 может некорректно работать функции LCD_Set_Pos и LCD_Print_String. Перепроверь!


char ddram_adr = 0;

void BF_check(void)
{
#ifdef PIN8
	CMD_PORT &= ~((1 << RS)|(1 << RW)|(1 << E));																											// Установка в порту CMD нуля
	DATA_DDR &= ~((1 << DATA_PIN0)|(1 << DATA_PIN1)|(1 << DATA_PIN2)|(1 << DATA_PIN3)|(1 << DATA_PIN4)|(1 << DATA_PIN5)|(1 << DATA_PIN6)|(1 << DATA_PIN7));	// Порт DATA на вход
	
	do
	{
		CMD_PORT |= (1 << RW);
		CMD_PORT |= (RW|E);
		CMD_PORT |= (1 << RW);
	}
	while (DATA_PIN & (1 << BF_PIN));
	
	DATA_DDR |= ((1 << DATA_PIN0)|(1 << DATA_PIN1)|(1 << DATA_PIN2)|(1 << DATA_PIN3)|(1 << DATA_PIN4)|(1 << DATA_PIN5)|(1 << DATA_PIN6)|(1 << DATA_PIN7));	// Порт DATA на вЫход
#endif /* PIN8 */	
}


void Send_Cmd(char comand)
{
#ifdef PIN8
	BF_check();
	
	DATA_PORT = comand;
	
	CMD_PORT &= ~(1 << RS);				// 0 в пин RS (команда/данные)	- 0 - передача команды
	CMD_PORT &= ~(1 << RW);				// 0 в пин RW (запись/чтение)	- 0 - запись данных
	
	CMD_PORT |= (1 << E);				// тактирование для отправки данных
	CMD_PORT &= ~(1 << E);
#endif /* PIN8 */	
}


void Send_Data(char data)
{
#ifdef PIN8	
	BF_check();
	
	DATA_PORT = data;
	
	CMD_PORT |= (1 << RS);				// 1 в пин RS (команда/данные)	- 0 - передача данных
	CMD_PORT &= ~(1 << RW);				// 0 в пин RW (запись/чтение)	- 0 - запись данных
	
	CMD_PORT |= (1 << E);				// тактирование для отправки данных
	CMD_PORT &= ~(1 << E);
#endif /* PIN8 */
}


void LCD_Init(void)
{
	CMD_DDR |= (1 << RS)|(1 << RW)|(1 << E);
	CMD_PORT &= ~((1 << RS)|(1 << RW)|(1 << E));
	
#ifdef PIN8			
	DATA_DDR |= ((1 << DATA_PIN0)|(1 << DATA_PIN1)|(1 << DATA_PIN2)|(1 << DATA_PIN3)|(1 << DATA_PIN4)|(1 << DATA_PIN5)|(1 << DATA_PIN6)|(1 << DATA_PIN7));
	DATA_PORT &= ~((1 << DATA_PIN0)|(1 << DATA_PIN1)|(1 << DATA_PIN2)|(1 << DATA_PIN3)|(1 << DATA_PIN4)|(1 << DATA_PIN5)|(1 << DATA_PIN6)|(1 << DATA_PIN7));
#endif /* PIN8	 */

#ifdef LCD1602
	Send_Cmd(0b00111000);					// Function set // Sets to 8-bit operation and selects 2-line display and 5 ? 8	dot character font.
	Send_Cmd(0b00001100);					// Display on/off control
	Send_Cmd(0b00000110);					// Entry mode set
	Send_Cmd(CMD_DISPLAYCLR);				// Clear
#endif /* LCD1602 */

#ifdef LCD2004
	Send_Cmd(0b00111000);					// Function set // Sets to 8-bit operation and selects 2-line display and 5 ? 8	dot character font.
	Send_Cmd(0b00001100);					// Display on/off control
	Send_Cmd(0b00000110);					// Entry mode set
	Send_Cmd(CMD_DISPLAYCLR);				// Clear
#endif /* LCD2004 */
}


void LCD_Set_Pos(char x, char y)
{
#ifdef LCD1602
	ddram_adr = (0x40 * y + x);
	Send_Cmd(ddram_adr | 0b10000000);
	
#endif /* LCD1602 */
	
#ifdef LCD2004	
	switch (y)
	{
		case 0:
			ddram_adr = (0 + x);
			break;
		case 1:
			ddram_adr = (64 + x);
			break;
		case 2:
			ddram_adr = (20 + x);
			break;
		case 3:
			ddram_adr = (84 + x);
			break;
		
		default:
			break;
	}
	
	Send_Cmd(ddram_adr | 0b10000000);
	
#endif /* LCD2004 */	
}


void LCD_Print_Char(char c_ascii)
{
	Send_Data((char)c_ascii);
	ddram_adr++;
	
#ifdef LCD1602
	// Это нужно перепроверить!
	switch (ddram_adr)
	{
		case 17:
		ddram_adr = 64;
		break;
		case 84:
		ddram_adr = 0;
		break;
		
		default:
		break;
	}
	Send_Cmd(CMD_DDRAM_ADR | ddram_adr);
#endif /* LCD1602 */

#ifdef LCD2004
	switch (ddram_adr)
	{
		case 20:
		ddram_adr = 64;
		break;
		case 84:
		ddram_adr = 20;
		break;
		case 40:
		ddram_adr = 84;
		break;
		case 104:
		ddram_adr = 0;
		break;
		
		default:
		break;
	}
	
	Send_Cmd(CMD_DDRAM_ADR | ddram_adr);
#endif /* LCD2004 */
}


void LCD_Print_String(char* s_ascii)
{	
	int array_counter = 0;
	
	while (s_ascii[array_counter] != '\0')
	{	
		Send_Data((char)s_ascii[array_counter++]);
		ddram_adr++;

#ifdef LCD1602
		// Это нужно перепроверить!
		switch (ddram_adr)
		{
			case 17:
				ddram_adr = 64;
				break;
			case 84:
				ddram_adr = 0;
				break;
			
			default:
				break;
		}
		Send_Cmd(CMD_DDRAM_ADR | ddram_adr);
#endif /* LCD1602 */	

#ifdef LCD2004
		switch (ddram_adr)
		{
			case 20:
				ddram_adr = 64;
				break;
			case 84:
				ddram_adr = 20;
				break;
			case 40:
				ddram_adr = 84;
				break;	
			case 104:
				ddram_adr = 0;
				break;
				
			default:
				break;
		}
		
		Send_Cmd(CMD_DDRAM_ADR | ddram_adr);
#endif /* LCD2004 */					
	}
}


void LCD_Load_to_CGRAM(char CGRAM_adr, char symbol_map[8])
{	
#ifdef LCD2004
	char temp = ddram_adr;					// Почему-то при записи в CGRAM увеличивается адрес DDRAM. Поэтому его надо сохранять в temp и перезаписывать заново, в конце функции, чтобы он не терялся
	
	Send_Cmd(CMD_CGRAM_ADR | (CGRAM_adr * 8));
	
	for (int i = 0; i < 8; i++)
	{
		Send_Data(symbol_map[i]);
	}
		
	Send_Cmd(CMD_DDRAM_ADR | temp);		
#endif /* LCD2004 */
}


void LCD_Load_pack_to_CGRAM(char pack_size, char pack_map[][8])
{
#ifdef LCD2004
	for (int i = 1; i < (pack_size + 1); i++)
	{
		LCD_Load_to_CGRAM(i, pack_map[i - 1]);
	}
	
#endif /* LCD2004 */	
}
