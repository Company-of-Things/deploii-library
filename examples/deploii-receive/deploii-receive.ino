#define DEPLOII_MEDIUM DEPLOII_MEDIUM_WIFI
#define DEPLOII_PROTOCOL DEPLOII_PROTOCOL_WEBSOCKETS

#include <deploii.h>

Deploii oi("Board ID");

void setup() {
   oi.receive(receiveData);
   oi.connect("WiFi SSID", "WiFi Password");
}

void loop() {
   oi.loop();
}

void receiveData(String ID, String data){
  Serial.print(ID);
  Serial.print(": ");
  Serial.println(data);
}
