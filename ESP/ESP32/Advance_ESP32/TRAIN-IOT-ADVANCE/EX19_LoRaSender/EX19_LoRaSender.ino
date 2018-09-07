/*
 * EX19_LoRaSender
 * ทดสอบส่งข้อมูลผ่านเครือข่าย LoRa พร้อมแสดงค่า RSSI
 * บนคลื่นความถี่ 433MHz
 */
 
#include <U8x8lib.h>
#include <LoRa.h>

String receivedText;
String receivedRssi;

// WIFI_LoRa_32 ports

// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    923.2E6
#define PABOOST true

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15,4,16);

int counter = 0;

void setup() {
  pinMode(25,OUTPUT); //Send success, LED will bright 1 second

  Serial.begin(115200);
  delay(1000);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  Serial.println("LoRa Sender");
  u8x8.drawString(0, 1, "LoRa Sender");
  
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(5);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setPreambleLength(8);

  if (!LoRa.begin(BAND,PABOOST)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
}

void loop() {
  Serial.print("Sending packet: ");
   u8x8.drawString(0, 4, "PacketID");
  Serial.println(counter);
  String txt = String(counter);
  
  char currentid[5];
  txt.toCharArray(currentid, 64);
  u8x8.drawString(9, 4, currentid);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
   u8x8.drawString(0, 5, "Payload");
   u8x8.drawString(9, 5, "hello");

  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  
  delay(3000);
}
