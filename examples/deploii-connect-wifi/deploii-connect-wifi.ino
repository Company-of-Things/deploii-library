#include <deploii.h>

Deploii oi("BOARD-ID", Medium::WiFi, Protocol::WebSockets);

void setup() {
   oi.connect("WiFi-SSID", "WiFi-PASSWORD");
}

void loop() {
   oi.loop();
}
