#ifndef LCD_H_
#define LCD_H_

// ���������� �����
// ����� 
#define CMD_PORT		PORTB
#define CMD_DDR			DDRB
#define DATA_PORT		PORTA
#define DATA_DDR		DDRA
#define DATA_PIN		PINA

// ����
#define DATA_PIN0		PA0
#define DATA_PIN1		PA1
#define DATA_PIN2		PA2
#define DATA_PIN3		PA3
#define DATA_PIN4		PA4
#define DATA_PIN5		PA5
#define DATA_PIN6		PA6
#define DATA_PIN7		PA7
#define BF_PIN			DATA_PIN7				// *��� �� ��������

#define CMD_PIN0		PB0
#define CMD_PIN1		PB1
#define CMD_PIN2		PB2
#define E				CMD_PIN0				// *��� �� ��������		// ��� �������/������	���� �� ������� �� ������ ����� ���������� 0, �� ������ ����� �������, ���� 1 � �� ��� ������.
#define RW				CMD_PIN1				// ��� ������/������	���� ����� 0 � �� �� � ���������� ������� ����� ������, � ���� 1 � �� ����� ������ ������ �� ����������� �������.
#define RS				CMD_PIN2				// ��� �����������		�� ���������� ������ (����� 1 �������� � 0) �� ������� ���������� ������� ��������, ��� ������ ������ �������� ������ ������ ������ �� ������ ������ D0 � D7, ���� �������� ������ �� ������ � ����������� ����� �� ��������� ����� RW.


// ������������ �����
// �������:						
// 1. Clear display - 0
#define CMD_DISPLAYCLR			0b00000001

// 7. Set CGRAM address
#define CMD_CGRAM_ADR			0b01000000		// ������� � ���� - Send_Cmd(����� ������� | ��������) - Send_Cmd(CMD_CGRAM_ADR | cgram_adr);

// 8. Set DDRAM address
#define CMD_DDRAM_ADR			0b10000000		// ������� � ���� - Send_Cmd(����� ������� | ��������) - Send_Cmd(CMD_DDRAM_ADR | ddram_adr);


void Send_Cmd(char comand);
void Send_Data(char data);
void LCD_Init(void);
void LCD_Set_Pos(char cur_x, char cur_y);
void LCD_Print_Char(char c_ascii);
void LCD_Print_String(char* s_ascii);
void LCD_Load_to_CGRAM(char CGRAM_adr, char symbol_map[8]);
void LCD_Load_pack_to_CGRAM(char pack_size, char pack_map[][8]);

#endif /* LCD_H_ */