#ifndef Deploii_h
#define Deploii_h

#include "Arduino.h"

#include <WiFi.h>
#include <WebSocketsClient.h>

#define DEPLOII_HOST "www.deploii.no"
#define DEPLOII_PORT 5215
#define DEPLOII_URL "/ws"

class Deploii {
 private:
   WebSocketsClient _ws;
   const char *_mcuID;
   void eventHandler(WStype_t type, unsigned char *payload, size_t length);

   bool _connectedWiFi = false;

 public:
   Deploii(const char *mcuID);
   void loop();
   void connect_wifi(const char *ssid, const char *password);
};

#endif