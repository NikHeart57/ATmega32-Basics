#include "ILI9488.h"

//////////////////////////////////////////////////////////////////////////
//  ���������� ���������� ���������
//////////////////////////////////////////////////////////////////////////

uint16_t ili9488_cursor_x = 0;									///< ������� ������� X ��� ������
uint16_t ili9488_cursor_y = 0;									///< ������� ������� Y ��� ������
uint16_t ili9488_font_size = ILI9488_FONT;						///< ������� ������ ������
uint16_t ili9488_font_color =  ILI9488_FONT_COLOR;				///< ������� ���� ������
uint16_t ili9488_font_bg_color = ILI9488_FONT_BACKGROUND;		///< ������� ���� ���� ������

//////////////////////////////////////////////////////////////////////////
//  ������ ����������� ������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ������ ����������� ������ � ������� RGB565
 * @note ������� ������ ��������������� ILI9488_ColorIndex
 */
const uint16_t ili9488_colors[ILI9488_COLOR_COUNT] = {
    0xF800, // RED:    11111 000000 00000
    0x07E0, // GREEN:  00000 111111 00000
    0x001F, // BLUE:   00000 000000 11111
    0xFFE0, // YELLOW: 11111 111111 00000
    0x07FF, // CYAN:   00000 111111 11111
    0xF81F, // MAGENTA:11111 000000 11111
    0xFFFF, // WHITE:  11111 111111 11111
    0x0000  // BLACK:  00000 000000 00000
};

//////////////////////////////////////////////////////////////////////////
//  �������������� ������� ���������� ��������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ��������� ����� �����-������ �� ��� ������ � ��������
 * @note �������������� ����� ������ � ���������� ��� ������ � ��������� ������� 0.
 */
void ILI9488_Setup(void)
{
    // ��������� ����� ������ (D0-D7) �� �����
    ILI9488_DATA_DDR    |=   (1 << ILI9488_DATA_DB0) | (1 << ILI9488_DATA_DB1) | (1 << ILI9488_DATA_DB2) | 
                        (1 << ILI9488_DATA_DB3) | (1 << ILI9488_DATA_DB4) | (1 << ILI9488_DATA_DB5) | 
                        (1 << ILI9488_DATA_DB6) | (1 << ILI9488_DATA_DB7);
    ILI9488_DATA_PORT   &= ~((1 << ILI9488_DATA_DB0) | (1 << ILI9488_DATA_DB1) | (1 << ILI9488_DATA_DB2) | 
                        (1 << ILI9488_DATA_DB3) | (1 << ILI9488_DATA_DB4) | (1 << ILI9488_DATA_DB5) | 
                        (1 << ILI9488_DATA_DB6) | (1 << ILI9488_DATA_DB7));
    
    // ��������� ����� ���������� (CS, RS, WR, RST) �� �����
    ILI9488_CMD_DDR     |=   (1 << ILI9488_CMD_CS) | (1 << ILI9488_CMD_RS) | (1 << ILI9488_CMD_WR) | (1 << ILI9488_CMD_RST);
    ILI9488_CMD_PORT    &= ~((1 << ILI9488_CMD_CS) | (1 << ILI9488_CMD_RS) | (1 << ILI9488_CMD_WR) | (1 << ILI9488_CMD_RST));    
}

//////////////////////////////////////////////////////////////////////////
//  ������� ������������� �������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ���������� ������� ������������� �������
 * @param[in] state ��������� �������
 * @param[in] orientation ���������� �������
 * @param[in] ili9488_font_size ������ ������
 * @param[in] ili9488_font_color ���� ������
 * @param[in] bg_color ���� ����
 * @return 1 � ������ ������, 0 ��� ������
 * @note ���������� ����������, ������������ ����� ���������� ��������� Init
 */
