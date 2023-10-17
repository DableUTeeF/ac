#include <DYIRDaikin.h>

#define DAIKINSPIN 3

DYIRDaikin irdaikin;
int isOn = 0;
int brightness = 512;
int onoff_delay = 10;
int cycles = onoff_delay;

void setup() {
  Serial.begin(9600);
  pinMode(13, INPUT);
  pinMode(9, OUTPUT);
  // daikins
  irdaikin.begin(DAIKINSPIN);
  irdaikin.on();
  irdaikin.setSwing_on();
  irdaikin.setMode(1);
  irdaikin.setFan(4);//FAN speed to MAX
  irdaikin.setTemp(18);
  irdaikin.sendCommand();
  delay(1000);
}

void set_on(){
  isOn = 1;
  irdaikin.setTemp(18);
  irdaikin.setFan(4);//FAN speed to MAX
  // delay(1000*onoff_delay);
  irdaikin.sendCommand();
}

void set_off(){
  isOn = 0;
  irdaikin.setTemp(32);
  irdaikin.setFan(0);
  // delay(1000*onoff_delay);
  irdaikin.sendCommand();
}

void loop() {
  isOn = digitalRead(13);
  Serial.println(isOn);
  if (cycles >= onoff_delay){  
    if (isOn){
      set_on();
    }
    else{
      set_off();
    }
    cycles = 0;  
  }  
  if (isOn){
    brightness = analogRead(A0);
    analogWrite(9, brightness >> 2);
  }
  else{
    digitalWrite(9, LOW);
  }
  delay(1000);
}