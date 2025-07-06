/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h
#if defined(ESP32)

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_debug.h"

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_WIFI
#include <WiFi.h>
#endif // DEPLOII_MEDIUM

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
#include <WebSocketsClient.h>
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
#include <HTTPClient.h>
#endif // DEPLOII_PROTOCOL

/*************************************************************************************/

class DeploiiHandler
{
public:
/*************************************************************************************/

  DeploiiHandler()
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
      : _ws()
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
      : _http()
#endif // DEPLOII_PROTOCOL
  {
  };

/*************************************************************************************/

  ~DeploiiHandler()
  {
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    _ws.~WebSocketsClient();
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    _http.~HTTPClient();
#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

  void loop()
  {
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    _ws.loop();
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    // poll data from server
#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

  void send(const uint8_t *data, size_t size)
  {
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    _ws.sendBIN(data, size);
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    _http.POST((uint8_t *)data, size);
#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_WIFI
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

    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "Connected to wifi");

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    static char authHeader[60];
    sprintf(authHeader, "%s%s", "Authorization: ", boardID);
    _ws.setExtraHeaders(authHeader);
    _ws.beginSSL(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_WS_URL);

#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    _http.addHeader("Authorization", boardID, false, false);

#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS

private:
  WebSocketsClient _ws;
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
private:
  HTTPClient _http;
#endif // DEPLOII_PROTOCOL

#else // OTHER MEDIUMS

#endif // DEPLOII_MEDIUM
};

/*************************************************************************************/

#endif // ESP32
#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
