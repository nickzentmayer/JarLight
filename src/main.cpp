//libraries
#include <Arduino.h>
#include <wifiHelper.h>
#include <AnimationHelper.h>
#include <mpu6500.h>
//custom files
#include "config.h"
#include "animations/animations.h"

// create leds in memory
//declare mpu objuect
MPU mpu(MPUADDR);
AnimationHelper strip(NUMLEDS, DATAPIN);


void setup() {
  //Serial.begin(115200);
  // init leds
  delay(1000);
  strip.begin();
  Serial.println("begun");
  
  for(int i = 0; i < (sizeof(animations)/sizeof(Animation)); i++) strip.addAnimation(&(animations[i].name), animations[i].anim);
  strip.setAnimationSemaphore(setSemaphore);
  mpu.setupInt();
  pinMode(MPUINT, INPUT);
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
  mpu.clearInt();
}

void loop() {
  handleWiFi();
}
