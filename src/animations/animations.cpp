#define ANIM
#include "animations.h"
#define FADEBY 3

SemaphoreHandle_t* xSemaphore;

void setSemaphore(SemaphoreHandle_t* s) {
  xSemaphore = s;
}

void cycle(void* s)  {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
    float fph;
    //strip->setPin(strip->getPin());
    Serial.println("start cycle");
    for(;;) {
    xSemaphoreTake( *xSemaphore, portMAX_DELAY);
    float hue = fph;
    for(int i = 0; i < helper->pixelCount(); i++) {
      
      helper->setPixelColorHsv(i, hue*255, 255, 255);
      hue += (float)1/helper->pixelCount();
    }
    fph += 1.0/(helper->pixelCount()*5);
    if(fph >= 1.0) fph = 0;
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(5 * (1.0 - helper->getSpeed())));
    }
}

void fadeall(AnimationHelper* helper, byte dec) {
  uint32_t color;
  for(int i = 0; i < helper->pixelCount(); i++) {
    color = helper->getPixelColor(i);  
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
    helper->setPixelColor(i, r, g, b);
  }
}

void cylon(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  
  bool dir;
  int pos;
  float hue;
  //strip->setPin(strip->getPin());
  for(;;) {
    xSemaphoreTake( *xSemaphore, portMAX_DELAY);
    // Set the i'th led to red 
    if(dir) helper->setPixelColorHsv(pos--/2, hue++, 255, 255);
    else helper->setPixelColorHsv(pos++/2, hue++, 255, 255);
    // Show the leds
    hue += 1/helper->pixelCount();
    helper->show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall(helper, 10);
    // Wait a little bit before we loop around and do it again
    if(pos/2 >= helper->pixelCount() - 1 || pos < 0) dir = !dir;
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(25 * (1.0 - helper->getSpeed())));
  }
}

void halloween(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  
  //strip->setPin(strip->getPin());
  for(;;) {
    xSemaphoreTake( *xSemaphore, portMAX_DELAY);
  for(int i = 0; i < 1; i++) {
    helper->setPixelColor(random(helper->pixelCount()-1), 255, 85, 0);
    helper->setPixelColor(random(helper->pixelCount()-1), 255, 0, 185);
  }
  fadeall(helper, 10);
  helper->show();
  xSemaphoreGive(*xSemaphore);
  vTaskDelay(abs(100 * (1.0 - helper->getSpeed())));
  }
}

void fall(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    for(int i = 0; i < 1; i++) {
      switch (random(3))
      {
        case 0:
          helper->setPixelColor(random(helper->pixelCount()-1), 255, 150, 0);
          break;
        case 1:
          helper->setPixelColor(random(helper->pixelCount()-1), 255, 85, 0);
          break;
        case 2:
          helper->setPixelColor(random(helper->pixelCount()-1), 255, 0, 0);
        break;
      default:
        break;
      }
  }
    fadeall(helper, 7);
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(50 * (1.0 - helper->getSpeed())));
  }
}

void christmas(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    for(int i = 0; i < 3; i++) {
      switch (random(5))
      {
        case 0:
          helper->setPixelColor(random(helper->pixelCount()-1), 255, 0, 0);
          break;
        case 1:
          helper->setPixelColor(random(helper->pixelCount()-1), 0, 255, 0);
          break;
        case 2:
          helper->setPixelColor(random(helper->pixelCount()-1), 100, 100, 100);
        break;
      default:
        break;
      }
  }
    fadeall(helper, 5);
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(50 * (1.0 - helper->getSpeed())));
  }
}

void twinkle(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    for(int i = 0; i < 3; i++) {
      switch (random(3))
      {
        case 0:
          helper->setPixelColor(random(helper->pixelCount()-1), helper->getPrimeAnimColor());
          break;
      default:
        break;
      }
  }
    fadeall(helper, 5);
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(50 * (1.0 - helper->getSpeed())));
  }
}

