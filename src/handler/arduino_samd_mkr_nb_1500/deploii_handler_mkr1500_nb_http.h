/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_debug.h"

#include <MKRNB.h>

void _defaultCallback(uint8_t *data, size_t size) {};
void (*_dataCallback)(uint8_t *data, size_t size) = &_defaultCallback;

/*************************************************************************************/

class DeploiiHandler
{
public:
  /*************************************************************************************/

  DeploiiHandler(char *boardID)
      : _boardID(boardID),
        _gprs(),
        _nbAccess()
#if DEPLOII_SSL
        ,
        _client(NB_ROOT_CERTS_LOCAL, NB_NUM_ROOT_CERTS_LOCAL)
#else
        ,
        _client()
#endif
  {
  };

  /*************************************************************************************/

  ~DeploiiHandler()
  {
#if DEPLOII_SSL
    _client.~NBSSLClient();
#else
    _client.~NBClient();
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
      _client.println("Connection: Close");
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
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[HTTP] Failed to connect to server");
    }
    int t0 = millis();
    while (!_client.available())
    { // Wait for response
      if (millis() - t0 > DEPLOII_HTTP_RESPONSE_TIMEOUT)
      {
        DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[HTTP] Timeout waiting for HTTP response");
        break;
      }
    }
    while (_client.available())
    { // Read response
      _client.read();
    }
    safeClose();
  };

  void setDataCallback(void (*cb)(uint8_t *data, size_t size))
  {
    _dataCallback = cb;
  }

  /*************************************************************************************/

  void connect(const char *pin)
  {
    _pin = pin;
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[Narrowband] Attempting to connect to Narrowband...");
    while (!((_nbAccess.begin(pin) == NB_READY) && (_gprs.attachGPRS() == GPRS_READY)))
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[Narrowband] Connection failed. Retrying...");
      delay(1000);
    }
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[Narrowband] Connected to Narrowband.");
  };

private:
  char *_boardID;
  const char *_pin;
  GPRS _gprs;
  NB _nbAccess;
  unsigned long previousPollTimeHTTP = 0;

#if DEPLOII_SSL
  NBSSLClient _client;
#else
  NBClient _client;
#endif // DEPLOII_SSL

  /*************************************************************************************/

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
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[HTTP] Error fetching HTTP data");
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
      safeClose();
      _dataCallback(payload, contentLength);
    }
    else
    {
      while (_client.available())
        _client.read();
      safeClose();
    }
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[HTTP] GET status %u", status);
  }

  void safeClose()
  {
    int t0 = millis();
    _client.stop();
    if (millis() - t0 > 10000)
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[NARROWBAND] Modem timeout, resetting.");
      MODEM.hardReset();
      connect(_pin);
    }
  }
};

/*************************************************************************************/

#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
