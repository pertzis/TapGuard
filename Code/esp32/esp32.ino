#include <HTTPClient.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

HTTPClient http;

void setup() {
  Serial.begin(9600);

  delay(5000);


  const char* ssid = "pertzis";
  const char* password = "12345678";
  WiFi.begin(ssid, password);

  Serial.println("\n\n\n--- TapGuard ---");

  Serial.print("Connecting to WiFi");

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 20) { // 10 seconds timeout
      Serial.println("\nFailed to connect to WiFi!");
      return;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nSuccessfully connected to WiFi! [");
    Serial.print(WiFi.localIP());
    Serial.print("]\n");
  }

  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("Waiting for serial with Arduino...");
  while (!Serial2) {
    delay(10);
  }
  Serial.println("Serial established.");


}

void loop() {


   if (Serial2.available()) {
    String cardUID;
    cardUID = Serial2.readStringUntil('\n');
    Serial.println(cardUID);

    String url = "http://172.20.10.3:3001/api/validate/" + cardUID;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
        String payload = http.getString(); // Get the response body
        Serial.println(payload);
        delay(500);
      } else {
        Serial.printf("Error code: %d\n", httpCode);
      }
    http.end();
  }

  
  // Serial.print("hjksdfhjkfsdhjksdfahjkshjdkfshjkdfa");
  
  // put your main code here, to run repeatedly:

}
