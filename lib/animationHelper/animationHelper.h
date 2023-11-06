#pragma once
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "animations/animations.h"

#ifndef ANIMHELPER
#define ANIMHELPER
class AnimationHelper {
    public:
    AnimationHelper(int n, uint8_t p);

    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b, bool sho = false);
    void setColorHsv(uint16_t h, uint8_t s, uint8_t v, bool sho = false);
    void setColor(uint32_t c, bool sho = false);
    void showColor();
    void setAnimation(String a);
    void setBrightness(byte b);
    void setPower(bool p);

    bool getPower();
    byte getBrightness();
    String getAnimation();
    uint32_t getColor();
    Adafruit_NeoPixel* getStrip();
    void setStrip(Adafruit_NeoPixel* s);

    private:
    uint8_t pin;
    Adafruit_NeoPixel* strip; 
    uint32_t color;
    int NLEDS = 0;
    bool power = true;
    byte brightness = 100;
    String animation = "none";
    SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();
};
#endif