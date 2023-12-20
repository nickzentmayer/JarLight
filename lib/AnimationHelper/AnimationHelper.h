#pragma once
#include <NeoPixelBus.h>
#include <NeoPixelBrightnessBus.h>
#include <Arduino.h>
#include "config.h"

/*GOALS OF THIS LIBARARY
1. Make me feel good about finally using c++ classes
2. Clean up the usually stringy and messy code of these projects
3. *Hopefully* support multiple strands if necessary (Havent tested)
4. USING FREERTOS TASKS FOR ANIMATIONS(real proud of this one)
5. Create and handle all task related functions
This ensures you can make your animations as blocking as u want, using as many delays as you want
and still have a responsive web interface*/

#ifndef ANIMHELPER
#define ANIMHELPER

typedef void (*animPtr) (void*);
typedef void (*semaPtr) (SemaphoreHandle_t*);

class AnimationHelper {
    public:
    AnimationHelper(int n, uint8_t p);

    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b, bool sho = false);
    void setColorHsv(uint8_t h, uint8_t s, uint8_t v, bool sho = false);
    void setColor(uint32_t c, bool sho = false);
    void show();
    void addAnimation(String* name, void (*anim)(void*));
    void setAnimation(int a);
    void setBrightness(byte b);
    void setSpeed(byte s);
    void setPower(bool p);
    void setAnimationSemaphore(semaPtr);

    void setPixelColor(int p, uint8_t r, uint8_t g, uint8_t b, bool sho = false);
    void setPixelColorHsv(int p, uint8_t h, uint8_t s, uint8_t v, bool sho = false);
    void setPixelColor(int p, uint32_t c, bool sho = false);
    uint32_t getPixelColor(int p);

    bool getPower();
    byte getBrightness();
    float getSpeed();
    int getAnimation();
    String** getAnimationNames();
    int getNumberAnimations();
    uint32_t getColor();
    NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* getStrip();
    void setStrip(NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* s);

    private:
    void fill(RgbColor c);
    uint8_t pin;
    NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* strip;
    RgbColor color;
    int NLEDS = 0;
    bool power = true;
    byte brightness = 100;
    float speed = 0.5;
    int animation = -1;
    SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();
    animPtr* animations = NULL;
    String** animNames;
    int numAnims = 0;
};
#endif