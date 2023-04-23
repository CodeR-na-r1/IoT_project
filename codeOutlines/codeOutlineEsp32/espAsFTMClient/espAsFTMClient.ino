// https://www.adior.ru/index.php/robototekhnika/311-esp32-c3-ftm

#include "WiFi.h"
/*
   THIS FEATURE IS SUPPORTED ONLY BY ESP32-C3, ESP32-S2 AND ESP32-S3
*/
// Настройки для подключения к точке доступа Wi-Fi FTM
const char * WIFI_FTM_SSID = "ESP32_FTM"; // SSID of AP that has FTM Enabled
const char * WIFI_FTM_PASS = "";          // STA Password
// Натройки FTM
const uint8_t FTM_FRAME_COUNT = 16;    // Количество запрашиваемых кадров FTM (0 или 16, 24, 32, 64)
const uint16_t FTM_BURST_PERIOD = 5;   // Период между пакетами FTM в 100 миллисекунд (0 или 2-255)
xSemaphoreHandle ftmSemaphore;         // Семафор для подачи сигнала о получении отчета FTM
bool ftmSuccess = true;                // Статус полученного отчета FTM

// Обработчик отчетов FTM с вычислением расстояния до Wi-Fi FTM точки доступа
void onFtmReport(arduino_event_t *event) {
  const char * status_str[5] = {"SUCCESS", "UNSUPPORTED", "CONF_REJECTED", "NO_RESPONSE", "FAIL"};
  wifi_event_ftm_report_t * report = &event->event_info.wifi_ftm_report;
  ftmSuccess = report->status == FTM_STATUS_SUCCESS;  // обновить статус глобального отчета
  if (ftmSuccess) {
    Serial.printf("FTM Estimate: Distance: %.2f m, Return Time: %u ns\n", (float)report->dist_est / 100.0 - 27.4, report->rtt_est);
    free(report->ftm_report_data);   // Указатель на отчет FTM освобожден после использования
  } else {
    Serial.print("FTM Error: ");
    Serial.println(status_str[report->status]);
  }
  xSemaphoreGive(ftmSemaphore);      // Сигнал о том, что отчет получен
}

// Инициализация сеанса FTM и ожидание отчета FTM
bool getFtmReport() {
  if (!WiFi.initiateFTM(FTM_FRAME_COUNT, FTM_BURST_PERIOD)) {
    Serial.println("FTM Error: Initiate Session Failed");
    return false;
  }
  return xSemaphoreTake(ftmSemaphore, portMAX_DELAY) == pdPASS && ftmSuccess;     // Если отчёт получен, вернуть true
}

void setup() {
  Serial.begin(115200);
  ftmSemaphore = xSemaphoreCreateBinary();     // Создать двоичный семафор (ввод/вывод из любого потока/ISR)
  WiFi.onEvent(onFtmReport, ARDUINO_EVENT_WIFI_FTM_REPORT);             // Прослушивание событий отчета FTM

  Serial.println("Connecting to FTM Responder");
  WiFi.begin(WIFI_FTM_SSID, WIFI_FTM_PASS);    // Подключение к точке доступа с включенным FTM
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi Connected");

  while (getFtmReport());    // Запрашивать отчеты FTM до тех пор, пока один из них не вернёт false
}

void loop() {
  delay(10000);
}
