#include "UTFR_RTD_MEGA.h"

UTFR_RTD_MEGA::UTFR_RTD_MEGA()
{   
    // Initialize Pins
    pinMode(RTD_OUT_PIN, OUTPUT);
    pinMode(RTD_IN_PIN, INPUT);
    pinMode(IGNITION_OUT_PIN, OUTPUT);
    pinMode(MICRO_IN_PIN, INPUT);

    #ifdef debugMode
    Serial.println("RTD instantiated correctly on micro.");
    #endif
}

bool UTFR_RTD_MEGA::confirmReady(bool sdc, bool inv, bool cooling)
{
    #ifdef rtdInCheck
    if (digitalRead(RTD_IN_PIN) == LOW) {
        #ifdef debugMode
        Serial.println("UTFR_RTD_MEGA::confirmReady: RTD_IN_PIN low, returning False");
        #endif
        return false;
    }
    #endif

    #ifdef ignitionInCheck
    if (digitalRead(IGNITION_IN_PIN) == LOW) {
        #ifdef debugMode
        Serial.println("UTFR_RTD_MEGA::confirmReady: IGNITION_IN_PIN low, returning False");
        #endif
        return false;
    }
    #endif

    #ifdef sdcCheck
    if (!sdc ) {
        #ifdef debugMode
        Serial.println("UTFR_RTD_MEGA::confirmReady: SDC low, returning False");
        #endif
        return false;
    }
    #endif

    #ifdef inverterCheck
    if (!inv) {
        #ifdef debugMode
        Serial.println("UTFR_RTD_MEGA::confirmReady: INV low, returning False");
        #endif
        return false;
    }
    #endif

    #ifdef microCheck
    if (digitalRead(MICRO_IN_PIN) == LOW) {
        #ifdef debugMode
        Serial.println("UTFR_RTD_MEGA::confirmReady: Micro low, returning False");
        #endif
        return false;
    }
    #endif

    #ifdef coolingCheck
    if (!cooling) {
        #ifdef debugMode
        Serial.println("UTFR_RTD_MEGA::confirmReady:Cooling low, returning False");
        #endif
        return false;
    }
    #endif

    digitalWrite(RTD_OUT_PIN, HIGH);
    digitalWrite(IGNITION_OUT_PIN, HIGH);
    #ifdef debugMode
    Serial.println("UTFR_RTD_MEGA::confirmReady: RTD ready, set RTD_OUT_PIN, IGNITION_OUT_PIN to HIGH");
    #endif
    return true;
}