static uint8_t ILI9488_Init_Internal(ILI9488_DisplayState state,  ILI9488_Orientation orientation, uint16_t ili9488_font_size, uint16_t ili9488_font_color, uint16_t bg_color)
{
    // ���������� ����� �������
    ILI9488_RESET();

    // ��������� ���� (CS = LOW)
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_CS);        

    // ������������������ ������ �������������
    ILI9488_WriteCommand(0x01);    // Software Reset
    _delay_ms(10);

    ILI9488_WriteCommand(0x11);    // Exit Sleep
    _delay_ms(10);
    
    // ��������� ��������� ������� (������ 16-������ RGB565)
    ILI9488_WriteCommand(0x3A);    // Pixel Format Set
    ILI9488_WriteData(0x55);       // 16-bit color (RGB565)
    _delay_ms(10);

    // ��������� ����������
    ILI9488_WriteCommand(0x36);    // Memory Access Control
    ILI9488_WriteData(orientation);
    _delay_ms(10);
    
    // ���������/���������� �������
    if (state == ILI9488_DISPLAY_ON) 
	{
        ILI9488_WriteCommand(0x29);    // Display ON
    } else 
	{
        ILI9488_WriteCommand(0x28);    // Display OFF
    }
    _delay_ms(10);

    // ����������� ���� (CS = HIGH)
    ILI9488_CMD_PORT |= (1 << ILI9488_CMD_CS);        
    
    // ��������� ���������� ������
	ili9488_font_size = ili9488_font_size;
	ili9488_font_color = ili9488_font_color;
    ili9488_font_bg_color = bg_color;
    
    // ����� ������� �������
    ili9488_cursor_x = 0;
    ili9488_cursor_y = 0;
    
    return 1; // �������� �������������
}

/**
 * @brief ������� ������������� ����������� ������� ILI9488
 * @param[in] state ��������� ������� (���/����)
 * @return 1 � ������ ������, 0 ��� ������
 */
uint8_t ILI9488_Init(ILI9488_DisplayState state)
{
    return ILI9488_Init_Internal(state, ILI9488_ORIENTATION_LANDSCAPE, ILI9488_FONT, ili9488_font_color, ili9488_font_bg_color);
}

/**
 * @brief ������������� ����������� ������� ILI9488 � ������� ����������
 * @param[in] state ��������� ������� (���/����)
 * @param[in] orientation ���entation �������
 * @return 1 � ������ ������, 0 ��� ������
 */
uint8_t ILI9488_Init(ILI9488_DisplayState state, ILI9488_Orientation orientation)
{
    return ILI9488_Init_Internal(state, orientation, ILI9488_FONT, ili9488_font_color, ili9488_font_bg_color);
}

/**
 * @brief ����������� ������������� ����������� ������� ILI9488
 * @param[in] state ��������� ������� (���/����)
 * @param[in] orientation ���������� �������
 * @param[in] ili9488_font_size ������ ������ �� ���������
 * @param[in] ili9488_font_color ���� ������ �� ���������
 * @param[in] bg_color ���� ���� �� ���������
 * @return 1 � ������ ������, 0 ��� ������
 */
uint8_t ILI9488_Init(ILI9488_DisplayState state, 
                    ILI9488_Orientation orientation,
                    uint16_t ili9488_font_size, 
                    uint16_t ili9488_font_color, 
                    uint16_t bg_color)
{
    return ILI9488_Init_Internal(state, 
                                orientation,
                                ili9488_font_size,
                                ili9488_font_color,
                                bg_color);
}



/**
 * @brief ���������� ����� (Reset) �������
 * @note ��������� ������� ������� ������ (~100 ���) �� ����� RST.
 */
void ILI9488_RESET(void)
{
    ILI9488_CMD_PORT  &= ~(1 << ILI9488_CMD_RST); // RESET = 0
    _delay_us(100);
    ILI9488_CMD_PORT  |= (1 << ILI9488_CMD_RST);  // RESET = 1
    _delay_us(900);
}

/**
 * @brief �������� ������� �� �������
 * @param[in] cmd ��� ������� ��� ��������
 * @note ������������� RS=0 (����� �������) � ��������� ����� ������.
 */
void ILI9488_WriteCommand(uint8_t cmd) 
{
    // ���������� ������ (WR � HIGH)
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);             
    
    // �������� ������: ����� ������� (RS=0) � ����������� ���� ������� �� ���� ������
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_RS);             
    ILI9488_DATA_PORT =  cmd;                           
    
    // ������������ ������ (��������� ����� WR)
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_WR);             
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);
}

/**
 * @brief �������� ������ �� �������
 * @param[in] data ������ ��� ��������
 * @note ������������� RS=1 (����� ������) � ��������� ����� ������.
 */
void ILI9488_WriteData(uint8_t data) 
{    
    // ���������� ������ (WR � HIGH)
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);             
    
    // �������� ������: ����� ������ (RS=1) � ����������� ������ �� ����
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_RS);             
    ILI9488_DATA_PORT =  data;                          
    
    // ������������ ������ (��������� ����� WR)
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_WR);             
    ILI9488_CMD_PORT |=  (1 << ILI9488_CMD_WR);
}

