#include "UTFR_APPS.h"

#ifndef _UTFR_COMMS_MICRO_H_
#define _UTFR_COMMS_MICRO_H_

#define HW_PIN_MEGA_TX 10
#define HW_PIN_MEGA_RX 13

#define HW_PIN_BRAKE_IN  A2
#define HW_PIN_WHLSPD_FL A3
#define HW_PIN_WHLSPD_FR A4



class UTFR_COMMS_MICRO
{
    private:

        enum _COMMS_msgNames_E
        {
            COMMS_MSG_LOGDATA,

            COMMS_MSG_COUNT
        };

        unsigned char[9] _sendData;

        uint16_t _throttle_out = 0;
        uint16_t _brake_in = 0;
        uint16_t _whlspd_fl = 0;
        uint16_t _whlspd_fr = 0;

    public:

        SoftwareSerial megaSerial(HW_PIN_MEGA_TX, HW_PIN_MEGA_RX);

        UTFR_COMMS_MICRO();
        void sendSerialMega(UTFR_APPS &APPS, _COMMS_msgNames_E msgName);

};

#endif