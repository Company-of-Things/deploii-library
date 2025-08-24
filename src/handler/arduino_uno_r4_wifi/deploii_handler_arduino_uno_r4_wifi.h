/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_debug.h"

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_WIFI
#include "WiFiS3.h"

#endif // DEPLOII_MEDIUM

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
#include <WebSocketsClient.h> 
void _wsEvent(WStype_t type, uint8_t* payload, size_t length);

#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
#if DEPLOII_SSL
#include "WiFiSSLClient.h"
#endif // DEPLOII_SSL

#endif // DEPLOII_PROTOCOL

void _defaultCallback(uint8_t*data, size_t size){};
void (*_dataCallback)(uint8_t* data, size_t size)= &_defaultCallback;

/*************************************************************************************/

class DeploiiHandler
{
public:

/*************************************************************************************/

  DeploiiHandler()
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
      : _ws()
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    : _client()
#endif // DEPLOII_PROTOCOL
  {
  };

/*************************************************************************************/

  ~DeploiiHandler()
  {
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    _ws.~WebSocketsClient();
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP

#if DEPLOII_SSL
    _client.~WiFiSSLClient();
#else
    _client.~WiFiClient();
#endif // DEPLOII_SSL

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
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Sending WEBSOCKET data of size 0x%x", size);

#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP

#if DEPLOII_SSL
    if(_client.connect(DEPLOII_HOST, DEPLOII_PORT))
#else 
    if(_client.connect(DEPLOII_HOST, DEPLOII_PORT_NO_SSL))
#endif // DEPLOII_SSL
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Sending HTTP data of size 0x%x", size);
      _client.println("POST " DEPLOII_HTTP_URL " HTTP/1.1");
      _client.println("Host: " DEPLOII_HOST);
      _client.print("Authorization: ");
      _client.println(_boardID);
      _client.println("Content-Type: application/octet-stream");
      _client.print("Content-Length: ");
      _client.println((int)size);
      _client.println();
      _client.write(data, size);
    }else{
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "Failed to connect to HTTP server");
    }
    _client.stop();
#endif // DEPLOII_PROTOCOL
  };
   void setDataCallback(void (*cb)(uint8_t* data, size_t size)) {
      _dataCallback = cb;
   }

/*************************************************************************************/

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_WIFI
  void connect(
      char *boardID,
      const char *ssid,
      const char *pwd)
  {
    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Attempting to connect to WiFi");
      delay(DEPLOII_WIFI_RECONNECT_TIME);
    }

    IPAddress ip = WiFi.localIP();
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "Connected to WiFi with IP %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
    static char authHeader[60];
    sprintf(authHeader, "%s%s", "Authorization: ", boardID);
    _ws.setExtraHeaders(authHeader);
    _ws.onEvent(_wsEvent);

    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Attempting to connect to Websocket server");
#if DEPLOII_SSL
#pragma message("Websockets with SSL is currently not supported for this device, please set the DEPLOII_SSL macro to false.")
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "Websockets with SSL is currently not supported for this device, please set the DEPLOII_SSL macro to false.");
    while(true);
#else
    _ws.begin(DEPLOII_HOST, DEPLOII_PORT_NO_SSL, DEPLOII_WS_URL);
#endif // DEPLOII_SSL

#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    _boardID = boardID;
#if DEPLOII_SSL
    _client.setCACert(buypass_cert);
#endif

#endif // DEPLOII_PROTOCOL

    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "Connected to Deploii server");
  };

 private:
#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
   WebSocketsClient _ws;


#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
    char* _boardID;
#if DEPLOII_SSL
    WiFiSSLClient _client;
#else
    WiFiClient _client;
#endif // DEPLOII_SSL

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

#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/