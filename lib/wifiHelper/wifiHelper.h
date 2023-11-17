#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "config.h"
#include "SPIFFS.h"
#include "animationHelper.h"

bool wifiSetup(AnimationHelper* s);

void handleIndex(AsyncWebServerRequest *req);
void sendFile(AsyncWebServerRequest *req);
void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, uint32_t id);
void dataOnConnect();
void handleWiFi();
#ifdef BATTPIN
void sendBattery();
#endif