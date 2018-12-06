/////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * EX1_GPS_Blynk
 * Sensor station with GPS
 * [ รองรับบอร์ด ESP32 ]
 * 
 * แสดงการเชื่อมต่อ sensor dht22 วัดอุณหภูมิและความชื้น
 * sensor วัดค่าอนาลอก A6, A7 และวัดค่าพิกัด GPS
 * จากนั้นส่งค่าไปยัง Blynk server
 * 
 * กำหนดขาต่ออุปกรณ์
 * 1. sensor dht22 วัดอุณหภูมิและความชื้น ต่อขา 21
 * 2. sensor Analog ต่อขา A6, A7 //ขา GPIO34, 35
 * 3. GPS ต่อขา RXPin = 38, TXPin = 39 , VCC เข้า 5V และ GND เข้า GND
 * 
 * สร้างเมื่อ 28 Aug 2017 (vesion 1.0)
 * โดย วิสิทธิ์ เวียงนาค , RSU MISSMT
 * อีเมล์ visit01@gmail.com
*/
/////////////////////////////////////////////////////////////////////////////////////////////////

#define BLYNK_PRINT Serial              
#include <WiFi.h>                      
#include <BlynkSimpleEsp32.h>          
#include <SimpleDHT.h>                
#include <TinyGPS++.h>                
#include <Wire.h>
#include "SSD1306.h"
SSD1306  display(0x3c, 4, 15);

//------------------------------------------///

#define DHTTYPE dht22
#define DHTPIN 21

/*คอนฟิก token เข้า Blynk server เปลี่ยนค่าที่นี่*/
char auth[] = "5848e29f6cd14f41b9b5a9c556aa4479";
char ssid[] = "plutter";
char pass[] = "visit012";

const double Home_LAT = 14.005691;                      // กำหนดตำแหน่งต้นทางที่จะวัดระยะขจัด Latitude
const double Home_LNG = 100.562408;                     // กำหนดตำแหน่งต้นทางที่จะวัดระยะขจัด Longitude

static const int RXPin = 38, TXPin = 39;                // กำหนดขา Ublox
static const uint32_t GPSBaud = 9600;                   // กำหนด Ublox GPS Rate 9600

//------------------------------------------///

TinyGPSPlus gps;                                        // สร้างออปเจค GPS
HardwareSerial Serial1(1);   

SimpleDHT22 DHTTYPE;            //ระบุรุ่นเซ็นเซอร์รุ่น dht22
byte temperature = 0;           //กำหนดตัวแปรเก็บค่าอุณหภูมิ
byte humidity = 0;              //กำหนดตัวแปรเก็บค่าความชื้นสัมสัทธ์

BlynkTimer timer;               // เรียกใช้การตั้งเวลาส่ง
WidgetMap myMap(V5);            // สร้างขา V5 เพื่อส่ง lat,lng ให้ Blynk
WidgetLCD lcd(V6);              // สร้างขา V6 เพื่อส่ง lat,lng ให้ Blynk LCD

void sendSensor() {
  
  dht22.read(DHTPIN, &temperature, &humidity, NULL);  //อ่านค่าจากเซ็นเซอร์

  /* if need make sure DHT22 can get data */
  while (temperature == 0) {
    dht22.read(DHTPIN, &temperature, &humidity, NULL);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    Serial.println("DHT22 read failed!");
  } //while
  
  Blynk.virtualWrite(V1, humidity);                   // V1 คือ ค่าความชื้น
  Blynk.virtualWrite(V2, temperature);                // V2 คือ ค่าอุณหภูมิ

  int analog1 = analogRead(A6);                      // PIN34 / อ่านค่าอนาลอก จะเป็นอะไรก็ได้เข่น sensor แสง,ความชื้นดิน,เสียง
  Blynk.virtualWrite(V3, analog1);                   // V3 คือ ค่าอนาลอกจาก A6

  int analog2 = analogRead(A7);                      // PIN35 / อ่านค่าอนาลอก จะเป็นอะไรก็ได้เข่น sensor แสง,ความชื้นดิน,เสียง
  Blynk.virtualWrite(V4, analog2);                   // V4 คือ ค่าอนาลอกจาก A7

  //Show on blynk LCD
  lcd.clear(); //clear the LCD Widget
  lcd.print(0, 0, gps.location.lat());
  lcd.print(0, 1, gps.location.lng());
    
 //แสดงผลบน console
  Serial.print("Latitude  : ");
  Serial.println(gps.location.lat(), 6);  //ค่า lat แสดงทศนิยม 6 ตำแหน่ง
  Serial.print("Longitude : ");
  Serial.println(gps.location.lng(), 6);  //ค่า lng แสดงทศนิยม 6 ตำแหน่ง
  Serial.print("Satellites: ");
  Serial.println(gps.satellites.value());
  Serial.print("Elevation : ");
  Serial.print(gps.altitude.meters());
  Serial.println("m"); 
  Serial.print("Time UTC  : ");
  Serial.print(gps.time.hour());                       // GPS time UTC 
  Serial.print(":");
  Serial.print(gps.time.minute());                     // Minutes
  Serial.print(":");
  Serial.println(gps.time.second());                   // Seconds
  Serial.print("Heading   : ");
  Serial.println(gps.course.deg());
  Serial.print("Speed     : ");
  Serial.println(gps.speed.mph());

  Serial.print("Temperature =  ");
  Serial.println(temperature);

  Serial.print("Humidity =  ");
  Serial.println(humidity);

  Serial.print("Analog Sensor1 =  ");
  Serial.println(analog1);

  Serial.print("Analog Sensor2 =  ");
  Serial.println(analog2);

  Serial.print("Hall sensor: ");  //เซนเซอร์คลื่นแม่เหล็ก buil-in ในบอร์ด
  Serial.println(hallRead()); 
  
  unsigned long Distance_To_Home = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),Home_LAT, Home_LNG);
  Serial.print("M to Home: ");                        // Have TinyGPS Calculate distance to home and display it
  Serial.print(Distance_To_Home);
  Serial.println("\n\n");

  int pointIndex = 1;
  myMap.location(pointIndex, gps.location.lat(), gps.location.lng(), "RSU-MISSMT Location");
    
  smartDelay(1000);                                      // Run Procedure smartDelay

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

  showdisplayGPS();
}

static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

//แสดงผลบนจอ OLED GPS
void showdisplayGPS(){
    display.clear();
    display.flipScreenVertically(); //กลับหัวจอ
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 12, "Lat: " + String(gps.location.lat(), 6));
    display.drawString(0, 40, "Lng:" + String(gps.location.lng(), 6));
    display.display();
}

//แสดงผลบนจอ OLED
void showdisplay(String txt){
    display.clear();
    display.flipScreenVertically(); //กลับหัวจอ
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 12, txt);
    display.drawString(0, 40, "Wait...");
    display.display();
}

void setup() {

  Serial.begin(115200);

  /* If need 10bit analog */
  //analogReadResolution(10);

  pinMode(16,OUTPUT);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  display.init();         // Initialising the UI will init the display too.
  
  Serial1.begin(9600, SERIAL_8N1, RXPin, TXPin);
  Serial.println("GPS Blynk");  
  Serial.println(TinyGPSPlus::libraryVersion());
  
  showdisplay("Blynk Connect");
  Blynk.begin(auth, ssid, pass, IPAddress(128,199,204,127), 8442);
  timer.setInterval(5000L, sendSensor);   // ส่งค่าทุก 5 วินาที
  
}

void loop() {
  
  Blynk.run();
  timer.run();
  
}

