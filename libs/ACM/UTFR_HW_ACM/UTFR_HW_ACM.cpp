#include "UTFR_HW_ACM.h"

HW_pin_S HW_pins[HW_PIN_COUNT] = 
{
  [HW_PIN_MUX] = 
  {
    .pinNum = A0,
    .isOutput = false,
    .activeLow = false, // Does not matter
    .physicalState = false,
  },
  [HW_PIN_SCK] = 
  {
    .pinNum = 13,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false,
  },
  [HW_PIN_EN_AIRP] = 
  {
    .pinNum = 5,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false,
  },
  [HW_PIN_MUX_S0] = 
  {
    .pinNum = A1,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_MUX_S1] = 
  {
    .pinNum = A2,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_MUX_S2] = 
  {
    .pinNum = A3,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_MUX_S3] = 
  {
    .pinNum = A4,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_EN_PRECHARGE] = 
  {
    .pinNum = 9,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_AMS_FAULT_EN] = 
  {
    .pinNum = A5,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_RTD_BUZZER_EN] = 
  {
    .pinNum = 8,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_EN_AIRN] = 
  {
    .pinNum = 6,
    .isOutput = true,
    .activeLow = false,
    .physicalState = false
  },
  [HW_PIN_SDC_EN] = 
  {
    .pinNum = 7,
    .isOutput = true,
    .activeLow = false,
    .physicalState = true             // Enabled immediately on startup to complete SDC
  }
};

void HW_digitalWrite(HW_pin_E pin, bool physState)
{
  if (pin < HW_PIN_COUNT)
  {
    digitalWrite(HW_pins[pin].pinNum, physState);
    HW_pins[pin].physicalState = physState;
  }
}

void HW_setupPins(void)
{
  for (int pin = 0; pin < HW_PIN_COUNT; ++pin)
  {
    if (HW_pins[pin].isOutput)
    {
      pinMode(HW_pins[pin].pinNum, OUTPUT);
      digitalWrite(HW_pins[pin].pinNum, HW_pins[pin].physicalState);
    }
    else
    {
      pinMode(HW_pins[pin].pinNum, INPUT);
    }
  }
}

bool HW_digitalRead(HW_pin_E pin)
{
  bool state = false;
  if (pin < HW_PIN_COUNT)
  {
    state = digitalRead(HW_pins[pin].pinNum);
    HW_pins[pin].physicalState = state;
  }
  
  return state;
}

float HW_readMux(HW_MUX_E muxPin) // Returns the voltage read for a given mux pin, from 0-5V
{
  bool S0_state = (muxPin & S0_MASK);
  bool S1_state = (muxPin & S1_MASK) >> 1;
  bool S2_state = (muxPin & S2_MASK) >> 2;
  bool S3_state = (muxPin & S3_MASK) >> 3;

  // Set the mux select pins
  HW_digitalWrite(HW_PIN_MUX_S0, S0_state);
  HW_digitalWrite(HW_PIN_MUX_S1, S1_state);
  HW_digitalWrite(HW_PIN_MUX_S2, S2_state);
  HW_digitalWrite(HW_PIN_MUX_S3, S3_state);

  float readVal = analogRead(HW_pins[HW_PIN_MUX].pinNum);
  float analogVoltage = (readVal / ANALOG_CONSTANT) * 5.0;
  analogVoltage += MEASURED_ANALOG_ERROR;
  return analogVoltage;
}