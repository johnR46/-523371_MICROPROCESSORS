/*
 * EX12_FlashMemory_SPIFFS_Line
 * เชื่อมต่อ WiFi และดึงค่าเวลาจาก NTP server
 * อ่านค่าอุณหภูมิและความชื้นจากเซนเซอร์ DHT22 มาแสดงผลในคอนโซล
 * บันทึกข้อมูลไว้ใน SPIFFS พร้อมเวลาที่ดึงมาจาก NTP
 * รายงานค่าอุณหภูมิและความชื้นเข้า Line Notify
 * แสดงขั้นตอนการทำงานบนจอ OLED เป็นระยะ เก็บจำนวนการบูตไว้ใน RTC
 * เข้าสู่ deep sleep และตั้งเวลาตื่นอัตโนมัติหรือโดยกดปุ่มสวิตซ์
 */

#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include "esp_deep_sleep.h"
#include <Wire.h>
#include "SSD1306.h"
SSD1306  display(0x3c, 4, 15);

#define DHTTYPE dht22
#define DHTPIN 21

//------------------------------------------///

#define TokenLine "yff3kysyIvgkgbNbSeHapzHaCW9jK8lzV5hxvAK1k9s" //รหัสเชื่อมต่อ Line
const char*  server = "notify-api.line.me";   //Line Server URL

#include <SimpleDHT.h>          //โหลดไลบารี่ dht22
SimpleDHT22 DHTTYPE;            //ระบุรุ่นเซ็นเซอร์รุ่น DHT22
byte temperature = 0;           //กำหนดตัวแปรเก็บค่าอุณหภูมิ
byte humidity = 0;              //กำหนดตัวแปรเก็บค่าความชื้นสัมสัทธ์

const char* ssid     = "plutter";
const char* password = "visit012";

int timezone = 7 * 3600;        //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;

///------------------------------------------///
RTC_DATA_ATTR int bootCount = 0; //เก็บจำนวนการบูตแต่ละครั้ง
WiFiClientSecure client;

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

String gettime() {

  //ดึงเวลาจาก NTP Server
  configTime(timezone, dst, "time.navy.mi.th", "clock.nectec.or.th");     //ดึงเวลาจาก Server
    Serial.println("\nLoading time from NTP server"); 
    
    while (!time(nullptr)) {
      Serial.print("*");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

//แสดงเวลาปัจจุบัน
  time_t now = time(nullptr);
  struct tm* p = localtime(&now);
  int hh = p->tm_hour;
  int mm = p->tm_min;
  int ss = p->tm_sec;
  int wday = p->tm_wday;
  int iday = p->tm_mday;
  int imon = p->tm_mon;
  int iyear = p->tm_year; /* ปีคริสศักราช ตั้งแต่ 1900 */
  int nyear = iyear + 1900;
  
  String mydate = String(iday) + "/" + String(imon) + "/" + String(nyear);
  String mytime = String(hh) + ":" + String(mm) + ":" + String(ss);

  //check valid date
  if (mydate != "1/0/1970") {
    String txt = mydate + " " + mytime;
    Serial.println(txt);
    return txt;
  } else {
    return "false";
  }
}

String gettemp() {
  //Get DHT22  
  dht22.read(DHTPIN, &temperature, &humidity, NULL);
  while (temperature == 0) {
    dht22.read(DHTPIN, &temperature, &humidity, NULL);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    Serial.println("DHT22 read failed!");
  } //while
  String txt = String(temperature) + "," + String(humidity);
  Serial.println(txt);
  return txt;
}

void linenotify(String txt) {

  Serial.println("\nStarting connection to line server...");
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
  }//else
}

//แสดงผลบนจอ OLED
void showdisplay(String txt){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 12, txt);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, "Boot:" + String(bootCount));
    display.display();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void setup(){
    Serial.begin(115200);
    pinMode(25, OUTPUT); //Builin LED
    digitalWrite(25, HIGH); //Builin LED

    pinMode(16,OUTPUT);
    digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
    display.init(); // Initialising the UI will init the display too.
    
    esp_deep_sleep_enable_timer_wakeup(1 * 60 * 1000 * 1000); // กำหนดให้ตื่นเมื่อครบ 1 นาทีข้างหน้า
  //esp_deep_sleep_enable_timer_wakeup(15 * 1000000); // กำหนดให้ตื่นเมื่อครบ 15 วินาทีข้างหน้า
    esp_deep_sleep_enable_ext0_wakeup((gpio_num_t)2, HIGH); // กำหนดขาที่ต่อกับเซ็นเซอร์ และกำหนด Active เป็น HIGH


    showdisplay("WAKE UP!"); // แสดงผลบนจอ OLED  
    Serial.println();
    Serial.println();
    Serial.print("Connecting to WiFi SSID: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
  
   while (WiFi.status() != WL_CONNECTED) {
      vTaskDelay(500 / portTICK_PERIOD_MS);
   Serial.print(".");
    }

    showdisplay("CONNECT"); // แสดงผลบนจอ OLED
    Serial.println("");
    Serial.println("WiFi Connected");  
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    long rssi = WiFi.RSSI();
    Serial.print("RSSI:");
    Serial.println(rssi);
    Serial.println("----------------------------------");

  //File system
    if(!SPIFFS.begin()){
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    /*
     * ตัวอย่างการเข้าถึงไฟล์ 
    listDir(SPIFFS, "/", 0);
    writeFile(SPIFFS, "/hello.txt", "Hello ");
    appendFile(SPIFFS, "/hello.txt", "World!\n");
    readFile(SPIFFS, "/hello.txt");
    deleteFile(SPIFFS, "/foo.txt");
    renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    readFile(SPIFFS, "/foo.txt");
    testFileIO(SPIFFS, "/test.txt");
    *
    */

  //get DHT22
  String mytemp = gettemp(); 

  //get NTP
  String ntptime = gettime(); 
  while (ntptime == "false"){
    ntptime = gettime(); 
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    Serial.println("Get NTP failed");
  }

  String txt = ntptime + " " + mytemp + "\n";

  showdisplay("SEND LINE"); // แสดงผลบนจอ OLED
  linenotify(txt); // ส่งข้อมูลเข้า linenotify
  
  char mylog[30];
  txt.toCharArray(mylog, 30);

    //check file avaiable
     File file = SPIFFS.open("/temperature.csv", "r");
      if (!file) {
      writeFile(SPIFFS, "/temperature.csv", "");
      }

    appendFile(SPIFFS, "/temperature.csv", mylog);
    showdisplay("UPDATE.."); // แสดงผลบนจอ OLED
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    listDir(SPIFFS, "/", 0);
    readFile(SPIFFS, "/temperature.csv");

    bootCount++;  //เพิ่มจำนวนการบูตแต่ละครั้ง
    Serial.println("Boot number: " + String(bootCount));
    Serial.println("deep sleep...");
    digitalWrite(25, LOW);
    showdisplay("COMPLETE"); // แสดงผลบนจอ OLED
    showdisplay("SLEEP"); // แสดงผลบนจอ OLED
    
    esp_deep_sleep_start(); // เริ่มเข้าสู่โหมด Deep Sleep

}

void loop(){

}
