//
//  LedControlModule.h
//  esp8266
//
//  Created by <author> on 10/02/2018.
//
//

#ifndef LedControlModule_H
#define LedControlModule_H

#include <stdio.h>
#include <NeoPixelBus.h>
#include <RtcDateTime.h>
#include "LedWord.h"

typedef  NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>  NeoPixelBusType;

class LedControlModule {
private:
    NeoPixelBusType* pixelStrip;
    LedWord[] getTimeAffixes();
    LedWord getHourWord(RtcDateTime rtcDateTime);
    LedWord[] getMinuteWords(RtcDateTime rtcDateTime);
    LedWord[] getMinuteDots(RtcDateTime rtcDateTime);
public:
    LedControlModule();
    ~LedControlModule();
    void setup(NeoPixelBusType& pixelStrip);
    void setTime(RtcDateTime rtcDateTime);
protected:

};


#endif /* LedControlModule_h */
