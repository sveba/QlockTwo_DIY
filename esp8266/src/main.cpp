#include <Arduino.h>
#include "LedControlModule.h"
#include "ClockModule.h"
#include "WifiModule.h"
#include "Settings.h"

const uint8_t PanelWidth = 11;  // 8 pixel x 8 pixel matrix of leds
const uint8_t PanelHeight = 10;
const uint16_t PixelCount = PanelWidth * PanelHeight + 4;

NeoTopology<MyPanelLayout> topo(PanelWidth, PanelHeight);
LedControlModule ledControlModule(topo);
NeoPixelBusType pixelStrip(PixelCount);

ClockModule clockModule(Wire, CLOCK_UPDATE_INTERVAL);

WifiModule wifiModule(DEVICE_NAME);

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


/*
//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setupWifiManager() {
  WiFiManager wifiManager;
  //reset settings - for testing
  wifiManager.resetSettings();

  wifiManager.setAPCallback(configModeCallback);

  if(!wifiManager.autoConnect("QlockTwo No.1")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  Serial.println("connected...yeey :)");
}

RgbColor white(128);
*/

void setup() {
    Serial.begin(9600);

    pinMode(BUILTIN_LED, OUTPUT);

    clockModule.setup(CLOCK_TIMEZONE_OFFSET);

    wifiModule.setup();

    wifiModule.reset();

    wifiModule.connect();

    ledControlModule.setup(&pixelStrip);
}


void loop() {
    /* pixelStrip.SetPixelColor(topo.Map(0, 0), white);
     pixelStrip.Show();

    /*const RtcDateTime time(2018, 6, 29, 10, 52, 0);*/

    if(clockModule.isUpdateNeeded()) {
        Serial.println("Update Clock");
        if (!wifiModule.isConnected()) {
            wifiModule.connect();
        }

        clockModule.update();
    }

    printDateTime(clockModule.getTime());

    ledControlModule.setTime(clockModule.getTime());

    Serial.println("loop start");
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
}
