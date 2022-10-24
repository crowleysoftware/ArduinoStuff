#include <FastLED.h>

const int LED_PIN = 7;
const int NUM_LEDS = 8;
CRGB leds[NUM_LEDS];
const int maxBrightness = 128;
unsigned long delayDuration = 3000;

CRGB bgColor;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial Starting....");
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(maxBrightness);
}

void loop() {

  switch (colorIdx)
  {
    case 0:
      bgColor = CRGB::Green;
      colorIdx = 1;
      break;
    case 1:
      bgColor = CRGB::Blue;
      colorIdx = 2;
      break;
    case 2:
      bgColor = CRGB::Purple;
      colorIdx = 3;
      break;
    case 3:
      bgColor = CRGB::Red;
      colorIdx = 4;
      break;
    case 4:
      bgColor = CRGB::Orange;
      colorIdx = 0;
      break;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = bgColor;
  }

  for (int i = 0; i < maxBrightness; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }

  FastLED.show();

  delay(delayDuration);

  for (int i = 0; i < maxBrightness; i++) {
    FastLED.setBrightness(maxBrightness - i);
    FastLED.show();
    delay(10);
  }
}
