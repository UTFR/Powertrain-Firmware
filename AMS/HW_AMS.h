/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "MCP_DAC.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/
// Micro Shield Inputs and Outputs

typedef enum
{
    HW_PIN_RX_MICRO = 0,
    HW_PIN_TX_MICRO,
    HW_PIN_MS_COM_1_DIGITAL,
    HW_PIN_MS_COM_2_DIGITAL,
    HW_PIN_MS_COM_3_DIGITAL,
    HW_PIN_MS_COM_4_DIGITAL,
    HW_PIN_MS_COM_5_DIGITAL,
    HW_PIN_CS_DPOT,
    HW_PIN_CS_DAC,
    HW_PIN_SPARE_1_PWM_ANALOG,
    HW_PIN_SPARE_2_PWM_ANALOG,
    HW_PIN_SPEED_RL_DIGITAL,
    HW_PIN_SPEED_FL_DIGITAL,
    HW_PIN_APPS_IN_1_ANALOG,
    HW_PIN_APPS_IN_2_ANALOG,
    HW_PIN_BRAKE_IN_ANALOG,
    HW_PIN_SPEED_RL_ANALOG,
    HW_PIN_SPEED_FL_ANALOG,
    HW_PIN_APPS_OUT_ANALOG,

    // note to self - didn't declare all pins
    // like SCK, MISO, MOSI, etc

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
void HW_setupPins(void);
void HW_setupDAC(void);
bool HW_digitalRead(HW_pin_E pin);
void HW_digitalWrite(HW_pin_E pin, bool physState);
bool HW_writeToDAC(uint16_t value, uint8_t channel);


/******************************************************************************
 *              P U B L I C   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/

// array def should be in .c file when created?
extern HW_pin_S HW_pins[HW_PIN_COUNT];
HW_pin_S HW_pins[HW_PIN_COUNT] = 
{
    [HW_PIN_RX_MICRO] =
    {
        .pinNum = 0,
        .isOutput = true,
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
    [HW_PIN_MS_COM_1_DIGITAL] =
    {
        .pinNum = 2,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MS_COM_2_DIGITAL] =
    {
        .pinNum = 3,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MS_COM_3_DIGITAL] =
    {
        .pinNum = 4,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MS_COM_4_DIGITAL] =
    {
        .pinNum = 5,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_MS_COM_5_DIGITAL] =
    {
        .pinNum = 6,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_CS_DPOT] =
    {
        .pinNum = 7,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_CS_DAC] =
    {
        .pinNum = 8,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPARE_1_PWM_ANALOG] =
    {
        .pinNum = 9,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPARE_2_PWM_ANALOG] =
    {
        .pinNum = 10,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_RL_DIGITAL] =
    {
        .pinNum = 11,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_FL_DIGITAL] =
    {
        .pinNum = 12,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_APPS_IN_1_ANALOG] =
    {
        .pinNum = A0,
        .isOutput = false,  // confirm for both apps'
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_APPS_IN_2_ANALOG] =
    {
        .pinNum = A1,
        .isOutput = false,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_BRAKE_IN_ANALOG] =
    {
        .pinNum = A2,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_RL_ANALOG] =
    {
        .pinNum = A3,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_SPEED_FL_ANALOG] =
    {
        .pinNum = A4,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    },
    [HW_PIN_APPS_OUT_ANALOG] =
    {
        .pinNum = A5,
        .isOutput = true,
        .activeLow = false,
        .physicalState = false,
    }
};

extern MCP4911 DAC; // HW DAC

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/


/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/


