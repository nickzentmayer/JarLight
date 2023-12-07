#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "config.h"
#include "SPIFFS.h"
#include "AnimationHelper.h"
#include <TinyUPnP.h>

bool wifiSetup(AnimationHelper* s);

void handleIndex(AsyncWebServerRequest *req);
void sendFile(AsyncWebServerRequest *req);
void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, uint32_t id, AsyncWebSocket* server);
void dataOnConnect(AsyncWebSocket* server);
void updateClients();
void handleWiFi();
#ifdef BATTPIN
void sendBattery();
#endif