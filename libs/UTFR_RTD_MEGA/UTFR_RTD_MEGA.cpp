UTFR_RTD_MEGA::UTFR_RTD(uint8_t CS)
{   
    // Initialize Pins
    pinMode(RTD_OUT_PIN, OUTPUT);
    pinMode(IGNITION_OUT_PIN, OUTPUT);

    #ifdef debugMode
    Serial.println("RTD instantiated correctly on micro.");
    #endif
}

bool UTFR_RTD_MEGA::confirmReady(bool sdc, bool inv, bool micro, bool cooling){

    if (digitalRead(RTD_IN_PIN) == LOW || digitalRead(IGNITION_IN_PIN) == LOW) {
        #ifdef debugMode
        Serial.println("UTFR_RTD::confirmReady: RTD_IN_PIN or IGNITION_IN_PIN low, returning False");
        #endif
        return false;
    }

    if (!sdc || !inv || !micro || !cooling) {
        #ifdef debugMode
        Serial.println("UTFR_RTD::confirmReady: SDC, INV, Micro or Cooling low, returning False");
        #endif
        return false
    }

    digitalWrite(RTD_OUT_PIN, HIGH);
    digitalWrite(IGNITION_OUT_PIN, HIGH);
    #ifdef debugMode
    Serial.println("UTFR_RTD::confirmReady: RTD ready, set RTD_OUT_PIN, IGNITION_OUT_PIN to HIGH");
    #endif
    return true;
    }
}