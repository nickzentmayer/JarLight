//libraries
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
//custom files
#include "config.h"
#include <wifiHelper.h>
#include <animationHelper.h>

// create leds in memory
//declare mpu objuect
Adafruit_MPU6050 mpu;
AnimationHelper strip(NUMLEDS, DATAPIN);
//Adafruit_NeoPixel st(NUMLEDS, DATAPIN, NEO_GRB + NEO_KHZ800);


void setup() {
  //Serial.begin(115200);
  // init leds
  strip.begin();
  // init mpu6050, if failed stop program
  if(mpu.begin()) while(1) delay(100);
  mpu.reset();
  pinMode(D3, INPUT);
  mpu.enableSleep(true); //sleep cuz int is wack
  strip.setColor(255, 0, 0);Serial.println("nuh uh");
  strip.update();
  
  if(!wifiSetup(&strip)) {
    Serial.println("FAIL");
    delay(5000);
    esp_restart();
  }
  strip.setColor(0, 255, 0);
  strip.update();
  delay(1000);
  strip.setColor(100, 100, 100);
}

void loop() {
  //test anim
  strip.update();
  handleWiFi();
}
