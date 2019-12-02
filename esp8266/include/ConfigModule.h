//
// Created by Jonas Lauener on 21.09.18.
//

#ifndef ESP8266_CONFIGMODULE_H
#define ESP8266_CONFIGMODULE_H

#include "FS.h"

#include "SimpleTime.h"
#include <ArduinoJson.h>

static const String ENABLE_TIME_ID = "enable_time";
static const String DISABLE_TIME_ID = "disable_time";
static const String SET_LED_COLOR_ID = "set_led_color";
static const String BRIGHTNESS_CORRECTION_ID = "brightness_correction";

struct Config {
    SimpleTime enableTime;
    SimpleTime disableTime;
    int setLedColor;
    int brightnessCorrection;
};

/**
 * Save and Load Config to/from SPIFF of the ESP.
 */
class ConfigModule {
private:
    const String configFilePath;
    Config parseJsonConfig(const JsonDocument &jsonObject);
    void copyToJsonConfig(const Config &config, JsonDocument &doc);
public:
    ConfigModule(String _configFilePath);
    void setup();
    bool saveConfig(const Config &config);
    const Config loadConfig();
};


#endif //ESP8266_CONFIGMODULE_H
