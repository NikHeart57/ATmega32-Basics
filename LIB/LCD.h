#ifndef LCD_H_
#define LCD_H_

// Изменяемая часть
// Порты 
#define CMD_PORT		PORTB
#define CMD_DDR			DDRB
#define DATA_PORT		PORTA
#define DATA_DDR		DDRA
#define DATA_PIN		PINA

// Пины
#define DATA_PIN0		PA0
#define DATA_PIN1		PA1
#define DATA_PIN2		PA2
#define DATA_PIN3		PA3
#define DATA_PIN4		PA4
#define DATA_PIN5		PA5
#define DATA_PIN6		PA6
#define DATA_PIN7		PA7
#define BF_PIN			DATA_PIN7				// *Это не меняется

#define CMD_PIN0		PB0
#define CMD_PIN1		PB1
#define CMD_PIN2		PB2
#define E				CMD_PIN0				// *Это не меняется		// Пин команда/данные	Если мы подадим на данную ножку логический 0, то значит будет команда, если 1 — то это данные.
#define RW				CMD_PIN1				// Пин запись/чтение	Если будет 0 — то мы в контроллер дисплея будем писать, а если 1 — то будем читать данные из контроллера дисплея.
#define RS				CMD_PIN2				// Пин тактировния		По спадающему фронту (когда 1 меняется в 0) на которой контроллер дисплея понимает, что именно сейчас наступил момент чтения данных на ножках данных D0 — D7, либо передачи данных из модуля в зависимости также от состояния ножки RW.


// Неизменяемая часть
// Команды:						
// 1. Clear display - 0
#define CMD_DISPLAYCLR			0b00000001

// 7. Set CGRAM address
#define CMD_CGRAM_ADR			0b01000000		// Пишутся в виде - Send_Cmd(Слово конмады | параметр) - Send_Cmd(CMD_CGRAM_ADR | cgram_adr);

// 8. Set DDRAM address
#define CMD_DDRAM_ADR			0b10000000		// Пишутся в виде - Send_Cmd(Слово конмады | параметр) - Send_Cmd(CMD_DDRAM_ADR | ddram_adr);


void Send_Cmd(char comand);
void Send_Data(char data);
void LCD_Init(void);
void LCD_Set_Pos(char cur_x, char cur_y);
void LCD_Print_Char(char c_ascii);
void LCD_Print_String(char* s_ascii);
void LCD_Load_to_CGRAM(char CGRAM_adr, char symbol_map[8]);
void LCD_Load_pack_to_CGRAM(char pack_size, char pack_map[][8]);

#endif /* LCD_H_ */