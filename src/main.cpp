//libraries
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
//#include <Adafruit_MPU6050.h>
//#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
//custom files
#include "config.h"
#include <wifiHelper.h>
#include <animationHelper.h>
#include <mpu6500.h>

// create leds in memory
//declare mpu objuect
//Adafruit_MPU6050 mpu;
MPU mpu(0x68);
AnimationHelper strip(NUMLEDS, DATAPIN);
//Adafruit_NeoPixel st(NUMLEDS, DATAPIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  
  // init leds
  strip.begin();
  // init mpu6050, if failed stop program
  //if(mpu.begin()) while(1) delay(100);
  //mpu.setupInt();
  pinMode(MPUINT, INPUT);
  pinMode(BATTPIN, INPUT);
  //mpu.enableSleep(true); //sleep cuz int is wack
  strip.setColor(0, 0, 100, true);Serial.println("nuh uh");
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
}

void loop() {
  /*if(digitalRead(MPUINT) == HIGH) {
    strip.setColor(255, 0, 0, true);
  }*/
  handleWiFi();
}
