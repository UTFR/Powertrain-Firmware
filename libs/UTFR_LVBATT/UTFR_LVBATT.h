/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_ERROR.h"
#include "lib_util.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_LVBATT_H_
#define _UTFR_LVBATT_H_

//#define debugLVBatt           // Uncomment this line for debug prints

//Pins:
#define HW_PIN_MUX_SA 35        // TO DO: replace with pin abstraction
#define HW_PIN_MUX_SB 34
#define HW_PIN_MUX_SC 36
#define HW_PIN_BMSTEMP_1 A0        
#define HW_PIN_BMSTEMP_2 A1        
#define HW_PIN_BMSVOLT A12  

class UTFR_LVBATT
{     
    private:

    /******************************************************************************
    *              P R I V A T E   D A T A   D E F I N I T I O N S               *
    *****************************************************************************/

        const uint8_t _total_cells = 8;

        const float _measured_analog_error = 0.01;                          // TO DO: Determine experimentally (or set to zero)

        const float _highVoltRaw[8] = { 1023.0, 1023.0, 1023.0, 1023.0,     // TO DO: Calibrate via testing
                                        1023.0, 1023.0, 1023.0, 1023.0};     
        const float _highVolt[8] = {4.1, 8.2, 12.3, 16.4,
                                    4.1, 8.2, 12.3, 16.4};                 

        const float _lowTempRaw = 102.0;    // 0.5/5 * 1023 = 102
        const float _highTempRaw = 921.0;   // 4.5/5 * 1023 = 921
        const float _lowTemp = 0.0;         // TO DO: determine based on sensor datasheet/testing
        const float _highTemp = 100.0;      // TO DO: determine based on sensor datasheet/testing

        const uint8_t _SA_MASK = 0b001;
        const uint8_t _SB_MASK = 0b010;
        const uint8_t _SC_MASK = 0b100;

        const float _volt_cutoff[8] = { 2.5, 5.0, 7.5, 10.0,    // TO DO: determine via testint/datasheet for LV batteries
                                        2.5, 5.0, 7.5, 10.0};
        const float _temp_cutoff = 60.0;                        // TO DO: determine via testint/datasheet for LV batteries

        const uint8_t _maxFailed = 3;        // How many times a shutdown-causing state should be re-checked before car shutdown
        uint8_t _failedChecks = 0;           // Counts up to _maxFailed then shuts down the car

        float _readVoltage = 0.0;
        float _readTemp = 0.0;
        uint8_t _cell = _total_cells;


        /******************************************************************************
        *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
        *****************************************************************************/

        void readCellVoltage(uint8_t muxPin);              // Checks a single cell voltage     
        void readBattTemps(void);                                          


    public:

        UTFR_LVBATT();                                      // Constructor
        bool checkLVBatt(UTFR_ERROR& ERROR);                // Check if LV battery voltages and temps are in safe range
};

#endif
