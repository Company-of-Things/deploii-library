/*************************************************************************************/

/*
 * esp32 + wifi + http
 */

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_debug.h"

#include <HTTPClient.h>
#include <WiFi.h>

void _defaultCallback(uint8_t *data, size_t size) {};
void (*_dataCallback)(uint8_t *data, size_t size) = &_defaultCallback;
int _connectionFailedCountWIFI = 0;

#if DEPLOII_SSL
#include "deploii_certs.h"
#include <WiFiClientSecure.h>
#endif

/*************************************************************************************/

class DeploiiHandler
{
public:
  /*************************************************************************************/

  DeploiiHandler(char *boardID)
      : _http(),
        _boardID(boardID)
#if DEPLOII_SSL
        ,
        _client()
#endif
  {
  };

  /*************************************************************************************/

  ~DeploiiHandler()
  {
    _http.~HTTPClient();
  };

  /*************************************************************************************/

  void loop()
  {
    // poll data from server
    if ((millis() - previousPollTimeHTTP) > DEPLOII_HTTP_POLL_RATE && (WiFi.status() == WL_CONNECTED))
    {
      _httpPollData();
      previousPollTimeHTTP = millis();
    }

    if (_connectionFailedCountWIFI >= DEPLOII_WIFI_CONNECTION_FAIL_LIMIT)
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WiFi] Connection fail limit exeeded, attempting to recover");
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[WiFi] Resetting ESP core...");

      ESP.restart();
    }
  };

  /*************************************************************************************/

  void send(const uint8_t *data, size_t size)
  {
#if DEPLOII_SSL
    _http.begin(_client, "https://" DEPLOII_HOST ":" STRINGIFY(DEPLOII_PORT) DEPLOII_HTTP_URL);
#else
    _http.begin("http://" DEPLOII_HOST ":" STRINGIFY(DEPLOII_PORT_NO_SSL) DEPLOII_HTTP_URL);
#endif

    _http.addHeader("Authorization", _boardID, false, false);

    _http.POST((uint8_t *)data, size);
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[HTTP] Attempting to send data of size 0x%zx", size);

    _http.end();
  };

  /*************************************************************************************/

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

    WiFi.mode(WIFI_STA);
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

#if DEPLOII_SSL
    _client.setCACert(deploii_cert);
#endif
  };

  /*************************************************************************************/

private:
  HTTPClient _http;
  char *_boardID;
  unsigned long previousPollTimeHTTP = 0;

#if DEPLOII_SSL
  WiFiClientSecure _client;
#endif

  void _httpPollData()
  {
#if DEPLOII_SSL
    _http.begin(_client, "https://" DEPLOII_HOST ":" STRINGIFY(DEPLOII_PORT) DEPLOII_HTTP_URL);
#else
    _http.begin("http://" DEPLOII_HOST ":" STRINGIFY(DEPLOII_PORT_NO_SSL) DEPLOII_HTTP_URL);
#endif
    _http.addHeader("Authorization", _boardID, false, false);

    int httpCode = _http.GET();
    if (httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK)
      {
        String payload = _http.getString();
        _dataCallback((uint8_t *)payload.c_str(), (size_t)payload.length());
      }
    }
    else
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[HTTP] Error fetching data");
    }
    _http.end();
  }
}; // DeploiiHandler

/*************************************************************************************/

#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
