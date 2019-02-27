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

#ifndef SSD1803a_I2C_4x20_H
#define SSD1803a_I2C_4x20_H

#include <Arduino.h>
/*
#if ARDUINO >= 100
#include <Arduino.h>       // for delayMicroseconds, digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif
*/

//#include <LCD.h> // from newLiquidCrystal
#include <Print.h>
#include <inttypes.h>
#include <Wire.h>

typedef enum { POSITIVE, NEGATIVE } Polarity;

#define BACKLIGHT_ON 255
#define BACKLIGHT_OFF 0

class SSD1803a_I2C_4x20 : public Print
{
public:
	/*!
	 @method
	 @abstract   Class constructor.
	 @discussion Initializes class variables and defines the I2C address of the
	 LCD. The constructor does not initialize the LCD.

	 @param      lcd_Addr[in] I2C address of the IO expansion module. For I2CLCDextraIO,
	 the address can be configured using the on board jumpers.
	 */
	SSD1803a_I2C_4x20(uint8_t lcd_Addr);
	
	// Constructor with reset pin
	SSD1803a_I2C_4x20(uint8_t lcd_Addr, uint8_t resetPin);

	/*!
	 @function
	 @abstract   LCD initialization and associated HW.
	 @discussion Initializes the LCD to a given size (col, row). This methods
	 initializes the LCD, therefore, it MUST be called prior to using any other
	 method from this class or parent class.
	 
	 The begin method can be overloaded if necessary to initialize any HW that
	 is implemented by a library and can't be done during construction, here
	 we use the Wire class.
	 
	 Note: support don't yes implemented for all this parameters
	 
	 @param      cols[in] the number of columns that the display has
	 @param      rows[in] the number of rows that the display has
	 @param      charsize[in] size of the characters of the LCD: LCD_5x8DOTS or
	 LCD_5x10DOTS.
	*/
	virtual void begin(uint8_t cols=20, uint8_t rows=4);
	
	/*!
	 @function
	 @abstract   Clears the LCD.
	 @discussion Clears the LCD screen and positions the cursor in the upper-left
	 corner.

	 This operation is time consuming for the LCD.

	 @param      none
	 */
	void clear();

	/*!
	 @function
	 @abstract   Sets the cursor to the upper-left corner.
	 @discussion Positions the cursor in the upper-left of the LCD.
	 That is, use that location in outputting subsequent text to the display.
	 To also clear the display, use the clear() function instead.

	 This operation is time consuming for the LCD.

	 @param      none
	 */
	void home();

	/*!
	 @function
	 @abstract   Turns off the LCD display.
	 @discussion Turns off the LCD display, without losing the text currently
	 being displayed on it.

	 @param      none
	 */
	void noDisplay();

	/*!
	 @function
	 @abstract   Turns on the LCD display.
	 @discussion Turns on the LCD display, after it's been turned off with
	 noDisplay(). This will restore the text (and cursor location) that was on
	 the display prior to calling noDisplay().

	 @param      none
	 */
	void display();

	/*!
	 @function
	 @abstract   Turns off the blinking of the LCD cursor.

	 @param      none
	 */
	void noBlink();

	/*!
	 @function
	 @abstract   Display the cursor of the LCD.
	 @discussion Display the blinking LCD cursor. If used in combination with
	 the cursor() function, the result will depend on the particular display.

	 @param      none
	 */
	void blink();

	/*!
	 @function
	 @abstract   Hides the LCD cursor.

	 @param      none
	 */
	void noCursor();

	/*!
	 @function
	 @abstract   Display the LCD cursor.
	 @discussion Display the LCD cursor: an underscore (line) at the location
	 where the next character will be written.

	 @param      none
	 */
	void cursor();

	/*!
	 @function
	 @abstract   Scrolls the contents of the display (text and cursor) one space
	 to the left.

	 @param      none
	 */
	void scrollDisplayLeft() {};

	/*!
	 @function
	 @abstract   Scrolls the contents of the display (text and cursor) one space
	 to the right.

	 @param      none
	 */
	void scrollDisplayRight() {};

	/*!
	 @function
	 @abstract   Set the direction for text written to the LCD to left-to-right.
	 @discussion Set the direction for text written to the LCD to left-to-right.
	 All subsequent characters written to the display will go from left to right,
	 but does not affect previously-output text.

	 This is the default configuration.

	 @param      none
	 */
	void leftToRight();

	/*!
	 @function
	 @abstract   Set the direction for text written to the LCD to right-to-left.
	 @discussion Set the direction for text written to the LCD to right-to-left.
	 All subsequent characters written to the display will go from right to left,
	 but does not affect previously-output text.

	 left-to-right is the default configuration.

	 @param      none
	 */
	void rightToLeft();

	/*!
	 @function
	 @abstract   Moves the cursor one space to the left.
	 @discussion
	 @param      none
	 */
	void moveCursorLeft();


	/*!
	 @function
	 @abstract   Moves the cursor one space to the right.

	 @param      none
	 */
	void moveCursorRight();

