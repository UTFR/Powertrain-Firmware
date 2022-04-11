#include "UTFR_COMMS_MICRO.h"
#include "UTFR_APPS.h"

UTFR_COMMS_MICRO COMMS;
UTFR_APPS APPS;
const int DAC_CS = 7;      // DAC CS pin

 
void setup() {
  // initialize APPS:
  APPS.begin(DAC_CS);
}

void loop() {
    APPS.processThrottlePosition();
    COMMS.sendSerialMega(APPS, COMMS_MSG_LOGDATA);
    delay(1000);
}