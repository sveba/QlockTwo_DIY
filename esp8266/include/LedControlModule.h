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

/**
 * Control the LEDs resp show the words and minute dots.
 */
class LedControlModule {
private:
    NeoPixelBusType* pixelStrip;
    NeoTopology<MyPanelLayout> topo;
    void enableLedWords(const SimpleTime& simpleTime, const RgbwColor &ledColor);
    void enableLedWord(const LedWord* ledWord, const RgbwColor &ledColor);
    void enableMinuteDots(int subMinute, const RgbwColor &ledColor);
public:
    LedControlModule(NeoTopology<MyPanelLayout> _topo);
    ~LedControlModule();
    void setup(NeoPixelBusType* _pixelStrip);
    void disableLeds();
    void showTime(const SimpleTime &simpleTime, const RgbwColor &ledColor = RgbwColor(0, 0, 0, 255));
    void showConfigWifi(const RgbwColor &ledColor = RgbwColor(0, 0, 0, 255));
};


#endif /* LedControlModule_h */
