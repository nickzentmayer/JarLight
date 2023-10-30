#include <Arduino.h>
#include <FastLED.h>
#include "config.h"

// create leds in memory
CRGB leds[68];

void setup() {
  // init leds
  FastLED.addLeds<WS2811,DATAPIN,GRB>(leds, NUMLEDS);
  FastLED.show();

}

void loop() {
  //test anim
  static int pos = 0;
  static byte hue = 0;
  leds[pos++] = CHSV(hue++, 255, 255);
  FastLED.show();
  if(pos >= NUMLEDS) pos = 0;
  delay(10);
}
