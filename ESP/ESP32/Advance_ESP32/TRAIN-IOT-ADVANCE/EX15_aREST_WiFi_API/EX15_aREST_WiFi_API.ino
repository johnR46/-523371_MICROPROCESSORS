/*
 * EX15_aREST WIFI API
 * ควบคุมบอร์ด ESP32 โดยตรงผ่าน IP ด้วย aREST API
 */

// Import required libraries
#include "WiFi.h"
#include <aREST.h>

#include <Wire.h>
#include "SSD1306.h"
SSD1306  display(0x3c, 4, 15);

///------------------------------------------///

#define DHTTYPE dht22
#define DHTPIN 21

#include <SimpleDHT.h>           //โหลดไลบารี่ DHT22
SimpleDHT22 DHTTYPE;             //ระบุรุ่นเซ็นเซอร์รุ่น DHT22
float temperature = 0;           //กำหนดตัวแปรเก็บค่าอุณหภูมิ
float humidity = 0;              //กำหนดตัวแปรเก็บค่าความชื้นสัมสัทธ์

// WiFi parameters
const char* ssid = "plutter";
const char* password = "visit012";

// Create an instance of the server
WiFiServer server(80);

///------------------------------------------///

// Create aREST instance
aREST rest = aREST();

// Declare functions to be exposed to the API
int ledRedControl(String command);
int ledGreenControl(String command);
int ledBlueControl(String command);

//แสดงผลบนจอ OLED
void showdisplay(String txt, String txt2){
    display.clear();
    //display.flipScreenVertically(); //กลับหัวจอ
    //display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 12, txt);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, txt2);
    display.display();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

unsigned long last1;

void setup()
{
  
  // Start Serial
  Serial.begin(115200);             

  pinMode(16,OUTPUT);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  display.init(); // Initialising the UI will init the display too.
  display.clear();
  display.flipScreenVertically(); //กลับหัวจอ
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  pinMode(25,OUTPUT); //LED builin
  pinMode(32,OUTPUT); //Relay1
  pinMode(33,OUTPUT); //Relay2

  // Function to be exposed
  rest.function("red",ledRedControl);
  rest.function("blue",ledBlueControl);
  rest.function("green",ledGreenControl);


  // Init variables and expose them to REST API
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("1");
  rest.set_name("rsusmt_esp32");

  // Connect to WiFi
  Serial.println("WiFi connecting..");
  showdisplay("CONNECT","aREST Framework"); // แสดงผลบนจอ OLED

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  showdisplay("CONNECTED","aREST Framework"); // แสดงผลบนจอ OLED

  // Start the server
  server.begin();
  Serial.println("Server started");

  //Print the IP address
  String myip = WiFi.localIP().toString();
  Serial.println(WiFi.localIP());
  showdisplay("READY", myip); // แสดงผลบนจอ OLED
}

void loop() {
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  while(!client.available()){
    delay(1);
  }

    // update variables to REST API
    dht22.read2(DHTPIN, &temperature, &humidity, NULL);

    Serial.print(temperature);                      //แสดงค่าอุณหภูมิที่ได้ลงบนคอนโซล
    Serial.print("*C : "); 
    Serial.print(humidity);                      //แสดงค่าความชื้นสัมพัทธ์ที่ได้ลงบนคอนโซล
    Serial.println("RH%");
    
    rest.handle(client);
}

// Custom function accessible by the API
int ledRedControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(25, 1);
  digitalWrite(32, 1);
  digitalWrite(33, 1);
  for (int i=1; i <= state; i++){
    digitalWrite(25, 0);
    delay(500);
    digitalWrite(25, 1);
    delay(500);
  }
  
  return 1;
}

// Relay1 - Custom function accessible by the API
int ledBlueControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(25, 1);
  digitalWrite(32, 1);
  digitalWrite(33, 1);
  for (int i=1; i <= state; i++){
    digitalWrite(33, 0);
    delay(500);
    digitalWrite(33, 1);
    delay(500);
  }
  
  return 1;
}

// Relay2 - Custom function accessible by the API
int ledGreenControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(25, 1);
  digitalWrite(32, 1);
  digitalWrite(33, 1);
  for (int i=1; i <= state; i++){
    digitalWrite(32, 0);
    delay(500);
    digitalWrite(32, 1);
    delay(500);
  }
  
  return 1;
}

