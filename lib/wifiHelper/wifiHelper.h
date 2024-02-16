#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <WebSocketsClient.h>
#include "config.h"
#include "SPIFFS.h"
#include "AnimationHelper.h"
#include "Timer.h"
#include <time.h>
#include "FifoBuffer.h"
#ifdef USEUPNP
#include <TinyUPnP.h>
#endif
#ifdef USE_DDNS
#include <EasyDDNS.h>
#endif

bool wifiSetup(AnimationHelper* s);

void handleIndex(AsyncWebServerRequest *req);
void handleManifest(AsyncWebServerRequest *req);
void handleSync(AsyncWebServerRequest *req);
void sendFile(AsyncWebServerRequest *req);
void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, uint32_t id, AsyncWebSocket* server);
void updateData(AsyncWebSocket* server);
void updateClients();
void handleWiFi();
#ifdef BATTPIN
void sendBattery();
#endif