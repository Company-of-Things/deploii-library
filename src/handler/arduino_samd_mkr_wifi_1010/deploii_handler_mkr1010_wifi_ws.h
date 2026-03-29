/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_debug.h"

#include "WiFiNINA.h"
#include <WebSocketsClient.h>

void _wsEvent(WStype_t type, uint8_t *payload, size_t length);
void _defaultCallback(uint8_t *data, size_t size) {};
void (*_dataCallback)(uint8_t *data, size_t size) = &_defaultCallback;

/*************************************************************************************/

class DeploiiHandler
{
public:
  /*************************************************************************************/

  DeploiiHandler(char *boardID)
      : _ws(),
        _boardID(boardID) {};

  /*************************************************************************************/

  ~DeploiiHandler()
  {
    _ws.~WebSocketsClient();
  };

  /*************************************************************************************/

  void loop()
  {
    _ws.loop();
  };

  /*************************************************************************************/

  void send(const uint8_t *data, size_t size)
  {
    _ws.sendBIN(data, size);
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[WS] Attempting to send data of size 0x%x", size);
  };

  void setDataCallback(void (*cb)(uint8_t *data, size_t size))
  {
    _dataCallback = cb;
  }

  /*************************************************************************************/

  void connect(
      const char *ssid,
      const char *pwd)
  {
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WiFi] Attempting to connect to WiFi...");

    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[WiFi] Attempting to connect to WiFi...");
      delay(DEPLOII_WIFI_RECONNECT_TIME);
    }

    // Wait until IP address is assigned (not 0.0.0.0)
    while (WiFi.localIP()[0] == 0)
      ;

    IPAddress ip = WiFi.localIP();
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WiFi] Connected");
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[WiFi] IP address: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    static char authHeader[60];
    sprintf(authHeader, "%s%s", "Authorization: ", _boardID);
    _ws.setExtraHeaders(authHeader);
    _ws.onEvent(_wsEvent);

#if DEPLOII_SSL
#pragma message("Websockets with SSL is currently not supported for this device, please set the DEPLOII_SSL macro to false.")
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WS] Websockets with SSL is currently not supported for this device, please set the DEPLOII_SSL macro to false.");
    while (true)
      ;
#else
    _ws.begin(DEPLOII_HOST, DEPLOII_PORT_NO_SSL, DEPLOII_WS_URL);
#endif // DEPLOII_SSL
  };

  /*************************************************************************************/

private:
  char *_boardID;
  WebSocketsClient _ws;
};

/*************************************************************************************/

void _wsEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_CONNECTED:
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WS] Connected to Deploii");
    break;

  case WStype_DISCONNECTED:
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WS] Disconnected from Deploii");
    break;

  case WStype_BIN:
    _dataCallback(payload, length);
    break;

  case WStype_ERROR:
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[WS] Error with payload: %x, length: %d", payload, length);
    break;

  case WStype_PING:
  case WStype_PONG:
    break;

  default:
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[WS] Unhandled event code: %u", type);
    break;
  }
}

/*************************************************************************************/

#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
