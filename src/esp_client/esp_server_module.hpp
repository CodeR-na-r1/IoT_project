#ifndef SERVER_MODULE
#define SERVER_MODULE


#ifndef Arduino_LIB
#define Arduino_LIB
#include <Arduino.h>
#endif

#ifndef ESP8266WiFi_LIB
#define ESP8266WiFi_LIB
#include <ESP8266WiFi.h>
#endif

#ifndef AESPAsyncTCP_LIB
#define ESPAsyncTCP_LIB
#include <ESPAsyncTCP.h>
#endif

#ifndef ESPAsyncWebSrv_LIB
#define ESPAsyncWebSrv_LIB
#include <ESPAsyncWebSrv.h>
#endif

namespace SERVER_NAMESPACE {

AsyncWebServer server(80);

void (*userCallback)(String, String) = nullptr;

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

String html = "";

int max_connections = 8;
int channel = 1;
bool hidden = false;

int max_attempts = 20;

String wifiSSID = "";
String password = "01234567";

void setUserCallback(void (*_userCallback)(String, String)) {
  userCallback = _userCallback;

  return;
}

void initHTML() {
  html = "<!DOCTYPE html> <html>\n";

  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>Setup WIFI</title>\n";
  html += "</head>\n";

  html += "<body>\n";

  html += "<h1>Enter Wi-Fi data for esp connect to her:</h1>\n";

  html += "<form method=\"POST\" action=\"commitData\">\n";
  html += "<p>\n";
  html += "<label for=\"SSID\">SSID:</label>\n";
  html += "<input type=\"text\" name=\"SSID\" required>\n";
  html += "</p>\n";
  html += "<p>\n";
  html += "<label for=\"password\">Password:</label>\n";
  html += "<input type=\"password\" name=\"password\" required>\n";
  html += "</p>\n";
  html += "<button type=\"submit\">Commit</button>";
  html += "</form>\n";

  html += "</body>\n";
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void mainPage(AsyncWebServerRequest *request) {
  request->send(200, "text/html", html);
}

void processsingPostReq(AsyncWebServerRequest *request) {
  String _wifiSSID;
  String _password;

  if (request->hasParam("SSID", true)) {
    _wifiSSID = request->getParam("SSID", true)->value();
  } else {
    request->send(200, "text/plain", "Error with SSID field!");
    return;
  }

  if (request->hasParam("password", true)) {
    _password = request->getParam("password", true)->value();
  } else {
    request->send(200, "text/plain", "Error with password field!");
    return;
  }
  
  request->send(200, "text/plain", "Succesfull!");
  delay(2000);

  if (userCallback != nullptr) {
    userCallback(_wifiSSID, _password);
    Serial.println("_userCallback called");
  } else {
    Serial.println("_userCallback is nullpointer");
  }
}

int start() {
  Serial.println("Starting server...");

  Serial.print("WiFi.softAPConfig -> ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  
  wifiSSID = "ESP_" + WiFi.macAddress();
  
  Serial.print("WiFi.softAP -> ");
  Serial.println(WiFi.softAP(wifiSSID, password, channel, hidden, max_connections) ? "Ready" : "Failed!");

  initHTML();
  server.onNotFound(notFound);
  server.on("/", HTTP_GET, mainPage);
  server.on("/commitData", HTTP_POST, processsingPostReq);

  server.begin();
  
  Serial.println("Server started!");

  return 0;
}

}

#endif