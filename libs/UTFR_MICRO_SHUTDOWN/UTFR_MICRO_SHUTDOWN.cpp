#include "UTFR_MICRO_SHUTDOWN.h"


void SDC_ISR(UTFR_MICRO_SHUTDOWN &SELF)
{
  SELF.checkZeroTorqueOut();
}


// Constructor
UTFR_MICRO_SHUTDOWN::UTFR_MICRO_SHUTDOWN(UTFR_APPS& APPS)
{
  attachInterrupt(digitalPinToInterrupt(HW_PIN_MSCOM1), SDC_ISR, RISING);

  pinMode(HW_PIN_MSCOM3, OUTPUT);
  digitalWrite(HW_PIN_MSCOM3, LOW);

  _APPS = APPS;
}


void UTFR_MICRO_SHUTDOWN::carOff()
{
  while(1)                                  // TO DO: Re-enter RTD loop if certain input given (ignition button?)
  {
    #ifdef debug_MicroShutdown
    Serial.print("Car off. \n";)
    #endif
    delay(1000);
  }
}


// Call from SDC_ISR() - confirms DAC is commanding zero torque to inverter
void UTFR_MICRO_SHUTDOWN::checkZeroTorqueOut()
{
  _APPS.shutdown();                         // Write 0 throttle to DAC

  unsigned long start_time, current_time = millis(); 

  delayMicroseconds(_kREADBACK_DELAY);

  while(!_APPS.confirmShutdown()) 
  { 
    current_time = millis();
    if (current_time - start_time > _kTIMEOUT)
    {
      return;                               // Return to normal functioning if failure to command zero torque
    }
  }

  digitalWrite(HW_PIN_MSCOM3, HIGH);        // Let Mega know 0 torque was commanded successfully
  carOff();
}