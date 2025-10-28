void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Test");

}

void loop() {

   if (Serial2.available()) {
    Serial.print("From Arduino: ");
    Serial.println(Serial2.readStringUntil('\n'));
  }
  // Serial.print("hjksdfhjkfsdhjksdfahjkshjdkfshjkdfa");
  
  // put your main code here, to run repeatedly:

}
