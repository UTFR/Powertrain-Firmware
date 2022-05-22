/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_CAN_MEGA.h"        // TO DO: Add this entire library to CAN library
#include "UTFR_PIN_DRIVER_MEGA.h"
#include "lib_util.h"


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_LVBATT_MEGA_H_
#define _UTFR_LVBATT_MEGA_H_

//#define debug_LVBatt           // Uncomment this line for debug prints


class UTFR_LVBATT_MEGA
{     
    private:

    /******************************************************************************
    *              P R I V A T E   D A T A   D E F I N I T I O N S               *
    *****************************************************************************/

        /*  Old way of muxing thru individual cells, not used now (mux fucked)
        const float _highVoltRaw[8] = { 1023.0, 1023.0, 1023.0, 1023.0,     // TO DO: Calibrate via testing
                                        1023.0, 1023.0, 1023.0, 1023.0};     
        const float _highVolt[8] = {4.1, 8.2, 12.3, 16.4,
                                    4.1, 8.2, 12.3, 16.4};     
        
        const uint8_t _SA_MASK = 0b001;
        const uint8_t _SB_MASK = 0b010;
        const uint8_t _SC_MASK = 0b100;

        const float _volt_cutoff[8] = { 2.5, 5.0, 7.5, 10.0,    // TO DO: determine via testint/datasheet for LV batteries
                                        2.5, 5.0, 7.5, 10.0};

        const uint8_t _total_cells = 8;
        uint8_t _cell = _total_cells;
        */      

        const float _measured_analog_error = 0.01;                          // TO DO: Determine experimentally (or set to zero)
        
        const float _lowVoltRaw = 0.0;
        const float _highVoltRaw = 1023.0;
        const float _lowVolt = 0.0;
        const float _highVolt = 16.4;       // TO DO: determine based on sensor datasheet/testing

        const float _lowTempRaw = 102.0;    // 0.5/5 * 1023 = 102
        const float _highTempRaw = 921.0;   // 4.5/5 * 1023 = 921
        const float _lowTemp = 0.0;         // TO DO: determine based on sensor datasheet/testing
        const float _highTemp = 100.0;      // TO DO: determine based on sensor datasheet/testing
        
        const float _volt_cutoff = 13.0;                        // TO DO: determine via testing/datasheet for LV batteries
        const float _temp_cutoff = 60.0;                        // TO DO: determine via testing/datasheet for LV batteries

        const uint8_t _maxFailed = 3;        // How many times a shutdown-causing state should be re-checked before car shutdown
        uint8_t _failedChecks = 0;           // Counts up to _maxFailed then shuts down the car

        float _readVoltage = 0.0;
        float _readTemp = 0.0;


        /******************************************************************************
        *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
        *****************************************************************************/

        //void readCellVoltage(uint8_t muxPin);             // Checks a single cell voltage     
        void readBattVoltage(void);
        void readBattTemps(void);                                          


    public:

        UTFR_LVBATT_MEGA();                                      // Constructor
        bool checkLVBatt(UTFR_CAN_MEGA& CAN);                 // Check if LV battery voltages and temps are in safe range
};

#endif
