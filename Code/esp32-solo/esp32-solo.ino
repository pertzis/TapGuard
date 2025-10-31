#include <MFRC522.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(14, 27, 26, 25, 33, 32);

#define GREEN_LED 21
#define RED_LED 17
#define BUZZER 13

// RC522 pins
#define SS_PIN 18   // SDA / SS
#define RST_PIN 16  // RST

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {

  Serial.begin(115200);
  SPI.begin(5, 19, 23, 18);  // SCK, MISO, MOSI, SS
  delay(500);
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();

  lcd.begin(20, 4);
  lcd.print("Scan a card:");

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);


}

void loop() {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Display UID on Serial + LCD
  Serial.print("UID: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UID:");

  String cardUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    lcd.print(rfid.uid.uidByte[i], HEX);
    lcd.print(" ");
    cardUID += String(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  delay(1500);
  lcd.clear();
  lcd.print("Scan next card...");
}
