
#define PWM_Speed     1000
#define SamplingTime  4
#define Sensor_1      D5
#define Sensor_2      D6
#define Sensor_3      D7

#define led_pin         D0
#define motor_pwm_left  D1
#define motor_pwm_right D2
#define motor_dir_left  D3
#define motor_dir_right D4
#define dir_back        LOW
#define dir_fwd         HIGH

unsigned long Time;

void setup() {
  pinMode(Sensor_1, INPUT);
  pinMode(Sensor_2, INPUT);
  pinMode(Sensor_3, INPUT);
  pinMode(motor_dir_left, OUTPUT);
  pinMode(motor_pwm_left, OUTPUT);
  pinMode(motor_dir_right, OUTPUT);
  pinMode(motor_pwm_right, OUTPUT);
  Serial.begin(115200);
  Serial.println("Robot Test V1.00");
}

void loop() {
  Time = millis();
  int ReadSensor = digitalRead(Sensor_1);
  ReadSensor = ReadSensor * 10 + digitalRead(Sensor_2);
  ReadSensor = ReadSensor * 10 + digitalRead(Sensor_3);
  Serial.print("Read Sernsor = ");
  Serial.println(ReadSensor);

  if (ReadSensor == 101 || ReadSensor == 000)  Robot_Forward();
  else if (ReadSensor == 001 || ReadSensor == 011)  Robot_Left();
  else if (ReadSensor == 100 || ReadSensor == 110)  Robot_Right();
  else if (ReadSensor == 111 || ReadSensor == 010)  Robot_Backward();
}
