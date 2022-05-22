#include <Arduino.h>

#ifndef _UTFR_PIN_DRIVER_MEGA_H_
#define _UTFR_PIN_DRIVER_MEGA_H_

// Define names of all Mega pins here
enum HW_pin_E
{
    // split with lines to indicate groups of pins on Arduino
    // spaces indicate the next pin being on the opposite side
    
    // -----------------------------------------------
    HW_PIN_CAN_2_INT,                           // 2    DAQ CAN node general INT pin
    HW_PIN_CAN_1_INT,                           // 3    Powertrain CAN node general INT pin
    HW_PIN_RX1,                                 // 4    Powertrain CAN node RX buffer INT pin
    //HW_PIN_PRE_CHARGE_DIGITAL,                // 5    Useless pin (was backup in case powertrain CAN didn't work)
    HW_PIN_RX0,                                 // 6    Powertrain CAN node RX buffer INT pin
    HW_PIN_BMS_MOS_DIGITAL,                     // 7    Energizing this MOS gate grounds everything besides the Mega (which is always grounded)

    // -----------------------------------------------
    HW_PIN_MTR_PUMP_PWM,                        // 8        
    HW_PIN_INV_PUMP_PWM,                        // 9 
    HW_PIN_RX3,                                 // 10   DAQ CAN node RX buffer INT pin
    HW_PIN_GYRO_INT,                            // 11   Programmable interrupt pin on Gyro IC
    HW_PIN_GYRO_FIFO_INT,                       // 12   Data ready pin on Gyro IC (FIFO Mode only)
    HW_PIN_RX2,                                 // 13   DAQ CAN node RX buffer INT pin

    // -----------------------------------------------
    HW_PIN_MEGA_MICRO_TX,                       // 14
    HW_PIN_MEGA_MICRO_RX,                       // 15

    // -----------------------------------------------
    HW_PIN_INV_FAULT_INT,                       // 18     
    HW_PIN_SDC_FAULT_INT,                       // 19

    // -----------------------------------------------
    HW_PIN_MEGA_MICRO_3_DIGITAL,                // 20   Mega-Micro comms - Mega receives interrupts indicating APPS failure on this pin
    HW_PIN_IGNITION_SWITCH_INT,                 // 21

    // -----------------------------------------------
    HW_PIN_DASH_SWITCH_2_DIGITAL,               // 22

    HW_PIN_DASH_SWITCH_1_DIGITAL,               // 23

    HW_PIN_READY_TO_DRIVE_OUT,                  // 24

    HW_PIN_READY_TO_DRIVE_IN,                   // 25

    HW_PIN_START_OUTPUT_DIGITAL,                // 26

    HW_PIN_SPARE_PIN_3_DIGITAL,                 // 27

    HW_PIN_FORWARD_ENABLE_DIGITAL,              // 28

    HW_PIN_BRAKE_DIGITAL,                       // 29

    HW_PIN_PUMP_DIGITAL,                        // 30 

    HW_PIN_FAN_L_DIGITAL,                       // 31   

    HW_PIN_OE,                                  // 32

    HW_PIN_FAN_R_DIGITAL,                       // 33

    HW_PIN_B_SELECT,                            // 34 
    
    HW_PIN_A_SELECT,                            // 35

    HW_PIN_C_SELECT,                            // 36

    HW_PIN_SPARE_PIN_2_DIGITAL,                 // 37

    HW_PIN_IGNITION_DIGITAL,                    // 38

    HW_PIN_MEGA_MICRO_1_DIGITAL,                // 40   Mega-Micro comms - Micro receives interrupt from Mega on this pin (unassigned)

    HW_PIN_MEGA_MICRO_2_DIGITAL,                // 41   Mega-Micro comms - Micro receives interrupts from Mega telling it to enable/disable APPS outputs

    HW_PIN_MEGA_MICRO_4_DIGITAL,                // 42   Mega-Micro comms - No interrupts on either side (unassigned)

    HW_PIN_MEGA_MICRO_5_DIGITAL,                // 43   Mega-Micro comms - No interrupts - Micro confirms RTD on this pin by setting it high

    HW_PIN_LED_DATA,                            // 44

    HW_PIN_RGB_DATA,                            // 45

    HW_PIN_IGNITION_LED_DIGITAL,                // 46   Indicates inverter is getting LV power

    HW_PIN_CS_CAN_1,                            // 47   Powertrain CAN node CS pin

    HW_PIN_CS_CAN_2,                            // 48   DAQ CAN node CS pin

    HW_PIN_CS_ACCEL,                            // 49   

    //HW_PIN_MISO_MEGA,                         // 50   Setup automatically as default SPI pins

    //HW_PIN_MOSI_MEGA,                         // 51   

    //HW_PIN_SCK_MEGA,                          // 52

    HW_PIN_CS_GYRO,                             // 53

    // -----------------------------------------------
    HW_PIN_BMS_TEMP_ANALOG_1,                   // A0   Coolant loop monitoring pins
    HW_PIN_BMS_TEMP_ANALOG_2,                   // A1
    HW_PIN_FLOW_RATE_INV_ANALOG,                // A2
    HW_PIN_FLOW_RATE_MTR_ANALOG,                // A3
    HW_PIN_COOL_TEMP_INV_IN_ANALOG,             // A4
    HW_PIN_COOL_TEMP_INV_OUT_ANALOG,            // A5
    HW_PIN_COOL_PSR_INV_IN_ANALOG,              // A6
    HW_PIN_COOL_PSR_INV_OUT_ANALOG,             // A7
    // -----------------------------------------------
    HW_PIN_COOL_TEMP_MTR_IN_ANALOG,             // A8
    HW_PIN_COOL_TEMP_MTR_OUT_ANALOG,            // A9
    HW_PIN_COOL_PSR_MTR_IN_ANALOG,              // A10
    HW_PIN_COOL_PSR_MTR_OUT_ANALOG,             // A11
    HW_PIN_BMS_CHARGE_MEGA_ANALOG,              // A12  LV battery total voltage reading (divided down to Arduino logic levels)
    HW_PIN_SDC_CURRENT_ANALOG,                  // A13  Shunt resistor reads LOW when SDC is tripped and current stops flowing
    HW_PIN_INV_OK_DIGITAL,                      // A14  Goes HIGH when inverter is finshed shutting itself off (cuts off it's own power)
    HW_PIN_SPARE_ANALOG,                        // A15
    // -----------------------------------------------

    HW_PIN_COUNT
};


struct HW_pin_S                                 // Holds all the info that defines a pin
{
    const uint8_t pinNum;
    bool isOutput;
    bool activeLow;
    bool physicalState;
};


extern HW_pin_S Mega_HW_pins[HW_PIN_COUNT];     // Array of pins
    

void HW_setupPins(void);
bool HW_digitalRead(HW_pin_E pin);
void HW_digitalWrite(HW_pin_E pin, bool physState);
int HW_analogRead(HW_pin_E pin);
void HW_analogWrite(HW_pin_E pin, int value);


#endif