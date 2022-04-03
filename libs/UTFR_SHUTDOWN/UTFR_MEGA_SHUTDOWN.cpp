
UTFR_MEGA_SHUTDOWN::UTFR_MEGA_SHUTDOWN()
{
  pinMode(HW_PIN_MSCOM1, OUTPUT);

  pinMode(HW_PIN_MSCOM2, INPUT);
  pinMode(HW_PIN_MSCOM3, INPUT);
  attachInterrupt(digitalPinToInterrupt(19), SDC_ISR, RISING);
}

void UTFR_MEGA_SHUTDOWN::checkShutdown()
{

  if (SDC_TRIPPED == true)
  {

    start_time = millis()

        if (digitalRead(HW_PIN_MSCOM3) == HIGH)
    { // Checks to make sure that zero torque has been sent to the inverter

      shutdown_carOff(); // If not send signal back to micro to shutdown
    }

    current_time = start_time;

    if ((current_time - start_time) < kRETRY_INTERVAL)
    {
      current_time = millis();
      digitalWrite(HW_PIN_MSCOM1, LOW);
      digitalWrite(HW_PIN_MSCOM1, HIGH);
    }

    start_time = millis();
    digitalWrite(HW_PIN_MSCOM1, LOW);
  }
}

void UTFR_MEGA_SHUTDOWN::shutdown_carOff()
{

  digitalWrite(38, LOW); // Tell inverter to shutdown
  digitalWrite(7, LOW);  // LV Battery Mosfet -> turn off most LV systems
  while (1)
  {
// ERROR.sendErr(carOff);                        // Kelvin's error library
#ifdef debug_RC_Mega
    Serial.print("Car off. \n";)
#endif
        delay(1000);
  }
}

void UTFR_MEGA_SHUTDOWN::SDC_ISR()
{
  digitalWrite(HW_PIN_MSCOM1, HIGH);
  SDC_TRIPPED = true;
  return SDC_TRIPPED;
}
