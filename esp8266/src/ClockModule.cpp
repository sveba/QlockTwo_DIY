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
ClockModule::ClockModule(RtcDS3231<TwoWire> _rtc, Timezone _localTZ) : rtc(_rtc), localTZ(_localTZ) {}
ClockModule::~ClockModule() {}

void ClockModule::setup() {
    ntpUDP.begin(localPort);

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

void printDateTime(const RtcDateTime &dt) {
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

void printDateTime(const time_t &dt) {
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               month(dt),
               day(dt),
               year(dt),
               hour(dt),
               minute(dt),
               second(dt));
    Serial.println(datestring);
}

/**
 * Get current time from NTP server and update RTC.
 * @return true if updating successful
 */
void ClockModule::update() {
    Serial.println("ClockModule: Update Clock.");

    int ntpAttempt = 0;
    time_t ntpTime = 0;
    while (ntpTime == 0 && ntpAttempt < 10) {
        ntpTime = getNtpTime();
        ntpAttempt++;
    }

    if(ntpTime == 0) {
        Serial.println("Update Clock FAILED");
        return;
    }

    printDateTime(ntpTime);
    Serial.println(ntpTime);

    RtcDateTime ntpRtcDateTime;
    ntpRtcDateTime.InitWithEpoch32Time(ntpTime);
    Serial.print("Converted NTPtime: ");
    printDateTime(ntpRtcDateTime);

    rtc.SetDateTime(ntpRtcDateTime);
}

SimpleTime convertToSimpleTime(const time_t &time) {
    return SimpleTime(hour(time), minute(time));
}

/*
 * Get current time from RTC.
 */
time_t ClockModule::getUtcTime() {
    return rtc.GetDateTime().Epoch32Time();
}

SimpleTime ClockModule::getLocalSimpleTime() {
    return convertToSimpleTime(localTZ.toLocal(getUtcTime()));
}

/*-------- NTP code ----------*/
// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//const int timeZone = 1;     // Central European Time

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t ClockModule::getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (ntpUDP.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  this->sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = ntpUDP.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      ntpUDP.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void ClockModule::sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ntpUDP.beginPacket(address, 123); //NTP requests are to port 123
  ntpUDP.write(packetBuffer, NTP_PACKET_SIZE);
  ntpUDP.endPacket();
}
