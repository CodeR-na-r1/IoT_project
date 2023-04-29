# Итоговый проект по курсу IoT

## Как запустить проект

### > Server python FastApi

desc

### > Esp beacon

desc

### > Android client

desc

## Компоненты, библиотеки (список, способ установки)

desc

## Схемы работы программных частей:

### > Server python FastApi

desc

### > Android client

desc

### > Esp beacon

desc

## Основные компоненты:

1. Плата мк esp8266
2. Адресная светодиодная лента, плотность не менее 60 светодиодов на метр, длина зависит от размера корпуса
3. Аккумулятор 18350 *2
4. Модуль для зарядки аккумулятора
5. Кнопка управления питанием (вкл/выкл)

*Печатные компоненты:*

1. Корпус для всех компонент

*Возможные компоненты:*

1. Светодиод - индикатор работы системы - возможно без него, индикатором может быть и светодиодная лента, подмигивающая с некоторым периодом

*Программные компоненты:*

1. Arduino IDE
    - Web sockets for esp8266 ([morrissinger/ESP8266-Websocket: A websocket library for the ESP-8266. (github.com)](https://github.com/morrissinger/ESP8266-Websocket))
    - FastLed для управления адресной лентой ([https://github.com/FastLED/FastLED.git](https://github.com/FastLED/FastLED.git))
2. VS Code
    - FastAPI для сервера - взаимодействие с мобильным app клиентом и микроконтроллером
3. Android Studio
    - Indoor Service - для обнаружения esp по wi-fi, а также замера rsi. ([https://github.com/CodeR-na-r1/IndoorService.git](https://github.com/CodeR-na-r1/IndoorService.git))