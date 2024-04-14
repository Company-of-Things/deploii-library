#include "deploii_handler.h"

/*
   Deploii handler for communication using WiFI and WebSockets
*/

/*
   Constants
*/
#define Deploii_WIFI_RECONNECT_TIME 1000

/*
   Class definitions
*/

/*
   Public methods
*/

DeploiiHandlerWiFiWS::DeploiiHandlerWiFiWS()
#if defined(ESP32)
    : _ws()
#elif defined(ARDUINO)
#else
#endif
{
}

DeploiiHandlerWiFiWS::~DeploiiHandlerWiFiWS() : {
#if defined(ESP32)
   delete _ws;
#elif defined(ARDUINO)
#else
#endif
}

void DeploiiHandlerWiFiWS::send() {
}

void DeploiiHandlerWiFiWS::loop() {
#if defined(ESP32)
   _ws.loop();
#elif defined(ARDUINO)
#else
#endif
}

void DeploiiHandlerWiFiWS::connect(
    char* boardID,
    char* ssid,
    const char* pwd,
    const char* host,
    const int port,
    const char* url,
    bool ssl) {
   connectWiFi(ssid, pwd);
   connectWS(boardID, host, port, url, ssl);
}

/*
   Private methods
*/

#if defined(ESP32)

void DeploiiHandlerWiFiWS::connectWiFi(char* ssid, const char* pwd) {
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);
   while (WiFi.status() != WL_CONNECTED) delay(Deploii_WIFI_RECONNECT_TIME);
}

void DeploiiHandlerWiFiWS::connectWS(char* boardID, const char* host, const int port, const char* url, bool ssl) {
   char authHeader[40];
   sprintf(authHeader, "%s%s", "Authorization: ", boardID);
   _ws.setExtraHeaders(authHeader);

   if (ssl)
      _ws.beginSSL(host, port, url);
   else
      _ws.begin(host, port, url);
}

#elif defined(ARDUINO)

void DeploiiHandlerWiFiWS::connectWiFi(char* ssid, const char* pwd) {
   while (WiFi.begin(ssid, pwd) != WL_CONNECTED) delay(Deploii_WIFI_RECONNECT_TIME);
}

void DeploiiHandlerWiFiWS::connectWS(char* boardID, const char* host, const int port, const char* url, bool ssl) {
}

#else
void DeploiiHandlerWiFiWS::connectWiFi(char* ssid, const char* pwd) {}

void DeploiiHandlerWiFiWS::connectWS(char* boardID, const char* host, const int port, const char* url, bool ssl) {
}

#endif