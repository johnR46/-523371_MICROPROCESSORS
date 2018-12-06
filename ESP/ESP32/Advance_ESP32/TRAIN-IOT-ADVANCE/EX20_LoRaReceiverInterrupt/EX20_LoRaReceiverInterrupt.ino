/*
 * EX20_LoRaReceiverImterrupt
 * ทดสอบรับข้อมูลผ่านเครือข่าย LoRa พร้อมแสดงค่า RSSI
 * บนคลื่นความถี่ 433MHz
 */
 
#include <LoRa.h>
#include <U8x8lib.h>

String receivedText;
String receivedRssi;

// Pin definetion of WIFI LoRa 32
#define SCK     5    // GPIO5  -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    27   // GPIO27 -- SX127x's MOSI
#define SS      18   // GPIO18 -- SX127x's CS
#define RST     14   // GPIO14 -- SX127x's RESET
#define DI0     26   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    433.125E6  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST true

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15,4,16);

void setup() {

  pinMode(25,OUTPUT); //Send success, LED will bright 1 second
  
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  Serial.begin(115200);
  delay(1000);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  Serial.println("LoRa Receiver Callback");
  u8x8.drawString(0, 1, "LoRa Callback");
  
  if (!LoRa.begin(BAND,PABOOST)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
    while (1);
  }

  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  // do nothing
}

void onReceive(int packetSize)
{
  // received a packet
  Serial.print("Received packet '");
  u8x8.drawString(0, 4, "TXT");

  String incoming = "";
  // read packet
  for (int i = 0; i < packetSize; i++)
  {
      incoming += (char)LoRa.read();
  }

     receivedText = incoming;
     char currentid[64];
     receivedText.toCharArray(currentid, 64);
     u8x8.drawString(6, 4, currentid);
     //print Received Text
     Serial.print(receivedText);

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    u8x8.drawString(0, 5, "RSSI");
    receivedRssi = LoRa.packetRssi();
    char currentrs[64];
    receivedRssi.toCharArray(currentrs, 64);
    u8x8.drawString(6, 5, currentrs);

    digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
}

