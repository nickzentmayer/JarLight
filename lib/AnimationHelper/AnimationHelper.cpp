#include "AnimationHelper.h"

AnimationHelper::AnimationHelper(int n, uint8_t p) {
    NLEDS = n;
    pin = p;
}
void AnimationHelper::begin() {
    FastLED.addLeds<PIXELSPEED,DATAPIN,PIXELTYPE>(leds, NUMLEDS);
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
fill(color);
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
        fill(CRGB(0, 0, 0));
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
    leds[p] = CRGB(r, g, b);
    if(sho) show();
}
void AnimationHelper::setPixelColorHsv(int p, uint8_t h, uint8_t s, uint8_t v, bool sho) {
    leds[p] = CHSV(h, s, v);
    if(sho) show();
}
void AnimationHelper::setPixelColor(int p, uint32_t c, bool sho) {
    leds[p] = CRGB(c);
    if(sho) show();
}
void AnimationHelper::show() {
    FastLED.show();
}
uint32_t AnimationHelper::getPixelColor(int p) {
    return (uint32_t)leds[p];
}

void AnimationHelper::fill(CRGB c) {
    for(int i=0; i<NUMLEDS; i++) 
        leds[i] = c;
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
    return sizeof(leds)/sizeof(CRGB);
}
String** AnimationHelper::getAnimationNames() {
    return animNames;
}
int AnimationHelper::getNumberAnimations() {
    return numAnims;
}
uint32_t AnimationHelper::getColor() {
    return (color.r << 16 + color.g << 8 + color.b);
}
CRGB* AnimationHelper::getStrip() {
    return leds;
}
void AnimationHelper::setStrip(CRGB* s) {
    leds = s;   
}