/*
 * EX5_TochSensor_Read
 * รออ่านค่าเซ็นเซอร์สัมผัส T4 T5 T2 แล้วมาแสดงผลในคอนโซล
 */

void gotTouch1(){
  Serial.print("Touch sensor T4 detected : ");
  Serial.println(touchRead(T4));
}

void gotTouch2(){
  Serial.print("Touch sensor T5 detected : ");
  Serial.println(touchRead(T5));
}

void gotTouch3(){
  Serial.print("Touch sensor T2 detected : ");
  Serial.println(touchRead(T2));
}

void setup() {
  Serial.begin(115200);
 
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T4, gotTouch1, 40);
  touchAttachInterrupt(T5, gotTouch2, 40);
  touchAttachInterrupt(T2, gotTouch3, 40);
}

void loop(){

}
