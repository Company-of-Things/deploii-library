#include "deploii_handler.h"

/*
   Deploii handler for communication using WiFI and WebSockets
*/

/*
   Constants
*/
#define Deploii_WIFI_RECONNECT_TIME 1000

/*
   Helper function declarations
*/
void connectWiFi(char* ssid, const char* pwd);

/*
   Class definitions
*/

DeploiiHandlerWiFiWS::DeploiiHandlerWiFiWS() : _ws() {
}

void DeploiiHandlerWiFiWS::send() {
}

void DeploiiHandlerWiFiWS::loop() {
   _ws.loop();
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

   char authHeader[40];
   sprintf(authHeader, "%s%s", "Authorization: ", boardID);
   _ws.setExtraHeaders(authHeader);

   if (ssl)
      _ws.beginSSL(host, port, url);
   else
      _ws.begin(host, port, url);
}

/*
   Helper function definitions
*/

#if defined(ESP32)

void connectWiFi(char* ssid, const char* pwd) {
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);
   while (WiFi.status() != WL_CONNECTED) delay(Deploii_WIFI_RECONNECT_TIME);
}

#elif defined(ARDUINO)

void connectWiFi(char* ssid, const char* pwd) {
   while (WiFi.begin(ssid, pwd) != WL_CONNECTED) delay(Deploii_WIFI_RECONNECT_TIME);
}

#else
void connectWiFi(const char* ssid, const char* pwd) {}

#endif