#include "RGB.h"

void RGB::RGBSetup(int kGRBSlavePin) {
  
  pinMode(kGRBSlavePin)
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
}

void RGB::RGBColor(int _redLightValue, int _greenLightValue, int _blueLightValue){
  analogWrite(red_light_pin, _redLightValue);
  analogWrite(greenLightPin, _greenLightValue);
  analogWrite(blueLightPin, _blueLightValue);
} 

// function for controlling the colour for the RGB LEDs

void RGB::RGBControl(colour type, int duration, int brightness){ // brightness is set from 0-255
  int _delayTime = duration* 1000;
  
    if (type == _red){
    RGBColor(brightness, 0, 0); // Red
  }
    if (type == _blue){
      RGBColor(0, 0, brightness); // Blue
  }
    if (type == _raspberry){
      RGBColor(brightness, brightness, brightness/2); // Raspberry
  }
    if (type == _cyan){
      RGBColor(0, brightness, brightness); // Cyan
  }
    if (type == _magenta){
      RGBColor(brightness, 0, brightness); // Magenta
  }
    if (type == _yellow){
      RGBColor(brightness, brightness, 0); // Yellow
  }
    if (type == _white){
      RGBColor(brightness, brightness, brightness); // White
  }
    if (type == _off){
      RGBColor(0, 0, 0); // off
  }
    delay(_delayTime);
}