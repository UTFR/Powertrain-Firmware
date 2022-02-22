#include "RGB.h"

void setup() {
  RGB.kRGBSlavePin = 10;
  RGB.RGBSetup(RGB.kRGBSlavePin);
}

void loop() {
  RGB.colour = _red; // values can be set as: _red, _green, _blue, _raspberry, _cyan, _magenta, _yellow, _white, _off
  RGB.duration = 1; // unit in seconds
  RGB.brightness = 255; // range from 0-255
  RGB.RGBControl(RGB.colour, RGB.duration, RGB.brightness);
}
