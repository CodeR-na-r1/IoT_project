// MAIN FILE HERE

// Сначало заголовки библиотек

#include <vector>

// Затем заголовки модулей скетча

#include "esp_epprom_module.hpp"
#include "esp_wifi_module.hpp"
#include "esp_server_module.hpp"
#include "esp_led_module.hpp"
#include "esp_webSockets_module.hpp"

String errorDescription = "";
bool isError = false;

ConfigManager configManager;
WebSocketsManager webSocketsManager("", "", 8000);
// std::vector<int> colorsData;

// Config data

String wifiSSID = "";
String password = "";

String host = "";
int port = 8000;

void setup() {
  Serial.begin(115200);

  // colorsData.reserve(10);

  // Check config and load

  if (configManager.hasConfig()) {
    Serial.println("\nConfig load");
    configManager.loadConfig();
    Serial.println("\nConfig loaded");

    wifiSSID = configManager.getWiFiSSID();
    password = configManager.getPassword();
    host = configManager.getHost();
    port = configManager.getPort();
    Serial.println("\nConfig getting");
    Serial.println(wifiSSID);
    Serial.println(password);
    Serial.println(host);
    Serial.println(port);

    // Check failures

    configManager.commitStartSystem();

    Serial.print("FailureCounter -> ");
    Serial.println(configManager.getFailureCounter());

    if (!configManager.isFailure()) {

      // Try and Connect to the Network
      WiFiManager wifiManager(wifiSSID, password, 20);
      wifiManager.connect(true);

      if (wifiManager.isConnected()) {
        Serial.println("Create websocket");
        webSocketsManager = WebSocketsManager(host, "/esp/reallyESP", port);  // ?path see on server

        int retVal = webSocketsManager.connect(true);
        Serial.print("webSocketsManager.connect returns -> ");
        Serial.println(retVal);

        webSocketsManager.sendData("1234");
        //delay(30000);

        configManager.commitSuccessfulStartSystem();
      } else {
        errorDescription = "Wifi no connected! Check data for remote wifi ap (SSID and password)";
        isError = true;
      }
    } else {
      Serial.println("Failure system!");
      errorDescription = "Failure system! Maybe the remote server is not available";
      isError = true;
    }
  } else {
    Serial.println("\nConfig not found");
    errorDescription = "Config not found! Save it now";
    isError = true;
  }

  Serial.print("IsError = ");
  Serial.println(isError);

  // If isError then server up and setting esp from Web-browser on http://192.168.4.22 (before connects to the access point of the esp -> password = 01234567)

  if (isError) {
    SERVER_NAMESPACE::setData(wifiSSID, password, host, port);
    SERVER_NAMESPACE::setErrorDescription(errorDescription);

    int ret = SERVER_NAMESPACE::start();

    if (ret == 0) {
      Serial.println("Server started;");

      SERVER_NAMESPACE::setUserCallback(serverCallback);
      Serial.println("setUserCallback is set");

      configManager.commitSuccessfulStartSystem();
    } else {
      Serial.print("Server started, with error, retValue -> ");
      Serial.println(ret);
    }
  }
}

void loop() {}

void serverCallback(String __wifi, String __password, String __host, int __port) {
  configManager.saveConfig(__wifi, __password, __host, __port);
  Serial.println("Esp wil be restarted");
  ESP.restart();
}