#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> // needed?
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NeoPixelBus.h>
#include <WiFiManager.h>

void setup() {

}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setupWifiManager() {
  SDA;
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);

  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
}

void loop() {

}
