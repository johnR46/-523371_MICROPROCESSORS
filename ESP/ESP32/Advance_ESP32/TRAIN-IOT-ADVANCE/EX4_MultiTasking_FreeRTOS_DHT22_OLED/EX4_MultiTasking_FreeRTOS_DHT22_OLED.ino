/*
 * EX4_MultiTasking_FreeRTOS_DHT22_OLED
 * การแบ่ง task ทำงาน โดยควบคุมหลอดไฟ LED กระพริบคนละจังหวะ
 * อ่านค่าอุณหภูมิและความชื้นจากเซนเซอร์ DHT22 มาแสดงผลในคอนโซล
 * และแสดงค่าในจอ OLED
 */

///-------------------ตั้งค่าที่นี่------------------///
 
#define LED1 32
#define LED2 33
#define LED3 25 //build-in LED on board
#define DHTTYPE dht22
#define DHTPIN 21

///------------------------------------------///

#include <Wire.h>
#include "SSD1306.h"
SSD1306  display(0x3c, 4, 15);

#include <SimpleDHT.h>          //โหลดไลบารี่ DHT22
SimpleDHT22 DHTTYPE;              //ระบุรุ่นเซ็นเซอร์รุ่น DHT2
byte temperature = 0;           //กำหนดตัวแปรเก็บค่าอุณหภูมิ
byte humidity = 0;              //กำหนดตัวแปรเก็บค่าความชื้นสัมสัทธ์

///------------------------------------------///
void Task1(void *p){
  pinMode(LED1, OUTPUT);
  while(1) {
    digitalWrite(LED1, !digitalRead(LED1));
    vTaskDelay(100 / portTICK_PERIOD_MS); }
}

///------------------------------------------///
void Task2(void *p){
  pinMode(LED2, OUTPUT);
  while(1) {
    digitalWrite(LED2, !digitalRead(LED2));
    vTaskDelay(500 / portTICK_PERIOD_MS); }
}

///------------------------------------------///
void Task3(void *p){
  pinMode(LED3, OUTPUT);
  while(1) {
    digitalWrite(LED3, !digitalRead(LED3));
    vTaskDelay(1000 / portTICK_PERIOD_MS); }
}

///------------------------------------------///
int count = 1;
void Task4(void *p){
  while(1) {
  dht22.read(DHTPIN, &temperature, &humidity, NULL);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.print(temperature);                      //แสดงค่าอุณหภูมิที่ได้ลงบนคอนโซล
  Serial.print("C : "); 
  Serial.print(humidity);                         //แสดงค่าความชื้นสัมพัทธ์ที่ได้ลงบนคอนโซล
  Serial.println("%");

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "MultiTasking Test");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 12, "Temp: " + String(temperature) + " C");
  display.drawString(0, 28, "Humi: " + String(humidity) + " %");
  display.drawString(0, 44, "Count: " + String(count));
  display.display(); 
  count++; }
}

void setup() {
  Serial.begin(115200);
  pinMode(16,OUTPUT);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  display.init(); // Initialising the UI will init the display too.
  
  xTaskCreate(&Task1, "Task1", 1024, NULL, 10, NULL);
  xTaskCreate(&Task2, "Task2", 1024, NULL, 10, NULL);
  xTaskCreate(&Task3, "Task3", 1024, NULL, 10, NULL);
  xTaskCreatePinnedToCore(&Task4,"Task4",10000,NULL,10,NULL,1);                 
}

void loop() {
}
