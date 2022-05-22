#include <Arduino.h>

#ifndef _UTFR_PIN_DRIVER_MICRO_H_
#define _UTFR_PIN_DRIVER_MICRO_H_

// Define names of all Micro pins here
enum HW_pin_E
{   
    // -----------------------------------------------
    HW_PIN_MEGA_MICRO_1_DIGITAL,                // SS   Mega-Micro comms - Micro receives interrupts from Mega (unassigned))

    HW_PIN_RX_MICRO,                            // 0

    HW_PIN_TX_MICRO,                            // 1   

    HW_PIN_CAN_3_INT,                           // 2   Micro CAN node general INT pin

    HW_PIN_MEGA_MICRO_2_DIGITAL,                // 3   Mega-Micro comms - Micro receives interrupts from Mega telling it to enable/disable APPS outputs

    HW_PIN_MEGA_MICRO_4_DIGITAL,                // 4   Mega-Micro comms - No interrupts on either side (unassigned)

    HW_PIN_MEGA_MICRO_5_DIGITAL,                // 5   Mega-Micro comms - No interrupts - Micro confirms RTD on this pin by setting it high

    HW_PIN_MEGA_MICRO_3_DIGITAL,                // 6   Mega-Micro comms - Mega receives interrupts indicating APPS failure on this pin

    HW_PIN_CS_APPS,                             // 7   CS pin for APPS DAC

    HW_PIN_CS_CAN_3,                            // 9   Micro CAN node CS pin

    HW_PIN_MEGA_MICRO_TX,                       // 10  Software serial port Rx pin for comms with Mega (currently unused)

    HW_PIN_SPEED_FL_FREQ,                       // 11  Front left wheel speed sensor raw frequency signal   

    HW_PIN_SPEED_RL_FREQ,                       // 12  Front right wheel speed sensor raw frequency signal

    HW_PIN_MEGA_MICRO_RX,                       // 13  Software serial port Tx pin for comms with Mega (currently unused)

    // ---------------------------------------------------------------- 
    HW_PIN_APPS_IN_1,                           // A0  APPS voltage reading off first potentiometer

    HW_PIN_APPS_IN_2,                           // A1  APPS voltage reading off second potentiometer

    HW_PIN_BRAKE_IN,                            // A2  Brake pedal voltage reading off potentiometer

    HW_PIN_SPEED_FL_ANALOG,                     // A3  Front left wheel speed sensor analog signal

    HW_PIN_SPEED_RL_ANALOG,                     // A4  Front right wheel speed sensor analog signal 

    HW_PIN_APPS_OUT,                            // A5  Readback from output side of APPS DAC

    HW_PIN_COUNT
};


struct HW_pin_S                                 // Holds all the info that defines a pin
{
    const uint8_t pinNum;
    bool isOutput;
    bool activeLow;
    bool physicalState;
};


extern HW_pin_S Micro_HW_pins[HW_PIN_COUNT];     // Array of pins
    

void HW_setupPins(void);
bool HW_digitalRead(HW_pin_E pin);
void HW_digitalWrite(HW_pin_E pin, bool physState);
int HW_analogRead(HW_pin_E pin);
void HW_analogWrite(HW_pin_E pin, int value);


#endif