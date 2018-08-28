void setup() {
  Serial.begin(115200);
}
void loop()
{ unsigned int x;
  x = 0; Serial.println(x, BIN);  // X =
  x |= (1 << 0); Serial.println(x, BIN); // X =
  x |= (1 << 1); Serial.println(x, BIN); // X =
  x |= (1 << 2); Serial.println(x, BIN); // X =
  x |= (1 << 3); Serial.println(x, BIN); // X =
  x |= (3 << 6); Serial.println(x, BIN); // X =
  x &= ~(1 << 0); Serial.println(x, BIN); // X =
  x &= ~(1 << 3); Serial.println(x, BIN); // X =
  x ^= (1 << 0); Serial.println(x, BIN); // X =
  x ^= (1 << 0); Serial.println(x, BIN); // X =
  x ^= (1 << 0); Serial.println(x, BIN); // X =
  while (1);
}
