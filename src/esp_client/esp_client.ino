// Сначало заголовки библиотек

#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

// Затем заголовки модулей скетча

#include "esp_epprom_module.hpp"
#include "esp_wifi_module.hpp"
#include "esp_server_module.hpp"
#include "esp_led_module.hpp"
#include "esp_webSockets_module.hpp"

bool isError = false;

ConfigManager configManager;
String wifiSSID;
String password;

void setup() {
  Serial.begin(115200);

  // Check config and load

  if (configManager.hasConfig()) {
    Serial.println("\nConfig load");
    configManager.loadConfig();
    Serial.println("\nConfig loaded");

    wifiSSID = configManager.getWiFiSSID();
    password = configManager.getPassword();
    Serial.println("\nConfig getting");
    Serial.println(wifiSSID);
    Serial.println(password);

    // Try and Connect to the Network
    WiFiManager wifiManager(wifiSSID, password, 20);
    wifiManager.connect(true);

    if (!wifiManager.isConnected()) {
      isError = true;
    }
  } else {
    Serial.println("\nConfig not found");
    isError = true;
  }

  Serial.print("IsError = ");
  Serial.println(isError);

  // If isError then server up and setting esp from Web-browser on http://192.168.4.22 (before connects to the access point of the esp -> password = 01234567)

  if (isError) {
    int ret = SERVER_NAMESPACE::start();

    if (ret == 0) {
      Serial.println("Server started;");
      
      SERVER_NAMESPACE::setUserCallback(serverCallback);
      Serial.print("setUserCallback is set");
    } else {
      Serial.print("Server started, with error, retValue -> ");
      Serial.println(ret);
    }
  }
}

void loop() {}

void serverCallback(String __wifi, String __password) {
  configManager.saveConfig(__wifi, __password);
  Serial.println("Esp wil be restarted");
  ESP.restart();
}