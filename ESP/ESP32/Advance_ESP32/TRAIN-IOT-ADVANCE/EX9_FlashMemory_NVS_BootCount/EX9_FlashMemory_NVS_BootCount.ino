/*
 * EX9_FlashMemory_NVS_BootCount
 * นับจำนวน ESP รีบูตแล้วเขียนเก็บไว้ใน NVS
 * ข้อมูลที่เก็บจะเป็นแบบถาวร
 */
 
 #include <Preferences.h>

Preferences preferences;

void setup() {
  
  Serial.begin(115200);
  Serial.println();

  //ตั้งชื่อ proferences ไม่เกิน 15 ตัวอักษร
  preferences.begin("my-app", false);

  //ตั้งค่า defaul ถ้าครั้งแรกยังไม่มีค่าอะไร
  unsigned int counter = preferences.getUInt("counter", 0);
  String mess_1 = preferences.getString("mess_1", "none");

  // แสดงผลใน console
  Serial.printf("Current counter value: %u\n", counter);
  Serial.println("Current text : " + mess_1);

  /* ถ้าต้องการลบ preferences ทั้งหมด */
  //preferences.clear();
  /*------------------------*/

  // อ่านค่าจาก NVS
  counter++;
  preferences.putUInt("counter", counter);
  
  mess_1 = "TextNumber-" + String(counter);
  preferences.putString("mess_1", mess_1);
  
  // ปิด Preferences
  preferences.end();

  // รอ 10 seconds
  Serial.println("Restarting in 10 seconds...");
}

void loop() {

  //เทสแล้วให้ปิด function นี้ด้วย
 ESP.restart();
 delay(10000);
  
  }