//////////////////////////////////////////////////////////////////////////
//  ������� ��������� ����������� ����������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ���������� ����� ������ ��������� ������
 * @param[in] color ���� ���������� � ������� RGB565
 * @note ������������� ������� ������ �� ���� ����� � ��������������� ���������� ���� ������� �������.
 * @warning ����� �������� �������� �� ������ �������� ��.
 */
void ILI9488_FillScreen(uint16_t color) 
{
    uint8_t hi = color >> 8;   // ������� ���� ����� (R+G)
    uint8_t lo = color & 0xFF; // ������� ���� ����� (G+B)

    // ��������� ����
    ILI9488_CMD_PORT  &= ~(1 << ILI9488_CMD_CS);    

    // ��������� ������ �������� (X: 0-479)
    ILI9488_WriteCommand(0x2A); // Column Address Set
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x01);
    ILI9488_WriteData(0xDF);    // 480 columns (0x01DF = 479)

    // ��������� ������ ����� (Y: 0-319)
    ILI9488_WriteCommand(0x2B); // Page Address Set
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x01);
    ILI9488_WriteData(0x3F);    // 320 rows (0x013F = 319)

    // ������� ������ ������ � ������
    ILI9488_WriteCommand(0x2C); // Memory Write

    // ���������� ������ ������
    for (uint16_t i = 0; i < ILI9488_SCREEN_WIDTH; i++) 
    {
        for (uint16_t j = 0; j < ILI9488_SCREEN_HEIGHT; j++)
        {
            ILI9488_WriteData(hi); // ������� ����
            ILI9488_WriteData(lo); // ������� ����
        }
    }

    // ����������� ����
    ILI9488_CMD_PORT  |= (1 << ILI9488_CMD_CS);   
}

/**
 * @brief ��������� ������ �������
 * @param[in] x ���������� X (0..SCREEN_WIDTH-1)
 * @param[in] y ���������� Y (0..SCREEN_HEIGHT-1)
 * @param[in] color ���� ������� � ������� RGB565
 */
void ILI9488_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    // ��������� ����
    ILI9488_CMD_PORT &= ~(1 << ILI9488_CMD_CS);  

    // ��������� ������� (1x1 �������) �� X
    ILI9488_WriteCommand(0x2A);  // Column Address Set
    ILI9488_WriteData(x >> 8);
    ILI9488_WriteData(x & 0xFF);
    ILI9488_WriteData(x >> 8);
    ILI9488_WriteData(x & 0xFF);

    // ��������� ������� (1x1 �������) �� Y
    ILI9488_WriteCommand(0x2B);  // Page Address Set
    ILI9488_WriteData(y >> 8);
    ILI9488_WriteData(y & 0xFF);
    ILI9488_WriteData(y >> 8);
    ILI9488_WriteData(y & 0xFF);

    // ������ ����� �������
    ILI9488_WriteCommand(0x2C);  // Memory Write
    ILI9488_WriteData(hi);
    ILI9488_WriteData(lo);

    // ����������� ����
    ILI9488_CMD_PORT |= (1 << ILI9488_CMD_CS);  
}

/**
 * @brief ��������� ����� �� ��������� ����������
 * @param[in] x0 ��������� ���������� X (0..SCREEN_WIDTH-1)
 * @param[in] y0 ��������� ���������� Y (0..SCREEN_HEIGHT-1)
 * @param[in] x1 �������� ���������� X (0..SCREEN_WIDTH-1)
 * @param[in] y1 �������� ���������� Y (0..SCREEN_HEIGHT-1)
 * @param[in] color ���� ����� � ������� RGB565
 * 
 * @note ����������� ����������:
 *  - ���������� ������������� �������� ����������
 *  - �������� � ����� ������������ ����� (������� ���������)
 *  - �� ��������� �������� ������ �� ������� ������
 *  - ������ ������� ������� ��������� � �������� �����
 * 
 * @warning ������������������ ������� �� ����� �����
 *          (��� ������� ����� �������� ��������)
 */
