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

    /*RgbwColor color(255);
    enableMinuteDots(2, color);
    pixelStrip->Show();*/
};

void LedControlModule::setTime(const RtcDateTime rtcDateTime, RgbwColor ledColor){
    pixelStrip->ClearTo(RgbwColor(0));
    enableLedWords(rtcDateTime, ledColor);

    int minuteDots = rtcDateTime.Minute() % 5;
    enableMinuteDots(2, ledColor);
    pixelStrip->Show();
};

void LedControlModule::enableLedWords(const RtcDateTime rtcDateTime, RgbwColor& ledColor) {
    enableLedWord(&PREFIX_IT, ledColor);
    enableLedWord(&PREFIX_IS, ledColor);
    switch(rtcDateTime.Minute()/5){
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

    enableLedWord(&HOURS[rtcDateTime.Hour() - 1], ledColor);

};

void LedControlModule::enableLedWord(const LedWord* ledWord, RgbwColor& ledColor) {
    for (int j = 0; j < ledWord->getLength(); j++) {
        pixelStrip->SetPixelColor(topo.Map(ledWord->getFirstPixelX() + j, ledWord->getFirstPixelY()), ledColor);
    }
}

void LedControlModule::enableMinuteDots(int n, RgbwColor& ledColor){
    //pixelStrip->SetPixelColor(114, ledColor);
    for (int i = 1; i <= n; i++) {
        // TODO write it prettier
        switch (i) {
            case 1:
                pixelStrip->SetPixelColor(114, ledColor);
                break;
            case 2:
                pixelStrip->SetPixelColor(111, ledColor);
                break;
            case 3:
                pixelStrip->SetPixelColor(112, ledColor);
                break;
            case 4:
                pixelStrip->SetPixelColor(113, ledColor);
                break;
        }

    }
};
