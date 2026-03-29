/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_debug.h"

#include "WiFiS3.h"
#include <WebSocketsClient.h>

void _wsEvent(WStype_t type, uint8_t *payload, size_t length);

void _defaultCallback(uint8_t *data, size_t size) {};
void (*_dataCallback)(uint8_t *data, size_t size) = &_defaultCallback;
int _connectionFailedCountWIFI = 0;

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

    if (_connectionFailedCountWIFI >= DEPLOII_WIFI_CONNECTION_FAIL_LIMIT)
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WiFi] Connection fail limit exeeded, attempting to recover");
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WiFi] Resetting ESP modem...");

      // tell ESP to reset
      std::string res = "";
      modem.write_nowait(std::string(PROMPT(_RESET)), res, "%s", CMD(_RESET));

      // wait for ESP to come back
      while (!modem.write(std::string(PROMPT(_SOFTRESETWIFI)), res, "%s", CMD(_SOFTRESETWIFI)))
        ;

      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WiFi] Resetting ARM core...");

      // system reset of the Renesas ARM core
      NVIC_SystemReset();
    }
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

    _connectionFailedCountWIFI = 0;

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

    _ssid = (char *)ssid;
    _pwd = (char *)pwd;
  };

  /*************************************************************************************/

private:
  char *_boardID;
  char *_ssid;
  char *_pwd;
  WebSocketsClient _ws;

}; // DeploiiHandler

/*************************************************************************************/

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
void _wsEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_CONNECTED:
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WS] Connected to Deploii");
    _connectionFailedCountWIFI = 0;
    break;

  case WStype_DISCONNECTED:
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WS] Disconnected from Deploii");
    _connectionFailedCountWIFI++;
    break;

  case WStype_BIN:
    _dataCallback(payload, length);
    _connectionFailedCountWIFI = 0;
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
#endif

/*************************************************************************************/

#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
