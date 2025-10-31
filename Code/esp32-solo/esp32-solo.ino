#include <HTTPClient.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <WiFi.h>

LiquidCrystal lcd(14, 27, 26, 25, 33, 32);

#define GREEN_LED 21
#define RED_LED 17
#define BUZZER 13

// RC522 pins
#define SS_PIN 18   // SDA / SS
#define RST_PIN 16  // RST

MFRC522 rfid(SS_PIN, RST_PIN);
HTTPClient http;


void setup() {

  Serial.begin(115200);
  SPI.begin(5, 19, 23, 18);  // SCK, MISO, MOSI, SS

  Serial.println("\n--- TapGuard ---");
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();
  Serial.println("[*] Initialized RFID Module.");

  pinMode(GREEN_LED, OUTPUT);
  Serial.println("[*] Initialized Green LED.");
  pinMode(RED_LED, OUTPUT);
  Serial.println("[*] Initialized Red LED.");
  pinMode(BUZZER, OUTPUT);
  Serial.println("[*] Initialized Buzzer.");

  lcd.begin(20, 4);
  Serial.println("[*] Initialized LCD.");

  const char* ssid = "pertzis";
  const char* password = "12345678";
  WiFi.begin(ssid, password);
  Serial.print("[*] Initializing WiFi");

  lcd.setCursor(0, 0);
  lcd.print("[-] Init. WiFi...");

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 20) { // 10 seconds timeout
      Serial.println("\nFailed to connect to WiFi!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[!] Init. WiFi");
      return;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nSuccessfully connected to WiFi. [");
    Serial.print(WiFi.localIP());
    Serial.print("]\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("[*] Init. WiFi");
    delay(2000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please scan your"); // Print a message to the LCD
  lcd.setCursor(0, 1);
  lcd.print("card.");

}

void loop() {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Display UID on Serial + LCD
  Serial.print("[*] New card Scanned with UID ");

  String cardUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    cardUID += String(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  String url = "http://172.20.10.3:3001/api/validate/" + cardUID;

  Serial.println("[*] (>) GET " + url);

  String response = ""; // Get the response body
  http.begin(url);
  int httpCode = http.GET();

  lcd.setCursor(0, 3);
  lcd.print("Authorizing...");

  if (httpCode > 0) {
    response = http.getString(); // Get the response body
  } else {
    Serial.printf("Error code: %d\n", httpCode);
  }
  http.end();

  Serial.println("[*] (<) " + response);

  int firstCommaIndex = response.indexOf(',');
  int secondCommaIndex = response.indexOf(',', firstCommaIndex + 1);
  if (firstCommaIndex == -1 || secondCommaIndex == -1) {
    Serial.println("[!] Invalid response from server.");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("Internal error.");
    digitalWrite(RED_LED, HIGH);
    playUnauthorizedTone();
    delay(2600);
    lcd.setCursor(0, 3);
    lcd.print("                ");
    digitalWrite(RED_LED, LOW);
    return;
  }

  String status = response.substring(0, firstCommaIndex);
  String name = response.substring(firstCommaIndex + 1, secondCommaIndex);
  String message = response.substring(secondCommaIndex + 1);

  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);

  if (status == "0") {
    lcd.print("Unauthorized.");
    digitalWrite(RED_LED, HIGH);
    playUnauthorizedTone();
    delay(2600);
    lcd.setCursor(0, 3);
    lcd.print("                ");
    digitalWrite(RED_LED, LOW);
  } else {
    lcd.setCursor(0, 2);
    lcd.print("Thank you, " + name + ".");
    lcd.setCursor(0, 3);
    lcd.print(message);
    digitalWrite(GREEN_LED, HIGH);
    playAuthorizedTone();
    delay(2600);
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    digitalWrite(GREEN_LED, LOW);
  }

}

void playUnauthorizedTone() {
  tone(BUZZER, 1000);
  delay(200);
  tone(BUZZER, 500);
  delay(200);
  tone(BUZZER, 1000);
  delay(400);
  noTone(BUZZER);
}

void playAuthorizedTone() {
  tone(BUZZER, 1000);
  delay(200);
  noTone(BUZZER);
}
