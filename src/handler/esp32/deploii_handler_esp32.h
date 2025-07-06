/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h
#if defined(ESP32)

/*************************************************************************************/

#include "Arduino.h"

#if DEPLOII_MEDIUM == DEPLOII_WIFI
#include <WiFi.h>
#endif // DEPLOII_MEDIUM

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
#include <WebSocketsClient.h>
void _wsEvent(WStype_t type, uint8_t* payload, size_t length);
void (*_dataCallback)(uint8_t* data, size_t size){nullptr};
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
#include <HTTPClient.h>
#endif // DEPLOII_PROTOCOL

/*************************************************************************************/

class DeploiiHandler
{
public:
/*************************************************************************************/

  DeploiiHandler()
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
      : _ws()
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
      : _http()
#endif // DEPLOII_PROTOCOL
  {
  };

/*************************************************************************************/

  ~DeploiiHandler()
  {
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
    _ws.~WebSocketsClient();
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
    _http.~HTTPClient();
#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

  void loop()
  {
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
    _ws.loop();
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
    // poll data from server
#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

  void send(const uint8_t *data, size_t size)
  {
#pragma message(strx(DEPLOII_PROTOCOL))

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
    _ws.sendBIN(data, size);
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
    _http.POST((uint8_t *)data, size);
#endif // DEPLOII_PROTOCOL
  };
   void setDataCallback(void (*cb)(uint8_t* data, size_t size)) {
      _dataCallback = cb;
   }

/*************************************************************************************/

#if DEPLOII_MEDIUM == DEPLOII_WIFI
  void connect(
      char *boardID,
      const char *ssid,
      const char *pwd)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(DEPLOII_WIFI_RECONNECT_TIME);
    }

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
    static char authHeader[60];
    sprintf(authHeader, "%s%s", "Authorization: ", boardID);
    _ws.setExtraHeaders(authHeader);
    _ws.onEvent(_wsEvent);
    _ws.beginSSL(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_WS_URL);

#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
    _http.addHeader("Authorization", boardID, false, false);

#endif // DEPLOII_PROTOCOL
  };

 private:
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS

   WebSocketsClient _ws;

#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
   HTTPClient _http;
#endif  // DEPLOII_PROTOCOL

#else // OTHER MEDIUMS

#endif // DEPLOII_MEDIUM
};

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
void _wsEvent(WStype_t type, uint8_t* payload, size_t length) {
   switch (type) {
   case WStype_BIN:
      _dataCallback(payload, length);
      break;

   default:
      break;
   }
}
#endif

#endif // ESP32
#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