void ILI9488_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) 
{
	int16_t dx = abs(x1 - x0);
	int16_t dy = abs(y1 - y0);
	int16_t sx = (x0 < x1) ? 1 : -1;
	int16_t sy = (y0 < y1) ? 1 : -1;
	int16_t err = dx - dy;

	while (1) 
	{
		ILI9488_DrawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		
		int16_t e2 = 2 * err;
		
		if (e2 > -dy) 
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

/**
 * @brief ��������� �������������� ��� ������������ ��������������
 * @param[in] x ���������� ������ �������� ���� X (0..SCREEN_WIDTH-1)
 * @param[in] y ���������� ������ �������� ���� Y (0..SCREEN_HEIGHT-1)
 * @param[in] w ������ �������������� � �������� (>=1)
 * @param[in] h ������ �������������� � �������� (>=1)
 * @param[in] color ���� � ������� RGB565
 * @param[in] filled ���� ������� (0 - ������ ������, 1 - �����������)
 * 
 * @note ����������� ����������:
 *  - ��� ������� �������� 4 ��������� �����
 *  - ��� ������� �������� �������������� ����� �� ���� ������
 *  - ������������� ������������ ���������� (x+w-1, y+h-1)
 * 
 * @warning �� ��������� �������� �� ����� �� ������� ������
 *          ��� ������� �������� ������������ ��������������
 *          �������� ������������ ��������
 */
void ILI9488_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint8_t filled) 
{
	if (filled) 
	{
		for (uint16_t i = y; i < y + h; i++) 
		{
			ILI9488_DrawLine(x, i, x + w - 1, i, color);
		} 
	}
	else 
	{
		ILI9488_DrawLine(x, y, x + w - 1, y, color);					// �������
		ILI9488_DrawLine(x, y + h - 1, x + w - 1, y + h - 1, color);	// ������
		ILI9488_DrawLine(x, y, x, y + h - 1, color);					// �����
		ILI9488_DrawLine(x + w - 1, y, x + w - 1, y + h - 1, color);	// ������
	}
}

/**
 * @brief ��������� ���������� ��� ������������ �����
 * @param[in] x0 ���������� ������ X (0..SCREEN_WIDTH-1)
 * @param[in] y0 ���������� ������ Y (0..SCREEN_HEIGHT-1)
 * @param[in] r ������ ���������� � �������� (>=1)
 * @param[in] color ���� � ������� RGB565
 * @param[in] filled ���� ������� (0 - ������ ������, 1 - �����������)
 * 
 * @note ����������� ����������:
 *  - ���������� �������� ���������� ��� �����������
 *  - ��� ������� ������ 8 ������������ ����� �� ������ ����
 *  - ��� ������� ������ �������������� ����� ����� ������������� �������
 *  - ������������ ����������� ����������
 * 
 * @warning ������������������ ������ ������� �� �������
 *          �� ��������� �������� �� ����� �� ������� ������
 *          ��� ������� �������� �������� ������������ ��������
 */
void ILI9488_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint8_t filled) 
{

	int16_t x = r;
	int16_t y = 0;
	int16_t err = 0;

	while (x >= y) 
	{
		if (filled) 
		{
			ILI9488_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
			ILI9488_DrawLine(x0 - y, y0 + x, x0 + y, y0 + x, color);
			ILI9488_DrawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
			ILI9488_DrawLine(x0 - y, y0 - x, x0 + y, y0 - x, color);
		} 
		else
		{
			ILI9488_DrawPixel(x0 + x, y0 + y, color);
			ILI9488_DrawPixel(x0 + y, y0 + x, color);
			ILI9488_DrawPixel(x0 - y, y0 + x, color);
			ILI9488_DrawPixel(x0 - x, y0 + y, color);
			ILI9488_DrawPixel(x0 - x, y0 - y, color);
			ILI9488_DrawPixel(x0 - y, y0 - x, color);
			ILI9488_DrawPixel(x0 + y, y0 - x, color);
			ILI9488_DrawPixel(x0 + x, y0 - y, color);
		}

		if (err <= 0) 
		{
			y++;
			err += 2 * y + 1;
		}
		
		if (err > 0) 
		{
			x--;
			err -= 2 * x + 1;
		}
	}

}

//////////////////////////////////////////////////////////////////////////
//  ������� ������ ������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ����� ������ ������� �� �����
 * @param[in] c ������ ��� �����������
 * @note ������������ ����������� ������� � �������������� �������.
 */
