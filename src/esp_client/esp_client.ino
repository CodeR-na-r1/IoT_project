// Сначало заголовки библиотек

#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

// Затем заголовки модулей скетча

#include "esp_epprom_module.hpp"
#include "esp_server_module.hpp"

bool isError = false;

ConfigManager configManager;
String wifiSSID;
String password;

void setup() {
  Serial.begin(115200);

  if (configManager.hasConfig()) {
    Serial.println("\nConfig load");
    configManager.loadConfig();
    Serial.println("\nConfig loaded");
    wifiSSID = configManager.getWiFiSSID();
    password = configManager.getPassword();
    Serial.println("\nConfig getting");
    Serial.println(wifiSSID);
    Serial.println(password);

    //Try and Connect to the Network
    WiFi.begin(wifiSSID, password);
    Serial.print("Connecting to ");
    Serial.print(wifiSSID);

    int retries(0);

    //Wait for WiFi to connect for a maximum timeout of 20 seconds
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      Serial.print(".");
      retries++;
      delay(1000);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)  //WiFi has succesfully Connected
    {
      Serial.print("Successfully connected to ");
      Serial.println(wifiSSID);
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Error connect");
      isError = true;
    }
  } else {
    Serial.println("\nConfig not found");
    isError = true;
  }

  Serial.print("IsError = ");
  Serial.println(isError);

  if (isError) {
    bool ret = SERVER_NAMESPACE::start();
    Serial.print("SERVER_NAMESPACE::start() = ");
    Serial.println(ret);
    if (ret == 0) {
      Serial.println("Server started;");
      SERVER_NAMESPACE::setUserCallback(serverCallback);
      Serial.print("setUserCallback is set");
    } else {
      Serial.println("Server not started, error!");
    }
  }
}

void loop() {}

void serverCallback(String __wifi, String __password) {
  configManager.saveConfig(__wifi, __password);
  Serial.println("Esp wil be restarted");
  ESP.restart();
}