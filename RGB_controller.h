#ifndef RGB_H	
#define RGB_H	
#include "Arduino.h"
#include <SPI.h>

class GRB{

    public:
        enum colour { _red, _green, _blue, _raspberry, _cyan, _magenta, _yellow, _white, _off };
        int duration, brightness;
        const int kRGBSlavePin;
        void RGBSetup(int kGRBSlavePin);
        void RGBControl(colour type, int duration, int brightness);

    private:
        void RGBColor(int _redLightValue, int _greenLightValue, int _blueLightValue);

};


#endif
