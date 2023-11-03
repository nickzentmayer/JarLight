#include "animationHelper.h"

AnimationHelper::AnimationHelper(int n, uint8_t p) {
    NLEDS = n;
    pin = p;
}
void AnimationHelper::begin() {
    strip = new Adafruit_NeoPixel(NLEDS, pin, NEO_GRB + NEO_KHZ800);
    strip->begin();
    strip->show();
    anim = new Animator(strip);
}
void AnimationHelper::setColor(uint8_t r, uint8_t g, uint8_t b) {
    color = strip->Color(r, g, b);
    animation = "none";
}
void AnimationHelper::setColorHsv(uint16_t h, uint8_t s, uint8_t v) {
    color = strip->ColorHSV(h, s, v);
    animation = "none";
}
void AnimationHelper::setAnimation(String a) {
    animation = a;
}
void AnimationHelper::setBrightness(byte b) {
    brightness = b;
    strip->setBrightness(brightness);
}
void AnimationHelper::setPower(bool p) {
    power = p;
}
void AnimationHelper::update() {
    if(power) {
        if(!animation.equals("none")) {
            if(animation.equals("cycle")) anim->cycle();
            if(animation.equals("cylon")) anim->cylon();
            if(animation.equals("halloween")) anim->halloween();
        }
        else {
            strip->fill(color);
        }
    }
    else strip->clear();
    strip->show();
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