#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "wifiSettings.h"
#include "SPIFFS.h"
#include "animationHelper.h"

bool wifiSetup(AnimationHelper* s);

void handleIndex(AsyncWebServerRequest *req);
void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void dataOnConnect();
void handleWiFi();