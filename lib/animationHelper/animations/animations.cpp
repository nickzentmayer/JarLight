#include "animations.h"

SemaphoreHandle_t* xSemaphore;

void setSemaphore(SemaphoreHandle_t* xSem) {
  xSemaphore = xSem;
}

void cycle(void* s)  {
  Adafruit_NeoPixel* strip = static_cast<Adafruit_NeoPixel *>(s);
    uint16_t fph;
    //strip->setPin(strip->getPin());
    for(;;) {
    xSemaphoreTake( *xSemaphore, portMAX_DELAY);
    uint16_t hue = fph;
    for(int i = 0; i < strip->numPixels(); i++) {
      
      strip->setPixelColor(i, strip->ColorHSV(hue, 255, 255));
      hue += UINT16_MAX/(strip->numPixels()*2);
    }
    fph += UINT16_MAX/(strip->numPixels()*10);
    strip->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(1);
    }
}

void fadeall(Adafruit_NeoPixel* strip, byte dec) {
  for(int i = 0; i < strip->numPixels(); i++) {
    uint32_t color = strip->getPixelColor(i);
    byte r = color >> 16;
    byte g = color >> 8;
    byte b = color;
    byte m = max(r, g);
    m = max(m, b);
    if(r < dec) r = 0;
    else r -= map(dec, 0, m, 0, r);
    if(g < dec) g = 0;
    else g -= map(dec, 0, m, 0, g);
    if(b < dec) b = 0;
    else b -= map(dec, 0, m, 0, b);
    strip->setPixelColor(i, strip->Color(r, g, b));
  }
}

void cylon(void* s) {
  Adafruit_NeoPixel* strip = static_cast<Adafruit_NeoPixel *>(s);
  bool dir;
  int pos;
  uint16_t hue;
  //strip->setPin(strip->getPin());
  for(;;) {
    xSemaphoreTake( *xSemaphore, portMAX_DELAY);
    // Set the i'th led to red 
    if(dir) strip->setPixelColor(pos--/2, strip->ColorHSV(hue++, 255, 255));
    else strip->setPixelColor(pos++/2, strip->ColorHSV(hue++, 255, 255));
    // Show the leds
    hue += UINT16_MAX/(strip->numPixels()*3);
    strip->show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall(strip, 10);
    // Wait a little bit before we loop around and do it again
    if(pos/2 >= strip->numPixels() - 1 || pos < 0) dir = !dir;
    strip->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(1);
  }
}

void halloween(void* s) {
  Adafruit_NeoPixel* strip = static_cast<Adafruit_NeoPixel *>(s);
  //strip->setPin(strip->getPin());
  for(;;) {
    xSemaphoreTake( *xSemaphore, portMAX_DELAY);
  for(int i = 0; i < 1; i++) {
    strip->setPixelColor(random(strip->numPixels()-1), strip->Color(255, 125, 0));
    strip->setPixelColor(random(strip->numPixels()-1), strip->Color(255, 0, 185));
  }
  fadeall(strip, 10);
  strip->show();
  xSemaphoreGive(*xSemaphore);
  vTaskDelay(25);
  }
}