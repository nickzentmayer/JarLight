#pragma once
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#ifndef ANIMHELPER
#define ANIMHELPER

typedef void (*animPtr) (void*);
typedef void (*semaPtr) (SemaphoreHandle_t*);

class AnimationHelper {
    public:
    AnimationHelper(int n, uint8_t p);

    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b, bool sho = false);
    void setColorHsv(uint16_t h, uint8_t s, uint8_t v, bool sho = false);
    void setColor(uint32_t c, bool sho = false);
    void showColor();
    void addAnimation(String name, void (*anim)(void*));
    void setAnimation(int a);
    void setBrightness(byte b);
    void setPower(bool p);
    void setAnimationSemaphore(semaPtr);

    bool getPower();
    byte getBrightness();
    int getAnimation();
    String* getAnimationNames();
    int getNumberAnimations();
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
    int animation = -1;
    SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();
    animPtr *animations = NULL;
    String *animNames = NULL;
    int numAnims = 0;
};
#endif