void ILI9488_PrintChar(char c)
{
	if (c == '\a')			// ������
	{
		return;	// �� �����������
	}
	else if (c == '\b')		// backspace
	{
		// ���������� ������ �� ������ ������ ������� �����
		if (ili9488_cursor_x >= ili9488_font_size * 6) 
		{
			ili9488_cursor_x -= ili9488_font_size * 6;
		} 
		else 
		{
			// ���� �� � ������ ������, ��������� �� ���������� ������
			if (ili9488_cursor_y >= ili9488_font_size * 8) 
			{
				ili9488_cursor_y -= ili9488_font_size * 8;
				ili9488_cursor_x = ILI9488_SCREEN_WIDTH - ili9488_font_size * 6; // ����� ���������� ������
			} 
			else 
			{
				// ���� �� � ����� ������ ������, ������ �� ������
				ili9488_cursor_x = 0;
			}
		}
		// ������� ������ (������ ������ �������������)
		for (uint8_t x = 0; x < 6; x++) 
		{
			for (uint8_t y = 0; y < 8; y++) 
			{
				ILI9488_DrawRect(ili9488_cursor_x + x*ili9488_font_size, ili9488_cursor_y + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, BLACK, true);
			}
		}
		return;
	}
	else if (c == '\t')		// ���������
	{
		// ���������� ������ � ��������� ������� ��������� (������ 8 �������)
		uint16_t tab_size = 8 * 6 * ili9488_font_size; // 4 ������� * 6 �������� * �������
		uint16_t tab_pos = ((ili9488_cursor_x / tab_size) + 1) * tab_size;
		
		// ���� ��������� ������� �� ������� ������, ��������� �� ��������� ������
		if (tab_pos + (ili9488_font_size * 6) > ILI9488_SCREEN_WIDTH) {
			ili9488_cursor_x = 0;
			ili9488_cursor_y += ili9488_font_size * 8;
			
			// �������� ������������ �� ���������
			if (ili9488_cursor_y + (ili9488_font_size * 8) > ILI9488_SCREEN_HEIGHT) {
				ili9488_cursor_y = 0;
			}
			} else {
			ili9488_cursor_x = tab_pos;
		}
		return;
	}
	else if (c == '\n')		// ������� �������
	{
		ili9488_cursor_y += ili9488_font_size * 8;
	}
	else if (c == '\r')		// ������� �������
	{
		ili9488_cursor_x = 0;
	}
	else if (c >= 32 && c <= 127) // ���������� �������
	{
		for(uint8_t x = 0; x < 5; x++)
		{
			for (uint8_t y = 0; y < 8; y++)
			{
				uint8_t mask = 1;
				
				if(monocraft[(uint8_t)c - 32][x] & (mask << (7 - y)))
				{
					ILI9488_DrawRect(ili9488_cursor_x, ili9488_cursor_y + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, ili9488_font_color, true);
				}
				else
				{
					ILI9488_DrawRect(ili9488_cursor_x, ili9488_cursor_y  + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, ili9488_font_bg_color, true);
				}
			}
			ili9488_cursor_x += ili9488_font_size;
		}
		
		// ��������� ���� ������� �������������� ���������
		for (uint8_t y = 0; y < 8; y++)
		{
			ILI9488_DrawRect(ili9488_cursor_x, ili9488_cursor_y + y*ili9488_font_size, ili9488_font_size, ili9488_font_size, ili9488_font_bg_color, true);
		}
		ili9488_cursor_x += ili9488_font_size;
	}
		
	// �������� ������������ �� �����������
	if (ili9488_cursor_x + (ili9488_font_size * 6) > ILI9488_SCREEN_WIDTH)
	{
		ili9488_cursor_x = 0;
		ili9488_cursor_y += ili9488_font_size * 8;
	}
	
	// �������� ������������ �� ���������
	if (ili9488_cursor_y + (ili9488_font_size * 8) > ILI9488_SCREEN_HEIGHT)
	{
		ili9488_cursor_x = 0;
		ili9488_cursor_y = 0;
	}
}


/**
 * @brief ����� ������ �� �����
 * @param[in] str ��������� �� ������ (����-���������������)
 */
void ILI9488_PrintStr(char* str)
{
    while(*str)
    {
        ILI9488_PrintChar(*str++);
    }
}

//////////////////////////////////////////////////////////////////////////
//  ��������������� �������
//////////////////////////////////////////////////////////////////////////

/**
 * @brief ����������� ����� �� 24-������� RGB � 16-������ RGB565
 * @param[in] r ���������� Red (0-255)
 * @param[in] g ���������� Green (0-255)
 * @param[in] b ���������� Blue (0-255)
 * @return ���� � ������� RGB565
 */
uint16_t ILI9488_RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

