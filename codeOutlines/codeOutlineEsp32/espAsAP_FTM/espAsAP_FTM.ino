#include "WiFi.h"
const char * WIFI_FTM_SSID = "ESP32_FTM";
const char * WIFI_FTM_PASS = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Starting SoftAP with FTM Responder support");
  WiFi.softAP(WIFI_FTM_SSID, WIFI_FTM_PASS, 1, 0, 4, true);
}

void loop() {
  delay(1000);
}
