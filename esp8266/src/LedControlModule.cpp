//
//  LedControlModule.cpp
//  esp8266
//
//  Created by <author> on 10/02/2018.
//
//

#include "LedControlModule.h"

LedControlModule::LedControlModule(NeoTopology<MyPanelLayout> _topo) : topo(_topo) {}
LedControlModule::~LedControlModule() {}

void LedControlModule::setup(NeoPixelBusType* _pixelStrip) {
    pixelStrip = _pixelStrip;
    pixelStrip->Begin();
    pixelStrip->Show();
};

void LedControlModule::showTime(const SimpleTime simpleTime, RgbwColor ledColor){
    pixelStrip->ClearTo(RgbwColor(0));
    enableLedWords(simpleTime, ledColor);

    int minuteDots = simpleTime.getMinute() % 5;
    enableMinuteDots(minuteDots, ledColor);
    pixelStrip->Show();
};

void LedControlModule::enableLedWords(const SimpleTime simpleTime, RgbwColor& ledColor) {
    enableLedWord(&PREFIX_IT, ledColor);
    enableLedWord(&PREFIX_IS, ledColor);
    int fiveminutes = simpleTime.getMinute() / 5;

    switch(fiveminutes){
        case 0:
            enableLedWord(&SUFFIX_OCLOCK, ledColor);
            break;
        case 1:
            enableLedWord(&MINUTE_FIVE, ledColor);
            enableLedWord(&INFIX_AFTER, ledColor);
            break;
        case 2:
            enableLedWord(&MINUTE_TEN, ledColor);
            enableLedWord(&INFIX_AFTER, ledColor);
            break;
        case 3:
            enableLedWord(&MINUTE_QUARTER, ledColor);
            enableLedWord(&INFIX_AFTER, ledColor);
            break;
        case 4:
            enableLedWord(&MINUTE_TWENTY, ledColor);
            enableLedWord(&INFIX_AFTER, ledColor);
            break;
        case 5:
            enableLedWord(&MINUTE_FIVE, ledColor);
            enableLedWord(&INFIX_BEFORE, ledColor);
            enableLedWord(&MINUTE_HALF, ledColor);
            break;
        case 6:
            enableLedWord(&MINUTE_HALF, ledColor);
            break;
        case 7:
            enableLedWord(&MINUTE_FIVE, ledColor);
            enableLedWord(&INFIX_AFTER, ledColor);
            enableLedWord(&MINUTE_HALF, ledColor);
            break;
        case 8:
            enableLedWord(&MINUTE_TWENTY, ledColor);
            enableLedWord(&INFIX_BEFORE, ledColor);
            break;
        case 9:
            enableLedWord(&MINUTE_THREEQUARTER, ledColor);
            break;
        case 10:
            enableLedWord(&MINUTE_TEN, ledColor);
            enableLedWord(&INFIX_BEFORE, ledColor);
            break;
        case 11:
            enableLedWord(&MINUTE_FIVE, ledColor);
            enableLedWord(&INFIX_BEFORE, ledColor);
            break;
    }

    //enableLedWord(&HOURS[5], ledColor);

    if (fiveminutes <= 5){
      enableLedWord(&HOURS[simpleTime.getHour() - 1], ledColor);
    } else{
      enableLedWord(&HOURS[simpleTime.getHour()], ledColor);
    } 
};

void LedControlModule::enableLedWord(const LedWord* ledWord, RgbwColor& ledColor) {
    for (int j = 0; j < ledWord->getLength(); j++) {
        pixelStrip->SetPixelColor(topo.Map(ledWord->getFirstPixelX() + j, ledWord->getFirstPixelY()), ledColor);
    }
}

void LedControlModule::enableMinuteDots(int n, RgbwColor& ledColor) {
    for (int i = 1; i <= n; i++) {
        int j = 111 + ((i + 3) % 4);
        pixelStrip->SetPixelColor(j, ledColor);
    }
}

void LedControlModule::disableLeds() {
    pixelStrip->ClearTo(RgbwColor(0));
    pixelStrip->Show();

//    state = DISABLED;
};
