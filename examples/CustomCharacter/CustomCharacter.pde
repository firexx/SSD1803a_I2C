
#include <SSD1803a_I2C.h>

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};

byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

const byte i2caddr  = 0x3c;
const byte resetPin = PIN_E6;

SSD1803a_I2C lcd(i2caddr,resetPin);

void setup() {  
  lcd.begin(20,4);
  // create a new character
  lcd.createChar(0, heart);
  // create a new character
  lcd.createChar(1, smiley);
  // create a new character
  lcd.createChar(2, frownie);
  // create a new character
  lcd.createChar(3, armsDown);
  // create a new character
  lcd.createChar(4, armsUp);  
}

void loop() {  
  lcd.setCursor(0,0); 
  lcd.print("I "); 
  lcd.print('\0'); 
  lcd.print(" DERMALOG");
  lcd.print('\1'); 
  lcd.setCursor(0,1);
  lcd.print('\0');
  lcd.print('\1');
  lcd.print('\2');
  lcd.print('\3');
  lcd.print('\4');
  for(;;);
}
