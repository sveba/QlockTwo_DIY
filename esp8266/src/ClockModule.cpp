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

/**
 * Return when the RTC was updated last.
 * @return RtcDateTime when RTC was updated last.
 */
RtcDateTime ClockModule::lastUpdated() {
    return RtcDateTime();
}

/**
 * Get current time from NTP server and update RTC.
 * @return true if updating successful
 */
bool ClockModule::update() {
    return false;
}

/*
 * Get current time from RTC.
 */
RtcDateTime ClockModule::getTime() {
    return RtcDateTime();
}
