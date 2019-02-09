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

void WifiModule::setup(void (*configModeCallback)(WiFiManager *myWiFiManager), void (*saveConfig)(void)) {
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfig);

    // timeout for case of power outage, retry connect after 180s
    wifiManager.setConfigPortalTimeout(180);

    wifiManager.addParameter(&parameterDisableTime);
    wifiManager.addParameter(&parameterEnableTime);
  //void (WifiModule::*saveConfig)(WiFiManager *myWiFiManager);
  //saveConfig = &WifiModule::configModeCallback;
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

SimpleTime WifiModule::getEnableTime() {
    return SimpleTime::parse(parameterEnableTime.getValue());
}

SimpleTime WifiModule::getDisableTime() {
    return SimpleTime::parse(parameterDisableTime.getValue());
}