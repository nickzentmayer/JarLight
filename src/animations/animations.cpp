#define ANIM
#include "animations.h"

SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();

void cycle(void* s)  {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  helper->setAnimationSemaphore(&xSemaphore);
  NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip = helper->getStrip();
    float fph;
    //strip->setPin(strip->getPin());
    Serial.println("start cycle");
    for(;;) {
    xSemaphoreTake( xSemaphore, portMAX_DELAY);
    float hue = fph;
    for(int i = 0; i < strip->PixelCount(); i++) {
      
      strip->SetPixelColor(i, HsbColor(hue, 1, 1));
      hue += (float)1/strip->PixelCount();
    }
    fph += UINT16_MAX/(strip->PixelCount()*10);
    strip->Show();
    xSemaphoreGive(xSemaphore);
    vTaskDelay(1);
    }
}

void fadeall(NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip, byte dec) {
  for(int i = 0; i < strip->PixelCount(); i++) {
    strip->SetPixelColor(i, strip->GetPixelColor(i).Dim(dec));
    /*
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
    strip->SetPixelColor(i, RgbColor(r, g, b));
    */
  }
}

void cylon(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  helper->setAnimationSemaphore(&xSemaphore);
  NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip = helper->getStrip();
  bool dir;
  int pos;
  float hue;
  //strip->setPin(strip->getPin());
  for(;;) {
    xSemaphoreTake( xSemaphore, portMAX_DELAY);
    // Set the i'th led to red 
    if(dir) strip->SetPixelColor(pos--/2, HsbColor(hue++, 255, 255));
    else strip->SetPixelColor(pos++/2, HsbColor(hue++, 255, 255));
    // Show the leds
    hue += 1/strip->PixelCount();
    strip->Show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall(strip, 10);
    // Wait a little bit before we loop around and do it again
    if(pos/2 >= strip->PixelCount() - 1 || pos < 0) dir = !dir;
    strip->Show();
    xSemaphoreGive(xSemaphore);
    vTaskDelay(1);
  }
}

void halloween(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  helper->setAnimationSemaphore(&xSemaphore);
  NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip = helper->getStrip();
  //strip->setPin(strip->getPin());
  for(;;) {
    xSemaphoreTake( xSemaphore, portMAX_DELAY);
  for(int i = 0; i < 1; i++) {
    strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(255, 85, 0));
    strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(255, 0, 185));
  }
  fadeall(strip, 10);
  strip->Show();
  xSemaphoreGive(xSemaphore);
  vTaskDelay(50);
  }
}

void fall(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  helper->setAnimationSemaphore(&xSemaphore);
  NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip = helper->getStrip();
  for(;;) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    for(int i = 0; i < 1; i++) {
      switch (random(3))
      {
        case 0:
          strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(255, 150, 0));
          break;
        case 1:
          strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(255, 85, 0));
          break;
        case 2:
          strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(255, 0, 0));
        break;
      default:
        break;
      }
  }
    fadeall(strip, 7);
    strip->Show();
    xSemaphoreGive(xSemaphore);
    vTaskDelay(25);
  }
}

void christmas(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  helper->setAnimationSemaphore(&xSemaphore);
  NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip = helper->getStrip();
  for(;;) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    for(int i = 0; i < 3; i++) {
      switch (random(5))
      {
        case 0:
          strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(255, 0, 0));
          break;
        case 1:
          strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(0, 255, 0));
          break;
        case 2:
          strip->SetPixelColor(random(strip->PixelCount()-1), RgbColor(100, 100, 100));
        break;
      default:
        break;
      }
  }
    fadeall(strip, 5);
    strip->Show();
    xSemaphoreGive(xSemaphore);
    vTaskDelay(25);
  }
}

void twinkle(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  helper->setAnimationSemaphore(&xSemaphore);
  NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip = helper->getStrip();
  for(;;) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    for(int i = 0; i < 3; i++) {
      switch (random(3))
      {
        case 0:
          strip->SetPixelColor(random(strip->PixelCount()-1), helper->getColor());
          break;
      default:
        break;
      }
  }
    fadeall(strip, 5);
    strip->Show();
    strip->Show();
    xSemaphoreGive(xSemaphore);
    vTaskDelay(25);
  }
}
