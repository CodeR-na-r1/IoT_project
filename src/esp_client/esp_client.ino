// MAIN FILE HERE

#define FREQUENCY_CHECK_DATA_FROM_SERVER 1000  // ms

#define JSON_BUFER_CAPACITY 256  // bytes

// Сначало заголовки библиотек

#include <ArduinoJson.h>

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
LedManager ledManager;

DynamicJsonDocument dynamicJsonDocument(JSON_BUFER_CAPACITY);

// Config data

String wifiSSID = "";
String password = "";

String host = "";
int port = 8000;

void setup() {
  Serial.begin(115200);

  // Check config and load

  if (configManager.hasConfig()) {
    Serial.println("\nConfig load");
    configManager.loadConfig();
    Serial.println("Config loaded");

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
        webSocketsManager = WebSocketsManager(host, "/esp/" + WIFI_AP_NAMESPACE::wifiSSID, port);  // ?path see on server

        int retVal = webSocketsManager.connect(true);
        Serial.print("webSocketsManager.connect returns -> ");
        Serial.println(retVal);

        if (retVal == 0) {
          Serial.println("webSockets created!");

          if (WIFI_AP_NAMESPACE::start() == 0) {
            Serial.println("WIFI AP created");
            Serial.println("Esp beacon ready for work!");
            configManager.commitSuccessfulStartSystem();
          } else {
            Serial.println("Wifi AP created error!");
            errorDescription = "Wifi AP created error";
            isError = true;
          }
        } else {
          Serial.println("WebSockets created error!");
          errorDescription = "WebSockets created error";
          isError = true;
        }
      } else {
        Serial.println("Wifi no connected! Check data for remote wifi ap (SSID and password)");
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
    } else {
      Serial.print("Server started, with error, retValue -> ");
      Serial.println(ret);
    }
  }

  //while (isError) { delay(1000); }
}

String jData;
ArduinoJson::DeserializationError error;
auto timer = millis();
String tempMode = "";
ColorRGB tempColor(0, 0, 0);

void loop() {

  if (!isError) {
    if (millis() - timer > FREQUENCY_CHECK_DATA_FROM_SERVER) {
      timer = millis();
      Serial.print("Data availability check...");

      if (webSocketsManager.hasData()) {

        Serial.print((" has data -> "));

        jData = webSocketsManager.getData();
        Serial.println(jData);

        // Parse JSON object
        error = deserializeJson(dynamicJsonDocument, jData);

        if (!error) {

          tempMode = dynamicJsonDocument["mode"].as<String>();
          tempColor.r = dynamicJsonDocument["color"]["r"].as<String>().toInt();
          tempColor.g = dynamicJsonDocument["color"]["g"].as<String>().toInt();
          tempColor.b = dynamicJsonDocument["color"]["b"].as<String>().toInt();

          Serial.println(tempMode);
          Serial.println(tempColor.r);
          Serial.println(tempColor.g);
          Serial.println(tempColor.b);

          if (tempMode == "1") {
            ledManager.addColor(tempColor);
          } else if (tempMode == "2") {
            ledManager.removeColor(tempColor);
          }
        } else {
          Serial.println("Parsing failed!");
        }

        webSocketsManager.clearData();
      } else {
        Serial.println(" no data!");
      }
    }
  }
}

void serverCallback(String __wifi, String __password, String __host, int __port) {
  configManager.saveConfig(__wifi, __password, __host, __port);
  configManager.commitSuccessfulStartSystem();
  Serial.println("Esp wil be restarted");
  ESP.restart();
}