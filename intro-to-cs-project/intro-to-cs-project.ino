#include <LiquidCrystal.h>  // Include LCD Library
#include<SPI.h>             // Include SPI.h for SPI Communication with the Arduino
#include<MFRC522.h>         // Include RFID library

// LCD Pins
#define RS 7
#define ENABLE 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2

//Define RFID Reader SPI Pins
#define RST 9
#define SS 10
MFRC522 rc(SS, RST);

int readSuccess;


LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);        // Initialize the 16x2 LCD
  lcd.print("Welcome!"); // Print a message to the LCD
  SPI.begin();
rc.PCD_Init(); //initialize the receiver  
rc.PCD_DumpVersionToSerial(); //show details of card reader module
}

//function to get the UID of the card
int getId(){  
  if(!rc.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!rc.PICC_ReadCardSerial()){
    return 0;
  }
 
  
  Serial.println("THE UID OF THE SCANNED CARD IS:");
  
  for(int i=0;i<4;i++){
    //readcard[i]=rc.uid.uidByte[i]; //storing the UID of the tag in readcard
    // Serial.print(readcard[i],HEX);
    
  }
  rc.PICC_HaltA();
  return 1;
}

void loop() {
  readSuccess = getId();
  if(readSuccess){
    Serial.println("Read success!!!");
  }
}
