#include "wifiHelper.h"

AsyncWebServer server(PORT);
WebSocketsClient* syncer;
#ifdef USEUPNP
AsyncWebServer wanServer(UPNP_PORT);
TinyUPnP *tinyUPnP = new TinyUPnP(20000);
AsyncWebSocket wws("/ws");
#endif
AsyncWebSocket ws("/ws");
AnimationHelper *strp;
bool recon = false;
bool timers = false;
bool sync_ = false;
int syncNum = 0;


Timer* timerOn = nullptr;
Timer* timerOff = nullptr;

void powerOn() {
  strp->powerOn();
  ws.textAll("p:1");
  wws.textAll("p:1");
}
void powerOff() {
  strp->powerOff();
  ws.textAll("p:0");
  wws.textAll("p:0");
}

bool wifiConnect(bool showLeds)
{
  Serial.println("wifi connect");
  uint32_t c = strp->getColor();
  bool res = true;
  if (showLeds)
  {
    strp->setColor(0x000077, true);
  }
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(DEVICE_NAME);
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
  else {
    #ifdef USEUPNP
      tinyUPnP->addPortMappingConfig(WiFi.localIP(), UPNP_PORT, RULE_PROTOCOL_TCP, LEASE_DURATION, DEVICE_NAME);
      tinyUPnP->commitPortMappings();
      #endif
      #ifdef USE_DDNS
        String name = DEVICE_NAME;
        name.toLowerCase();
        while (name.indexOf(' ') > 0)
        name.remove(name.indexOf(' '), 1);
        EasyDDNS.service(DDNS_SERVICE);
        EasyDDNS.client(name + ".duckdns.org", DDNS_TOKEN);
        EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP){
          Serial.print("EasyDDNS - IP Change Detected: ");
          Serial.println(newIP);
      });
        #endif
  }
  if (showLeds)
  {
    if (res)
      strp->setColor(0x00FF00, true);
      WiFi.setHostname(DEVICE_NAME);
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
  SPIFFS.begin(false);
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
  server.on("/manifest.json", handleManifest);
  server.onNotFound(sendFile);
  server.begin();
  #ifdef USEUPNP
  wanServer.on("/", handleIndex);
  wanServer.on("/manifest.json", handleManifest);
  wanServer.onNotFound(sendFile);
  wanServer.begin();
  wws.onEvent(wsOnEvent);
  wanServer.addHandler(&wws);
  #endif
  ws.onEvent(wsOnEvent);
  server.addHandler(&ws);
  MDNS.addService("http", "tcp", PORT);
  MDNS.addService("JLED", "udp", SYNCPORT);
  tm timeOn = getTimer(SPIFFS, "on");
  tm timeOff = getTimer(SPIFFS, "off");
  if(timeOn.tm_hour != 69) {
    timerOn = new Timer(timeOn, powerOn);
    timerOn->begin();
    timers = true;
  }
  if(timeOff.tm_hour != 69) {
    timerOff = new Timer(timeOff, powerOff);
    timerOff->begin();
    timers = true;
  }

  return res;
}

void handleIndex(AsyncWebServerRequest *req)
{
  if(SPIFFS.exists("/ui.html"))req->send(SPIFFS, "/ui.html", "text/html");
  else req->send(404, "text", "ui.html not found");
}

void handleManifest(AsyncWebServerRequest *req)
{
  File m = SPIFFS.open("/manifest.json");
  String data;
  while(m.available()) {
    char c = m.read();
    if(c == '|') data += DEVICE_NAME;
    else data += c;
  }
  req->send(200, "text/json", data);
}



void sendFile(AsyncWebServerRequest *req)
{
  String url = req->url();
  String type = "text/";
  if(url.indexOf('.') >= 0) type.concat(url.substring(url.indexOf('.')+1));
  else type.concat("html");
  if(SPIFFS.exists(url))req->send(SPIFFS, url, type);
  else req->send(404, "text", url+" not found");
}

void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    updateData(server);
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len, client->id(), server);
    break;
  case WS_EVT_PONG:
    break;
  case WS_EVT_ERROR:
    break;
  }
}

