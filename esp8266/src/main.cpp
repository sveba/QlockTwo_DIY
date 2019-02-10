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

//-----------------------------------------------------
// Member Variables
//-----------------------------------------------------
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

unsigned long lastClockUpdate = 0;
unsigned long lastShowTime = 0;
bool showTimeDisabled = false;

int currentLedColorId = 0;
RgbwColor currentLedColor = LED_COLORS[currentLedColorId];

Config config;

//-----------------------------------------------------
// Function Declarations
//-----------------------------------------------------
void handleButtonOneEvent(AceButton*, uint8_t, uint8_t);
void handleButtonTwoEvent(AceButton*, uint8_t, uint8_t);
void handleButtonThreeEvent(AceButton*, uint8_t, uint8_t);
void handleButtonFourEvent(AceButton*, uint8_t, uint8_t);

void updateClock();
void updateLedColor();

void setButtonConfig(ButtonConfig* buttonConfig, ButtonConfig::EventHandler eventHandler);
void setupButtons();
void showTime();

void configModeCallback (WiFiManager *myWiFiManager);
void saveConfigCallback();


//-----------------------------------------------------
// Function Definitions
//-----------------------------------------------------
void setup() {
    Serial.begin(9600);

    pinMode(BUILTIN_LED, OUTPUT);

    Serial.println("Setup start.");

    Serial.println("loadConfig:");
    configModule.setup();
    config = configModule.loadConfig();
    currentLedColorId = config.setLedColor;
    updateLedColor();

    setupButtons();

    wifiModule.setup(configModeCallback, saveConfigCallback);
    //wifiModule.reset();
    wifiModule.connect();

    clockModule.setup();

    ledControlModule.setup(&pixelStrip);

    updateClock();
    lastClockUpdate = millis();

    showTime();
    lastShowTime = millis();

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
    if((millis() - lastClockUpdate) > (CLOCK_UPDATE_INTERVAL * 1000)) {
        updateClock();
        lastClockUpdate = millis();
    }

    if((millis() - lastShowTime) > (TIME_UPDATE_INTERVAL * 1000) && !showTimeDisabled) {
        showTime();
        lastShowTime = millis();
    }

    buttonOne.check();
    buttonTwo.check();
    buttonThree.check();
    buttonFour.check();
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");

    showTimeDisabled = true;
    ledControlModule.showConfigWifi();
}

void saveConfigCallback() {
    Serial.println("Save callback.");
    config.enableTime = wifiModule.getEnableTime();
    config.disableTime = wifiModule.getDisableTime();
    configModule.saveConfig(config);

    showTimeDisabled = false;
    showTime();
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
        ledControlModule.showTime(st, currentLedColor);
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

    clockModule.update();
}

void updateLedColor() {
    currentLedColor = LED_COLORS[currentLedColorId];
}

void handleButtonOneEvent(AceButton* button, uint8_t eventType,
                           uint8_t buttonState) {
    switch (eventType) {
        case AceButton::kEventClicked:
            Serial.println("Button One Clicked");
            currentLedColorId = (currentLedColorId + 1) % LED_COLORS_SIZE;

            config.setLedColor = currentLedColorId;
            configModule.saveConfig(config);

            updateLedColor();
            showTime();
            break;
        case AceButton::kEventLongPressed:
            Serial.println("Button One Long Press");
            if(!showTimeDisabled) {
                Serial.println("Disable LED");
                ledControlModule.disableLeds();
                showTimeDisabled = true;
            } else {
                Serial.println("Enable LED");
                showTime();
                showTimeDisabled = false;
            }
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
            delay(1000);
            wifiModule.connect();
            break;
    }
}
