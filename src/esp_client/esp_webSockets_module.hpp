#ifndef WEBSOCKETS_MODULE
#define WEBSOCKETS_MODULE

#ifndef WEBSOCKETS_LIB
#define WEBSOCKETS_LIB
#include <WebSocketClient.h>
#endif

// definitions for libraries

#ifndef WIFI_MODULE
#include "esp_wifi_module.hpp"
#endif

class WebSocketsManager {

  WebSocketClient webSocketClient;
  WiFiClient client;

  String host;
  String path;
  int port;

  String data;

public:
  WebSocketsManager(String _host, String _path, int _port = 80) {
    this->host = _host;
    this->path = _path;
    this->port = _port;
  }

  int connect(bool withOutputLogs = true) {
    if (!WiFiManager::isConnected()) {
      if (withOutputLogs) { Serial.println("WiFi not connected!"); }
      return 1;
    }

    if (!client.connect(this->host, this->port)) {
      if (withOutputLogs) { Serial.println("WiFiClient connection failed!"); }
      return 2;
    }

    if (withOutputLogs) { Serial.println("WiFiClient connected"); }

    // Handshake with the server

    if (withOutputLogs) {
      Serial.print("webSocketClient.path = ");
      Serial.println(this->path.begin());
    }

    webSocketClient.path = this->path.begin();
    webSocketClient.host = this->host.begin();

    if (webSocketClient.handshake(this->client)) {
      if (withOutputLogs) { Serial.println("Handshake successful!"); }
    } else {
      if (withOutputLogs) { Serial.println("Handshake failed!"); }
      return 3;
    }

    return 0;
  }

  bool isConnected() {
    return client.connected();
  }

  bool hasData() {
    this->webSocketClient.getData(this->data);
    return this->data.length() > 0;
  }

  String getData() {
    return this->data;
  }

  void sendData(String data) {
    this->webSocketClient.sendData(data);

    return;
  }
};

#endif