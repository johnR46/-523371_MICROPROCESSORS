/*
 * EX14_aREST_Serial
 * ทดสอบควบคุมบอร์ดผ่านทาง Serial
 * ใช้ aREST Framework API เป็นตัวจัดการ
 */
 
#include <SPI.h>
#include <aREST.h>

// Create aREST instance
aREST rest = aREST();

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("rsu-smt_serial");
}

void loop() {

  // Handle REST calls
  rest.handle(Serial);

} 
