#include <DYIRDaikin.h>
#include <dht11.h>

//#define DYIRDAIKIN_SOFT_IR
#define DHT11PIN A0 // Analog Pin sensor is connected to
dht11 DHT11;

DYIRDaikin irdaikin;

int isOn;

void setup()
{
  Serial.begin(115200);
  #ifdef DYIRDAIKIN_SOFT_IR
  irdaikin.begin(3);
  #else
  irdaikin.begin();
  #endif
  irdaikin.on();
  irdaikin.setSwing_on();
  irdaikin.setMode(1);
  irdaikin.setFan(5);//FAN speed to MAX
  irdaikin.setTemp(25);
  //----everything is ok and to execute send command-----
  irdaikin.sendCommand();
  isOn = 0;
}

void loop() {
//  while (Serial.available() > 0) {
//
//    if (Serial.read() == '\n') {
//      if (isOn == 0){
//        isOn = 1;
//        irdaikin.off();
//        Serial.println("Turn Off");
//      }else{
//        isOn = 0;
//        irdaikin.on();
//        Serial.println("Turn On");
//      }
//      irdaikin.sendCommand();
//      Serial.println("Execute Command!");
//    }
//  }
/////////////////////////////////
  DHT11.read(DHT11PIN);
  int temp = DHT11.temperature;
  int hum = DHT11.humidity;

  int R = hum;
  int T = temp;
  double c[10] = {0.0, -8.78469475556, 1.61139411, 2.33854883889, -0.14611605,
                  -0.012308094, -0.0164248277778, 0.002211732, 0.00072546, -0.000003582
                 };
  double hi = c[1] + c[2]*T + c[3]*R + c[4]*T*R + c[5]*T*T + c[6]*R*R + c[7]*T*T*R + c[8]*T*R*R + c[9]*T*T*R*R; 
  Serial.println(hi);
  if(hi < 27){
    irdaikin.setTemp(32);
//    irdaikin.setTemp(24);
//    irdaikin.off();
    irdaikin.sendCommand();
    Serial.print("temp: ");
    Serial.print(temp);
    Serial.print(" - hum: ");
    Serial.print(hum);
    Serial.print(" - HI: ");
    Serial.println(hi);
    Serial.println("off");
    delay(60000);
  }
  else if (hi > 29){
    irdaikin.setTemp(20);
//    irdaikin.setTemp(24);
//    irdaikin.on();
    irdaikin.sendCommand();
    Serial.print("temp: ");
    Serial.print(temp);
    Serial.print(" hum: ");
    Serial.print(hum);
    Serial.print(" - HI: ");
    Serial.println(hi);
    Serial.println("on");
    delay(60000);
  }
//  irdaikin.off();
//  irdaikin.sendCommand();
  delay(5000);

}
