#include "wifiHelper.h"

AsyncWebServer server(PORT);
AsyncWebSocket ws("/ws");
AnimationHelper *strp;
bool recon = false;

bool wifiConnect(bool showLeds)
{
  Serial.println("wifi connect");
  uint32_t c = strp->getColor();
  bool res = true;
  if (showLeds)
  {
    strp->setColor(0x000055, true);
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PSWD);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    if (showLeds)
      strp->setColor(0xFF0000, true);
    if (USE_SOFT_AP)
    {
      WiFi.mode(WIFI_AP);
#ifdef SOFTAP_PSWD
#ifdef SOFTAP_SSID
      WiFi.softAP(SOFTAP_SSID, SOFTAP_PSWD);
#else
      WiFi.softAP(DEVICE_NAME, SOFTAP_PSWD);
#endif

#else
      WiFI.softAP(SOFTAP_SSID);
#endif
    }
    res = false;
  }
  if (showLeds)
  {
    if (res)
      strp->setColor(0x00FF00, true);
    delay(2000);
    strp->setColor(c, true);
  }
  return res;
}

bool wifiSetup(AnimationHelper *s)
{
  // start wifi setup
  strp = s;
  Serial.println("wifi setup");
  bool res = true;
  #ifdef BATTPIN
  pinMode(BATTPIN, INPUT);
  #endif
  SPIFFS.begin(true);
  res = wifiConnect(false);
#ifdef USEOTA
  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();
#endif
  String name = DEVICE_NAME;
  name.toLowerCase();
  while (name.indexOf(' ') > 0)
    name.remove(name.indexOf(' '), 1);
  MDNS.begin(name);
  WiFi.setHostname(DEVICE_NAME);
  server.on("/", handleIndex);
  server.onNotFound(sendFile);
  server.begin();
  ws.onEvent(wsOnEvent);
  server.addHandler(&ws);
  MDNS.addService("http", "tcp", PORT);

  return res;
}

void handleIndex(AsyncWebServerRequest *req)
{
  req->send(SPIFFS, "/ui.html", "text/html");
}
void sendFile(AsyncWebServerRequest *req)
{
  req->send(SPIFFS, req->url(), "file");
}

void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    dataOnConnect();
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len, client->id());
    break;
  case WS_EVT_PONG:
    break;
  case WS_EVT_ERROR:
    break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, uint32_t id)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    String msg = String((char *)data);
    if (msg.equals("getAnimations"))
    {
      for (int i = 0; i < strp->getNumberAnimations(); i++)
        ws.text(id, "a:" + *(strp->getAnimationNames()[i]));
    }
    if (msg.startsWith("p:"))
    {
      // power = msg.substring(msg.indexOf(":")+1).equals("true");
      // if(animation.equals("none")) upd = true;
      strp->setPower(msg.substring(msg.indexOf(":") + 1).equals("true"));
    }
    if (msg.startsWith("c:"))
    {
      String color = msg.substring(msg.indexOf(":") + 2);
      int red = strtoul(color.substring(0, 2).c_str(), NULL, 16);
      int green = strtoul(color.substring(2, 4).c_str(), NULL, 16);
      int blue = strtoul(color.substring(4).c_str(), NULL, 16);
      strp->setColor(red, green, blue, true);
    }
    if (msg.startsWith("a:"))
    {
      // ledColor(0,0,0);
      // lAnim = animation;
      strp->setAnimation(msg.substring(msg.indexOf(":") + 1).toInt());
      Serial.println(strp->getAnimation());
    }
    if (msg.startsWith("b:"))
    {
      // brightness = msg.substring(msg.indexOf(":")+1).toInt();
      // FastLED.setBrightness(brightness);
      // upd = animation.equals("none");
      strp->setBrightness(msg.substring(msg.indexOf(":") + 1).toInt());
    }
    if (msg.startsWith("w:"))
    {
      if (msg.substring(msg.indexOf(":") + 1).equals("recon"))
      {
        recon = true;
      }
    }
    if (msg.startsWith("s:"))
    {
      strp->setColor(0, true);
      esp_deep_sleep_enable_gpio_wakeup((1ULL << 5), ESP_GPIO_WAKEUP_GPIO_HIGH);
      esp_deep_sleep_start();
    }
  }
}

void dataOnConnect()
{
  byte r = strp->getColor() >> 16;
  byte g = strp->getColor() >> 8;
  byte b = strp->getColor();
  ws.textAll("p:" + String(strp->getPower()));
  String index;
  if (r < 10)
    index += "0";
  index += String(r, HEX);
  if (g < 10)
    index += "0";
  index += String(g, HEX);
  if (b < 10)
    index += "0";
  index += String(b, HEX);
  ws.textAll("c:#" + index);
  ws.textAll("b:" + String(strp->getBrightness()));
  if (WiFi.getMode() == WIFI_AP)
    ws.textAll("w:AP");
  else
    ws.textAll("w:STA");
  #ifdef BATTPIN
  ws.textAll("type:battery");
  sendBattery();
  #else
  ws.textAll("type:wall");
  #endif
  ws.textAll("n:" + String(DEVICE_NAME));
}
#ifdef BATTPIN
void sendBattery()
{
  int battVolt = analogReadMilliVolts(BATTPIN) * 2;
  if (strp->getPower())
  {
    battVolt += map(strp->getBrightness(), 5, 255, 1, 100);
  }
  if (battVolt > 4200)
    battVolt = 4200;
  int percent = map(battVolt, 3100, 4200, 0, 20) * 5;
  ws.textAll("batt:" + String(percent));
}
#endif
void updateClients() {
  byte r = strp->getColor() >> 16;
  byte g = strp->getColor() >> 8;
  byte b = strp->getColor();
  ws.textAll("p:" + String(strp->getPower()));
  String index;
  if (r < 10)
    index += "0";
  index += String(r, HEX);
  if (g < 10)
    index += "0";
  index += String(g, HEX);
  if (b < 10)
    index += "0";
  index += String(b, HEX);
  ws.textAll("c:#" + index);
  ws.textAll("b:" + String(strp->getBrightness()));
}
void handleWiFi()
{
#ifdef BATTPIN
  static unsigned long t = millis();
  if (millis() - t > 10000)
  {
    sendBattery();
    t = millis();
  }
#endif
#ifdef USEOTA
  ArduinoOTA.handle();
#endif
  ws.cleanupClients();
  if (!WiFi.isConnected() && WiFi.getMode() == WIFI_STA)
  {
    if (!USE_SOFT_AP)
    {
      if (!wifiConnect(true))
        delay(5000);
    }
    else
      wifiConnect(true);
  }
  if (recon)
  {
    wifiConnect(true);
    recon = false;
  }
}