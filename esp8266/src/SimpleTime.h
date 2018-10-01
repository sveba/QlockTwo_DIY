//
// Created by Jonas Lauener on 21.09.18.
//

#ifndef ESP8266_TIME_H
#define ESP8266_TIME_H

#include "WString.h"

class SimpleTime {
private:
    uint16_t hour;
    uint16_t minute;
public:
    SimpleTime() : hour(0), minute(0) {}

    SimpleTime(SimpleTime const &_simpleTime) : hour(_simpleTime.hour), minute(_simpleTime.minute) {}

    SimpleTime(uint16_t _hour, uint16_t _minute) : hour(_hour), minute(_minute) {}

    SimpleTime(String timeString) : SimpleTime(SimpleTime::parse(timeString)) {}

    uint16_t getHour() const {
        return hour;
    }

    void setHour(uint16_t hour) {
        SimpleTime::hour = hour;
    }

    uint16_t getMinute() const {
        return minute;
    }

    void setMinute(uint16_t minute) {
        SimpleTime::minute = minute;
    }

    const int totalMinutes() const {
        return hour * 60 + minute;
    }

    operator int() const
    {
        return totalMinutes();
    }

    static SimpleTime parse(String timeString) {
        SimpleTime time;

        char string[10];
        strcpy(string, timeString.c_str());

        sscanf(timeString.c_str(), "%hu:%hu", &time.hour, &time.minute);

        return time;
    }

    String toString() const {
        char str[6];
        sprintf(str, "%02hu:%02hu", hour, minute);
        return String(str);
    }
};

#endif //ESP8266_TIME_H
