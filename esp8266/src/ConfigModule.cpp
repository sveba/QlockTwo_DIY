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
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();

    copyToJsonConfig(json, config);

    File configFile = SPIFFS.open(configFilePath, "w");
    if (!configFile) {
        Serial.println("failed to open config file for writing");
        return false;
    }

    json.printTo(Serial);
    json.printTo(configFile);
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
            size_t size = configFile.size();
            // Allocate a buffer to store contents of the file.
            std::unique_ptr<char[]> buf(new char[size]);

            configFile.readBytes(buf.get(), size);
            DynamicJsonBuffer jsonBuffer;
            JsonObject &json = jsonBuffer.parseObject(buf.get());
            json.printTo(Serial);
            if (json.success()) {
                Serial.println("\nparsed json");

                return parseJsonConfig(json);
            } else {
                Serial.println("failed to load json config");
            }
            configFile.close();
        }
    }

    return Config();
}

/**
 * Fill config values from JsonObject to Config struct.
 * @param jsonObject
 * @return Filled Config Object.
 */
Config ConfigModule::parseJsonConfig(const JsonObject &jsonObject) {
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
void ConfigModule::copyToJsonConfig(JsonObject &json, const Config &config) {
    json[ENABLE_TIME_ID] = config.enableTime.toString();
    json[DISABLE_TIME_ID] = config.disableTime.toString();
    json[SET_LED_COLOR_ID] = config.setLedColor;
    json[BRIGHTNESS_CORRECTION_ID] = config.brightnessCorrection;
}
