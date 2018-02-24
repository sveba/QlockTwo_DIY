#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> // needed?
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> // needed?
#include <NeoPixelBus.h>
#include <WiFiManager.h>

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
}

void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);
  setupWifiManager();
}

void loop() {
  digitalWrite(BUILTIN_LED, HIGH);
  delay(1000);
  digitalWrite(BUILTIN_LED, LOW);
  delay(1000);
}
