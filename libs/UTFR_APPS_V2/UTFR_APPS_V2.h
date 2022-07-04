#ifndef _UTFR_APPS_H_
#define _UTFR_APPS_H_

#include "SoftwareSerial.h"
#include "MCP_DAC.h"
#include "lib_util.h"

#define debugMode         // Uncomment this line to enable debug prints

#define THR_1_IN_PIN  A0
#define THR_2_IN_PIN  A1
#define BRAKE_IN_PIN  A2
#define DAC_IN_PIN    A5

//#define MEGA_TX_PIN   10
//#define MEGA_RX_PIN   13

class UTFR_APPS {                      

    public:

        UTFR_APPS(uint8_t dataOut = 255, uint8_t clock = 255);
        void begin(int CS);
        void processThrottlePosition();
        float getThrottlePosition();              // returns _thr_out_;
        void shutdown();
        bool getShutdownState();                // returns true if shutdown() function has been called        


    private:

        MCP4911 DAC;                                    // UTFR_APPS contains our DAC object (UTFR_APPS is now a composite class)

                                                        // "Composite" simply means it has a member which is an object of another class type (MCP4911 here)
                                                            // Composition helps avoid multiple inheritance
                                                            // Multiple inheritance is dangerous - can lead to the diamond problem 
                                                            // Multiple inheritance is slow (chains of function pointers in compiled program) 
                                                            // Multiple inheritance takes up more space (Function lookup tables generated by compiler) 
                                                            
                                                        // Therefore, Composition is advantageous over inheritance for safety and performance reasons

        const int _kCALIBRATIONRATE_ = 50; //[ms] 
        const float _kTHR_MAX_DEVIATION_ = 0.1; //[percent]
        const float _kERROR_TIME_THRESHOLD_ = 100; //[ms]
        const float _kBRAKE_ON_THRESHOLD_ = 0.5;  //[percent]
        
        float _thr_1_high_lim_ = 0;
        float _thr_2_high_lim_ = 0;
        float _brake_in_high_lim_ = 0;
        float _thr_1_low_lim_ = 1023;
        float _thr_2_low_lim_ = 1023;
        float _brake_in_low_lim_ = 1023;

        float _lim_margin_ = 5.12;

        float _thr_1_in_ = 0;
        float _thr_2_in_ = 0;
        float _brake_in_ = 0;
        float _dac_in_ = 0;
          
        float _normalized_max_ = static_cast<float>(1023);

        float _thr_1_normalized_ = 0;
        float _thr_2_normalized_ = 0;
        float _brake_normalized_ = 0;

        float _thr_out_ = 0;

        bool _brake_limit_error_active_ = false;
        bool _brake_plausibility_error_active_ = false;
        bool _thr_plausibility_error_active_ = false;
        bool _thr_limit_error_active_ = false;
        bool _thr_output_error_active_ = false;

        unsigned long _brake_limit_error_time_ = 0;
        unsigned long _thr_plausibility_error_time_ = 0;
        unsigned long _thr_limit_error_time_ = 0;
        unsigned long _thr_output_error_time_ = 0;

        uint8_t _log_counter_ = 0;

        SoftwareSerial megaSerial;

        bool _shutdown_active_ = false;

        void calibrateInputs();

        bool checkErrorCountdown();

        bool checkBrakeConditions();

        bool checkThrottleConditions();

        bool checkOutputConditions();

        void sendOutput();

        void logData();
};

#endif