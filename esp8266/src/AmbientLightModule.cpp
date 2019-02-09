//
// Created by Jonas Lauener on 11.09.18.
//

#include "AmbientLightModule.h"
#include <Arduino.h>

AmbientLightModule::AmbientLightModule(int _pinNumber) : pinNumber(_pinNumber) {
  for(int i = 0; i < 10; i++){
    brightnessLevel[i] = i*maxLightValue/10 - deadBand;
  };

}
AmbientLightModule::~AmbientLightModule() {}

void AmbientLightModule::setBrightnessCorrection(int correction){
  brightnessCorrection = correction;
};

int AmbientLightModule::getBrightnessCorrection(){
  return brightnessCorrection;
};




int AmbientLightModule::getBrightness(){
  int lightSensorRaw = analogRead(pinNumber);
  int lightSensorFiltered = 0.25*(lightSensorRaw + storedData[2] + storedData[1] +storedData[0]);
  storedData[0] = storedData[1];
  storedData[1] = storedData[2];
  storedData[2] = lightSensorRaw;


  int currentBrightnessLevel = 1;
  for(int i = 1; i < 10; i++){
    if(lightSensorFiltered > brightnessLevel[i]){
      currentBrightnessLevel = i+1;
    };
  };



  return 255*currentBrightnessLevel/10;
};