void candyCane(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    int shift;
    for(int i = 0; i < helper->pixelCount(); i++) {
      int p = (i+shift) % helper->pixelCount();
      if(i % 10 < 5) helper->setPixelColor(p, 255, 0, 0);
      else helper->setPixelColor(p, 255, 255, 255);
  }
    if(++shift >= helper->pixelCount()) shift = 0;
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(250 * (1.0 - helper->getSpeed())));
  }
}

void multiSparkle(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  byte sat[helper->pixelCount()];
  int pixelCount = helper->pixelCount();
  for(int i = 0; i < pixelCount; i++) sat[i] = 255;
  byte count = random(3, 8);
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    if(count-- == 0) {
    for(int i = 0; i < random(3); i++) {
      int r = random(pixelCount);
      sat[r] = 0;
    }
    count = random(3, 8);
    }

    for(int i = 0; i < pixelCount; i++) {
      switch (i % 4)
      {
      case 0:
        helper->setPixelColorHsv(i, 0, sat[i], map(sat[i], 0, 255, 255, 125));
        break;
        case 1:
        helper->setPixelColorHsv(i, 0.135 * 255, sat[i], map(sat[i], 0, 255, 255, 125));
        break;
        case 2:
        helper->setPixelColorHsv(i, 0.33 * 255, sat[i], map(sat[i], 0, 255, 255, 125));
        break;
        case 3:
        helper->setPixelColorHsv(i, 0.67 * 255, sat[i], map(sat[i], 0, 255, 255, 125));
        break;
      
      default:
        break;
      }
      if(sat[i] < 255 && ((int)sat[i] + FADEBY) > 255) sat[i] = 255;
      else if(sat[i] < 255) sat[i] += FADEBY;
  }
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(100 * (1.0 - helper->getSpeed())));
  }
}

void chSparkle(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  int pixelCount = helper->pixelCount();
  byte sat[pixelCount];
  for(int i = 0; i < pixelCount; i++) sat[i] = 255;
  byte count = random(0, 3);
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    if(count-- == 0) {
    for(int i = 0; i < random(3); i++) {
      int r = random(pixelCount);
      sat[r] = 0;
    }
    count = random(3, 8);
    }
    for(int i = 0; i < pixelCount; i++) {
      switch (i % 2)
      {
      case 0:
        helper->setPixelColorHsv(i, 0, sat[i], map(sat[i], 0, 255, 255, 125));
        break;
        case 1:
        helper->setPixelColorHsv(i, 0.333*255, sat[i], (float)map(sat[i], 0, 255, 255, 125));
        break;
      
      default:
        break;
      }
      if(sat[i] < 255 && ((int)sat[i] + FADEBY) > 255) sat[i] = 255;
      else if(sat[i] < 255) sat[i] += FADEBY;
  }
    helper->show();;
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(100 * (1.0 - helper->getSpeed())));
  }
}

void chase(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    int shift;
    for(int i = 0; i < helper->pixelCount(); i++) {
      int p = (i+shift) % helper->pixelCount();
      if(i % 30 < 15) helper->setPixelColor(p, helper->getPrimeAnimColor());
      else helper->setPixelColor(p, helper->getSecAnimColor());
  }
    if(++shift >= helper->pixelCount()) shift = 0;
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(250 * (1.0 - helper->getSpeed())));
  }
}

/*void fade(void* s) {
  AnimationHelper* helper = static_cast<AnimationHelper *>(s);
  bool isPrimary = true;
  int waitCycles = 50;
  for(;;) {
    xSemaphoreTake(*xSemaphore, portMAX_DELAY);
    if(waitCycles-- > 0) {
      if(isPrimary) helper->fill(helper->getPrimeAnimColor());
    }
    helper->getPrimeAnimColor();
    helper->show();
    xSemaphoreGive(*xSemaphore);
    vTaskDelay(abs(100 * (1.0 - helper->getSpeed())));
  }
}*/