void setup()
{
  Serial.begin(115200);
  pinMode(A0, INPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  int value = analogRead(A0);
  Serial.print(value);
  Serial.print(" ");
  Serial.println(value >> 5);
  delay(1000);
}

