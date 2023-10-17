#include <TM1637Display.h>
#include "DHT.h"
#include <DYIRDaikin.h>

#define DAIKINSPIN 3
#define DHTPIN 2
#define DHTTYPE DHT22

#define CLK 0
#define DIO 1

TM1637Display display = TM1637Display(CLK, DIO);

DHT dht(DHTPIN, DHTTYPE);
DYIRDaikin irdaikin;
int isOn;
int onoff_delay;

void setup() {
  // display
  const uint8_t allON[] = {0xff, 0xff, 0xff, 0xff};
  display.setBrightness(5);
  display.setSegments(allON);

  // dht
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void render(float h, float t){
  int dis_h = (int) h;
  int dis_t = (int) t;
  if (isOn){
    display.showNumberDecEx(dis_t * 100 + dis_h, 0b01000000);
  }
  else{
    display.showNumberDec(dis_t * 100 + dis_h);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);

  int dis_h = (int) h;
  int dis_t = (int) t;

  delay(5000);
  display.showNumberDec(dis_t * 100 + dis_h);
  delay(5000);
  display.showNumberDecEx(dis_t * 100 + dis_h, 0b01000000);
}
