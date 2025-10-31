#include <HTTPClient.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <WiFi.h>


// Instantiate LCD Display with its respective GPIO pins.
LiquidCrystal lcd(14, 27, 26, 25, 33, 32);

// Define Green LED, Red LED, and Buzzer GPIO pins.
#define GREEN_LED 21
#define RED_LED 17
#define BUZZER 13

// Define RFID Module SPI pins.
#define SS_PIN 18   // SDA / SS
#define RST_PIN 16  // RST

// Instantiate RFID module.
MFRC522 rfid(SS_PIN, RST_PIN);

// Instantiate HTTP Client for performing GET requests to the
// authentication server.
HTTPClient http;

void setup() {

  // Establish serial communication for logging.
  Serial.begin(115200);

  // Establish SPI communication with the RFID module.
  SPI.begin(5, 19, 23, 18); 

  Serial.println("\n--- TapGuard ---");
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial(); // For debugging / sanity check purposes.
  Serial.println("[*] Initialized RFID Module.");

  // Set pin modes for the LEDs and buzzer as output.
  pinMode(GREEN_LED, OUTPUT);
  Serial.println("[*] Initialized Green LED.");
  pinMode(RED_LED, OUTPUT);
  Serial.println("[*] Initialized Red LED.");
  pinMode(BUZZER, OUTPUT);
  Serial.println("[*] Initialized Buzzer.");

  // Initialize LCD Display.
  lcd.begin(20, 4);
  Serial.println("[*] Initialized LCD.");

  // Wi-Fi credentials to establish Wi-Fi connection.
  const char* ssid = "pertzis";
  const char* password = "12345678";
  WiFi.begin(ssid, password);
  Serial.print("[*] Initializing WiFi");

  // Use LCD for displaying current Wi-Fi status.
  lcd.setCursor(0, 0);
  lcd.print("[-] Init. WiFi...");

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 20) { // 10 seconds timeout.
      Serial.println("\nFailed to connect to WiFi!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[!] Init. WiFi");
      return; // Halt ESP operation if WiFi cannot be established.
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

  playInitializedTone();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please scan your"); // Print a message to the LCD
  lcd.setCursor(0, 1);
  lcd.print("card.");

}

void loop() {

  // Do nothing until a card is present on the RFID reader.
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Display card UID on serial.
  Serial.print("[*] New card Scanned with UID ");

  String cardUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    cardUID += String(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Call REST API with GET request to check authorization status,
  // with the Card UID as a parameter.
  String url = "http://172.20.10.3:3001/api/validate/" + cardUID;

  Serial.println("[*] (>) GET " + url);

  String response = ""; // Initialize variable to store the response body.
  http.begin(url);
  int httpCode = http.GET();

  lcd.setCursor(0, 3);
  lcd.print("Authorizing...");

  if (httpCode > 0) {
    response = http.getString();
  } else {
    Serial.println("[!] Internal error.");
    displayInternalError();
    return;
  }

  http.end();

  Serial.println("[*] (<) " + response); // Output REST API response to serial

  // Fields are comma (`,`) delimited for simple, lightweight responses.
  int firstCommaIndex = response.indexOf(',');
  int secondCommaIndex = response.indexOf(',', firstCommaIndex + 1);
  if (firstCommaIndex == -1 || secondCommaIndex == -1) {
    Serial.println("[!] Invalid response from server.");
    displayInternalError();
    return;
  }

  // Store the status, name of user, and message from the server
  // into three separate variables.
  String status = response.substring(0, firstCommaIndex);
  String name = response.substring(firstCommaIndex + 1, secondCommaIndex);
  String message = response.substring(secondCommaIndex + 1);

  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);

  // If the status returned from the server is `0`, the user is unauthorized, or some
  // other server-side issue has occured. To play it safe, the user is unauthorized.
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

void displayInternalError() {
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("Internal error.");
  digitalWrite(RED_LED, HIGH);
  playUnauthorizedTone();
  delay(2600);
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  digitalWrite(RED_LED, LOW);
}

void playInitializedTone() {
  tone(BUZZER, 1000);
  delay(50);
  noTone(BUZZER);
  delay(50);
  tone(BUZZER, 1000);
  delay(50);
  noTone(BUZZER);
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
