#include <deploii.h>

#define Deploii_medium WiFi
#define Deploii_protocol WebSockets

Deploii oi("BOARD-ID");

void setup() {
   oi.connect("WiFi-SSID", "WiFi-PASSWORD");
}

void loop() {
   oi.loop();
}
