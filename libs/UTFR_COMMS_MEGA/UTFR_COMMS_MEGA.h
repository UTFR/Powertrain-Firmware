#include "UTFR_CAN.h"

#ifndef _UTFR_COMMS_MEGA_H_
#define _UTFR_COMMS_MEGA_H_


class UTFR_COMMS_MEGA
{
    private:

        enum _COMMS_msgNames_E
        {
            COMMS_MSG_LOGDATA,

            COMMS_MSG_COUNT
        };

        String _rx_msg;

    public:

        UTFR_COMMS_MEGA();                            // Constructor
        void receiveSerialMicro(UTFR_CAN &CAN);       // Receives messages on Serial3 port from micro. Stores data when required.
};


#endif