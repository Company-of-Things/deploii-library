#include <Deploii.h>

Deploii oi("Board ID", Medium::WiFi, Protocol::WebSockets);

int myData = 10;
int myDataArray[] = {1, 2, 3};

void setup() {
   oi.connect("WiFi SSID", "WiFi Password");

   oi.interval(3000, sendData);
}

void loop() {
   oi.loop();
}

void sendData() {
   oi.send("DataStream ID", myData);
   oi.send("DataStream ID", myDataArray);
}