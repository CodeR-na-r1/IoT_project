// Сначало заголовки библиотек

#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

// Затем заголовки модулей скетча

#include "esp_epprom_module.hpp"

ConfigManager configManager;
String wifiSSID;
String password;

void setup() {
  Serial.begin(115200);
  if (!configManager.hasConfig()) {
    configManager.saveConfig("BASE", "12345678PASSQ");
    Serial.println("\nConfig saved");
  } else {
    Serial.println("\nConfig load");
    configManager.loadConfig();
    Serial.println("\nConfig loaded");
    wifiSSID = configManager.getWiFiSSID();
    password = configManager.getPassword();
    Serial.println("\nConfig getting");
    Serial.println(wifiSSID);
    Serial.println(password);
  }
}

void loop() {}