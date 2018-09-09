//
//  WifiModule.h
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#ifndef WifiModule_H
#define WifiModule_H

#include <stdio.h>
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

class WifiModule {
private:
    String deviceName;
    WiFiManager wifiManager;
    //void configModeCallback (WiFiManager *myWiFiManager);
public:
    WifiModule(String _deviceName);
    ~WifiModule();
    void setup();
    bool isConnected();
    bool connect();
    void reset();
protected:

};


#endif /* WifiModule_h */
