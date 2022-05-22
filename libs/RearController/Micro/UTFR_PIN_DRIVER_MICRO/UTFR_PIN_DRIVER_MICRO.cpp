/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_PIN_DRIVER_MICRO.h"

HW_pin_S Micro_HW_pins[HW_PIN_COUNT] = 
{
    // ----------------------------------------------------------------    
    [HW_PIN_MEGA_MICRO_1_DIGITAL] =
    {
        .pinNum = SS,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_RX_MICRO] =
    {
        .pinNum = 0,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_TX_MICRO] =
    {
        .pinNum = 1,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_CAN_3_INT] =
    {
        .pinNum = 2,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_MICRO_2_DIGITAL] =
    {
        .pinNum = 3,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_MICRO_4_DIGITAL] =
    {
        .pinNum = 4,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_MICRO_5_DIGITAL] =
    {
        .pinNum = 5,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_MICRO_3_DIGITAL] =
    {
        .pinNum = 6,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_CS_APPS] =
    {
        .pinNum = 7,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_CS_CAN_3] =
    {
        .pinNum = 9,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_MICRO_TX] =
    {
        .pinNum = 10,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_FL_FREQ] =
    {
        .pinNum = 11,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_RL_FREQ] =
    {
        .pinNum = 12,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_MICRO_RX] =
    {
        .pinNum = 13,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    // ----------------------------------------------------------------  
    [HW_PIN_APPS_IN_1] =
    {
        .pinNum = A0,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_APPS_IN_2] =
    {
        .pinNum = A1,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BRAKE_IN] =
    {
        .pinNum = A2,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_FL_ANALOG] =
    {
        .pinNum = A3,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_RL_ANALOG] =
    {
        .pinNum = A4,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_APPS_OUT] =
    {
        .pinNum = A5,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
};      

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

void HW_setupPins(void)
{
  for (int pin = 0; pin < HW_PIN_COUNT; ++pin)
  {
    if (Micro_HW_pins[pin].isOutput)
    {
      pinMode(Micro_HW_pins[pin].pinNum, OUTPUT);
      digitalWrite(Micro_HW_pins[pin].pinNum, Micro_HW_pins[pin].physicalState);
    }
    else
    {
      pinMode(Micro_HW_pins[pin].pinNum, INPUT);
    }
  }
}

bool HW_digitalRead(HW_pin_E pin)
{
  bool state = false;
  if (pin < HW_PIN_COUNT)
  {
    state = digitalRead(Micro_HW_pins[pin].pinNum);
    Micro_HW_pins[pin].physicalState = state;
  }
  
  return state;
}

void HW_digitalWrite(HW_pin_E pin, bool physState)
{
  if (pin < HW_PIN_COUNT)
  {
    digitalWrite(Micro_HW_pins[pin].pinNum, physState);
    Micro_HW_pins[pin].physicalState = physState;
  }
}

int HW_analogRead(HW_pin_E pin)
{
  if (pin < HW_PIN_COUNT)
  {
    return analogRead(Micro_HW_pins[pin].pinNum);
  }
}

void HW_analogWrite(HW_pin_E pin, int value)
{
  if (pin < HW_PIN_COUNT)
  {
    digitalWrite(Micro_HW_pins[pin].pinNum, value);
  }
}