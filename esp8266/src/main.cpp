#include <Arduino.h>
#include <Ticker.h>
#include "LedControlModule.h"
#include "ClockModule.h"
#include "WifiModule.h"
#include "Settings.h"
#include "AceButton.h"
using namespace ace_button;

// Member Variables
NeoTopology<MyPanelLayout> topo(PANEL_WIDTH, PANEL_HEIGHT);
LedControlModule ledControlModule(topo);
NeoPixelBusType pixelStrip(PIXEL_COUNT);

ClockModule clockModule(Wire, CLOCK_UPDATE_INTERVAL);

WifiModule wifiModule(DEVICE_NAME);

AceButton buttonOne(new ButtonConfig());
AceButton buttonTwo(new ButtonConfig());
AceButton buttonThree(new ButtonConfig());
AceButton buttonFour(new ButtonConfig());

Ticker showTimeTicker;

// Function Declaration
void handleButtonOneEvent(AceButton*, uint8_t, uint8_t);
void handleButtonTwoEvent(AceButton*, uint8_t, uint8_t);
void handleButtonThreeEvent(AceButton*, uint8_t, uint8_t);
void handleButtonFourEvent(AceButton*, uint8_t, uint8_t);

void updateClock();

void setButtonConfig(ButtonConfig* buttonConfig, ButtonConfig::EventHandler eventHandler);
void setupButtons();
void showTime();

void printDateTime(const RtcDateTime &dt);

void setup() {
    Serial.begin(9600);

    pinMode(BUILTIN_LED, OUTPUT);

    setupButtons();

    clockModule.setup(CLOCK_TIMEZONE_OFFSET);

    wifiModule.setup();
    //wifiModule.reset();
    wifiModule.connect();

    ledControlModule.setup(&pixelStrip);

    showTimeTicker.attach(TIME_UPDATE_INTERVAL, showTime);

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

    if(clockModule.isUpdateNeeded()) {
        updateClock();
    }
}

void showTime() {
    printDateTime(clockModule.getTime());
    ledControlModule.showTime(clockModule.getTime());
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
