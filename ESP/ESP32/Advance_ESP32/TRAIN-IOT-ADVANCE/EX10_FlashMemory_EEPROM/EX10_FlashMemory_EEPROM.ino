/*
 * EX10_FlashMemory_EEPROM
 * ทดสอบเขียนข้อมูลลงบน SPI flash memory ของโมดูล ESP32
 * ใน EEPROM ที่มีขนาด 4KB เป็นการเก็บข้อมูลแบบถาวร
 * อ่านค่าจาก EEPROM แปลง char array เป็น String มาแสดงผลใน Console
 */
 
#include "EEPROM.h"

char msg[] = "RSUwifi,2.4GHz,CH6,20MHz,rsu01,12345678"; //ข้อมูลทีต้องการเขียนลงใน EEPROM
int msgSize = sizeof(msg);  //นับจำนวนตัวอักษร

void setup() {
  Serial.begin(115200);
  EEPROM.begin(msgSize);

  //เขียนข้อมูลลง EEPROM เริ่มที่ตำแหน่งที่ 0
  for (int i=0; i < msgSize; i++) {
  EEPROM.write(i, msg[i]); }
  EEPROM.commit();
}

void loop() {

  //อ่านค่าจาก EEPROM แปลง char array เป็น String
  String txt;
  for (int i = 0; i < msgSize; i++) {
         txt += (char)EEPROM.read(i);
  }//for
      
  Serial.println(txt);
  delay(3000);

}
