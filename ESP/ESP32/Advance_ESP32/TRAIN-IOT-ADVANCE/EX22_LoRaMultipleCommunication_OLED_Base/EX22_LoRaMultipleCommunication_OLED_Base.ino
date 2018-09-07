/*
 * EXT22_LoRaMultiCommunication_OLED_Base 
 * LoRa Multiple Communication
 * 
 * การรับและส่งข้อมูลผ่านเครือข่าย LoRa แบบระบุต้นทางและปลายทาง
 * สามารถส่งแบบ broadcast ไปยัง device ทุกตัวได้โดยระบุปลายทางเป็น 0xFF
 * อุปกรณ์ทุกตัวต้องใช้ความถี่เดียวกัน
*/

#include <U8x8lib.h>
#include <LoRa.h>

String myTxt;

// Pin definetion of WIFI LoRa 32
// HelTec AutoMation 2017 support@heltec.cn 
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define BAND    923.2E6  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST true

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15,4,16);

String outgoing;              // outgoing message
byte localAddress = 0xAA;     // address of this device
byte destination = 0xFF;      // destination to send to
byte msgCount = 0;            // count of outgoing messages
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void setup()
{
  pinMode(25,OUTPUT); //Send success, LED will bright 1 second
  
  Serial.begin(115200);                   // initialize serial
  delay(1000);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  Serial.println("LoRa Receiver Callback");
  u8x8.drawString(0, 1, "LoRa Receiver");
  delay(1000);

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);// set CS, reset, IRQ pin

  /*
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  */

  if (!LoRa.begin(BAND,PABOOST))
  {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  u8x8.clearLine(1);
  u8x8.drawString(0, 1, "init succeeded");
  delay(1000);
  u8x8.clearLine(1);
  u8x8.drawString(0, 1, "on receive..");
}

void loop()
{

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
  
}

void sendMessage()
{
  String outgoing = "LedOn";
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
  LoRa.receive();
}

void onReceive(int packetSize)
{
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length())
  {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

    // if message is for this device, or broadcast, print details:
    sendMessage(); //feedback message
  
    Serial.println("Received from: 0x" + String(sender, HEX));
    Serial.println("Sent to: 0x" + String(recipient, HEX));
    Serial.println("Message ID: " + String(incomingMsgId));
    Serial.println("Message length: " + String(incomingLength));
    Serial.println("Message: " + incoming);
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));
    Serial.println();
    
    //OLED display
    u8x8.drawString(0, 1, "BASE RSSI");
    u8x8.drawString(0, 2, "SNR");
    u8x8.drawString(0, 3, "PacketID");
    u8x8.drawString(0, 4, "Get from");
    u8x8.drawString(0, 5, "Send to");
    u8x8.drawString(0, 6, "Message");

    myTxt = LoRa.packetRssi();
    char rssi[4];
    myTxt.toCharArray(rssi, 4);
    u8x8.drawString(9, 1, rssi);

    myTxt = LoRa.packetSnr();
    char snr[7];
    myTxt.toCharArray(snr, 7);
    u8x8.drawString(9, 2, snr);
    
    myTxt = incomingMsgId;
    char msgid[9];
    myTxt.toCharArray(msgid, 9);
    u8x8.drawString(9, 3, msgid);

    myTxt = String(sender, HEX);
    char sendr[3];
    myTxt.toCharArray(sendr, 3);
    u8x8.drawString(9, 4, sendr);

    myTxt = String(recipient, HEX);
    char rec[3];
    myTxt.toCharArray(rec, 3);
    u8x8.drawString(9, 5, rec);
    
    myTxt = incoming;
    char inc[10];
    myTxt.toCharArray(inc, 10);
    u8x8.drawString(9, 6, inc);
   
    digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
    delay(1000); 
}

