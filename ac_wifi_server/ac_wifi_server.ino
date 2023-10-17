#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D3  // D1 for MCU, 2 for UNO
#define DHTTYPE DHT22


const char *ssid = "poopssid";
const char *password = "pingu4prez";

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
int sensorValue = 0;
float h = 70.0;
float t = 30.0;
float hic = 30.0;
int temperature = 30;
int humid = 70;

DHT dht(DHTPIN, DHTTYPE);

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

void setup() {
  dht.begin();
  Serial.begin(115200);
  delay(10);

  pinMode(D2, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(A0, INPUT); 
  sendCommand(0x8f);  // activate and set brightness to max
  reset();

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

int get_state(float h, float t, float hic){
  if (hic > 30.0){
    return 1;
  }
  // else if (h > 70.0 && t > 25.7){
  //   return 1;
  // }
  else if (h > 55.0 && t > 27){
    return 1;
  }
  else if (h > 65.0 && t > 26.5){
    return 1;
  }
  else if (hic < 28.0){
    return 0;
  }
  else if (t < 25){
    return 0;
  }
  return 0;
}

void loop() {

  h = dht.readHumidity();
  humid = (int) h;
  t = dht.readTemperature();
  temperature = (int) t;
  hic = dht.computeHeatIndex(t, h, false);

  // read the analog in value:
  sensorValue = analogRead(A0);

  // dot, center, topleft, botleft, bot, botright, topright, top
  nums[0] = (int) t * 0.1;
  nums[1] = (int) t * 1 % 10;
  nums[2] = (int) (t * 10) % 10;
  nums[3] = (int) (t * 100) % 10;
  nums[4] = (int) h * 0.1;
  nums[5] = (int) h * 1 % 10;
  nums[6] = (int) (h * 10) % 10;
  nums[7] = (int) (h * 100) % 10;
  sendCommand(0x44);
  writedigits();
  delay(200);

  char intToPrint[4];
  itoa(get_state(h, t, hic), intToPrint, 10); //integer to string conversion for OLED library

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char * host = "192.168.4.1";
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request. Something like /data/?sensor_reading=123
  String url = "/data/";
  url += "?sensor_reading=";
  url += intToPrint;

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  delay(500);
}
