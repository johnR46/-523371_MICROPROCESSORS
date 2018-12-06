/*
 * EX2_MultiTasking_Milli_LED
 * ควบคุมหลอดไฟ LED กระพริบคนละจังหวะ
 */

///-------------------ตั้งค่าที่นี่------------------///

#define LED1 32
#define LED2 33
#define LED3 25  //ไฟ LED บนบอร์ด

///------------------------------------------///

unsigned long last1, last2, last3;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop() {
  
  if (millis() - last1 >= 700) {
    last1 = millis();
    digitalWrite(LED1, !digitalRead(LED1));
  }
  
  if (millis() - last2 >= 300) {
    last2 = millis();
    digitalWrite(LED2, !digitalRead(LED2));
  }
  
  if (millis() - last3 >= 100) {
    last3 = millis();
    digitalWrite(LED3, !digitalRead(LED3));
  }
  
}
