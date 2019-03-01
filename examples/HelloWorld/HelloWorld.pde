#include <SSD1803a_I2C.h>

const byte i2caddr  = 0x3c;
const byte resetPin = PIN_E6;

SSD1803a_I2C lcd(i2caddr,resetPin);

void setup() {  
  lcd.begin(20,4);
}

void loop() {  
  lcd.setCursor(0,0); lcd.println("hello world");
  lcd.setCursor(0,1); lcd.println("Hello world");
  lcd.setCursor(0,2); lcd.println("Hello World");
  lcd.setCursor(0,3); lcd.println("HELLO WORLD");  
  for(;;);
}