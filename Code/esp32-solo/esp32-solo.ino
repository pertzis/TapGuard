#include <LiquidCrystal.h>

#define D15 15
#define D13 13
#define D12 12
#define D14 14
#define D32 32
#define D35 35


LiquidCrystal lcd(14, 27, 26, 25, 32, 33);


void setup() {
  lcd.begin(20, 4);
  lcd.print("Hello ESP32!");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("It works!");
}

void loop() {
  // Optional animation
  delay(500);
}
