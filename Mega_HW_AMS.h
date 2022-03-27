/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/
// Mega Inputs and Outputs

typedef enum
{

    // split with lines to indicate groups of pins on Arduino
    // spaces indicate the next pin being on the opposite side

    // -----------------------------------------------
    HW_PIN_RESET = 0,                         // RESET
    HW_PIN_BMS_GND,                   // GND1 OR GND2?
    // -----------------------------------------------
    HW_PIN_IGNITION_SWITCH_INT,                 // SCL
    HW_PIN_MS_COM_3_DIGITAL,                    // SDA
    // -----------------------------------------------
    // TX (pin 1) was omitted
    // RX (pin 0) was omitted
    HW_PIN_CAN_2_INT,                           // 2
    HW_PIN_CAN_1_INT,                           // 3
    // RX1 (pin 4) was omitted
    HW_PIN_SPARE_PIN_1_DIGITAL,                 // 5
    // RX0 (pin 6) was omitted
    HW_PIN_BMS_DRIVER,                          // 7
    // -----------------------------------------------
    HW_PIN_MTR_PUMP_PWM,                        // 8
    HW_PIN_INV_PUMP_PWM,                        // 9 
    // RX3 (pin 10) was omitted
    // INT (pin 11) was omitted
    // FIFO (pin 12) was omitted
    // RX2 (pin 13) was omitted
    // -----------------------------------------------
    HW_PIN_MEGA_TX,                             // 14
    HW_PIN_MEGA_RX,                             // 15
    HW_PIN_PDM_TX,                              // 16
    HW_PIN_PDM_RX,                              // 17
    HW_PIN_INV_FAULT_INT,                       // 18     
    HW_PIN_SDC_FAULT_INT,                       // 19
    // -----------------------------------------------
    HW_PIN_DASH_SWITCH_2_DIGITAL,               // 22

    HW_PIN_DASH_SWITCH_1_DIGITAL,               // 23

    HW_PIN_READY_TO_DRIVE_OUT,                  // 24

    HW_PIN_READY_TO_DRIVE_IN,                   // 25

    HW_PIN_START_OUTPUT_DIGITAL,                // 26

    HW_PIN_SPARE_PIN_3_DIGITAL,                 // 27

    HW_PIN_FORWARD_ENABLE_DIGITAL,              // 28

    HW_PIN_BRAKE_DIGITAL,                       // 29
    // -----------------------------------------------
    HW_PIN_OE,                                  // 32

    HW_PIN_B_SELECT,                            // 34 
    
    HW_PIN_A_SELECT,                            // 35

    HW_PIN_C_SELECT,                            // 36

    HW_PIN_SPARE_PIN_2_DIGITAL,                 // 37

    HW_PIN_IGNITION_DIGITAL,                    // 38

    HW_PIN_CHASSIS_GND_DISCONNECT,              // 39

    HW_PIN_MS_COM_1_DIGITAL,                    // 40

    HW_PIN_MS_COM_2_DIGITAL,                    // 41

    HW_PIN_MS_COM_4_DIGITAL,                    // 42

    HW_PIN_MS_COM_5_DIGITAL,                    // 43

    HW_PIN_LED_PWM,                             // 44

    HW_PIN_RGB_PWM,                             // 45

    HW_PIN_IGNITION_LED_DIGITAL,                // 46

    HW_PIN_CS_CAN_1,                            // 47

    HW_PIN_CS_CAN_2,                            // 48

    HW_PIN_CS_ACCEL,                            // 49

    HW_PIN_MISO_MEGA,                           // 50

    HW_PIN_MOSI_MEGA,                           // 51

    HW_PIN_SCK_MEGA,                            // 52

    HW_PIN_CS_GYRO,                             // 53
    // -----------------------------------------------
    HW_PIN_BMS_TEMP_ANALOG_1,                   // A0
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
    HW_PIN_BMS_VOLT_MEGA,                       // A12
    HW_PIN_BMS_ANALOG_GND_1,                    // A13
    HW_PIN_BMS_ANALOG_GND_2,                    // A14
    HW_PIN_SPARE_ANALOG,                        // A15
    // -----------------------------------------------

    HW_PIN_COUNT

} HW_pin_E;


// from Jake Sprenger's HW_ACM.h
typedef struct
{
    int pinNum;
    bool isOutput;
    bool activeLow;
    bool physicalState;

} HW_pin_S;

/******************************************************************************
 *         P U B L I C   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/

void Mega_HW_setupPins(void);
bool Mega_HW_digitalRead(HW_pin_E pin);
void Mega_HW_digitalWrite(HW_pin_E pin, bool physState);


/******************************************************************************
 *              P U B L I C   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/

extern HW_pin_S Mega_HW_pins[HW_PIN_COUNT];
HW_pin_S Mega_HW_pins[HW_PIN_COUNT] = 
{
    
    // ----------------------------------------------------------------
    [HW_PIN_RESET] =
    {
        .pinNum = RESET,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BMS_GND] =
    {
        .pinNum = GND, // both GND1 and GND2?
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },

    // ----------------------------------------------------------------    
    [HW_PIN_IGNITION_SWITCH_INT] =
    {
        .pinNum = SCL, // P0?
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MS_COM_3_DIGITAL] =
    {
        .pinNum = SDA, // P1?
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

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
    [HW_PIN_SPARE_PIN_1_DIGITAL] =
    {
        .pinNum = 5,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BMS_DRIVER] =
    {
        .pinNum = 7,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
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
    
    // ----------------------------------------------------------------    
    [HW_PIN_MEGA_TX] =
    {
        .pinNum = 14,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MEGA_RX] =
    {
        .pinNum = 15,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_PDM_TX] =
    {
        .pinNum = 16,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_PDM_RX] =
    {
        .pinNum = 17,
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
        .isOutput = true,
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

    [ HW_PIN_READY_TO_DRIVE_IN] =
    {
        .pinNum = 25,
        .isOutput = true,
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

    // ----------------------------------------------------------------
    [HW_PIN_OE] =
    {
        .pinNum = 32,
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
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_CHASSIS_GND_DISCONNECT] =
    {
        .pinNum = 39,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MS_COM_1_DIGITAL] =
    {
        .pinNum = 40,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MS_COM_2_DIGITAL] =
    {
        .pinNum = 41,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MS_COM_4_DIGITAL] =
    {
        .pinNum = 42,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MS_COM_5_DIGITAL] =
    {
        .pinNum = 43,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_LED_PWM] =
    {
        .pinNum = 44,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_RGB_PWM] =
    {
        .pinNum = 45,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_IGNITION_LED_DIGITAL] =
    {
        .pinNum = 46,
        .isOutput = true,
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

    [HW_PIN_MISO_MEGA] =
    {
        .pinNum = 50,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_MOSI_MEGA] =
    {
        .pinNum = 51,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },

    [HW_PIN_SCK_MEGA] =
    {
        .pinNum = 52,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    }

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
    [HW_PIN_BMS_VOLT_MEGA] =
    {
        .pinNum = A12,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BMS_ANALOG_GND_1] =
    {
        .pinNum = A13,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BMS_ANALOG_GND_2] =
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

    // ----------------------------------------------------------------
};

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/


/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

