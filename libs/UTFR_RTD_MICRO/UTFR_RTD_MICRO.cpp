UTFR_RTD_MICRO::UTFR_RTD_MICRO(uint8_t CS)
{   
    // Initialize Pins
    pinMode(MEGA_OUT_PIN, OUTPUT);
    pinMode(TEMP_IN_PIN, INPUT);
    pinMode(THROTTLE_IN_PIN, INPUT);

    #ifdef debugMode
    Serial.println("RTD instantiated correctly on micro.");
    #endif
}

bool UTFR_RTD_MICRO::confirmReady(){

    int check_counter = 0;
    while (check_counter < kCHECK_COUNTER_) {
        if (!checkThrottle()) {
            #ifdef debugMode
            Serial.println("UTFR_RTD_MICRO::confirmReady: Throttle Invalid, returning False");
            #endif
            digitalWrite(MEGA_OUT, LOW);
            return false;
        }

        if (!checkBrake()) {
            #ifdef debugMode
            Serial.println("UTFR_RTD_MICRO::confirmReady: Brake Invalid, returning False");
            #endif
            digitalWrite(MEGA_OUT, LOW);
            return false;
        }
        check_counter++;
    }

    digitalWrite(MEGA_OUT, HIGH);
    digitalWrite(IGNITION_OUT_PIN, HIGH);
    #ifdef debugMode
    Serial.println("UTFR_RTD_MICRO::confirmReady: RTD ready, set MEGA_OUT_PIN HIGH");
    #endif
    return true;
    }
}

bool UTFR_RTD_MICRO::checkThrottle() {
    //int throttle_average = APPS->getThrottle();
    if (throttle_average != 0) {
        return false;
    }
    return true;
}

bool UTFR_RTD_MICRO::checkBrake() {
    if ((analogRead(A2) < kBRAKE_THRESHOLD_)){
        return false;
    }
    return true;
}