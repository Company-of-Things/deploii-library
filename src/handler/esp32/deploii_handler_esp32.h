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

void _defaultCallback(uint8_t*data, size_t size){};
void (*_dataCallback)(uint8_t* data, size_t size) = &_defaultCallback;

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
#include <WebSocketsClient.h>
void _wsEvent(WStype_t type, uint8_t* payload, size_t length);

#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
#include <HTTPClient.h>

#if DEPLOII_SSL
#include <WiFiClientSecure.h>
#include "deploii_certs.h"
#endif

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
#if DEPLOII_SSL
      , _client()
#endif
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
    _ws.loop(); // must be called in order to reconnect after disconnect
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    // poll data from server
#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

  void send(const uint8_t *data, size_t size)
  {
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    _ws.sendBIN(data, size);
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Sending WEBSOCKET data of size 0x%zx", size);

#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    _http.POST((uint8_t *)data, size);
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Sending HTTP data of size 0x%zx", size);

#endif // DEPLOII_PROTOCOL
  };

/*************************************************************************************/

  void setDataCallback(void (*cb)(uint8_t* data, size_t size))
  {
    _dataCallback = cb;
  }

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
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Attempting to connect to WiFi...");
      delay(DEPLOII_WIFI_RECONNECT_TIME);
    }

    IPAddress ip = WiFi.localIP();
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "Connected to WiFi with IP %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    static char authHeader[60];
    sprintf(authHeader, "%s%s", "Authorization: ", boardID);
    _ws.setExtraHeaders(authHeader);
    _ws.onEvent(_wsEvent);

#if DEPLOII_SSL
    _ws.beginSSL(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_WS_URL);
#else
    _ws.begin(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_WS_URL);
#endif

#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP

#if DEPLOII_SSL
    _client.setCACert(buypass_cert);
    _http.begin(_client, "https://" DEPLOII_HOST ":" STRINGIFY(DEPLOII_PORT) DEPLOII_HTTP_URL);
#else
    _http.begin("http://" DEPLOII_HOST ":" STRINGIFY(DEPLOII_PORT) DEPLOII_HTTP_URL);
#endif
    _http.addHeader("Authorization", boardID, false, false);

#endif // DEPLOII_PROTOCOL
  };

 private:
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
   WebSocketsClient _ws;
   bool _previousWebsocketConnectionStatus = false; 
   bool _websocketConnectionStatus = false;


#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
   HTTPClient _http;

#if DEPLOII_SSL
   WiFiClientSecure _client;
#endif

#endif  // DEPLOII_PROTOCOL

#else // OTHER MEDIUMS

#endif // DEPLOII_MEDIUM
};

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
void _wsEvent(WStype_t type, uint8_t* payload, size_t length) {
   DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Websocket event code: %u", type);
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
