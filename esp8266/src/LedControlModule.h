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
#include "LedMapping.h"

typedef  NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>  NeoPixelBusType;
typedef RowMajorAlternating270Layout MyPanelLayout;

class LedControlModule {
private:
    NeoPixelBusType* pixelStrip;
    NeoTopology<MyPanelLayout> topo;
    void enableLedWords(const RtcDateTime rtcDateTime, RgbwColor& ledColor);
    void enableLedWord(const LedWord* ledWord, RgbwColor& ledColor);
    void enableMinuteDots(int n, RgbwColor& ledColor);
public:
    LedControlModule(NeoTopology<MyPanelLayout> _topo);
    ~LedControlModule();
    void setup(NeoPixelBusType* _pixelStrip);
    void setTime(const RtcDateTime rtcDateTime, RgbwColor _ledColor = RgbwColor(0, 0, 0, 255));
};


#endif /* LedControlModule_h */
