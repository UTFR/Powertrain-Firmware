#include "UTFR_RGB.h"


UTFR_RGB::UTFR_RGB()
{   
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void UTFR_RGB::setColor(ledCOLOR_E color)
{
    switch (color){
        case OFF: {
            leds[0] = CRGB::Black;
            break;
        }
        case GREEN: {
            leds[0] = CRGB::Green;
            break;
        }
        case YELLOW: {
            leds[0] = CRGB::Yellow;
            break;
        }
        case RED: {
            leds[0] = CRGB::Red;
            break;
        }
        default:
            break;
    }
    return;
}