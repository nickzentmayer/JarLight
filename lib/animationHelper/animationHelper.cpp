#include "animationHelper.h"

AnimationHelper::AnimationHelper(int n, uint8_t p) {
    NLEDS = n;
    pin = p;
}
void AnimationHelper::begin() {
    strip = new Adafruit_NeoPixel(NLEDS, pin, NEO_GRB + NEO_KHZ800);
    strip->begin();
    strip->show();
}
void AnimationHelper::setColor(uint8_t r, uint8_t g, uint8_t b) {
    color = strip->Color(r, g, b);
}
void AnimationHelper::setColorHsv(uint16_t h, uint8_t s, uint8_t v) {
    color = strip->ColorHSV(h, s, v);
}
void AnimationHelper::setColor(uint8_t r, uint8_t g, uint8_t b, bool sho) {
    color = strip->Color(r, g, b);
    if(sho) showColor();
}
void AnimationHelper::setColorHsv(uint16_t h, uint8_t s, uint8_t v, bool sho) {
    color = strip->ColorHSV(h, s, v);
    if(sho) showColor();
}
void AnimationHelper::showColor() 
{
setAnimation("none");
strip->fill(color);
strip->show();
}
void AnimationHelper::setAnimation(String a) {
    animation = a;
    xTaskHandle animTask = xTaskGetHandle("Animation Task");
    if(animTask != NULL) vTaskDelete(animTask);
    if (animation.equals("cycle"))
    {
        xTaskCreate(
            cycle,
            "Animation Task",
            2048,
            strip,
            1,
            NULL);
    }
    if (animation.equals("cylon"))
    {
        xTaskCreate(
            cylon,
            "Animation Task",
            2048,
            strip,
            1,
            NULL);
    }
    if (animation.equals("halloween"))
    {
        xTaskCreate(
            halloween,
            "Animation Task",
            2048,
            strip,
            1,
            NULL);
    }
}
void AnimationHelper::setBrightness(byte b) {
    brightness = b;
    strip->setBrightness(brightness);
    if(animation.equals("none")) showColor();
}
void AnimationHelper::setPower(bool p) {
    power = p;
    if(p) 
    {
        if(animation.equals("none")) showColor();
        else 
        {
            xTaskHandle animTask = xTaskGetHandle("Animation Task");
            if(animTask != NULL) vTaskResume(animTask);
        }
    }
    else 
    {
        if(animation.equals("none")) 
        {
            strip->fill(0);
            strip->show();
        }
        else 
        {
            xTaskHandle animTask = xTaskGetHandle("Animation Task");
            if(animTask != NULL) vTaskSuspend(animTask);
        }
    }
}

bool AnimationHelper::getPower() {
    return power;
}
byte AnimationHelper::getBrightness() {
    return brightness;
}
String AnimationHelper::getAnimation() {
    return animation;
}
uint32_t AnimationHelper::getColor() {
    return color;
}
Adafruit_NeoPixel* AnimationHelper::getStrip() {
    return strip;
}
void AnimationHelper::setStrip(Adafruit_NeoPixel* s) {
    strip = s;
}