/*
	LCD MIDAS Display with SSD1803a controller for 4x20
	Copyright(C) 2019 Andrej Kostrov

	This library is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free
	Software Foundation; either version 2.1 of the License, or (at your option)
	any later version.

	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along	with this library;
	if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA
*/

#include "SSD1803a_I2C.h"

#define COMMAND_CLEAR_DISPLAY						0x01
#define COMMAND_RETURN_HOME							0x02
#define COMMAND_ENTRY_MODE_SET						0x04
	#define ENTRY_MODE_LEFT_TO_RIGHT                0x02
	#define ENTRY_MODE_SHIFT_INCREMENT				0x01
#define COMMAND_CURSORSHIFT							0x10
	#define CURSORSHIFT_MOVERIGHT					0x04
	#define CURSORSHIFT_MOVELEFT					0x00

#define COMMAND_ADDRESS_DDRAM						0x80
#define COMMAND_ADDRESS_CGRAM                       0x40

#define COMMAND_8BIT_4LINES_NORMAL_RE1_IS0			0x3A  //Extended command access RE = 1
#define COMMAND_8BIT_4LINES_REVERSE_RE1_IS0			0x3B  //Extended command access RE = 1
#define COMMAND_8BIT_4LINES_RE0_IS1					0x39  //Extended command access IS = 1
#define COMMAND_8BIT_4LINES_RE0_IS0					0x38  //Normal mode...


//Command from extended set (RE = 1, IS = 0)
#define COMMAND_BS1_1								0x1E
#define COMMAND_POWER_DOWN_DISABLE					0x02
#define COMMAND_SEGMENT_NORMAL_WAY					0x05
#define COMMAND_NW									0x09

//Command from extended set (RE = 0, IS = 1)
#define COMMAND_DISPLAY_ON_CURSOR_ON_BLINK_ON		0x0F
#define COMMAND_DISPLAY_ON_CURSOR_ON_BLINK_OFF		0x0E
#define COMMAND_DISPLAY_ON_CURSOR_OFF_BLINK_OFF		0x0C
#define COMMAND_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF	0x08
#define COMMAND_BS0_1								0x1C
#define COMMAND_INTERNAL_DIVIDER					0x13
#define COMMAND_CONTRAST							0x77
#define COMMAND_POWER_ICON_CONTRAST					0x5C
#define COMMAND_FOLLOWER_CONTROL					0x6E



SSD1803a_I2C::SSD1803a_I2C(uint8_t lcd_Addr)
	:_i2caddr(lcd_Addr), _resetPin(0xFF), _backlightPin(0xFF), _cursor_show(false), _cursor_blink(false)
{

}

SSD1803a_I2C::SSD1803a_I2C(uint8_t lcd_Addr, uint8_t resetPin)
	: _i2caddr(lcd_Addr), _resetPin(resetPin), _backlightPin(0xFF), _cursor_show(false), _cursor_blink(false)
{

}

void SSD1803a_I2C::begin(uint8_t cols, uint8_t rows)
{
	_cols = cols;
	_numlines = rows;
	Wire.begin();
	reset();
	init();

	clear();
	home();
	_entrymode = ENTRY_MODE_LEFT_TO_RIGHT;
	send_command(COMMAND_ENTRY_MODE_SET | _entrymode);
}

void SSD1803a_I2C::clear()
{
	send_command(COMMAND_CLEAR_DISPLAY);
}

void SSD1803a_I2C::home()
{
	send_command(COMMAND_RETURN_HOME);
}

void SSD1803a_I2C::noDisplay()
{
	send_command(COMMAND_8BIT_4LINES_RE0_IS1);  //Function set : RE = 0, IS = 1
	send_command(COMMAND_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF | ((_cursor_show) ? 0x02 : 0x00) | ((_cursor_blink) ? 0x01 : 0x00));
	send_command(COMMAND_8BIT_4LINES_RE0_IS0);  //Function set : RE = 0, IS = 0
}

void SSD1803a_I2C::display()
{
	send_command(COMMAND_8BIT_4LINES_RE0_IS1);  //Function set : RE = 0, IS = 1
	send_command(COMMAND_DISPLAY_ON_CURSOR_OFF_BLINK_OFF | ((_cursor_show) ? 0x02 : 0x00) | ((_cursor_blink) ? 0x01 : 0x00));
	send_command(COMMAND_8BIT_4LINES_RE0_IS0);  //Function set : RE = 0, IS = 0
}

void SSD1803a_I2C::noBlink()
{
	cursor_display(_cursor_show, false);
}

void SSD1803a_I2C::blink()
{
	cursor_display(_cursor_show, true);
}

void SSD1803a_I2C::noCursor()
{
	cursor_display(false, _cursor_blink);
}

void SSD1803a_I2C::cursor()
{
	cursor_display(false, _cursor_blink);
}

void SSD1803a_I2C::leftToRight()
{
	_entrymode |= ENTRY_MODE_LEFT_TO_RIGHT;
	send_command(COMMAND_ENTRY_MODE_SET | _entrymode);
}

void SSD1803a_I2C::rightToLeft()
{
	_entrymode &= ~ENTRY_MODE_LEFT_TO_RIGHT;
	send_command(COMMAND_ENTRY_MODE_SET | _entrymode);
}

void SSD1803a_I2C::moveCursorLeft()
{
	send_command(COMMAND_CURSORSHIFT | CURSORSHIFT_MOVELEFT);
}

void SSD1803a_I2C::moveCursorRight()
{
	send_command(COMMAND_CURSORSHIFT | CURSORSHIFT_MOVERIGHT);
}

