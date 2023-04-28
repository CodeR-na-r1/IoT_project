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

#ifndef LED_PIN
#define LED_PIN 5  // GPIO5 -> D1
#endif

#define LED_NUM 12

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
  bool isDataColorsChanged = false;

  // for dynamic animation (loading)

  ColorRGB foregroundColor = ColorRGB(0, 0, 0);
  ColorRGB backgroundColor = ColorRGB(0, 0, 0);
  int nowIndex = 0;
  int length = 3;
  int step = 1;

public:

  LedManager() {
    this->dataColors.reserve(4);

    pinMode(LED_PIN, OUTPUT);

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

    if (!this->isDataColorsChanged) { return; }

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
    isDataColorsChanged = true;

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

    int removedCount = this->dataColors.size() - tempSize;
    if (removedCount != 0) { this->isDataColorsChanged = true; }

    Serial.print("Colors removed: ");
    Serial.println(removedCount);

    return;
  }

  void initLoadAnimation(ColorRGB _foregroundcolor, ColorRGB _backgroundColor, int _startIndex, int _length, int _step = 1) {
    this->foregroundColor = _foregroundcolor;
    this->backgroundColor = _backgroundColor;

    this->nowIndex = _startIndex;
    this->length = _length;
    this->step = _step;

    return;
  }

  void tickLoadAnimation() {
    int tempIndex(0);

    for (int i = 0; i < this->length; i++) {
      tempIndex = this->nowIndex - i;
      leds[tempIndex < 0 ? LED_NUM - (-tempIndex) : tempIndex].setRGB(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b);
      leds[(this->nowIndex + i) % LED_NUM].setRGB(this->foregroundColor.r, this->foregroundColor.g, this->foregroundColor.b);
    }

    this->nowIndex = (this->nowIndex + this->step) % LED_NUM;

    FastLED.show();

    return;
  }

  void showColor(ColorRGB _color) {
    FastLED.showColor(CRGB(_color.r, _color.g, _color.b));

    return;
  }
};

#endif