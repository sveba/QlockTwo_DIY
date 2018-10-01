#include <Arduino.h>
#include <Ticker.h>
#include "LedControlModule.h"
#include "ClockModule.h"
#include "WifiModule.h"
#include "Settings.h"
#include "AceButton.h"
#include "SimpleTime.h"
#include "ConfigModule.h"

using namespace ace_button;

// Member Variables
NeoTopology<MyPanelLayout> topo(PANEL_WIDTH, PANEL_HEIGHT);
LedControlModule ledControlModule(topo);
NeoPixelBusType pixelStrip(PIXEL_COUNT);

ClockModule clockModule(Wire, CLOCK_UPDATE_INTERVAL);

WifiModule wifiModule(DEVICE_NAME);

ConfigModule configModule(CONFIG_FILE_PATH);

AceButton buttonOne(new ButtonConfig());
AceButton buttonTwo(new ButtonConfig());
AceButton buttonThree(new ButtonConfig());
AceButton buttonFour(new ButtonConfig());

Ticker showTimeTicker;
Ticker updateTimeTicker;

Config config;

// Function Declaration
void handleButtonOneEvent(AceButton*, uint8_t, uint8_t);
void handleButtonTwoEvent(AceButton*, uint8_t, uint8_t);
void handleButtonThreeEvent(AceButton*, uint8_t, uint8_t);
void handleButtonFourEvent(AceButton*, uint8_t, uint8_t);

void updateClock();

void setButtonConfig(ButtonConfig* buttonConfig, ButtonConfig::EventHandler eventHandler);
void setupButtons();
void showTime();

void saveConfigCallback();

void printDateTime(const RtcDateTime &dt);

void setup() {
    Serial.begin(9600);

    pinMode(BUILTIN_LED, OUTPUT);

    Serial.println("Setup start.");

    /*String timeString = "3:55";
    uint16_t a, b;
    sscanf(timeString.c_str(), "%hu:%hu", &a, &b);
    Serial.println(String("test: ") + String(a) + "::" + String(b));

    SimpleTime simpleTime("3:55");
    Serial.println(String("simpleTime: ") + String(simpleTime.getHour()) + "::" + String(simpleTime.getMinute()));
    String s = simpleTime.toString();
    Serial.println(s);*/

    configModule.setup();
    config = configModule.loadConfig();
    Serial.println("Config loaded:");
    Serial.println("Config.enableTime: " + config.enableTime.toString());
    Serial.println("Config.disableTime: " + config.disableTime.toString());

    /*SimpleTime st;
    st = parse("21:30");
    Serial.println("hour: " + String(st.tm_hour));
    Serial.println("minute: " + String(st.tm_min));
    Serial.println("simpleTime: " + toString(st));*/

    /*Config config;
    config.enableTime = parseSimpleTime("07:30");
    config.disableTime = parse("21:30");

    Serial.println("saveConfig:");
    configModule.saveConfig(config);*/

    Serial.println("loadConfig:");
    Config readConfig = configModule.loadConfig();

    setupButtons();

    clockModule.setup(CLOCK_TIMEZONE_OFFSET);

    wifiModule.setup(saveConfigCallback);
    //wifiModule.reset();
    wifiModule.connect();

    ledControlModule.setup(&pixelStrip);

    showTimeTicker.attach(TIME_UPDATE_INTERVAL, showTime);
    updateTimeTicker.attach(CLOCK_UPDATE_INTERVAL, updateClock);

    Serial.println("Setup done.");


}

void setButtonConfig(ButtonConfig* buttonConfig, ButtonConfig::EventHandler eventHandler) {
    buttonConfig->setEventHandler(eventHandler);
    buttonConfig->setFeature(ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
    // buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
}

void setupButtons() {
    pinMode(BUTTON_ONE_PIN, INPUT);
    buttonOne.init(BUTTON_ONE_PIN, LOW);

    pinMode(BUTTON_TWO_PIN, INPUT);
    buttonTwo.init(BUTTON_TWO_PIN, LOW);

    pinMode(BUTTON_THREE_PIN, INPUT);
    buttonThree.init(BUTTON_THREE_PIN, LOW);

    pinMode(BUTTON_FOUR_PIN, INPUT);
    buttonFour.init(BUTTON_FOUR_PIN, LOW);

    setButtonConfig(buttonOne.getButtonConfig(), handleButtonOneEvent);
    setButtonConfig(buttonTwo.getButtonConfig(), handleButtonTwoEvent);
    setButtonConfig(buttonThree.getButtonConfig(), handleButtonThreeEvent);
    setButtonConfig(buttonFour.getButtonConfig(), handleButtonFourEvent);
}

void loop() {
    buttonOne.check();
    buttonTwo.check();
    buttonThree.check();
    buttonFour.check();
}

void saveConfigCallback() {
    Serial.println("Save callback.");
    config = wifiModule.getConfig();
    configModule.saveConfig(config);
}

SimpleTime convert(const RtcDateTime& rtcDateTime) {
    return SimpleTime(rtcDateTime.Hour(), rtcDateTime.Minute());
}

void showTime() {
    Serial.println("disableTime: " + config.disableTime.toString());
    Serial.println("enableTime: " + config.enableTime.toString());

    if(!clockModule.isDateTimeValid()) {
        updateClock();
    }

    const RtcDateTime dt = clockModule.getTime();
    const SimpleTime st = convert(dt);

    if(config.disableTime != config.enableTime &&
        (config.disableTime <= st || config.enableTime >= st)) {
        printDateTime(dt);
        ledControlModule.showTime(dt);
    } else {
        ledControlModule.disableLeds();
    }
}

void updateClock() {
    Serial.println("Connect to wifi and update clock.");
    if (!wifiModule.isConnected()) {
        wifiModule.connect();
    }

    clockModule.update();
}

void handleButtonOneEvent(AceButton* button, uint8_t eventType,
                           uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventClicked:
            Serial.println("Button One Clicked");
            if(showTimeTicker.active()) {
                showTimeTicker.detach();
                ledControlModule.disableLeds();
            } else {
                showTimeTicker.attach(TIME_UPDATE_INTERVAL, showTime);
            }
            break;
        case AceButton::kEventLongPressed:
            Serial.println("Button One Long Press");
            break;
    }
}

void handleButtonTwoEvent(AceButton* button, uint8_t eventType,
                          uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventClicked:
            Serial.println("Button Two Clicked");
            break;
        case AceButton::kEventLongPressed:
            Serial.println("Button Two Long Press");
            break;
    }
}

void handleButtonThreeEvent(AceButton* button, uint8_t eventType,
                          uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventClicked:
            Serial.println("Button Three Clicked");
            break;
        case AceButton::kEventLongPressed:
            Serial.println("Button Three Long Press");
            break;
    }
}

void handleButtonFourEvent(AceButton* button, uint8_t eventType,
                           uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventClicked:
            Serial.println("Button Four Clicked");
            updateClock();
            break;
        case AceButton::kEventLongPressed:
            Serial.println("Button Four Long Press");
            wifiModule.reset();
            wifiModule.connect();
            break;
    }
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
