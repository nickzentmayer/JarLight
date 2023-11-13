#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

    void cylon(void* s);
    void cycle(void* s);
    //void fireworks();
    void fadeall(Adafruit_NeoPixel* strip, uint8_t dec);
    void halloween(void* s);
    void fall(void* s);
    void setSemaphore(SemaphoreHandle_t* xSem);
    
    //declare function ptr array
    #ifndef ANIMPTR
    #define ANIMPTR
    void (*anims[]) (void*) = {cylon, cycle, halloween, fall};
    String names[] = {"Cylon", "Cycle", "Halloween", "Fall"};
    #endif
