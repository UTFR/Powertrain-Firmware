#ifndef UTFR_HW_ACM_H
#define UTFR_HW_ACM_H

// DO NOT CHANGE THIS FILE IN ANY WAY

/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define ANALOG_CONSTANT 1023        // Arduino ADC resolution
#define MEASURED_ANALOG_ERROR 0.1   // Average ADC measurement error

#define S0_MASK 0b00001
#define S1_MASK 0b00010
#define S2_MASK 0b00100
#define S3_MASK 0b01000

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/

// ACM Inputs and Outputs
typedef enum
{
  HW_PIN_MUX = 0,
  HW_PIN_SCK,
  HW_PIN_EN_AIRP,
  HW_PIN_MUX_S0,
  HW_PIN_MUX_S1,
  HW_PIN_MUX_S2,
  HW_PIN_MUX_S3,
  HW_PIN_EN_PRECHARGE,
  HW_PIN_AMS_FAULT_EN,
  HW_PIN_RTD_BUZZER_EN,
  HW_PIN_EN_AIRN,
  HW_PIN_SDC_EN,
  HW_PIN_RTD_CONFIRM,
  HW_PIN_CAN2_INT,
  HW_PIN_CAN1_CS,
  HW_PIN_MISO,
  HW_PIN_MOSI,
  HW_PIN_CAN1_INT,
  HW_PIN_CAN2_CS,
  HW_PIN_AMS_HFL_FAULT,
  
  HW_PIN_COUNT
} HW_pin_E;

// Multiplexed inputs
typedef enum
{
  HW_MUX_5V_CURRENT_REF = 0,
  HW_MUX_5V_CURRENT_SENSE,
  HW_MUX_12V_CURRENT_REF,
  HW_MUX_12V_CURRENT_SENSE,
  HW_MUX_PCB_TEMP_1,
  HW_MUX_PCB_TEMP_2,
  HW_MUX_PCB_TEMP_3,
  HW_MUX_PCB_TEMP_4,
  HW_MUX_AIRP_CONDUCTING,
  HW_MUX_AIRN_CONDUCTING,
  HW_MUX_PRECHARGE_CONDUCTING,
  HW_MUX_PRECHARGE_DONE,
  HW_MUX_nTS_ENERGIZED,
  HW_MUX_TS_ENERGIZED,
  HW_MUX_RTD_REQUEST,
  HW_MUX_TRIMMER_CAL,
  HW_MUX_COUNT
} HW_MUX_E;

typedef struct
{
    int pinNum;
    bool isOutput;
    bool activeLow;
    bool physicalState;
} HW_pin_S;

/******************************************************************************
 *          P U B L I C   F U N C T I O N   D E C L A R A T I O N S           *
 *****************************************************************************/
 
void  HW_setupPins(void);
bool  HW_digitalRead(HW_pin_E pin);
void  HW_digitalWrite(HW_pin_E pin, bool physState);
float HW_readMux(HW_MUX_E muxPin);

/******************************************************************************
 *               P U B L I C   D A T A   D E F I N I T I O N S                *
 *****************************************************************************/

// Note to self - array definition should be in .c file when created.
extern HW_pin_S HW_pins[HW_PIN_COUNT];

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

#endif /* UTFR_HW_ACM_H */
