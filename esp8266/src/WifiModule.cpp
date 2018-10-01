//
//  WifiModule.cpp
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#include "WifiModule.h"

WifiModule::WifiModule(String _deviceName) : deviceName(_deviceName) { }

WifiModule::~WifiModule() {}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void WifiModule::setup(void (*func)(void)) {
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(func);

    wifiManager.addParameter(&parameterDisableTime);
    wifiManager.addParameter(&parameterEnableTime);
  //void (WifiModule::*func)(WiFiManager *myWiFiManager);
  //func = &WifiModule::configModeCallback;
}

bool WifiModule::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WifiModule::connect() {
    Serial.println("WifiModule: Connect.");
    if(!wifiManager.autoConnect(this->deviceName.c_str())) {
        Serial.println("failed to connect and hit timeout");
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(1000);

        return false;
    }

    Serial.println("connected...yeey :)");

    return true;
}

void WifiModule::reset() {
    Serial.println("WifiModule: Reset.");
    wifiManager.resetSettings();
}

Config WifiModule::getConfig() {
    Config config;
    config.enableTime = SimpleTime::parse(parameterEnableTime.getValue());
    config.disableTime = SimpleTime::parse(parameterDisableTime.getValue());

    Serial.println("Config.enableTime: " + config.enableTime.toString());
    Serial.println("Config.disableTime: " + config.disableTime.toString());
    return config;
}
