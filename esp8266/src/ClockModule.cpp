//
//  ClockModule.cpp
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#include "ClockModule.h"

ClockModule::ClockModule(RtcDS3231<TwoWire> _rtc) : rtc(_rtc) {}
ClockModule::~ClockModule() {}

void ClockModule::setup() {
    rtc.Begin();
}
