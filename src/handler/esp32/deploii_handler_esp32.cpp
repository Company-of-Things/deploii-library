#if defined (ESP32)

#include "deploii_handler_esp32.h"

DeploiiHandler::DeploiiHandler()
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
    : _ws()
#elif DEPLOII_PROTOCOL == HTTP
    : _http()
#endif  // DEPLOII_PROTOCOL
{
}

DeploiiHandler::~DeploiiHandler() {
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
   _ws.~WebSocketsClient();
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
   _http.~HTTP_client();
#endif  // DEPLOII_PROTOCOL
}

void DeploiiHandler::connect(char* boardID, const char* ssid, const char* pwd) {
#if DEPLOII_MEDIUM == DEPLOII_WIFI
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);
   while (WiFi.status() != WL_CONNECTED) {
      delay(DEPLOII_WIFI_RECONNECT_TIME);
   }
#endif  // DEPLOII_MEDIUM

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
   char authHeader[60];
   sprintf(authHeader, "%s%s", "Authorization: ", boardID);
   _ws.setExtraHeaders(authHeader);
   _ws.beginSSL(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_WS_URL);

#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
   http.begin(host, port, url, buypass_cert);
   http.addHeader("Authorization", boardID, false, false);
#endif  // DEPLOII_PROTOCOL
}

void DeploiiHandler::send(const uint8_t* data, size_t size) {
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
   _ws.sendBIN(data, size);
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
   _http.POST((uint8_t*)data, size);
#endif  // DEPLOII_PROTOCOL
}

void DeploiiHandler::loop() {
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
   _ws.loop();
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP

#endif  // DEPLOII_PROTOCOL
}

#endif // ESP32
