#include "deploii_handler.h"

DeploiiHandlerWiFiHTTP::DeploiiHandlerWiFiHTTP(bool debug)
#if defined(ESP32)
    : _http(), _debug(debug)
#elif defined(ARDUINO)
    : _debug(debug)
#else
    : _debug(debug)
#endif
{
}

DeploiiHandlerWiFiHTTP::~DeploiiHandlerWiFiHTTP() {
#if defined(ESP32)
#elif defined(ARDUINO)
#else
#endif
}

void DeploiiHandlerWiFiHTTP::connect(
    char* boardID,
    char* ssid,
    const char* pwd,
    const char* host,
    const int port,
    const char* url,
    bool ssl) {
   connectWiFi(ssid, pwd);
   if (ssl)
      _http.begin(host, port, url, buypass_cert);
   else
      _http.begin(host, port, url);

   _http.addHeader("Authorization", boardID, false, false);
}

void DeploiiHandlerWiFiHTTP::loop() {
}

void DeploiiHandlerWiFiHTTP::send(const uint8_t* data, size_t size) {
   _http.POST((uint8_t*)data, size);
}

#if defined(ESP32)

void DeploiiHandlerWiFiHTTP::connectWiFi(char* ssid, const char* pwd) {
   if (_debug) Serial.println("Connecting to WiFi");
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);
   while (WiFi.status() != WL_CONNECTED) {
      delay(Deploii_WIFI_RECONNECT_TIME);
      if (_debug) {
         Serial.println("Connecting to WiFi");
      }
   }
   if (_debug) {
      Serial.println("WiFi connected");
      Serial.println(WiFi.localIP());
   }
   if (_debug) Serial.println(WiFi.localIP());
}
#elif defined(ARDUINO)

void DeploiiHandlerWiFiHTTP::connectWiFi(char* ssid, const char* pwd) {
   while (WiFi.begin(ssid, pwd) != WL_CONNECTED) delay(Deploii_WIFI_RECONNECT_TIME);
}
#else
void DeploiiHandlerWiFiHTTP::connectWiFi(char* ssid, const char* pwd) {}
#endif