//
// Created by Jonas Lauener on 11.09.18.
//

#ifndef ESP8266_AMBIENTLIGHTMODULE_H
#define ESP8266_AMBIENTLIGHTMODULE_H


class AmbientLightModule {
private:

public:
    AmbientLightModule();
    ~AmbientLightModule();
    void setup(int pinNumber);
    void setBrightnessCorrection(int correctio);
    int getBrightnessCorrection();
    int getBrightness();
};


#endif //ESP8266_AMBIENTLIGHTMODULE_H
