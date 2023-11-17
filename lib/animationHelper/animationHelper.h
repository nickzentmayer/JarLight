#pragma once
#include <NeoPixelBus.h>
#include <NeoPixelBusLg.h>
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

class AnimationHelper {
    public:
    AnimationHelper(int n, uint8_t p);

    void begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b, bool sho = false);
    void setColorHsv(float h, float s, float v, bool sho = false);
    void setColor(RgbColor c, bool sho = false);
    void showColor();
    void addAnimation(String* name, void (*anim)(void*));
    void setAnimation(int a);
    void setBrightness(byte b);
    void setPower(bool p);
    void setAnimationSemaphore(SemaphoreHandle_t* s);

    bool getPower();
    byte getBrightness();
    int getAnimation();
    String** getAnimationNames();
    int getNumberAnimations();
    RgbColor getColor();
    NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* getStrip();
    void setStrip(NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* s);

    private:
    void fill(RgbColor c);
    uint8_t pin;
    NeoPixelBusLg<PIXELTYPE, PIXELSPEED>* strip;
    RgbColor color;
    int NLEDS = 0;
    bool power = true;
    byte brightness = 100;
    int animation = -1;
    SemaphoreHandle_t* xSemaphore;
    animPtr* animations = NULL;
    String** animNames;
    int numAnims = 0;
};
#endif