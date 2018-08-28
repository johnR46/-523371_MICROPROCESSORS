
void setup() {

  analogReference(A0);



  Serial.begin(9600);

}



void loop() {

  float volt = analogRead(A0) * 1.1 / 1023;

  volt *= 6;

  Serial.println("VCC Volt: " + String(volt));

  delay(500);

}
