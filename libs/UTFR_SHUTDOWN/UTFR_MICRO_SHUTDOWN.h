/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

#include "MCP_DAC.h"
/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
// Pins:
#define HW_PIN_MSCOM3 6

class UTFR_MIRCO_SHUTDOWN
{
public:
    UTFR_MIRCO_SHUTDOWN(); // Constructor
    void checkShutdown();  // starts the shutdown process

private:
    MCP4911 DAC; // Digital-to-Analog converter -> this sends drive signals to the inverter which, in turn, powers the motor.

    const uint8_t kCS_DAC = 8;
    const uint8_t HW_PIN_MSCOM1 = 2;

    bool SDC_TRIPPED = false; // if this is true, do nothing until 0 torque to inverter

    bool written = false;
    int readback = -1;
    uint8_t kREADBACK_DELAY = 10;
}

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/

/******************************************************************************
 *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/

/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/
