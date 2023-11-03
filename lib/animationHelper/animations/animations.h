#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class Animator {
    public:
    Animator(Adafruit_NeoPixel* s);
    void cylon();
    void cycle();
    //void fireworks();
    void fadeall(uint8_t dec);
    void halloween();
    private:
    Adafruit_NeoPixel* strip;
    byte speed = 100;
};