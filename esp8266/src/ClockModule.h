//
//  ClockModule.h
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#ifndef ClockModule_H
#define ClockModule_H

#include <stdio.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class ClockModule {
private:
  RtcDS3231<TwoWire> rtc;
  WiFiUDP ntpUDP;
  NTPClient timeClient;
public:
    ClockModule(RtcDS3231<TwoWire> _rtc, long _updateInterval);
    ~ClockModule();
    void setup(int timeOffset);
    bool isDateTimeValid();
    void update();
    RtcDateTime getTime();
    RtcDateTime getLocalTime();
protected:

};


#endif /* ClockModule_h */
