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
const int MAXIMUM_LIGHT_VALUE = 400; //tune for desired behaviour of ambient light adaption

const long TIME_UPDATE_INTERVAL = 10; // in seconds
const long CLOCK_UPDATE_INTERVAL = 86400; // in seconds

const String NTP_SERVER_NAME = "us.pool.ntp.org";
const TimeChangeRule TIME_CHANGE_RULE_DST = {"MESZ", Last, Sun, Mar, 2, 120};    // Daylight time = UTC+2 hours
const TimeChangeRule TIME_CHANGE_RULE_STD = {"MEZ", Last, Sun, Oct, 3, 60};     // Standard time = UTC+1 hours
const Timezone LOCAL_TIMEZONE(TIME_CHANGE_RULE_DST, TIME_CHANGE_RULE_STD);

const String DEVICE_NAME = "QlockTwo";

const char *const CONFIG_FILE_PATH = "/config.json";

const int LED_COLORS_SIZE = 4;

const RgbwColor LED_COLORS[] = {
      RgbwColor(255),
      RgbwColor(255, 0, 0, 0),
      RgbwColor(0, 255, 0, 0),
      RgbwColor(0, 0, 255, 0)
};

#endif //Settings_H
