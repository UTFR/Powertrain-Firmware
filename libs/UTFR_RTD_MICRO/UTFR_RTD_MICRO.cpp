#include "UTFR_RTD_MICRO.h"

UTFR_RTD_MICRO::UTFR_RTD_MICRO()
{   
    // Initialize Pins
    pinMode(MEGA_OUT_PIN, OUTPUT);
    pinMode(BRAKE_IN_PIN, INPUT);

    #ifdef debugMode
    Serial.println("RTD instantiated correctly on micro.");
    #endif
}

bool UTFR_RTD_MICRO::confirmReady(int throttle){

    int check_counter = 0;
    while (check_counter < kCHECK_COUNTER_) {
        if (!this->checkThrottle(throttle)) {
            #ifdef debugMode
            Serial.println("UTFR_RTD_MICRO::confirmReady: Throttle Invalid, returning False");
            #endif
            digitalWrite(MEGA_OUT_PIN, LOW);
            return false;
        }

        if (!this->checkBrake()) {
            #ifdef debugMode
            Serial.println("UTFR_RTD_MICRO::confirmReady: Brake Invalid, returning False");
            #endif
            digitalWrite(MEGA_OUT_PIN, LOW);
            return false;
        }
        check_counter++;
    }

    digitalWrite(MEGA_OUT_PIN, HIGH);
    #ifdef debugMode
    Serial.println("UTFR_RTD_MICRO::confirmReady: RTD ready, set MEGA_OUT_PIN HIGH");
    #endif
    return true;
}

bool UTFR_RTD_MICRO::checkThrottle(int throttle) {
    #ifdef throttleCheck
    if (throttle > kTHROTTLE_THRESHOLD_) {
        return false;
    }
    #endif
    return true;
}

bool UTFR_RTD_MICRO::checkBrake() {
    #ifdef brakeCheck
    if ((analogRead(A2) < kBRAKE_THRESHOLD_)){
        return false;
    }
    #endif
    return true;
}