void SSD1803a_I2C::autoscroll()
{
	_entrymode |= ENTRY_MODE_SHIFT_INCREMENT;
	send_command(COMMAND_ENTRY_MODE_SET | _entrymode);

}

void SSD1803a_I2C::noAutoscroll()
{
	_entrymode &= ~ENTRY_MODE_SHIFT_INCREMENT;
	send_command(COMMAND_ENTRY_MODE_SET | _entrymode);
}

void SSD1803a_I2C::createChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7;
	send_command(COMMAND_ADDRESS_CGRAM | (location << 3));

	for (uint8_t i = 0; i < 8; i++)
		send_byte(charmap[i]);
}

void SSD1803a_I2C::setCursor(uint8_t col, uint8_t row)
{
	row = min(row, _numlines - 1);
	col = min(col, _cols - 1);
	send_command(COMMAND_ADDRESS_DDRAM | (row * 0x20 + col));
}

void SSD1803a_I2C::backlight(void)
{
	setBacklight(BACKLIGHT_ON);
}

void SSD1803a_I2C::noBacklight(void)
{
	setBacklight(BACKLIGHT_OFF);
}

void SSD1803a_I2C::setBacklightPin(uint8_t pin, Polarity pol)
{
	_backlightPin = pin;
	_backlightPinPolarity = pol;
}

void SSD1803a_I2C::setBacklight(uint8_t value)
{
	if (_backlightPin == 0xFF)
		return;
	pinMode(_backlightPin, OUTPUT);
	analogWrite(_backlightPin, _backlightPinPolarity == POSITIVE?value:~value);
}

void SSD1803a_I2C::setContrast(uint8_t contrast)
{
	send_command(COMMAND_8BIT_4LINES_RE0_IS1);  //Function set : RE = 0, IS = 1
	send_command(0x70 | (contrast & 0x0F));     //Contrast
	send_command(COMMAND_POWER_ICON_CONTRAST | ((contrast >> 4) & 0x03)); //Power Icon control contrast
	send_command(COMMAND_8BIT_4LINES_RE0_IS0);  //Function set : RE = 0, IS = 0
}

size_t SSD1803a_I2C::write(uint8_t value)
{
	send_byte(value);
	return 1;
}

size_t SSD1803a_I2C::write(const uint8_t *buffer, size_t size)
{
	send_array(buffer, size);
	return size;
}

// -- private functions
void SSD1803a_I2C::reset()
{
	if (_resetPin == 0xFF)
		return;
	pinMode(_resetPin, OUTPUT);
	digitalWrite(_resetPin, LOW);
	delay(2);
	digitalWrite(_resetPin, HIGH);
	delay(2);
}

void SSD1803a_I2C::init()
{
	send_command(COMMAND_CLEAR_DISPLAY);  //Clear display

//RE = 1  |  IS = 0
	send_command(COMMAND_8BIT_4LINES_NORMAL_RE1_IS0);   //Function set : 8bit 4line RE = 1, IS = 0, Not reverse
	//LCD_Command(COMMAND_8BIT_4LINES_REVERSE_RE1_IS0); //Function set : RE = 1, REVERSE
	send_command(COMMAND_BS1_1);  //Set BS1 = 1 (1/6 bias)
	send_command(COMMAND_POWER_DOWN_DISABLE); //Power down disable
	send_command(COMMAND_SEGMENT_NORMAL_WAY); // Segment bidirection : in the right way
	send_command(COMMAND_NW); //NW = 1

	//RE = 0  |  IS = 1
	send_command(COMMAND_8BIT_4LINES_RE0_IS1);  //Function set : RE = 0, IS = 1
	send_command(COMMAND_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);  //Display ON (without cursor...)
	send_command(COMMAND_BS0_1);  //Set BS0 = 1 (1/6 bias) / Internal frequency..
	send_command(COMMAND_INTERNAL_DIVIDER); //Internal divider
	send_command(COMMAND_CONTRAST); //Constrast
	send_command(COMMAND_POWER_ICON_CONTRAST);  //Power Icon control contrast
	send_command(COMMAND_FOLLOWER_CONTROL); //Follower Control

	//RE = 0  | IS = 0
	send_command(COMMAND_8BIT_4LINES_RE0_IS0);  //Function set : 8bit 4line RE = 0, IS = 0

}

void SSD1803a_I2C::send_command(uint8_t cmd)
{
	Wire.beginTransmission(_i2caddr);
	Wire.write(0x00); // control byte for command
	Wire.write(cmd); // command clear display
	Wire.endTransmission();
}

void SSD1803a_I2C::send_byte(uint8_t val)
{
	Wire.beginTransmission(_i2caddr);
	Wire.write(0x40); // control byte for data
	Wire.write(val); // data byte
	Wire.endTransmission();
}

void SSD1803a_I2C::send_array(const uint8_t *buffer, size_t size) {
	Wire.beginTransmission(_i2caddr);
	Wire.write(0x40); // control byte for data
	while (size--)	Wire.write(*buffer++);
	Wire.endTransmission();
}


void SSD1803a_I2C::cursor_display(bool show, bool blink)
{
	_cursor_blink = blink;
	_cursor_show = show;

	send_command(COMMAND_8BIT_4LINES_RE0_IS1);  //Function set : RE = 0, IS = 1
	send_command(0x0C | ((show) ? 0x02 : 0x00) | ((blink) ? 0x01 : 0x00));
	send_command(COMMAND_8BIT_4LINES_RE0_IS0);  //Function set : RE = 0, IS = 0
}