void updateSync() {
  int l = syncNum;
      syncNum = MDNS.queryService("JLED", "udp");
      if(syncNum != l) {
        if(syncer != nullptr) for(int i = 0; i < l; i++) syncer[i].disconnect();
        delete syncer;
        syncer = new WebSocketsClient[syncNum]();
        for(int i = 0; i < syncNum; i++) {
          syncer[i].begin(MDNS.IP(i), 80, "/ws");
          syncer[i].setReconnectInterval(5000);
        }
      }
}

void parseMsg(String msg) {
  String value = msg.substring(msg.indexOf(":") + 1);
    if (msg.startsWith("p:"))
    {
      // power = msg.substring(msg.indexOf(":")+1).equals("true");
      // if(animation.equals("none")) upd = true;
      strp->setPower(value.equals("true"));
    }
    if (msg.startsWith("c:"))
    {
      String color = msg.substring(msg.indexOf(":") + 2);
      int red = strtoul(color.substring(0, 2).c_str(), NULL, 16);
      int green = strtoul(color.substring(2, 4).c_str(), NULL, 16);
      int blue = strtoul(color.substring(4).c_str(), NULL, 16);
      strp->setColor(red, green, blue, true);
    }
    if (msg.startsWith("ca:"))
    {
      msg = msg.substring(msg.indexOf(":")+1);
      int colorNum = msg.substring(0, msg.indexOf(":")).toInt();
      String color = msg.substring(msg.indexOf(":") + 2);
      int red = strtoul(color.substring(0, 2).c_str(), NULL, 16);
      int green = strtoul(color.substring(2, 4).c_str(), NULL, 16);
      int blue = strtoul(color.substring(4).c_str(), NULL, 16);
      if(colorNum == 1)strp->setPrimeAnimColor(red, green, blue);
      else strp->setSecAnimColor(red, green, blue);
    }
    if (msg.startsWith("a:"))
    {
      // ledColor(0,0,0);
      // lAnim = animation;
      if(value.toInt() <= strp->getNumberAnimations())strp->setAnimation(value.toInt());
      //Serial.println(strp->getAnimation());
    }
    if (msg.startsWith("b:"))
    {
      // brightness = msg.substring(msg.indexOf(":")+1).toInt();
      // FastLED.setBrightness(brightness);
      // upd = animation.equals("none");
      strp->setBrightness(value.toInt());
    }
    if (msg.startsWith("w:"))
    {
      if (value.equals("recon"))
      {
        recon = true;
      }
    }
    if (msg.startsWith("s:"))
    {
      strp->setSpeed((byte)value.toInt());
    }
    if(msg.startsWith("t:")) {
      //t:mode:hh:mm
      msg = msg.substring(msg.indexOf(':') + 1);
      String type = msg.substring(0, msg.indexOf(":"));
      msg = msg.substring(msg.indexOf(':') + 1);
      if(type.equals("on")) {
        tm time;
        time.tm_hour = msg.substring(0, msg.indexOf(':')).toInt();
        time.tm_min = msg.substring(msg.indexOf(':')+1).toInt();
        timerOn = new Timer(time, powerOn);
        saveTimer(SPIFFS, "on", time);
      }
      else if(type.equals("off")) {
        tm time;
        time.tm_hour = msg.substring(0, msg.indexOf(':')).toInt();
        time.tm_min = msg.substring(msg.indexOf(':')+1).toInt();
        timerOff = new Timer(time, powerOff);
        saveTimer(SPIFFS, "off", msg.substring(0, msg.indexOf(':')).toInt(), msg.substring(msg.indexOf(':')+1).toInt());
      }
      else if(type.equals("toggle")) {
        if(timerOn != nullptr && timerOff != nullptr) {
          if(msg.equals("true")) {
            timerOn->begin();
            timerOff->begin();
            timers = true;
          }
          else {
            timerOn->end();
            timerOff->end();
            timers = false;
          }
        }
      }
    }
    if(msg.startsWith("sync:")) {
      if(value.equals("true")) {
        sync_ = true;
        updateSync();
      }
      else {
        sync_ = false;
      }
    }
    #ifdef USEMPU
    if (msg.startsWith("sleep:"))
    {
      strp->setColor(0, true);
      WiFi.mode(WIFI_OFF); //idk if this will make esp draw less current in sleep but cant hurt
      #ifdef ESP32C3
      esp_deep_sleep_enable_gpio_wakeup((1ULL << MPUINT), ESP_GPIO_WAKEUP_GPIO_HIGH);
      #endif
      #ifdef ESP32S2
      esp_sleep_enable_ext0_wakeup(MPUINT, 1);
      #endif
      #ifdef ESP32DEV
      esp_sleep_enable_ext0_wakeup(MPUINT, 1);
      #endif
      delay(50); //let pixels turn off
      esp_deep_sleep_start();
    }
    #endif
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, uint32_t id, AsyncWebSocket* server)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    
    data[len] = 0;
    String msg = String((char *)data);
    bool relay = true;
    if(msg.startsWith("j:")) {
      if(!sync_) return;
      msg = msg.substring(msg.indexOf(":") + 1);
      relay = false;
    }
    if (msg.equals("getAnimations"))
    {
      for (int i = 0; i < strp->getNumberAnimations(); i++)
        server->text(id, "a:" + *(strp->getAnimationNames()[i]));
    }
    else if(msg.equals("update")) {
      updateClients();
    }
    else {
      if(sync_ && relay) 
        {
        for(int i = 0; i < syncNum; i++) syncer[i].sendTXT("j:" + msg);
        }
        parseMsg(msg);
    }
  }
}

