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

ClockModule clockModule(Wire, LOCAL_TIMEZONE);

WifiModule wifiModule(DEVICE_NAME);

ConfigModule configModule(CONFIG_FILE_PATH);

AceButton buttonOne(new ButtonConfig());
AceButton buttonTwo(new ButtonConfig());
AceButton buttonThree(new ButtonConfig());
AceButton buttonFour(new ButtonConfig());

//Ticker showTimeTicker;
//Ticker updateTimeTicker;

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

unsigned long lastClockUpdate = 0;
unsigned long lastShowTime = 0;
bool ledDisabled = false;

time_t timeTester;
SimpleTime simpleTimeTester;

void setup() {
    timeTester = clockModule.getUtcTime();
    simpleTimeTester.setHour(hour(timeTester));
    simpleTimeTester.setMinute(minute(timeTester));

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

    /*configModule.setup();
    config = configModule.loadConfig();
    Serial.println("Config loaded:");
    Serial.println("Config.enableTime: " + config.enableTime.toString());
    Serial.println("Config.disableTime: " + config.disableTime.toString());
*/
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
    configModule.setup();
    config = configModule.loadConfig();

    setupButtons();

    wifiModule.setup(saveConfigCallback);
    //wifiModule.reset();
    wifiModule.connect();

    clockModule.setup();

    ledControlModule.setup(&pixelStrip);

    /*updateClock();
    lastClockUpdate = millis();

    showTime();
    lastShowTime = millis();*/

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

SimpleTime convertToSimpleTimel(const time_t &time) {
    return SimpleTime(hour(time), minute(time));
}

void test() {
    //tt += 60;
    const SimpleTime st = convertToSimpleTimel(timeTester);
    ledControlModule.showTime(st);
    Serial.println("Show Test Time: " + st.toString());
}

void loop() {
    /*Serial.println("millis: " + String(millis()));
    Serial.println("lastClockUpdate: " + String(lastClockUpdate));
    Serial.println("lastClockUpdate diff: " + String(millis() - lastClockUpdate));*/

    if((millis() - lastClockUpdate) > (CLOCK_UPDATE_INTERVAL * 1000)) {
        updateClock();
        lastClockUpdate = millis();
    }

    if((millis() - lastShowTime) > (TIME_UPDATE_INTERVAL * 1000) && !ledDisabled) {
        showTime();
        lastShowTime = millis();
    }

    //showTime();
    //test();

    //delay(100);

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

void showTime() {
    Serial.println("disableTime: " + config.disableTime.toString());
    Serial.println("enableTime: " + config.enableTime.toString());

    if(!clockModule.isDateTimeValid()) {
        updateClock();
    }

    const SimpleTime st = clockModule.getLocalSimpleTime();

    if(config.disableTime == config.enableTime ||
        !(((config.disableTime > config.enableTime) && (config.disableTime <= st && config.enableTime < st)) ||
        ((config.disableTime < config.enableTime) && (config.disableTime <= st && config.enableTime > st)))) {
        Serial.println("Show Time: " + st.toString());
        ledControlModule.showTime(st);
    } else {
        Serial.println("Show Time: LED DISABLED");
        ledControlModule.disableLeds();
    }
}

void updateClock() {
    Serial.println("Connect to wifi and update clock.");
    if (!wifiModule.isConnected()) {
        wifiModule.connect();
    }

    //delay(2000);

    clockModule.update();
}

void handleButtonOneEvent(AceButton* button, uint8_t eventType,
                           uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventClicked:
            Serial.println("Button One Clicked");
            if(!ledDisabled) {
                Serial.println("Disable LED");
                ledControlModule.disableLeds();
                ledDisabled = true;
            } else {
                Serial.println("Enable LED");
                showTime();
                ledDisabled = false;
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
            timeTester += 3600;
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
            delay(1000);
            wifiModule.connect();
            break;
    }
}
