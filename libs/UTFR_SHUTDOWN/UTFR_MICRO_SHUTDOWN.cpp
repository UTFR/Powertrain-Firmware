
UTFR_MIRCO_SHUTDOWN::UTFR_MIRCO_SHUTDOWN()
{

  Serial.begin(115200);
  apps.begin(kCS_DAC);

  attachInterrupt(digitalPinToInterrupt(HW_PIN_MSCOM1), SDC_ISR, RISING);
  pinmode(HW_PIN_MSCOM3, OUTPUT);
}

void UTFR_MIRCO_SHUTDOWN::checkShutdown()
{
  if (SDC_TRIPPED)
  {
    if (readback != 0)
    {
      SDC_ISR()
    }
    else
    {
      digitalwrite(HW_PIN_MSCOM3, HIGH);
    }
  }
}

void UTFR_MIRCO_SHUTDOWN::SDC_ISR()
{
  DAC.analogWrite(0, 0); // Write 0 throttle to DAC
  delayMicroseconds(kREADBACK_DELAY);
  SDC_TRIPPED = true;
  readback = analogRead(A5);
}
