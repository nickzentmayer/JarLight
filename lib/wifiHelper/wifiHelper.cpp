#include "wifiHelper.h"

AsyncWebServer server(PORT);
AsyncWebSocket ws("/ws");
AnimationHelper* strp;

bool wifiSetup(AnimationHelper* s) {
    if(!SPIFFS.begin(true)) return false;
    WiFi.begin(SSID, PSWD);
    long t = millis();
    if(WiFi.waitForConnectResult() != WL_CONNECTED) return false;
    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
  MDNS.begin("jarlight");
  server.on("/", handleIndex);
  server.begin();
  ws.onEvent(wsOnEvent);
  server.addHandler(&ws);
  MDNS.addService("http", "tcp", PORT);

  strp = s;
  return true;
}

void handleIndex(AsyncWebServerRequest *req) {
    req->send(SPIFFS, "/ui.html", "text/html");
}

void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      dataOnConnect();
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    break;
    case WS_EVT_ERROR:
      break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String msg = String((char*)data);
    if (msg.startsWith("p:")) {
      //power = msg.substring(msg.indexOf(":")+1).equals("true");
      //if(animation.equals("none")) upd = true;
      strp->setPower(msg.substring(msg.indexOf(":")+1).equals("true"));
    }
    if (msg.startsWith("c:")) {
      String color = msg.substring(msg.indexOf(":")+2);
      int red = strtoul(color.substring(0, 2).c_str(), NULL, 16);
      int green = strtoul(color.substring(2, 4).c_str(), NULL, 16);
      int blue = strtoul(color.substring(4).c_str(), NULL, 16);
      strp->setColor(red, green, blue, true);
    }
    if (msg.startsWith("a:")) {
      //ledColor(0,0,0);
      //lAnim = animation;
      strp->setAnimation(msg.substring(msg.indexOf(":")+1));
    }
    if (msg.startsWith("b:")) {
      //brightness = msg.substring(msg.indexOf(":")+1).toInt();
      //FastLED.setBrightness(brightness);
      //upd = animation.equals("none");
      strp->setBrightness(msg.substring(msg.indexOf(":")+1).toInt());
    }
  }
}

void dataOnConnect() {
    ws.textAll("Hello");
    byte r = strp->getColor() >> 16;
    byte g = strp->getColor() >> 8;
    byte b = strp->getColor();
        ws.textAll("p:"+String(strp->getPower()));
      String index;
      if(r < 10) index += "0";
            index += String(r, HEX);
            if(g < 10) index += "0";
            index += String(g, HEX);
            if(b < 10) index += "0";
            index += String(b, HEX);
      ws.textAll("c:#"+index);
      ws.textAll("b:"+String(strp->getBrightness()));
}

void handleWiFi() {
  ArduinoOTA.handle();
  ws.cleanupClients();
  if(!WiFi.isConnected()) {
    strp->setPower(true);
    strp->setColor(0, 0, 100);
    if(wifiSetup(strp)) {
      strp->setColor(0, 255, 0);
      delay(1000);
    }
    else {
      strp->setColor(255, 0, 0);
      delay(1000);
      ESP.restart();
    }
  }
}