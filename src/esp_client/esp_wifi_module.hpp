#ifndef WIFI_MODULE
#define WIFI_MODULE

#ifndef WIFI_LIB
#define WIFI_LIB
#include <ESP8266WiFi.h>
#endif

class WiFiManager {

  String wifiSSID;
  String password;

  int maxRetries;

public:

  WiFiManager(String _wifiSSID, String _password, int _maxRetries = 20) {
    this->wifiSSID = _wifiSSID;
    this->password = _password;
    this->maxRetries = _maxRetries;
  }

  void connect(bool withOutputLogs = true) {
    if (withOutputLogs) {
      Serial.print("Connecting to ");
      Serial.print(this->wifiSSID);
    }

    WiFi.begin(this->wifiSSID, this->password);

    int retries(0);

    while (WiFi.status() != WL_CONNECTED && retries < this->maxRetries) {
      if (withOutputLogs) { Serial.print("."); }

      retries++;

      delay(1000);
    }

    if (withOutputLogs) {
      if (this->isConnected()) {
        Serial.print("Successfully connected to ");
        Serial.println(this->wifiSSID);
        Serial.print("IP Address: ");
        Serial.println(this->getlocalIP());
      }
    } else {
      Serial.println("Error connect");
    }
  }

  bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
  }

  IPAddress getlocalIP() {
    return WiFi.localIP();
  }
};

#endif