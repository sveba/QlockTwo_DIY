//
// Created by Jonas Lauener on 21.09.18.
//

#include "ConfigModule.h"

ConfigModule::ConfigModule(String _configFilePath) : configFilePath(_configFilePath) {};

void ConfigModule::setup() {
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
    }

    //SPIFFS.format();
}

/**
 * Serialize Config to JSON and save to SPIFF.
 * @param config
 * @return true on success
 */
bool ConfigModule::saveConfig(const Config &config) {
    Serial.println("saving config");
    const size_t capacity = JSON_OBJECT_SIZE(10);
    DynamicJsonDocument doc(capacity);

    copyToJsonConfig(config, doc);

    File configFile = SPIFFS.open(configFilePath, "w");
    if (!configFile) {
        Serial.println("failed to open config file for writing");
        return false;
    }

    serializeJson(doc, Serial);
    serializeJson(doc, configFile);

    configFile.close();
    //end save

    return true;
}

/**
 * Load config JSON from SPIFF and deserialize it.
 * @return On success: Saved Config; On Failure: Empty Config
 */
const Config ConfigModule::loadConfig() {
    if (SPIFFS.exists(configFilePath)) {
        //file exists, reading and loading
        Serial.println("reading config file");
        File configFile = SPIFFS.open(configFilePath, "r");
        if (configFile) {
            Serial.println("opened config file");

            const size_t capacity = JSON_OBJECT_SIZE(10) + 90;
            DynamicJsonDocument doc(capacity);
            DeserializationError error = deserializeJson(doc, configFile);

            if (error) {
                Serial.println("failed to load json config");
                Serial.println(error.c_str());
            }

            configFile.close();
            return parseJsonConfig(doc);
        }
    }

    return Config();
}

/**
 * Fill config values from JsonObject to Config struct.
 * @param jsonObject
 * @return Filled Config Object.
 */
Config ConfigModule::parseJsonConfig(const JsonDocument &jsonObject) {
    Config config;

    config.enableTime = SimpleTime::parse(jsonObject[ENABLE_TIME_ID]);
    config.disableTime = SimpleTime::parse(jsonObject[DISABLE_TIME_ID]);
    config.setLedColor = jsonObject[SET_LED_COLOR_ID];
    config.brightnessCorrection = jsonObject[BRIGHTNESS_CORRECTION_ID];

    return config;
}

/**
 *
 */

/**
 * Copy Config values to JsonObject
 * @param json To be filled JsonObject, by reference
 * @param config Source Config
 */
void ConfigModule::copyToJsonConfig(const Config &config, JsonDocument &doc) {
    doc[DISABLE_TIME_ID] = config.disableTime.toString();
    doc[ENABLE_TIME_ID] = config.enableTime.toString();
    doc[SET_LED_COLOR_ID] = config.setLedColor;
    doc[BRIGHTNESS_CORRECTION_ID] = config.brightnessCorrection;
}
