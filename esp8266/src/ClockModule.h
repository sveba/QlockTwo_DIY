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

class ClockModule {
private:
  RtcDS3231<TwoWire> rtc;
public:
    ClockModule(RtcDS3231<TwoWire> _rtc);
    ~ClockModule();
    void setup();
    bool update();
    bool isUpdated();
    RtcDateTime getTime();
protected:

};


#endif /* ClockModule_h */
