#include <SSD1803a_I2C.h>

const byte i2caddr  = 0x3c;
const byte resetPin = PIN_E6;

SSD1803a_I2C lcd(i2caddr,resetPin);

void setup() {  
  lcd.begin(20,4);
}

void loop() {
  lcd.moveCursorRight();
  lcd.moveCursorRight();
  lcd.print("DERMALOG");
  for(byte i=0; i<10;++i)lcd.moveCursorLeft();
  lcd.print("-=");
  for(byte i=0; i<8;++i)lcd.moveCursorRight();
  lcd.print("=-");
 
  for(;;);
}