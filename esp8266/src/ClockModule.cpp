//
//  ClockModule.cpp
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#include "ClockModule.h"

/**
 * ClockModule is handling the RTC and keeps it updated over NTP.
 * @param _rtc
 * @param _updateInterval Interval of updating RTC in Seconds
 */
ClockModule::ClockModule(RtcDS3231<TwoWire> _rtc, long _updateInterval) : rtc(_rtc), updateInterval(_updateInterval), timeClient(ntpUDP) {}
ClockModule::~ClockModule() {}

void ClockModule::setup(int timeOffset) {
    timeClient.begin();
    timeClient.setTimeOffset(timeOffset);

    rtc.Begin();

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    rtc.Enable32kHzPin(false);
    rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

/**
 * Either true if rtc time is not valid or the updateInterval is reached.
 * @return
 */
bool ClockModule::isUpdateNeeded() {
    return !rtc.IsDateTimeValid() || ((getTime().TotalSeconds() - lastUpdated.TotalSeconds()) >= updateInterval);
}

/**
 * Get current time from NTP server and update RTC.
 * @return true if updating successful
 */
void ClockModule::update() {
    Serial.println("ClockModule: Update Clock.");

    timeClient.forceUpdate();
    long ntpTime = timeClient.getEpochTime();

    RtcDateTime ntpRtcDateTime;
    ntpRtcDateTime.InitWithEpoch32Time(ntpTime);

    rtc.SetDateTime(ntpRtcDateTime);

    lastUpdated = ntpRtcDateTime;
}

/*
 * Get current time from RTC.
 */
RtcDateTime ClockModule::getTime() {
    return rtc.GetDateTime();
}
