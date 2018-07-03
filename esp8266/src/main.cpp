#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> // needed?
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> // needed?
#include <NeoPixelBus.h>
#include <WiFiManager.h>
#include "LedControlModule.h"

const uint8_t PanelWidth = 11;  // 8 pixel x 8 pixel matrix of leds
const uint8_t PanelHeight = 10;
const uint16_t PixelCount = PanelWidth * PanelHeight + 4;

NeoTopology<MyPanelLayout> topo(PanelWidth, PanelHeight);
LedControlModule ledControlModule(topo);
NeoPixelBusType pixelStrip(PixelCount);

/*
//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setupWifiManager() {
  WiFiManager wifiManager;
  //reset settings - for testing
  wifiManager.resetSettings();

  wifiManager.setAPCallback(configModeCallback);

  if(!wifiManager.autoConnect("QlockTwo No.1")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  Serial.println("connected...yeey :)");
}*/

//RtcDS3231<TwoWire> Rtc(Wire);

RgbColor white(128);

void setup() {
  Serial.begin(9600);

  pinMode(BUILTIN_LED, OUTPUT);
  //setupWifiManager();


 /* pixelStrip.Begin();
  pixelStrip.Show();*/

  ledControlModule.setup(&pixelStrip);
}

void loop() {
 /* pixelStrip.SetPixelColor(topo.Map(0, 0), white);
  pixelStrip.Show();*/

  const RtcDateTime time(2018, 6, 29, 10, 52, 0);
  ledControlModule.setTime(time);

  Serial.println("loop start");
  digitalWrite(BUILTIN_LED, HIGH);
  delay(1000);
  digitalWrite(BUILTIN_LED, LOW);
  delay(1000);
}
