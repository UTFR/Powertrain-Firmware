#include <Arduino.h>

#include "MCP_DAC.h"

class UTFR_APPS_DAC : public MCP4911 {                       // APPS_DAC inherits all of MCP4911's methods and properties

    public:

        UTFR_APPS_DAC(uint8_t dataOut = 255, uint8_t clock = 255);
        void processThrottlePosition();

    private:

        const int _kANALOG_MAX = 1023;
        const float _kAPPS_1_HIGH = 4.75;     
        const float _kAPPS_2_HIGH = 4.275;      
        const float _kAPPS_1_LOW = 1.25;       
        const float _kAPPS_2_LOW = 1.125;       

        const int _kTIME_ALLOWANCE = 100;                // DO NOT CHANGE: Maximum error time allowed by rules; milliseconds
        const float _kTHROTTLE_MAX_DEVIATION = 0.10;
        const float _kOUTPUT_DEVIATION = 0.03;           // do we know what we want this to be?
        const long _kBASE_TIME = -1;

        int _APPS_1_in = 0;
        int _APPS_2_in = 0;                              // 0 - 1023
        int _APPS_output = 0;
        int _APPS_out_verify = 0;                        // 0 - 1023
        float _APPS_1_throttle = 0;
        float _APPS_2_throttle = 0;
        long _time_at_error = _kBASE_TIME;                // unsigned long type according to arduino docs, not sure about this
        unsigned long _time_now = 0;

        bool _throttle_good = false;
        bool _output_good = false;
        bool _exceed_time_allowance = false;
        bool _error_flag_set = false;

        static int getDeviation(int value_1, int value_2);
        void sendOutput();
        void reportError();
        void shutDown();
        int roundOutput(float value);

};