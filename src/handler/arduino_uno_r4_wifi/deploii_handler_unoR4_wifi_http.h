/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_debug.h"

#include "WiFiS3.h"

#if DEPLOII_SSL
#include "WiFiSSLClient.h"
#endif // DEPLOII_SSL

void _defaultCallback(uint8_t *data, size_t size) {};
void (*_dataCallback)(uint8_t *data, size_t size) = &_defaultCallback;
int _connectionFailedCountWIFI = 0;

/*************************************************************************************/

class DeploiiHandler
{
public:
  /*************************************************************************************/

  DeploiiHandler(char *boardID)
      : _client(),
        _boardID(boardID) {};

  /*************************************************************************************/

  ~DeploiiHandler()
  {
#if DEPLOII_SSL
    _client.~WiFiSSLClient();
#else
    _client.~WiFiClient();
#endif // DEPLOII_SSL
  };

  /*************************************************************************************/

  void loop()
  {
    if ((millis() - previousPollTimeHTTP) > DEPLOII_HTTP_POLL_RATE)
    {
      _httpPollData();
      previousPollTimeHTTP = millis();
    }

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
#if DEPLOII_SSL
    if (_client.connect(DEPLOII_HOST, DEPLOII_PORT))
#else
    if (_client.connect(DEPLOII_HOST, DEPLOII_PORT_NO_SSL))
#endif // DEPLOII_SSL
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[HTTP] Attempting to send data of size 0x%x", size);
      _client.println("POST " DEPLOII_HTTP_URL " HTTP/1.1");
      _client.println("Host: " DEPLOII_HOST);
      _client.print("Authorization: ");
      _client.println(_boardID);
      _client.println("Content-Type: application/octet-stream");
      _client.print("Content-Length: ");
      _client.println((int)size);
      _client.println();
      _client.write(data, size);
    }
    else
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[HTTP] Failed to connect to server on POST");
      _connectionFailedCountWIFI++;
      return;
    }

    int t0 = millis();
    while (!_client.available())
    { // Wait for response
      if (millis() - t0 > DEPLOII_HTTP_RESPONSE_TIMEOUT)
      {
        DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[HTTP] Timeout waiting for HTTP response");
        return;
      }
    }

    while (_client.available())
    { // Read response
      _client.read();
    }
    _client.stop();
    _connectionFailedCountWIFI = 0;
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

#if DEPLOII_SSL
    _client.setCACert(deploii_cert);
#endif

    _ssid = (char *)ssid;
    _pwd = (char *)pwd;
  };

  /*************************************************************************************/

private:
  char *_boardID;
  char *_ssid;
  char *_pwd;
  unsigned long previousPollTimeHTTP = 0;

#if DEPLOII_SSL
  WiFiSSLClient _client;
#else
  WiFiClient _client;
#endif // DEPLOII_SSL

  void _httpPollData()
  {
#if DEPLOII_SSL
    if (_client.connect(DEPLOII_HOST, DEPLOII_PORT))
#else
    if (_client.connect(DEPLOII_HOST, DEPLOII_PORT_NO_SSL))
#endif // DEPLOII_SSL
    {
      _client.println("GET " DEPLOII_HTTP_URL " HTTP/1.1");
      _client.println("Host: " DEPLOII_HOST);
      _client.print("Authorization: ");
      _client.println(_boardID);
      _client.println();
    }
    else
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[HTTP] Failed to connect to server on GET");
      _connectionFailedCountWIFI++;
      return;
    }

    int t0 = millis();
    while (!_client.available())
    { // Wait for response
      if (millis() - t0 > DEPLOII_HTTP_RESPONSE_TIMEOUT)
      {
        DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[HTTP] Timeout waiting for HTTP response");
        return;
      }
    }

    int status;
    while (_client.available())
    {
      if (_client.read() == ' ')
      { // Read until status code
        String temp = "";
        while (_client.available())
        { // Read status
          char c = _client.read();
          if (c == '\n' || c == '\r')
            break;
          temp += c;
        }
        status = temp.toInt();
        break;
      }
    }

    size_t contentLength;
    char header[] = "Content-Length: ";
    int i = 0;
    while (_client.available())
    { // Read Content-length header
      char c = _client.read();
      if (header[i] == c)
        i++;
      else
        i = 0;
      if (header[i] == '\0')
      {
        String temp = "";
        while (_client.available())
        {
          char c = _client.read();
          if (c == '\n' || c == '\r')
            break;
          temp += c;
        }
        contentLength = temp.toInt();
        break;
      }
    }

    char payloadStart[] = "\r\n\r\n";
    i = 0;
    while (_client.available())
    { // Find start of payload
      char c = _client.read();
      if (payloadStart[i] == c)
        i++;
      else
        i = 0;
      if (payloadStart[i] == '\0')
        break;
    }

    if (status == 200 && contentLength > 0)
    { // Read payload
      uint8_t *payload = (uint8_t *)malloc(contentLength);
      _client.read(payload, contentLength);
      _client.stop();
      _dataCallback(payload, contentLength);
    }
    else
    {
      while (_client.available())
        _client.read();
      _client.stop();
    }

    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[HTTP] GET status %u", status);
    _connectionFailedCountWIFI = 0;
  }
};

/*************************************************************************************/

#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
