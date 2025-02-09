#include <deploii.h>

#define Deploii_medium Deploii_WiFi
#define Deploii_protocol Deploii_WebSockets

Deploii oi("Board ID");

int myData = 10;
int myDataArray[] = {1, 2, 3};

void setup() {
   oi.connect("WiFi SSID", "WiFi Password");
   oi.send("DataStream ID", myData);
   oi.send("DataStream ID", myDataArray);
}

void loop() {
   oi.loop();
}
