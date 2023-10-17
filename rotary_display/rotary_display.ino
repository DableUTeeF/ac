uint8_t digits[10] = {
  0b00111111, 
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
};
uint8_t digit_locs[8] = {0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce};
uint8_t dot = 0b10000000;

int value = 0;
int nums[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t digit;
int low_temp = 2500;
int current_temp = 2800;

void sendCommand(uint8_t value)
{
  digitalWrite(D2, LOW);
  shiftOut(D1, D0, LSBFIRST, value);
  digitalWrite(D2, HIGH);
}

void writedigits(){
  for(uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(D2, LOW);
    shiftOut(D1, D0, LSBFIRST, digit_locs[i]);
    digit = digits[nums[i]];
    if (i == 1) digit += dot;
    else if (i == 5) digit += dot;
    shiftOut(D1, D0, LSBFIRST, digit);
    digitalWrite(D2, HIGH);
  }
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
  writedigits();
}
 
void setup()
{
  pinMode(D2, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(A0, INPUT); 
  sendCommand(0x8f);  // activate and set brightness to max
  reset();
}

void loop()
{
  sendCommand(0x44);  // set single address

  // dot, center, topleft, botleft, bot, botright, topright, top
  value = analogRead(A0);
  nums[0] = value / 1000;
  nums[1] = value / 100 % 10;
  nums[2] = value / 10 % 10;
  nums[3] = value / 1 % 10;
  current_temp = low_temp + value;
  nums[4] = current_temp / 1000;
  nums[5] = current_temp / 100 % 10;
  nums[6] = current_temp / 10 % 10;
  nums[7] = current_temp / 1 % 10;
  writedigits();
  delay(200);
  
}