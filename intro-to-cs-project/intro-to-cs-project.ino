
#include <LiquidCrystal.h>  // Include LCD Library
#include <SPI.h>             // Include SPI.h for SPI Communication with the Arduino
#include <MFRC522.h>        // Include RFID Scanner Library

// LCD Pins
#define RS 7
#define ENABLE 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2

// Define RFID Reader SPI Pins
#define RST 9
#define SS 10
MFRC522 rc(SS, RST);

// LED Pins
uint8_t RED_LED = A0;
uint8_t GREEN_LED = A1;
// Buzzer
uint8_t BUZZER = A5;

// Authorized cards:
// 70 18 42 1F
// 3E 71 5E 64

byte cards[][4] = {
  {0x70, 0x18, 0x42, 0x1F},
  {0x3E, 0x71, 0x5E, 0x64}
};

String users[] = {
  "Petros",
  "Sam"
};

LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);




void setup() {
  SPI.begin();
  Serial.begin(9600);
  lcd.begin(20, 4);        // Initialize the 16x2 LCD
  lcd.print("Please scan your"); // Print a message to the LCD
  lcd.setCursor(0, 1);
  lcd.print("card.");
  rc.PCD_Init(); //initialize the receiver  
  rc.PCD_DumpVersionToSerial(); //show details of card reader module
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  tone(BUZZER, 1000);
  delay(300);
  noTone(BUZZER);

}

void loop() {
  // readSuccess = getId();

  if(!rc.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!rc.PICC_ReadCardSerial()){
    return 0;
  }
  

  byte scannedCard[4]; 

  for(int i = 0; i < 4; i++){
    scannedCard[i] = rc.uid.uidByte[i]; // Storing the UID of the scanned tag in scannedCard.
    Serial.print(rc.uid.uidByte[i], HEX);
    Serial.print('\n');
  }

  int successCard = -1;
  for (int i = 0; i < 2; i++) {
    int failed = false;
    for (int j = 0; j < 4; j++) {
      if (scannedCard[j] != cards[i][j]) {
        failed = true;
        break;
      }
    }
    if (!failed) {
      successCard = i;
      break;
    }
  }

  if (successCard == -1) {
    digitalWrite(RED_LED, HIGH);
    
    lcd.setCursor(0, 3);
    lcd.print("Access denied!  ");
    tone(BUZZER, 500);
    delay(400);
    noTone(BUZZER);
    delay(2600);
    digitalWrite(RED_LED, LOW);
    lcd.setCursor(0, 3);
    lcd.print("                ");

  } else {
    String authorizedUser = users[successCard];
    digitalWrite(GREEN_LED, HIGH);
    lcd.setCursor(0, 3);
    lcd.print("Welcome, ");
    lcd.print(authorizedUser);
    lcd.print("!");
    tone(BUZZER, 1000);
    delay(200);
    noTone(BUZZER);
    delay(2500);
    digitalWrite(GREEN_LED, LOW);
    lcd.setCursor(0, 3);
    lcd.print("                ");
  }
}
