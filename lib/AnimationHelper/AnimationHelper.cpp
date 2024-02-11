#include "AnimationHelper.h"

AnimationHelper::AnimationHelper(int n) {
    NLEDS = n;
}
void AnimationHelper::begin() {
    strip = new CRGB[NLEDS];
    FastLED.addLeds<PIXELSPEED, DATAPIN, PIXELTYPE>(strip, NLEDS);
    FastLED.setBrightness(brightness);
    FastLED.show();
    setColor(100, 100, 100);
}
void AnimationHelper::setColor(uint8_t r, uint8_t g, uint8_t b, bool sho) {
    color = CRGB(r, g, b);
    if(sho) showColor();
}
void AnimationHelper::setColorHsv(uint8_t h, uint8_t s, uint8_t v, bool sho) {
    color = CHSV(h, s, v);
    if(sho) showColor();
}
void AnimationHelper::setColor(uint32_t c, bool sho) {
    color = c;
    if(sho) showColor();
}
void AnimationHelper::showColor() 
{
if(animation != -1)setAnimation(-1);
fill((uint32_t)color);
if(!power) return;
FastLED.show();
}
void AnimationHelper::addAnimation(String* name, animPtr anim) {
    Serial.println(numAnims);
    if(animations != NULL) {
        animPtr* oldAnims = animations;
        String** oldNames = animNames;
        animations = new animPtr[++numAnims];
        animNames = new String*[numAnims];
        for(int i = 0; i < numAnims - 1; i++) {
            animations[i] = oldAnims[i];
            animNames[i] = oldNames[i];
        }
        animations[numAnims - 1] = anim;
        animNames[numAnims - 1] = name;
        delete oldAnims;
        delete oldNames;
    }
    else {
        animations = new animPtr[++numAnims];
        animNames = new String*[numAnims];
        animations[0] = anim;
        animNames[0] = name;
    }
}
void AnimationHelper::setAnimation(int a) {
    animation = a;
        xTaskHandle animTask = xTaskGetHandle("Animation Task");
        if(animTask != NULL) 
        {
            xSemaphoreTake(xSemaphore, portMAX_DELAY);
            vTaskDelete(animTask);
            xSemaphoreGive(xSemaphore);
            Serial.println("delete");
        }
        if(animation != -1) {
            #ifdef ESP32DEV
            xTaskCreatePinnedToCore(
                animations[animation],
                "Animation Task",
                2048,
                this,
                2,
                NULL,
                1);
            #endif
            #ifdef ESP32C3
            xTaskCreate(
                animations[animation],
                "Animation Task",
                2048,
                this,
                2,
                NULL);
            #endif
            #ifdef ESP32S2
            xTaskCreate(
                animations[animation],
                "Animation Task",
                2048,
                this,
                2,
                NULL);
            #endif
        }
    setPower(power);
}
void AnimationHelper::setBrightness(byte b) {
    brightness = b;
    FastLED.setBrightness(brightness);
    if(animation == -1 && power) showColor();
}
void AnimationHelper::setSpeed(byte s) {
    speed = (float)s / 255.0;
}
void AnimationHelper::setPower(bool p) {
    power = p;
    if(p) 
    {
        if(animation == -1) showColor();
        else 
        {
            xTaskHandle animTask = xTaskGetHandle("Animation Task");
            if(animTask != NULL) vTaskResume(animTask);
        }
    }
    else 
    {
        if(animation != -1) 
        {
            xTaskHandle animTask = xTaskGetHandle("Animation Task");
            if(animTask != NULL) 
            {
                xSemaphoreTake(xSemaphore, portMAX_DELAY);
                vTaskSuspend(animTask);
                xSemaphoreGive(xSemaphore);
            }
        }
        fill(0);
        FastLED.show();
    }
}
void AnimationHelper::powerOn() {
    setPower(true);
}
void AnimationHelper::powerOff() {
    setPower(false);
}
void AnimationHelper::setAnimationSemaphore(semaPtr s) {
    s(&xSemaphore);
}

void AnimationHelper::setPixelColor(int p, uint8_t r, uint8_t g, uint8_t b, bool sho) {
    strip[p] = CRGB(r, g, b);
    if(sho) show();
}
void AnimationHelper::setPixelColorHsv(int p, uint8_t h, uint8_t s, uint8_t v, bool sho) {
    strip[p] = CHSV(h, s, v);
    if(sho) show();
}
void AnimationHelper::setPixelColor(int p, uint32_t c, bool sho) {
    strip[p] = CRGB(c);
    if(sho) show();
}
void AnimationHelper::show() {
    FastLED.show();
}
void AnimationHelper::setPrimeAnimColor(byte r, byte g, byte b) {
    primaryAnimColor = ((uint32_t)r << 16) + ((uint32_t)g << 8) + (b);
}
    
void AnimationHelper::setSecAnimColor(byte r, byte g, byte b) {
    secondaryAnimColor = ((uint32_t)r << 16) + ((uint32_t)g << 8) + (b);
}
uint32_t AnimationHelper::getPrimeAnimColor() {
    return primaryAnimColor;
}
    
uint32_t AnimationHelper::getSecAnimColor() {
    return secondaryAnimColor;
}
uint32_t AnimationHelper::getPixelColor(int p) {
    return (uint32_t)strip[p];
}

void AnimationHelper::fill(uint32_t c) {
    for(int i=0; i<NUMLEDS; i++) 
        strip[i] = CRGB(c);
}

bool AnimationHelper::getPower() {
    return power;
}
byte AnimationHelper::getBrightness() {
    return brightness;
}
float AnimationHelper::getSpeed() {
    return speed;
}
int AnimationHelper::getAnimation() {
    return animation;
}
int AnimationHelper::pixelCount() {
    return NUMLEDS;
}
String** AnimationHelper::getAnimationNames() {
    return animNames;
}
int AnimationHelper::getNumberAnimations() {
    return numAnims;
}
uint32_t AnimationHelper::getColor() {
    return (uint32_t)color;
}
CRGB* AnimationHelper::getStrip() {
    return strip;
}
void AnimationHelper::setStrip(CRGB* s) {
    strip = s;   
}