#include <FastLED.h>  // https://kit.alexgyver.ru/tutorials/address-strip/

#define LED_PIN 2
#define LED_NUM 150

CRGB leds[LED_NUM];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(50);

  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(i * 255 / LED_NUM);
  }
  FastLED.show();
}

void loop() {
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setRGB(255, 0, 0);
  }
  FastLED.show();
  delay(1000);

  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setRGB(0, 0, 0);
  }
  FastLED.show();
  delay(1000);

  for (int i = 0; i < LED_NUM; i++) {
    leds[i] = 0x6F2DA8;
    delay(8);
    FastLED.show();
  }
  delay(1000);
}