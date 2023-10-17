// const int strobe = D2;
// const int clock = D0;
// const int data = D1;
 
void sendCommand(uint8_t value)
{
  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, value);
  digitalWrite(D2, HIGH);
}
 
void reset()
{
  sendCommand(0x40); // set auto increment mode
  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(D1, D0, LSBFIRST, 0x00);
  }
  digitalWrite(D2, HIGH);
}
 
void setup()
{
  pinMode(D2, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
 
  sendCommand(0x8f);  // activate and set brightness to max
  reset();
}

void loop()
{
  sendCommand(0x44);  // set single address
 
  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xc5); // 3rd LED
  shiftOut(D1, D0, LSBFIRST, 0x01);
  digitalWrite(D2, HIGH);

  // dot, center, topleft, botleft, bot, botright, topright, top
  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xc0); // 1st digit
  shiftOut(D1, D0, LSBFIRST, 0b00111111);
  digitalWrite(D2, HIGH);
  
  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xc2); // 2st digit
  shiftOut(D1, D0, LSBFIRST, 0b00000110);
  digitalWrite(D2, HIGH);

  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xc4); // 3st digit
  shiftOut(D1, D0, LSBFIRST, 0b11001111);
  digitalWrite(D2, HIGH);

  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xc6); // 4st digit
  shiftOut(D1, D0, LSBFIRST, 0xff);
  digitalWrite(D2, HIGH);

  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xc8); // 5st digit
  shiftOut(D1, D0, LSBFIRST, 0xff);
  digitalWrite(D2, HIGH);

  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xca); // 6st digit
  shiftOut(D1, D0, LSBFIRST, 0xff);
  digitalWrite(D2, HIGH);

  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xcc); // 7st digit
  shiftOut(D1, D0, LSBFIRST, 0xff);
  digitalWrite(D2, HIGH);

  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, 0xce); // last digit
  shiftOut(D1, D0, LSBFIRST, 0xff);
  digitalWrite(D2, HIGH);
}