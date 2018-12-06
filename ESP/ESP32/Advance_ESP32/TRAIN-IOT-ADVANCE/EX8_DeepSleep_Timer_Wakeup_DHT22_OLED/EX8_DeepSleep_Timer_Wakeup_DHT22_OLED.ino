/*
 * EX8_DeepSleep_Timer_Wakeup_DHT22_OLED_Line
 * อ่านค่าอุณหภูมิและความชื้นจากเซนเซอร์ DHT22 มาแสดงผลในคอนโซล
 * แสดงค่าในจอ OLED แล้วส่งข้อมูลไปยัง line ก่อนเข้าสู่ deep sleep อีกรอบ
 */

///-------------------ตั้งค่าที่นี่------------------///

#define TokenLine "yff3kysyIvgkgbNbSeHapzHaCW9jK8lzV5hxvAK1k9s" //รหัสเชื่อมต่อ Line
const char* ssid  = "plutter";                // your network SSID
const char* password = "visit012";            // your network password
const char* server = "notify-api.line.me";   //Line Server URL

#define LED1 32
#define LED2 33
#define LED3 25 //build-in LED on board
#define DHTTYPE dht22
#define DHTPIN 21

///------------------------------------------///
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_deep_sleep.h"
#include <Wire.h>
#include "SSD1306.h"
SSD1306  display(0x3c, 4, 15);

#include <SimpleDHT.h>          //โหลดไลบารี่ dht22
SimpleDHT22 DHTTYPE;              //ระบุรุ่นเซ็นเซอร์รุ่น dht22
byte temperature = 0;           //กำหนดตัวแปรเก็บค่าอุณหภูมิ
byte humidity = 0;              //กำหนดตัวแปรเก็บค่าความชื้นสัมสัทธ์

RTC_DATA_ATTR int bootCount = 0; //เก็บจำนวนการบูตแต่ละครั้ง

WiFiClientSecure client;

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
void setup() {
  Serial.begin(115200);
  
  pinMode(16,OUTPUT);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  display.init(); // Initialising the UI will init the display too.
  
  xTaskCreate(&Task1, "Task1", 1024, NULL, 10, NULL);
  xTaskCreate(&Task2, "Task2", 1024, NULL, 10, NULL);
  xTaskCreate(&Task3, "Task3", 1024, NULL, 10, NULL);
  
  esp_deep_sleep_enable_timer_wakeup(1 * 60 * 1000 * 1000); // กำหนดให้ตื่นเมื่อครบ 1 นาทีข้างหน้า
  //esp_deep_sleep_enable_timer_wakeup(15 * 1000000); // กำหนดให้ตื่นเมื่อครบ 15 วินาทีข้างหน้า
  esp_deep_sleep_enable_ext0_wakeup((gpio_num_t)2, HIGH); // กำหนดขาที่ต่อกับเซ็นเซอร์ และกำหนด Active เป็น HIGH

  //Get DHT22  
  while (temperature == 0) {
    dht22.read(DHTPIN, &temperature, &humidity, NULL);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    Serial.println("DHT22 read failed!");
  } //while
  String txt = "Temperature: " + String(temperature) + "C , " + "Humidity: " + String(humidity) + "%"; 
  Serial.println(txt);

  bootCount++;  //เพิ่มจำนวนการบูตแต่ละครั้ง
  Serial.println("Boot number: " + String(bootCount));
  
  showdisplay(txt); // แสดงผลบนจอ OLED
  linenotify(txt); // ส่งข้อมูลเข้า linenotify
  
}

void showdisplay(String txt){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "MultiTasking Test");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 12, "Temp: " + String(temperature) + " C");
    display.drawString(0, 28, "Humi: " + String(humidity) + " %");
    display.drawString(0, 44, "Boot:" + String(bootCount));
    display.display();
}

void linenotify(String txt) {
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 443))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");
    
  // Make a HTTP request:
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(TokenLine) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + txt).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + txt;
  
  Serial.println(req);
  client.print(req);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break; }
    } //while
  
  client.stop();
  vTaskDelay(30000 / portTICK_PERIOD_MS);
  esp_deep_sleep_start(); // เริ่มเข้าสู่โหมด Deep Sleep
  
  }//else
}

void loop() {
  
}
