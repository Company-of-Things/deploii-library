#include <deploii.h>

#define DEPLOII_MEDIUM DEPLOII_WIFI
#define DEPLOII_PROTOCOL DEPLOII_WEBSOCKETS

Deploii oi("BOARD-ID");

void setup() {
   oi.connect("WiFi-SSID", "WiFi-PASSWORD");
}

void loop() {
   oi.loop();
}
