#include <SSD1803a_I2C.h>

const byte i2caddr  = 0x3c;
const byte resetPin = PIN_E6;

SSD1803a_I2C lcd(i2caddr,resetPin);

void setup() {  
  lcd.begin(20,4);
}

void loop() {
  lcd.setCursor(6,0);
  lcd.print("DERMALOG");
  lcd.setCursor(6+7,1);
  lcd.rightToLeft();
  lcd.print("DERMALOG");
  for(;;);
}
