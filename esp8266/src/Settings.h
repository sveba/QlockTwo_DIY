//
// Created by Jonas Lauener on 09.09.18.
//

#ifndef Settings_H
#define Settings_H

const int PANEL_WIDTH = 11;  // 8 pixel x 8 pixel matrix of leds
const int PANEL_HEIGHT = 10;
const int PIXEL_COUNT = PANEL_WIDTH * PANEL_HEIGHT + 4;

const int BUTTON_ONE_PIN = D0;
const int BUTTON_TWO_PIN = D5;
const int BUTTON_THREE_PIN = D6;
const int BUTTON_FOUR_PIN = D7;

const int LIGHT_SENSOR_PIN = A0;

const int TIME_UPDATE_INTERVAL = 10;
const long CLOCK_UPDATE_INTERVAL = 30;
const int CLOCK_TIMEZONE_OFFSET = 7200;

const String DEVICE_NAME = "QlockTwo_No1";

static const char *const CONFIG_FILE_PATH = "/config.json";

#endif //Settings_H
