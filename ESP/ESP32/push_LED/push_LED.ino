
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  Serial.begin(115200);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(2, OUTPUT);
}

// the loop function runs over and over again forever
int x,y;
int Mode = -1;
void loop() {
x = digitalRead(22);
y = digitalRead(23);

if(x == 0){
  Mode = 0;
}
if(y == 0){
  Mode = 1;
}


switch(Mode){
  case 0 :
  blinK();
  break;

  case 1:
  ON();
  break;
}

 
}
void ON(){
digitalWrite(2,1);
delay(500);
}
void blinK() {
digitalWrite(2,1);
delay(500);
digitalWrite(2,0);
delay(500);

}
