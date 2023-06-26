#include "DHT.h"
#include <DYIRDaikin.h>
#include <TM1637Display.h>

#define DAIKINSPIN 3
#define DHTPIN 2  // D1 for MCU, 2 for UNO
// #define DYIRDAIKIN_SOFT_IR  // enable for MCU, off for UNO
#define DHTTYPE DHT22
#define CLK 0
#define DIO 1

TM1637Display display = TM1637Display(CLK, DIO);

DHT dht(DHTPIN, DHTTYPE);
DYIRDaikin irdaikin;
int isOn;
int onoff_delay;
int cycles;

void setup() {
  // dht
  dht.begin();

  // daikins
  irdaikin.begin(DAIKINSPIN);
  irdaikin.on();
  irdaikin.setSwing_on();
  irdaikin.setMode(1);
  irdaikin.setFan(5);//FAN speed to MAX
  irdaikin.setTemp(18);
  display.setBrightness(3);
  irdaikin.sendCommand();
  isOn = 1;
  onoff_delay = 60;
  cycles = onoff_delay;

  // display
  const uint8_t allON[] = {0xff, 0xff, 0xff, 0xff};
  display.setBrightness(3);
  display.setSegments(allON);
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int dis_h = (int) h;
  int dis_t = (int) t;
  display.showNumberDecEx(dis_t * 100 + dis_h, 0b01000000);
  delay(500);
}

void set_on(int condition){
  isOn = 1;
  irdaikin.setTemp(18);
  // delay(1000*onoff_delay);
  irdaikin.sendCommand();
}

void set_off(int condition){
  isOn = 0;
  irdaikin.setTemp(32);
  // delay(1000*onoff_delay);
  irdaikin.sendCommand();
}

void ac_control(float h, float t, float hic){
  if (hic > 30.0 && ~isOn){
    set_on(0);
  }
  else if (h > 70.0 && ~isOn && t > 25){
    set_on(1);
  }
  else if (h > 55.0 && ~isOn && t > 27){
    set_on(2);
  }
  else if (h > 65.0 && ~isOn && t > 26.5){
    set_on(3);
  }
  else if (hic < 28.0 && isOn){
    set_off(4);
  }
  else if (t < 25 && isOn){
    set_off(5);
  }
  // else{
  //   delay(2000);
  // }
}

void render(float h, float t, float hic){
  int dis_h = (int) h;
  int dis_t = (int) t;
  int b;
  if (isOn){
    b = 0b01000000;
  }
  else{
    b = 0;
  }
  display.showNumberDecEx(dis_t * 100 + dis_h, b);
  delay(1000);
  hic = hic * 10;
  int dhic = (int) hic;
  display.showNumberDecEx(dhic, b);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);
  render(h, t, hic);
  if (cycles >= onoff_delay){  
    ac_control(h, t, hic);
    cycles = 0;  
  }  
  delay(1000);
  cycles += 1;
}
