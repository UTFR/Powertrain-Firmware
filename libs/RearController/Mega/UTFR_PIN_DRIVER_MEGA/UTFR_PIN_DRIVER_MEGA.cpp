/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_PIN_DRIVER_MEGA.h"

HW_pin_S Mega_HW_pins[HW_PIN_COUNT] = 
{
    // ----------------------------------------------------------------    
    [HW_PIN_CAN_2_INT] =
    {
        .pinNum = 2,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_CAN_1_INT] =
    {
        .pinNum = 3,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_RX1] =
    {
        .pinNum = 4,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_RX0] =
    {
        .pinNum = 6,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BMS_MOS_DIGITAL] =
    {
        .pinNum = 7,
        .isOutput = true,
        .activeLow = false,
        .physicalState = true,            // This is first step when car turns on
    },
    // ----------------------------------------------------------------    
    [HW_PIN_MTR_PUMP_PWM] =
    {
        .pinNum = 8,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_INV_PUMP_PWM] =
    {
        .pinNum = 9,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_RX3] =
    {
        .pinNum = 10,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_GYRO_INT] =
    {
        .pinNum = 11,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_GYRO_FIFO_INT] =
    {
        .pinNum = 12,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_RX2] =
    {
        .pinNum = 13,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },

    // ----------------------------------------------------------------    
    [HW_PIN_MEGA_MICRO_TX] =
    {
        .pinNum = 14,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_MICRO_RX] =
    {
        .pinNum = 15,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_INV_FAULT_INT] =
    {
        .pinNum = 18,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SDC_FAULT_INT] =
    {
        .pinNum = 19,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },

    // ----------------------------------------------------------------   
    [HW_PIN_MEGA_MICRO_3_DIGITAL] =
    {
        .pinNum = 20,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },  
    [HW_PIN_IGNITION_SWITCH_INT] =
    {
        .pinNum = 21,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    
    // ----------------------------------------------------------------    
    [HW_PIN_DASH_SWITCH_2_DIGITAL] =
    {
        .pinNum = 22,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_DASH_SWITCH_1_DIGITAL] =
    {
        .pinNum = 23,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },


    [HW_PIN_READY_TO_DRIVE_OUT] =
    {
        .pinNum = 24,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_READY_TO_DRIVE_IN] =
    {
        .pinNum = 25,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },


    [HW_PIN_START_OUTPUT_DIGITAL] =
    {
        .pinNum = 26,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_SPARE_PIN_3_DIGITAL] =
    {
        .pinNum = 27,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_FORWARD_ENABLE_DIGITAL] =
    {
        .pinNum = 28,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    
    [HW_PIN_BRAKE_DIGITAL] =
    {
        .pinNum = 29,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_PUMP_DIGITAL] =
    {
        .pinNum = 30,
        .isOutput = true,
        .activeLow = false,
        .physicalState = true,          // Pumps should be on as soon as Mega gets power
    },

    [HW_PIN_FAN_L_DIGITAL] =
    {
        .pinNum = 31,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_OE] =
    {
        .pinNum = 32,
        .isOutput = true,
        .activeLow = false,
        .physicalState = true,          // Level shifters should always be enabled
    },

    [HW_PIN_FAN_R_DIGITAL] =
    {
        .pinNum = 33,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_B_SELECT] =
    {
        .pinNum = 34,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_A_SELECT] =
    {
        .pinNum = 35,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_C_SELECT] =
    {
        .pinNum = 36,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_SPARE_PIN_2_DIGITAL] =
    {
        .pinNum = 37,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_IGNITION_DIGITAL] =
    {
        .pinNum = 38,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MEGA_MICRO_1_DIGITAL] =
    {
        .pinNum = 40,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MEGA_MICRO_2_DIGITAL] =
    {
        .pinNum = 41,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MEGA_MICRO_4_DIGITAL] =
    {
        .pinNum = 42,
        .isOutput = true,               // TO DO: Update direction when decided what this does
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MEGA_MICRO_5_DIGITAL] =
    {
        .pinNum = 43,
        .isOutput = false,  
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_LED_DATA] =
    {
        .pinNum = 44,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_RGB_DATA] =
    {
        .pinNum = 45,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_IGNITION_LED_DIGITAL] =
    {
        .pinNum = 46,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_CS_CAN_1] =
    {
        .pinNum = 47,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_CS_CAN_2] =
    {
        .pinNum = 48,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_CS_ACCEL] =
    {
        .pinNum = 49,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_CS_GYRO] =
    {
        .pinNum = 53,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    // ----------------------------------------------------------------
    [HW_PIN_BMS_TEMP_ANALOG_1] =
    {
        .pinNum = A0,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BMS_TEMP_ANALOG_2] =
    {
        .pinNum = A1,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_FLOW_RATE_INV_ANALOG] =
    {
        .pinNum = A2,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_FLOW_RATE_MTR_ANALOG] =
    {
        .pinNum = A3,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_COOL_TEMP_INV_IN_ANALOG] =
    {
        .pinNum = A4,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_COOL_TEMP_INV_OUT_ANALOG] =
    {
        .pinNum = A5,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_COOL_PSR_INV_IN_ANALOG] =
    {
        .pinNum = A6,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_COOL_PSR_INV_OUT_ANALOG] =
    {
        .pinNum = A7,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },

    // ----------------------------------------------------------------    
    [HW_PIN_COOL_TEMP_MTR_IN_ANALOG] =
    {
        .pinNum = A8,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_COOL_TEMP_MTR_OUT_ANALOG] =
    {
        .pinNum = A9,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_COOL_PSR_MTR_IN_ANALOG] =
    {
        .pinNum = A10,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_COOL_PSR_MTR_OUT_ANALOG] =
    {
        .pinNum = A11,
        .isOutput = false, 
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BMS_CHARGE_MEGA_ANALOG] =
    {
        .pinNum = A12,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SDC_CURRENT_ANALOG] =
    {
        .pinNum = A13,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_INV_OK_DIGITAL] =
    {
        .pinNum = A14,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPARE_ANALOG] =
    {
        .pinNum = A15,
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
    if (Mega_HW_pins[pin].isOutput)
    {
      pinMode(Mega_HW_pins[pin].pinNum, OUTPUT);
      digitalWrite(Mega_HW_pins[pin].pinNum, Mega_HW_pins[pin].physicalState);
    }
    else
    {
      pinMode(Mega_HW_pins[pin].pinNum, INPUT);
    }
  }
}

bool HW_digitalRead(HW_pin_E pin)
{
  bool state = false;
  if (pin < HW_PIN_COUNT)
  {
    state = digitalRead(Mega_HW_pins[pin].pinNum);
    Mega_HW_pins[pin].physicalState = state;
  }
  
  return state;
}

void HW_digitalWrite(HW_pin_E pin, bool physState)
{
  if (pin < HW_PIN_COUNT)
  {
    digitalWrite(Mega_HW_pins[pin].pinNum, physState);
    Mega_HW_pins[pin].physicalState = physState;
  }
}

int HW_analogRead(HW_pin_E pin)
{
  if (pin < HW_PIN_COUNT)
  {
    return analogRead(Mega_HW_pins[pin].pinNum);
  }
}

void HW_analogWrite(HW_pin_E pin, int value)
{
  if (pin < HW_PIN_COUNT)
  {
    digitalWrite(Mega_HW_pins[pin].pinNum, value);
  }
}
