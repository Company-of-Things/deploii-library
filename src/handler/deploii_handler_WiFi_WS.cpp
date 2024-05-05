#include "deploii_handler.h"

/*
   Deploii handler for communication using WiFI and WebSockets
*/

/*
   Constants
*/
#define Deploii_WIFI_RECONNECT_TIME 1000
#define DEPLOII_WS_RECONNECT_TIME 1000

/*
   Class definitions
*/

/*
   Public methods
*/

DeploiiHandlerWiFiWS::DeploiiHandlerWiFiWS(bool debug)
#if defined(ESP32)

    : _ws(), _debug(debug)
#elif defined(ARDUINO)
    : _debug(debug)
#else
    : _debug(debug)
#endif
{
}

DeploiiHandlerWiFiWS::~DeploiiHandlerWiFiWS() {
#if defined(ESP32)
   _ws.~WebSocketsClient();
#elif defined(ARDUINO)
#else
#endif
}

void DeploiiHandlerWiFiWS::send(const uint8_t* data, size_t size) {
#if defined(ESP32)
   _ws.sendBIN(data, size);
   if (_debug) {
      Serial.println("Sending");
      for (auto i = 0; i < size; i++) {
         Serial.print(data[i], HEX);
         Serial.print(" ");
      }
      Serial.println();
   }
#elif defined(ARDUINO)
#else
#endif
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
   while (WiFi.status() != WL_CONNECTED) {
      delay(Deploii_WIFI_RECONNECT_TIME);
      if (_debug) {
         Serial.println("Connecting to WiFi");
      }
   }
   if (_debug) {
      Serial.println("WiFi connected");
   }
}

void DeploiiHandlerWiFiWS::connectWS(char* boardID, const char* host, const int port, const char* url, bool ssl) {
   char authHeader[40];
   sprintf(authHeader, "%s%s", "Authorization: ", boardID);
   _ws.setExtraHeaders(authHeader);
   while (!_ws.isConnected()) {
      if (ssl)
         _ws.beginSSL(host, port, url);
      else
         _ws.begin(host, port, url);
      if (_debug) {
         Serial.println("Connecting to WS");
      }
      delay(DEPLOII_WS_RECONNECT_TIME);
   }
   if (_debug) {
      Serial.println("WS connected");
   }
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