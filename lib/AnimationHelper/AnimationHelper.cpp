#include "AnimationHelper.h"

AnimationHelper::AnimationHelper(int n, uint8_t p) {
    NLEDS = n;
    pin = p;
}
void AnimationHelper::begin() {
    strip = new NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>(NLEDS, pin);
    strip->Begin();
    strip->SetBrightness(brightness);
    strip->Show();
    setColor(100, 100, 100);
}
void AnimationHelper::setColor(uint8_t r, uint8_t g, uint8_t b, bool sho) {
    color = RgbColor(r, g, b);
    if(sho) showColor();
}
void AnimationHelper::setColorHsv(float h, float s, float v, bool sho) {
    color = (RgbColor)HsbColor(h, s, v);
    if(sho) showColor();
}
void AnimationHelper::setColor(RgbColor c, bool sho) {
    color = c;
    if(sho) showColor();
}
void AnimationHelper::showColor() 
{
if(animation != -1)setAnimation(-1);
fill(color);
if(!power) return;
strip->Show();
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
            xTaskCreate(
                animations[animation],
                "Animation Task",
                2048,
                this,
                2,
                NULL);
        }
    setPower(power);
}
void AnimationHelper::setBrightness(byte b) {
    brightness = b;
    strip->SetBrightness(brightness);
    if(animation == -1 && power) showColor();
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
        fill(RgbColor(0, 0, 0));
        strip->Show();
    }
}
void AnimationHelper::setAnimationSemaphore(semaPtr s) {
    s(&xSemaphore);
}

void AnimationHelper::fill(RgbColor c) {
    for(int i=0; i<NUMLEDS; i++) 
        strip->SetPixelColor(i, c);
}

bool AnimationHelper::getPower() {
    return power;
}
byte AnimationHelper::getBrightness() {
    return brightness;
}
int AnimationHelper::getAnimation() {
    return animation;
}
String** AnimationHelper::getAnimationNames() {
    return animNames;
}
int AnimationHelper::getNumberAnimations() {
    return numAnims;
}
RgbColor AnimationHelper::getColor() {
    return color;
}
NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* AnimationHelper::getStrip() {
    return strip;
}
void AnimationHelper::setStrip(NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* s) {
    strip = s;
}