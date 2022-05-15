#include "UTFR_RTD_MICRO.h"

UTFR_RTD_MICRO::UTFR_RTD_MICRO()
{
}

bool UTFR_RTD_MICRO::confirmReady()
{
    int check_counter = 0;
    while (check_counter < kCHECK_COUNTER_)
    {
        if (!checkThrottle())
        {
            #ifdef debug_RTD_Micro
            Serial.println("UTFR_RTD_MICRO::confirmReady: Throttle Invalid, returning False");
            #endif
            HW_digitalWrite(HW_PIN_MEGA_MICRO_5_DIGITAL, false);
            return false;
        }

        if (!checkBrake())
        {
            #ifdef debug_RTD_Micro
            Serial.println("UTFR_RTD_MICRO::confirmReady: Brake Invalid, returning False");
            #endif
            HW_digitalWrite(HW_PIN_MEGA_MICRO_5_DIGITAL, false);
            return false;
        }
        check_counter++;
    }

    HW_digitalWrite(HW_PIN_MEGA_MICRO_5_DIGITAL, true);
    #ifdef debug_RTD_Micro
    Serial.println("UTFR_RTD_MICRO::confirmReady: RTD ready, set MEGA_OUT_PIN HIGH");
    #endif
    return true;
}

bool UTFR_RTD_MICRO::checkThrottle()
{
    if (HW_analogRead(HW_PIN_APPS_OUT) > kTHROTTLE_THRESHOLD_)
    {
        return false;
    }
    return true;
}

bool UTFR_RTD_MICRO::checkBrake()
{
    if (HW_analogRead(HW_PIN_BRAKE_IN) < kBRAKE_THRESHOLD_)
    {
        return false;
    }
    return true;
}