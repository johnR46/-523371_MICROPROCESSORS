void setup() {
  // put your setup code here, to run once:
pinMode(18, OUTPUT);
pinMode(19, OUTPUT);
pinMode(22, OUTPUT);
pinMode(23, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
bnk();
}

void bnk(){
  digitalWrite(18,1);
    delay(500);
  digitalWrite(18,0);
  delay(500);
  
  digitalWrite(19,1);
    delay(500);
  digitalWrite(19,0);
  delay(500);

  
  digitalWrite(22,1);
    delay(500);
  digitalWrite(22,0);
  delay(500);


  
  digitalWrite(23,1);
    delay(500);
  digitalWrite(23,0);
  delay(500);
  
}

