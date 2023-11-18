#include <Arduino.h>
#include <NeoPixelBrightnessBus.h>
#include <AnimationHelper.h>
#ifndef ANIM
#define ANIM
    struct Animation {
        animPtr anim;
        String name;
    };

    void cylon(void* s);
    void cycle(void* s);
    //void fireworks();
    void fadeall(NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* strip, uint8_t dec);
    void halloween(void* s);
    void fall(void* s);
    void setSemaphore(SemaphoreHandle_t* xSem);
    void christmas(void* s);
    void twinkle(void* s);
    
    //declare Animation array {void AnimationFunction, String name}
    Animation animations[] = {{cylon, "Cylon"}, 
                              {cycle, "Cycle"},
                              {halloween, "Halloween"}, 
                              {fall, "Fall"},
                              {christmas, "Christmas"},
                              {twinkle, "Twinkle"}};
    #endif
