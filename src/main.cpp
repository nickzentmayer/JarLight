//libraries
#include <Arduino.h>
#include <wifiHelper.h>
#include <AnimationHelper.h>
//custom files
#include "config.h"
#include "animations/animations.h"

// create leds in memory
//declare mpu objuect
#ifdef USEMPU
#include <mpu6500.h>
MPU mpu(MPUADDR);
#endif
AnimationHelper strip(NUMLEDS, DATAPIN, NEOTYPE);


void setup() {
  //Serial.begin(115200);
  // init leds
  delay(1000);
  strip.begin();
  Serial.println("begun");
  
  for(int i = 0; i < (sizeof(animations)/sizeof(Animation)); i++) strip.addAnimation(&(animations[i].name), animations[i].anim);
  strip.setAnimationSemaphore(setSemaphore);
  strip.setColor(0, 0, 100, true);
  if(!wifiSetup(&strip)) {
    Serial.println("FAIL");
    strip.setColor(255, 0, 0, true);
    if (!USE_SOFT_AP)
      {
        delay(5000);
        ESP.restart();
      }
  }
  else strip.setColor(0, 255, 0, true);
  delay(1000);
  strip.setColor(100, 100, 100, true);
  #ifdef USEMPU
  mpu.setupInt();
  pinMode(MPUINT, INPUT);
  mpu.clearInt();
  #endif
}

void loop() {
  handleWiFi();
  #ifdef USEMPU
  if(digitalRead(MPUINT)) {
    strip.setPower(!strip.getPower());
    mpu.clearInt();
    updateClients();
    delay(50); //"debounce"
  }
  #endif
}
