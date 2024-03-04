#include "deploii_handler.h"

/*
    Deploii handler for communication using WiFI and WebSockets
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
    const char* ssid,
    const char* pwd,
    const char* host,
    const int port,
    const char* url,
    bool ssl) {
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);
   while (WiFi.status() != WL_CONNECTED) delay(1000);

   char authHeader[40];
   sprintf(authHeader, "%s%s", "Authorization: ", boardID);
   _ws.setExtraHeaders(authHeader);

   if (ssl)
      _ws.beginSSL(host, port, url);
   else
      _ws.begin(host, port, url);
}