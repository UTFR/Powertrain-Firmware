#include "UTFR_MEGA_SHUTDOWN.h"

void UTFR_MEGA_SHUTDOWN::UTFR_MEGA_SHUTDOWN()   // Constructor
{
  pinMode(HW_PIN_MSCOM1, OUTPUT);               // Start interrupt on Micro with this pin
  digitalWrite(HW_PIN_MSCOM1, LOW);

  pinMode(HW_PIN_MSCOM3, INPUT);                // Check for Micro shutdown success with this pin

  attachInterrupt(digitalPinToInterrupt(HW_PIN_SDC_FAULT), SDC_ISR, RISING);

  _RETRY_ATTEMPTS = round(_kRETRY_TIMEOUT/_RETRY_ATTEMPTS);
}


// Checks for if Micro has successfully sent 0 torque to inverter, calls carOff() if yes or if timeout
void UTFR_MEGA_SHUTDOWN::checkZeroTorqueOut()
{
  while(digitalRead(HW_PIN_MSCOM3) != HIGH)     // If high, zero torque has been sent to the inverter successfully
  { 
    _current_time = millis();
    if ((_current_time - _start_time) > _kRETRY_INTERVAL)
    {
      _start_time = millis();
      digitalWrite(HW_PIN_MSCOM1, LOW);
      digitalWrite(HW_PIN_MSCOM1, HIGH); 
      _retries += 1; 
    }

    if (_retries >= _RETRY_ATTEMPTS)            // If it's been longer than 100ms, shut car off despite 0 torque not being commanded
    {
      break;
    }                 
  }

  carOff();                                     // If zero torque commanded, or if timeout, proceed with car shutdown
}


void UTFR_MEGA_SHUTDOWN::carOff()
{
  digitalWrite(HW_PIN_IGNITION_DIGITAL, LOW);     // Tell inverter to shutdown
  digitalWrite(HW_PIN_BMS_DRIVER, LOW);           // LV Battery Mosfet -> turns off most LV systems

  // TO DO: Turn off fans, pumps, set correct LEDs

  while (1)
  {
    //ERROR.sendErr(carOff);                      // Kelvin's error library
    #ifdef debug_RCMega
    Serial.print("Car off. \n";)
    #endif
    delay(1000);
  }
}


// Called when SDC_FAULT interrupt pin goes high
void UTFR_MEGA_SHUTDOWN::SDC_ISR()
{
  _start_time = millis();
  digitalWrite(HW_PIN_MSCOM1, HIGH);              // Start interrupt on Micro
  checkZeroTorqueOut();
}
