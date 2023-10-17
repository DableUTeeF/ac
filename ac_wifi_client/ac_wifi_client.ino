#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DYIRDaikin.h>

const char *ssid = "poopssid";
const char *password = "pingu4prez";
ESP8266WebServer server(80);
DYIRDaikin irdaikin;
int onoff_delay = 120;
int cycles = onoff_delay;

void handleSentVar() {
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
    int value = server.arg("sensor_reading").toInt();
    Serial.println(value);
    if (value == 2){
      cycles = onoff_delay;
    }
    if (value == 1){
      digitalWrite(D0, HIGH);
    }
    else{
      digitalWrite(D0, LOW);
    }
    if (cycles >= onoff_delay) {
      if (value){
        set_on();
      }
      else{
        set_off();
      }
      cycles = 0;
    }    
    delay(200);
    server.send(200, "text/html", "Data received");
  }
  cycles += 1;
}

void setup() {
  pinMode(D0, OUTPUT);
  Serial.begin(115200);
  Serial.println("WWW");
  // daikins
  delay(300);
  irdaikin.begin(D3);
  delay(300);
  irdaikin.on();
  irdaikin.setSwing_on();
  irdaikin.setMode(1);
  irdaikin.setFan(4);//FAN speed to MAX
  irdaikin.setTemp(18);
  irdaikin.sendCommand();

  delay(1000);
  WiFi.softAP(ssid, password);
  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
  String myIP = WiFi.localIP().toString();
  Serial.println(myIP);
  Serial.println(WiFi.softAPIP());
  Serial.println("TT");

}
void set_on(){
  irdaikin.setTemp(18);
  irdaikin.setFan(4);//FAN speed to MAX
  // delay(1000*onoff_delay);
  irdaikin.sendCommand();
}

void set_off(){
  irdaikin.setTemp(32);
  irdaikin.setFan(0);
  // delay(1000*onoff_delay);
  irdaikin.sendCommand();
}

void loop() {
  server.handleClient();
}
