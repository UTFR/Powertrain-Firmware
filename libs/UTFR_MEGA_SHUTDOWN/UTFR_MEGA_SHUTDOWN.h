/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
// Pins:
#define HW_PIN_MSCOM1 40
#define HW_PIN_MSCOM2 41
#define HW_PIN_MSCOM3 20

class UTFR_MEGA_SHUTDOWN
{
private:
    const int kRETRY_INTERVAL = 20;
    unsigned long current_time, start_time = 0;

    bool SDC_TRIPPED = false;
    int checkState = 0;

public:
    UTFR_MEGA_SHUTDOWN(); // Constructor
    void begin();         // Declares the pin aad interupt
    void checkShutdown(); // starts the shutdown process
};

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
