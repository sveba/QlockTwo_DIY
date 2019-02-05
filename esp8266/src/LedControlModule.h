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
#include "SimpleTime.h"

typedef  NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>  NeoPixelBusType;
typedef RowMajorAlternating270Layout MyPanelLayout;

class LedControlModule {
private:
    // enum States { DISABLED, SHOWING_TIME, WIRELESS_ANIMATION, FEEDBACK_ANIMATION };
    NeoPixelBusType* pixelStrip;
    NeoTopology<MyPanelLayout> topo;
    void enableLedWords(const SimpleTime simpleTime, RgbwColor& ledColor);
    void enableLedWord(const LedWord* ledWord, RgbwColor& ledColor);
    void enableMinuteDots(int n, RgbwColor& ledColor);
    // States state;
public:
    LedControlModule(NeoTopology<MyPanelLayout> _topo);
    ~LedControlModule();
    void setup(NeoPixelBusType* _pixelStrip);
    void disableLeds();
    void showTime(const SimpleTime simpleTime, RgbwColor _ledColor = RgbwColor(0, 0, 0, 255));
};


#endif /* LedControlModule_h */
