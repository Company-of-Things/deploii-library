#include "deploii.h"

Deploii::Deploii(const char *mcuID) {
   _mcuID = mcuID;
}

void Deploii::connect_wifi(char *ssid, const char *password) {

   Serial.begin(9600);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
      delay(1000);

   Serial.println("Wifi connected");

   _ws.begin(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_URL);
   _ws.onEvent(nullptr);
   _ws.setAuthorization(_mcuID, "");
   _ws.setReconnectInterval(5000);
   _connectedWiFi = true;
}

void Deploii::loop() {
   if (_connectedWiFi) {
      _ws.loop();
   }
}