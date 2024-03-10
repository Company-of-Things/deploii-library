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
void connectWS(WebSocketsClient ws, char* boardID, const char* host, const int port, const char* url, bool ssl);
void connectWS(char* boardID, const char* host, const int port, const char* url, bool ssl);

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
   connectWS(_ws, boardID, host, port, url, ssl);
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

void connectWS(WebSocketsClient ws, char* boardID, const char* host, const int port, const char* url, bool ssl) {
   char authHeader[40];
   sprintf(authHeader, "%s%s", "Authorization: ", boardID);
   ws.setExtraHeaders(authHeader);

   if (ssl)
      ws.beginSSL(host, port, url);
   else
      ws.begin(host, port, url);
}

#elif defined(ARDUINO)

void connectWiFi(char* ssid, const char* pwd) {
   while (WiFi.begin(ssid, pwd) != WL_CONNECTED) delay(Deploii_WIFI_RECONNECT_TIME);
}

void connectWS(WebSocketsClient ws, char* boardID, const char* host, const int port, const char* url, bool ssl) {
}

#else
void connectWiFi(char* ssid, const char* pwd) {}

void connectWS(WebSocketsClient ws, char* boardID, const char* host, const int port, const char* url, bool ssl) {
}

#endif