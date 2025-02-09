#include <deploii.h>

#define Deploii_medium Deploii_WiFi
#define Deploii_protocol Deploii_WebSockets

Deploii oi("BOARD-ID");

void setup() {
   oi.connect("WiFi-SSID", "WiFi-PASSWORD");
}

void loop() {
   oi.loop();
}
