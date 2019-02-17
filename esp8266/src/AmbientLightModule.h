//
// Created by Jonas Lauener on 11.09.18.
//

#ifndef ESP8266_AMBIENTLIGHTMODULE_H
#define ESP8266_AMBIENTLIGHTMODULE_H


class AmbientLightModule {
private:
    int pinNumber;
    int brightnessCorrection;
    int storedData[3] = {0,0,0};
    int deadBand = 5;
    int maxLightValue = 400; //tune for desired behaviour of ambient light adaption
    int brightnessLevel[10];

public:
    AmbientLightModule(int _pinNumber);
    ~AmbientLightModule();
    void setBrightnessCorrection(int correction);
    int getBrightnessCorrection();
    int getBrightness();
};


#endif //ESP8266_AMBIENTLIGHTMODULE_H
