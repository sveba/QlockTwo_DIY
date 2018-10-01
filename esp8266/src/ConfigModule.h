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

struct Config {
    SimpleTime enableTime;
    SimpleTime disableTime;
};

class ConfigModule {
private:
    const String configFilePath;
    Config parseJsonConfig(const JsonObject &jsonObject);
    void copyToJsonConfig(JsonObject& json, const Config &config);
public:
    ConfigModule(String _configFilePath);
    void setup();
    bool saveConfig(const Config &config);
    const Config& loadConfig();
};


#endif //ESP8266_CONFIGMODULE_H