	/*!
	 @function
	 @abstract   Turns on automatic scrolling of the LCD.
	 @discussion Turns on automatic scrolling of the LCD. This causes each
	 character output to the display to push previous characters over by one
	 space. If the current text direction is left-to-right (the default),
	 the display scrolls to the left; if the current direction is right-to-left,
	 the display scrolls to the right.
	 This has the effect of outputting each new character to the same location on
	 the LCD.

	 @param      none
	 */
	void autoscroll();

	/*!
	 @function
	 @abstract   Turns off automatic scrolling of the LCD.
	 @discussion Turns off automatic scrolling of the LCD, this is the default
	 configuration of the LCD.

	 @param      none
	 */
	void noAutoscroll();

	/*!
	 @function
	 @abstract   Creates a custom character for use on the LCD.
	 @discussion Create a custom character (glyph) for use on the LCD.
	 Most chipsets only support up to eight characters of 5x8 pixels. Therefore,
	 this methods has been limited to locations (numbered 0 to 7).

	 The appearance of each custom character is specified by an array of eight
	 bytes, one for each row. The five least significant bits of each byte
	 determine the pixels in that row. To display a custom character on screen,
	 write()/print() its number, i.e. lcd.print (char(x)); // Where x is 0..7.

	 @param      location[in] LCD memory location of the character to create
	 (0 to 7)
	 @param      charmap[in] the bitmap array representing each row of the character.
	 */
	void createChar(uint8_t location, uint8_t charmap[]);

	/*!
	 @function
	 @abstract   Position the LCD cursor.
	 @discussion Sets the position of the LCD cursor. Set the location at which
	 subsequent text written to the LCD will be displayed.

	 @param      col[in] LCD column
	 @param      row[in] LCD row - line.
	 */
	void setCursor(uint8_t col, uint8_t row);

	/*!
	 @function
	 @abstract   Switch-on the LCD backlight.
	 @discussion Switch-on the LCD backlight.
	 The setBacklightPin has to be called before setting the backlight for
	 this method to work. @see setBacklightPin.
	 */
	void backlight(void);

	/*!
	 @function
	 @abstract   Switch-off the LCD backlight.
	 @discussion Switch-off the LCD backlight.
	 The setBacklightPin has to be called before setting the backlight for
	 this method to work. @see setBacklightPin.
	 */
	void noBacklight(void);

	/*!
	 @function
	 @abstract   Switch on the LCD module.
	 @discussion Switch on the LCD module, it will switch on the LCD controller
	 and the backlight. This method has the same effect of calling display and
	 backlight. @see display, @see backlight
	 */
	void on(void) {};

	/*!
	 @function
	 @abstract   Switch off the LCD module.
	 @discussion Switch off the LCD module, it will switch off the LCD controller
	 and the backlight. This method has the same effect of calling noDisplay and
	 noBacklight. @see display, @see backlight
	 */
	void off(void) {};

	//
	// virtual class methods
	// --------------------------------------------------------------------------
	/*!
	 @function
	 @abstract   Sets the pin to control the backlight.
	 @discussion Sets the pin in the device to control the backlight.
	 This method is device dependent and can be programmed on each subclass. An
	 empty function call is provided that does nothing.

	 @param      pin: pin associated to backlight control.
	 @param      pol: backlight polarity control (POSITIVE, NEGATIVE)
	 */
	virtual void setBacklightPin(uint8_t pin, Polarity pol);

	/*!
	 @function
	 @abstract   Sets the pin to control the backlight.
	 @discussion Sets the pin in the device to control the backlight. The behavior
	 of this method is very dependent on the device. Some controllers support
	 dimming some don't. Please read the actual header file for each individual
	 device. The setBacklightPin method has to be called before setting the backlight
	 or the adequate backlight control constructor.
	 @see setBacklightPin.

	 NOTE: The preferred methods to control the backlight are "backlight" and
	 "noBacklight".

	 @param      0..255 - the value is very dependent on the LCD. However,
	 BACKLIGHT_OFF will be interpreted as off and BACKLIGHT_ON will drive the
	 backlight on.
	 */
	virtual void setBacklight(uint8_t value) {};

	virtual void setContrast(uint8_t contrast);
	/*!
	 @function
	 @abstract   Writes to the LCD.
	 @discussion This method writes character to the LCD in the current cursor
	 position.

	 This is the virtual write method, implemented in the Print class, therefore
	 all Print class methods will end up calling this method.

	 @param      value[in] Value to write to the LCD.
	 */

	virtual size_t write(uint8_t value);

	virtual size_t write(const uint8_t *buffer, size_t size);

private:
	void reset();
	void init();
	void send_command(uint8_t cmd);
	void send_byte(uint8_t val);
	void send_array(const uint8_t *buffer, size_t size);
	void cursor_display(bool show, bool blink);
	uint8_t _cols, _numlines;
	uint8_t _i2caddr;
	uint8_t _resetPin;
	uint8_t _backlightPin;
	Polarity _backlightPinPolarity;
	byte _entrymode;
	bool _cursor_show,_cursor_blink;


};
#endif