#include "animations.h"

Animator::Animator(Adafruit_NeoPixel* s) {
  strip = s;
}

void Animator::cycle() {
    static uint16_t fph;
    uint16_t hue = fph;
    for(int i = 0; i < strip->numPixels(); i++) {
      
      strip->setPixelColor(i, strip->ColorHSV(hue, 255, 255));
      hue += UINT16_MAX/(strip->numPixels()*2);
    }
    fph += UINT16_MAX/(strip->numPixels()*10);
    
}

void Animator::fadeall( byte dec) {
  for(int i = 0; i < strip->numPixels(); i++) {
    uint32_t color = strip->getPixelColor(i);
    byte r = color >> 16;
    byte g = color >> 8;
    byte b = color;
    if(r < dec) r = 0;
    else r -= dec;
    if(g < dec) g = 0;
    else g -= dec;
    if(b < dec) b = 0;
    else b -= dec;
    strip->setPixelColor(i, strip->Color(r, g, b));
  }
}

void Animator::cylon() {
  static bool dir;
  static int pos;
  static uint16_t hue;
    // Set the i'th led to red 
    if(dir) strip->setPixelColor(pos--/2, strip->ColorHSV(hue++, 255, 255));
    else strip->setPixelColor(pos++/2, strip->ColorHSV(hue++, 255, 255));
    // Show the leds
    hue += UINT16_MAX/(strip->numPixels()*3);
    strip->show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall(10);
    // Wait a little bit before we loop around and do it again
    if(pos/2 >= strip->numPixels() - 1 || pos < 0) dir = !dir;
}

void Animator::halloween() {
  for(int i = 0; i < 1; i++) {
    strip->setPixelColor(random(strip->numPixels()-1), strip->Color(255, 125, 0));
    strip->setPixelColor(random(strip->numPixels()-1), strip->Color(255, 0, 185));
  }
  fadeall(3);
}