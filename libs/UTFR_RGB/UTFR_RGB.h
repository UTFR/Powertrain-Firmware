/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include <FastLED.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_RGB_H_
#define _UTFR_RGB_H_

#define NUM_LEDS 1

#define DATA_PIN 3

enum ledCOLOR_E { //All possible types of errors
  OFF,
  GREEN,
  RED,
  YELLOW,
};

class UTFR_RGB{
    public:
      UTFR_RGB();
      void setColor(ledCOLOR_E color);
    private:
      CRGB leds[NUM_LEDS];
};

#endif
