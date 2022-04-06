/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define HW_PIN_MSCOM1 40
#define HW_PIN_MSCOM3 20

#define HW_PIN_SDC_FAULT 19

#define HW_PIN_BMS_DRIVER 7                         // Disconnect LVBATT low side in carOff() function
#define HW_PIN_IGNITION_DIGITAL 38                  // Tell inverter to shutdown in carOff() function       


class UTFR_MEGA_SHUTDOWN
{
    private:

        const int _kRETRY_INTERVAL = 20;                // milliseconds without response from Micro before retrying shutdown sequence 
        const uint8_t _kRETRY_TIMEOUT = 100;            // DO NOT CHANGE: in rulebook, must send 0 torque in under 100 milliseconds 
        uint8_t _RETRY_ATTEMPTS = 10;                   // Don't need to change - gets set in constructor based on _kRETRY_INTERVAL
        
        uint8_t _retries = 0;                           // Counts up after each retry attempt, carOff() is called if _retries > _RETRY_ATTEMPTS

        unsigned long _current_time = 0;
        volatile unsigned long _start_time = 0;          

    public:

        UTFR_MEGA_SHUTDOWN();           // Constructor
        void checkZeroTorqueOut(void);  // Checks if Micro has successfully sent 0 torque to inverter. Calls carOff() if yes or if timeout.
        void carOff(void);              // Tells inverter to shut down, disconnects low side LV battery.
        void SDC_ISR(void);             // Starts shutdown sequence from Mega.
};
