#include "deploii_handler_esp32.h"

DeploiiHandler::DeploiiHandler()
#if Deploii_protocol == WebSockets
    : _ws()
#elif Deploii_protocol == HTTP
    : _http()
#endif  // Deploii_protocol
{
}

DeploiiHandler::~DeploiiHandler() {
#if Deploii_protocol == WebSockets
   delete _ws();
#elif Deploii_protocol == HTTP
   delete _http();
#endif  // Deploii_protocol
}

void DeploiiHandler::connect(char* boardID, char* ssid, const char* pwd) {
#if Deploii_medium == WiFi
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);
   while (WiFi.status() != WL_CONNECTED) {
      delay(DEPLOII_WIFI_RECONNECT_TIME);
   }
#endif  // Deploii_medium

#if Deploii_protocol == WebSockets
   char authHeader[60];
   sprintf(authHeader, "%s%s", "Authorization: ", boardID);
   _ws.setExtraHeaders(authHeader);
   _ws.beginSSL(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_WS_URL);

#elif Deploii_protocol == HTTP
   http.begin(host, port, url, buypass_cert);
   http.addHeader("Authorization", boardID, false, false);
#endif  // Deploii_protocol
}

void DeploiiHandler::send(const uint8_t* data, size_t size) {
#if Deploii_protocol == WebSockets

#elif Deploii_protocol == HTTP
   _http.POST((uint8_t*)data, size);
#endif  // Deploii_protocol
}
