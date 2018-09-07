/*
 * EX3_MultiTasking_FreeRTOS_DHT22
 * การแบ่ง task ทำงาน โดยควบคุมหลอดไฟ LED กระพริบคนละจังหวะ
 * และอ่านค่าอุณหภูมิและความชื้นจากเซนเซอร์ DHT22 มาแสดงผลในคอนโซล
 */

///-------------------ตั้งค่าที่นี่------------------///
 
#define LED1 32
#define LED2 33
#define LED3 25 //build-in LED on board
#define DHTTYPE dht22
#define DHTPIN 21

///------------------------------------------///

#include <SimpleDHT.h>          //โหลดไลบารี่ dht22
SimpleDHT22 DHTTYPE;              //ระบุรุ่นเซ็นเซอร์รุ่น dht22
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
void Task4(void *p){
  while(1) {
  dht22.read(DHTPIN, &temperature, &humidity, NULL);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.print(temperature);                      //แสดงค่าอุณหภูมิที่ได้ลงบนคอนโซล
  Serial.print("C : "); 
  Serial.print(humidity);                         //แสดงค่าความชื้นสัมพัทธ์ที่ได้ลงบนคอนโซล
  Serial.println("%"); }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate(&Task1, "Task1", 1024, NULL, 10, NULL);
  xTaskCreate(&Task2, "Task2", 1024, NULL, 10, NULL);
  xTaskCreate(&Task3, "Task3", 1024, NULL, 10, NULL);
  xTaskCreatePinnedToCore(&Task4,"Task4",10000,NULL,10,NULL,1);                 
}

void loop() {
}
