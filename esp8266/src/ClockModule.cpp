//
//  ClockModule.cpp
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#include "ClockModule.h"
#include <Arduino.h>

#ifndef countof
#define countof(array) (sizeof(array)/sizeof(array[0]))
#endif

/**
 * ClockModule is handling the RTC and keeps it updated over NTP.
 * @param _rtc
 * @param _updateInterval Interval of updating RTC in Seconds
 */
ClockModule::ClockModule(RtcDS3231<TwoWire> _rtc, long _updateInterval) : rtc(_rtc), timeClient(ntpUDP) {}
ClockModule::~ClockModule() {}

void ClockModule::setup(int timeOffset) {
    timeClient.begin();
    timeClient.setTimeOffset(7200);

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
bool ClockModule::isDateTimeValid() {
    return rtc.IsDateTimeValid();
}

void printDateTimel(const RtcDateTime &dt) {
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.println(datestring);
}

/**
 * Get current time from NTP server and update RTC.
 * @return true if updating successful
 */
void ClockModule::update() {
    Serial.println("ClockModule: Update Clock.");

    timeClient.forceUpdate();
    Serial.println("real NtpTime: " + timeClient.getFormattedTime());
    long ntpTime = timeClient.getEpochTime();

    RtcDateTime ntpRtcDateTime;
    ntpRtcDateTime.InitWithEpoch32Time(ntpTime);
    Serial.print("Converted NTPtime: ");
    printDateTimel(ntpRtcDateTime);

    rtc.SetDateTime(ntpRtcDateTime);
}

/*
 * Get current time from RTC.
 */
RtcDateTime ClockModule::getTime() {
    return rtc.GetDateTime();
}

RtcDateTime ClockModule::getLocalTime() {

    return RtcDateTime();
}
