#include <cstddef>
#ifndef SERVER_MODULE
#define SERVER_MODULE


#ifndef WIFI_MODULE
#include "esp_wifi_module.hpp"
#endif

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

void (*userCallback)(String, String, String, int) = nullptr;

String data_wifiSSID = "";
String data_password = "";
String data_host = "";
int data_port = 80;

String errorDescription = "";

String html = "";

int max_attempts = 20;

void setUserCallback(void (*_userCallback)(String, String, String, int)) {
  userCallback = _userCallback;

  return;
}

void setData(String _wifi, String _password, String _host, int _port) {
  data_wifiSSID = _wifi;
  data_password = _password;
  data_host = _host;
  data_port = _port;
}

void setErrorDescription(String _errorDescription) {
  errorDescription = _errorDescription;
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

  html += "<input type=\"text\" name=\"SSID\" value=\"";
  html += data_wifiSSID;
  html += "\" required>\n";

  html += "</p>\n";
  html += "<p>\n";
  html += "<label for=\"password\">Password:</label>\n";

  html += "<input type=\"text\" name=\"password\" value=\"";
  html += data_password;
  html += "\" required>\n";

  html += "</p>\n";
  html += "<p>\n";
  html += "<label for=\"host\">Host ip:</label>\n";

  html += "<input type=\"text\" name=\"host\" value=\"";
  html += data_host;
  html += "\" required>\n";

  html += "</p>\n";
  html += "<p>\n";
  html += "<label for=\"port\">Port:</label>\n";

  html += "<input type=\"text\" name=\"port\" value=\"";
  html += data_port;
  html += "\" required>\n";

  html += "</p>\n";
  html += "<button type=\"submit\">Commit</button>";
  html += "</form>\n";

  html += "<div>\n";
  if (errorDescription != "") {
    html += "<p>Last error -> ";
    html += errorDescription;
    html += "</p>";
  }
  html += "</div>\n";

  html += "</body>\n";
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void mainPage(AsyncWebServerRequest *request) {
  request->send(200, "text/html", html);
}

void processsingPostReq(AsyncWebServerRequest *request) {

  if (request->hasParam("SSID", true)) {
    data_wifiSSID = request->getParam("SSID", true)->value();
  } else {
    request->send(200, "text/plain", "Error with SSID field!");
    return;
  }

  if (request->hasParam("password", true)) {
    data_password = request->getParam("password", true)->value();
  } else {
    request->send(200, "text/plain", "Error with password field!");
    return;
  }

  if (request->hasParam("host", true)) {
    data_host = request->getParam("host", true)->value();
  } else {
    request->send(200, "text/plain", "Error with host field!");
    return;
  }

  if (request->hasParam("port", true)) {
    data_port = request->getParam("port", true)->value().toInt();
  } else {
    request->send(200, "text/plain", "Error with port field!");
    return;
  }

  request->send(200, "text/plain", "Succesfull!");
  delay(2000);

  if (userCallback != nullptr) {
    userCallback(data_wifiSSID, data_password, data_host, data_port);
    Serial.println("_userCallback called");
  } else {
    Serial.println("_userCallback is nullpointer");
  }
}

int start() {
  Serial.println("Starting server...");

  int retValue = WIFI_AP_NAMESPACE::start();

  initHTML();
  server.onNotFound(notFound);
  server.on("/", HTTP_GET, mainPage);
  server.on("/commitData", HTTP_POST, processsingPostReq);

  server.begin();

  Serial.println("Server started!");

  return retValue;
}

}

#endif