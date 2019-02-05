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
#include <Timezone.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "SimpleTime.h"

class ClockModule {
private:
  RtcDS3231<TwoWire> rtc;
  Timezone localTZ;
  WiFiUDP ntpUDP;
  unsigned int localPort = 8888; // local port to listen for UDP packets
  time_t getNtpTime();
  void sendNTPpacket(IPAddress &address);
public:
    ClockModule(RtcDS3231<TwoWire> _rtc, Timezone _localTZ);
    ~ClockModule();
    void setup();
    bool isDateTimeValid();
    void update();
    time_t getUtcTime();
    SimpleTime getLocalSimpleTime();

protected:

};


#endif /* ClockModule_h */
