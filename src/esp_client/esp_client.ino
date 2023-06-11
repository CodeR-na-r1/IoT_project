// MAIN FILE HERE

#define FREQUENCY_CHECK_DATA_FROM_SERVER 200  // ms

#define FREQUENCY_UPDATE_LED_FOR_LOADING 300  // ms
#define FREQUENCY_UPDATE_LED_FOR_WORK 40      // ms
#define LED_NUM 21
#define LED_PIN 5                             // GPIO5 -> D1

#define JSON_BUFER_CAPACITY 256  // bytes

// Сначало заголовки библиотек

#include <ArduinoJson.h>
#include <Ticker.h>  //Ticker Library

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
Ticker ticker;

DynamicJsonDocument dynamicJsonDocument(JSON_BUFER_CAPACITY);

// Config data

String wifiSSID = "";
String password = "";

String host = "";
int port = 8000;

void setup() {
  Serial.begin(115200);

  // Initialize Ticker

  ledManager.initLoadAnimation(ColorRGB(255, 0, 0), ColorRGB(0, 0, 0), 0, 3, 1);
  ticker.attach_ms(FREQUENCY_UPDATE_LED_FOR_LOADING, interruptFunction);

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

        int retVal = webSocketsManager.connect(2, true);
        Serial.print("webSocketsManager.connect returns -> ");
        Serial.println(retVal);

        if (retVal == 0) {
          Serial.println("webSockets created!");

          if (WIFI_AP_NAMESPACE::start() == 0) {
            Serial.println("WIFI AP created");
            Serial.println("Esp beacon ready for work!");
            configManager.commitSuccessfulStartSystem();
            ticker.detach();
            ledManager.initLoadAnimation(ColorRGB(0, 255, 0), ColorRGB(204, 136, 153), 0, 3, 1);
            ticker.attach_ms(FREQUENCY_UPDATE_LED_FOR_WORK, interruptFunction);
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
    ticker.detach();

    SERVER_NAMESPACE::setData(wifiSSID, password, host, port);
    SERVER_NAMESPACE::setErrorDescription(errorDescription);

    int ret = SERVER_NAMESPACE::start();

    if (ret == 0) {
      Serial.println("Server started;");

      SERVER_NAMESPACE::setUserCallback(serverCallback);
      Serial.println("setUserCallback is set");
      
      ledManager.showColor(ColorRGB(255, 0, 0));
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

      if (!webSocketsManager.isConnected()) {
        Serial.print("WebSockets connection lost.\nReconecting...\n");
        int retVal = webSocketsManager.connect(3, true);
        if (retVal != 0) {
          Serial.print("WebSockets reconect failed.\nESP will be reset");
          delay(5000);
          ESP.reset();
        }
        Serial.println("WebSockets connection successfully restored");
      }

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

IRAM_ATTR void interruptFunction() {

  if (ledManager.hasUsers()) {

    ledManager.showUsers();

  } else {

    ledManager.tickLoadAnimation();
  }
}