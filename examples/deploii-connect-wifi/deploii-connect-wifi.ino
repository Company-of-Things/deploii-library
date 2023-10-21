#include <deploii.h>

Deploii oi("BOARD ID");

void setup() {
   Serial.begin(9600);
   oi.connect_wifi("SSID", "PASSWORD");
}

void loop() {
   oi.loop();
}