#ifndef LED_MODULE
#define LED_MODULE

#ifndef FastLED_LIB
#define FastLED_LIB
#include <FastLED.h>  // https://kit.alexgyver.ru/tutorials/address-strip/
#endif

#ifndef vector_LIB
#define vector_LIB
#include <vector>
#endif

#define LED_PIN 2
#define LED_NUM 30

CRGB leds[LED_NUM];

class ColorRGB {

public:

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  ColorRGB(uint8_t _r, uint8_t _g, uint8_t _b) {
    r = _r;
    g = _g;
    b = _b;
  }

  bool operator==(const ColorRGB& other) {
    return (this->r == other.r && this->g == other.g && this->b == other.b);
  }
};

class LedManager {

  std::vector<ColorRGB> dataColors;

public:

  LedManager() {
    this->dataColors.reserve(4);

    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);

    FastLED.setBrightness(255);

    FastLED.clearData();  // false

    FastLED.show();
  }

  void setBrightness(uint8_t scale) {
    FastLED.setBrightness(scale);
  }

  CFastLED getFastLED() {
    return FastLED;
  }

  bool hasUsers() {
    return this->dataColors.size() > 0;
  }

  void showUsers() {
    int step = LED_NUM / this->dataColors.size();

    for (int i = 0; i < this->dataColors.size(); ++i) {
      fillRangeColor(i * step, (i + 1) * step, this->dataColors[i]);
    }

    FastLED.show();

    return;
  }

  void fillRangeColor(int beginIndex, int endIndex, ColorRGB color) {
    for (int i = beginIndex; i < endIndex; ++i) {
      leds[i].setRGB(color.r, color.g, color.b);
    }

    return;
  }

  void shiftForward(int step) {


    return;
  }

  void addColor(uint8_t _r, uint8_t _g, uint8_t _b) {

    ColorRGB _newData(_r, _g, _b);

    this->addColor(_newData);

    return;
  }

  void addColor(ColorRGB newData) {

    for (int i(0); i < this->dataColors.size(); ++i) {
      if (this->dataColors[i] == newData) {
        Serial.println("Color ignored");
        return;
      }
    }

    this->dataColors.push_back(newData);
    Serial.println("Color added");

    return;
  }

  void removeColor(uint8_t _r, uint8_t _g, uint8_t _b) {

    ColorRGB _tempItem(_r, _g, _b);

    this->removeColor(_tempItem);

    return;
  }

  void removeColor(ColorRGB tempItem) {

    int tempSize = this->dataColors.size();

    this->dataColors.erase(std::remove(this->dataColors.begin(), this->dataColors.end(), tempItem), this->dataColors.end());

    Serial.print("Colors removed: ");
    Serial.println(this->dataColors.size() - tempSize);

    return;
  }
};

#endif