String colorToHex(uint32_t color) {
  byte r = color >> 16;
  byte g = color >> 8;
  byte b = color;
  String index;
  if (r < 16)
    index += "0";
  index += String(r, HEX);
  if (g < 16)
    index += "0";
  index += String(g, HEX);
  if (b < 16)
    index += "0";
  index += String(b, HEX);
  return index;
}

void updateData(AsyncWebSocket* server)
{
  server->textAll("p:" + String(strp->getPower()));
  server->textAll("c:#" + colorToHex(strp->getColor()));
  server->textAll("ca:1:#" + colorToHex(strp->getPrimeAnimColor()));
  server->textAll("ca:2:#" + colorToHex(strp->getSecAnimColor()));
  server->textAll("b:" + String(strp->getBrightness()));
  server->textAll("s:" + String(strp->getSpeed()*255));
  if (WiFi.getMode() == WIFI_AP)
    server->textAll("w:AP");
  else
    server->textAll("w:STA");
  #ifdef BATTPIN
  server->textAll("type:battery");
  sendBattery();
  #else
  server->textAll("type:wall");
  #endif
  server->textAll("n:" + String(DEVICE_NAME));
  if(SPIFFS.exists("/on.tmr")) {
    tm time = getTimer(SPIFFS, "on");
    String temp = "t:on:";
    if(time.tm_hour < 10) temp += "0";
    temp += time.tm_hour;
    temp += ':';
    if(time.tm_min < 10) temp += "0";
    temp += time.tm_min;
    server->textAll(temp);
    server->textAll("print:" + SPIFFS.open("/on.tmr").readString());
  }
  if(SPIFFS.exists("/off.tmr")) {
    tm time = getTimer(SPIFFS, "off");
    String temp = "t:off:";
    if(time.tm_hour < 10) temp += "0";
    temp += time.tm_hour;
    temp += ':';
    if(time.tm_min < 10) temp += "0";
    temp += time.tm_min;
    server->textAll(temp);
  }
  server->textAll("t:toggle:" + String(timers));
  server->textAll("sync:"+String(sync_));
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
  #ifdef USEUPNP
  wws.textAll("batt:" + String(percent));
  #endif
}
#endif
void updateClients() {
  updateData(&ws);
  #ifdef USEUPNP
  updateData(&wws);
  #endif
}
void handleWiFi()
{

  static unsigned long t = millis();
  if (millis() - t > 30000)
  {
    #ifdef BATTPIN
    sendBattery();
    #endif
    if(sync_)updateSync();
    t = millis();
  }
#ifdef USEOTA
  ArduinoOTA.handle();
#endif
#ifdef USEUPNP
tinyUPnP->updatePortMappings(600000);
#endif
#ifdef USE_DDNS
EasyDDNS.update(10000);
#endif
  ws.cleanupClients();
  if(sync_) for(int i = 0; i < syncNum; i++) syncer[i].loop();
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