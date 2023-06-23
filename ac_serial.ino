#include "DHT.h"
#include <DYIRDaikin.h>
#include <TM1637Display.h>

#define DAIKINSPIN 3
#define DHTPIN 2  // D1 for MCU, 2 for UNO
// #define DYIRDAIKIN_SOFT_IR  // enable for MCU, off for UNO
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
DYIRDaikin irdaikin;
int isOn;
int onoff_delay;

void setup() {
  // serial
  Serial.begin(9600);

  // dht
  Serial.println(F("DHTxx test!"));
  dht.begin();

  // daikins
  irdaikin.begin(DAIKINSPIN);
  irdaikin.on();
  irdaikin.setSwing_on();
  irdaikin.setMode(1);
  irdaikin.setFan(5);//FAN speed to MAX
  irdaikin.setTemp(18);
  display.setBrightness(5);
  irdaikin.sendCommand();
  isOn = 1;
  onoff_delay = 120;  // in seconds
}

void set_on(int condition){
  isOn = 1;
  irdaikin.setTemp(18);
  Serial.print(F("On, condition "));
  Serial.print(condition);
  delay(1000*onoff_delay);
}

void set_off(int condition){
  isOn = 0;
  irdaikin.setTemp(32);
  Serial.print(F("Off, condition "));
  Serial.print(condition);
  delay(1000*onoff_delay);
}

void ac_control(float h, float t, float hic){
  if (hic > 31.0 && ~isOn){
    set_on(0);
  }
  else if (hic < 29.0 && isOn){
    set_off(1);
  }
  else if (h > 70.0 && ~isOn && t > 25){
    set_on(2);
  }
  else{
    delay(2000);
  }
}


void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("% Temperature: "));
  Serial.print(t);
  Serial.print(F(" C "));
  Serial.print(F(" Heat index: "));
  Serial.print(hic);
  Serial.println(F(" C "));
  ac_control(h, t, hic);  
}
